//
// Created by Christopher Jung on 28.02.21.
//

#pragma once

#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <llvm/IR/InstrTypes.h>
#include <llvm/MC/MCAsmBackend.h>

#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"

#include "OptimizeException.h"

#include "../common.h"

#define BOTTOM 0
#define VALUE 1
#define TOP 2
#define TRUE 3
#define FALSE 4

#define NOT_REACHABLE 0
#define REACHABLE 1

#define NEGATIVE_INFINITY -1
#define NON_INFINITY 0
#define POSITIVE_INFINITY 1



class ConstantLatticeElement {
public:
    int state = BOTTOM;
    static ConstantLatticeElement *Bottom;
    static ConstantLatticeElement *Top;

    llvm::Constant *constant = nullptr;

    ConstantLatticeElement(){

    }

    ConstantLatticeElement(int state) : state(state){

    }

    bool isTop() {
        return state == TOP;
    }

    bool constIsTrue(){
        if(auto constInt = llvm::dyn_cast_or_null<llvm::ConstantInt>(constant)){
            return constInt->getValue().getBoolValue();
        }

        OPTIMIZE_ERROR();
    }

    bool isTrue() {
        return state == TOP || (state == VALUE && constIsTrue());
    }

    bool isFalse() {
        return state == TOP || (state == VALUE && !constIsTrue());
    }

    bool equals(ConstantLatticeElement *element) {
        return state == element->state && constant->isElementWiseEqual(element->constant);
    }

    bool isConstant() {
        return state == VALUE;
    }

    llvm::Constant * getConstant() {
        return constant;
    }

    ConstantLatticeElement *join(ConstantLatticeElement *other ) {
        if(equals(other)){
            return this;
        }

        if( state == BOTTOM && other->state == VALUE ){
            return other;
        }else if(state == VALUE && other->state == BOTTOM){
            return this;
        }else{
            return ConstantLatticeElement::Top;
        }
    }
};

class ReachableLatticeElement{
public:
    int state = NOT_REACHABLE;
};

class JumpingLatticeElement{
public:
    std::unordered_set<llvm::BasicBlock*> targets;
};

class Optimizer : public llvm::FunctionPass{
public:
    static char ID;

    llvm::LLVMContext &context;

    std::unordered_map<llvm::Value*, ConstantLatticeElement*> constant;
    std::unordered_map<llvm::BasicBlock*, ReachableLatticeElement> reachable;
    std::unordered_map<llvm::BasicBlock*, JumpingLatticeElement> branches;

    std::unordered_set<llvm::Instruction*> queue;

    Optimizer(llvm::LLVMContext &context) : llvm::FunctionPass(ID), context(context) {
    }

    bool runOnFunction(llvm::Function &func) override{
        analyse(func);
        optimize(func);

        return true;
    }

    void analyse(llvm::Function &func){
        constant.clear();
        reachable.clear();
        branches.clear();
        queue.clear();

#ifdef DEBUG
        std::cout << "\u001b[31m" << func.getName().str() << "\u001b[0m" << std::endl;
#endif

        auto *entryBlock = &func.getEntryBlock();

        makeReachable(nullptr, entryBlock);

        while(!queue.empty()){
            llvm::Instruction *instruction = *queue.begin();
            queue.erase(instruction);
            getValue(instruction);
        }
    }

    void optimize(llvm::Function &func){
        std::vector<llvm::Instruction*> unusedInstruction;

        for(auto pair : constant){
            if(pair.second != nullptr && pair.second->isConstant()){
                if(auto ins = llvm::dyn_cast_or_null<llvm::Instruction>(pair.first)){
                    llvm::Constant* constant = pair.second->getConstant();
                    for(auto &use : pair.first->uses()){
                        if(auto branch = llvm::dyn_cast_or_null<llvm::BranchInst>(use.getUser())){
                            llvm::BasicBlock *target;
                            if(pair.second->isTrue()){
                                target = branch->getSuccessor(0);
                            }else{
                                target = branch->getSuccessor(1);
                            }

                            auto targetBranch = llvm::BranchInst::Create(target);
                            targetBranch->insertAfter(branch);
                            branch->eraseFromParent();
                        }
                    }

                    pair.first->replaceAllUsesWith(constant);
                    unusedInstruction.push_back(ins);
                }
            }
        }

        std::vector<llvm::BasicBlock*> notReachable;
        for( auto &bb : func.getBasicBlockList() ){
            auto entry = reachable.find(&bb);
            if(entry == reachable.end() || entry->second.state == NOT_REACHABLE){
                notReachable.push_back(&bb);

            }
        }

        for(auto ins : unusedInstruction){
            ins->eraseFromParent();
        }

#ifdef DEBUG
        std::cout << "--unreachable--" << std::endl;
#endif
        for(auto *bb : notReachable){
#ifdef DEBUG
            std::cout << bb->getName().str() << std::endl;
#endif
            bb->eraseFromParent();
        }

/*
        start:
        for( auto &bb : func.getBasicBlockList() ){
            if(&bb == &func.getEntryBlock()){
                continue;
            }

            if(bb.size() == 1){
                bool found = false;
                for(auto &use : bb.uses()){
                    if(llvm::dyn_cast_or_null<llvm::PHINode>(use.getUser())){
                        found = true;
                        break;
                    }
                }
                if(!found){
                    llvm::Instruction *ins = &*bb.begin();
                    if(auto branch = llvm::dyn_cast_or_null<llvm::BranchInst>(ins)){
                        if(!branch->isConditional()){
                            bb.replaceAllUsesWith(branch->getSuccessor(0));
                            bb.eraseFromParent();
                            goto start;
                        }
                    }
                }
            }
        }*/
    }

    static bool isNotTop(ConstantLatticeElement *element){
        return element == nullptr || !element->isTop();
    }


    static void join(ConstantLatticeElement *& left, ConstantLatticeElement *right){
        if(right == nullptr){
            OPTIMIZE_ERROR();
        }

        if(left == nullptr){
            left = right;
        } else{
            left = left->join(right);
        }
    }

    void enqueue(llvm::Instruction *instruction){
        auto entry = reachable.find(instruction->getParent());

        if(entry != reachable.end() && entry->second.state == REACHABLE){
            queue.insert(instruction);
#ifdef DEBUG
            debug(instruction);
#endif
        }
    }

    void makeReachable(llvm::BasicBlock *current, llvm::BasicBlock *target){
        bool updated = false;
        if(reachable[target].state == NOT_REACHABLE){
            reachable[target].state = REACHABLE;
            updated = true;
        }

        if(current != nullptr){
            auto &ref = branches[current].targets;

            if(ref.find(target) == ref.end()){
                ref.insert(target);
                updated = true;
            }
        }

        if(updated){
#ifdef DEBUG
            std::cout << "-----------------------------" << std::endl;
            std::cout << "--made reachable--" << std::endl;
            std::cout <<  "\u001b[34m" << target->getName().str() << "\u001b[0m" << std::endl;
            std::cout << "--queued--" << std::endl;
#endif

            for( auto &instruction : *target ){
                enqueue(&instruction);
            }
#ifdef DEBUG
            std::cout << "-----------------------------" << std::endl;
#endif
        }
    }

    static bool getBool(llvm::Constant *constant){
        if(auto constInt = llvm::dyn_cast_or_null<llvm::ConstantInt>(constant)){
            return constInt->getValue().getBoolValue();
        }

        OPTIMIZE_ERROR();
    }

    void update(llvm::Value* value, ConstantLatticeElement* nextElement){
        ConstantLatticeElement *oldElement = constant[value];

        if(auto instruction = llvm::dyn_cast_or_null<llvm::Instruction>(value)){
#ifdef DEBUG
            std::cout << "--erasee--" << std::endl;
            debug(value);
            std::cout << "-----------------------------" << std::endl;
#endif
            queue.erase(instruction);
        }

        if(oldElement == nullptr || !oldElement->equals(nextElement)){

#ifdef DEBUG
            std::cout << "-----------------------------" << std::endl;
            std::cout << "--visited instruction--" << std::endl;
            debug(value);

            if(oldElement != nullptr){
                std::cout << "--before--" << std::endl;
                std::cout << oldElement->state << std::endl;
            }
            std::cout << "--after--" << std::endl;
            if(nextElement == nullptr){
                std::cout << "null" << std::endl;
            }else{
                std::cout << nextElement->state << std::endl;
            }
            std::cout << "--queued--" << std::endl;
#endif
            for( auto &use : value->uses() ){
                if(auto instruction = llvm::dyn_cast<llvm::Instruction>(use.getUser())){
                    enqueue(instruction);
                }
            }

#ifdef DEBUG
            std::cout << "-----------------------------" << std::endl;
#endif
        }

        if(nextElement == nullptr){
            OPTIMIZE_ERROR();
        }

        constant[value] = nextElement;
    }

    ConstantLatticeElement *getOrCreateLattice(llvm::Value *value){
        auto entry = constant.find(value);

        if(entry != constant.end()){
            return entry->second;
        }
        ConstantLatticeElement* element;
        if(auto constant = llvm::dyn_cast_or_null<llvm::Constant>(value)){
            element = new ConstantLatticeElement();
            element->constant = constant;
            element->state = VALUE;
        }else if(llvm::dyn_cast_or_null<llvm::Instruction>(value)){
            element = ConstantLatticeElement::Bottom;
        }else{
            element = ConstantLatticeElement::Top;
        }
        constant[value] = element;
        return element;
    }

    ConstantLatticeElement *getValue(llvm::Value *value){
        ConstantLatticeElement *target = getOrCreateLattice(value);

#ifdef DEBUG
        std::cout << "--hello--" << std::endl;
        debug(value);
#endif

        if(auto constValue = llvm::dyn_cast_or_null<llvm::Constant>(value)){
            return target;
        }else if(auto phiNode = llvm::dyn_cast_or_null<llvm::PHINode>(value)){
            auto currentBlock = phiNode->getParent();
            unsigned int incomingCount = phiNode->getNumIncomingValues();
            for(unsigned int i = 0 ; i < incomingCount; i++){
                auto incomingBlock = phiNode->getIncomingBlock(i);

                auto entry = branches.find(incomingBlock);

                if(entry != branches.end() && entry->second.targets.find(currentBlock) != entry->second.targets.end()){
                    join(target, getOrCreateLattice(phiNode->getIncomingValue(i)));
                }
            }

            update(value, target);
        }else if(auto branch = llvm::dyn_cast_or_null<llvm::BranchInst>(value)){
            auto currentBlock = branch->getParent();
            if(branch->isConditional()){
                auto condition = branch->getCondition();

                auto *conditionValue = getOrCreateLattice(condition);

                if(conditionValue->isTrue()){
                    auto trueSuccessor = branch->getSuccessor(0);
                    makeReachable(currentBlock, trueSuccessor);
                }

                if(conditionValue->isFalse()){
                    auto falseSuccessor = branch->getSuccessor(1);
                    makeReachable(currentBlock, falseSuccessor);
                }
            }else{
                auto successor = branch->getSuccessor(0);
                makeReachable(currentBlock, successor);
            }
        }else if(auto cmp = llvm::dyn_cast_or_null<llvm::CmpInst>(value)){
            if(isNotTop(target)){
                auto left = cmp->getOperand(0);
                auto right = cmp->getOperand(1);

                auto *leftValue = getOrCreateLattice(left);
                auto *rightValue = getOrCreateLattice(right);
                if(leftValue->isConstant() && rightValue->isConstant()){
                    auto resultA = llvm::ConstantExpr::getCompare(cmp->getPredicate(), leftValue->constant, rightValue->constant);
                    update(value, getOrCreateLattice(resultA));
                }else if(leftValue->state > 1 || rightValue->state > 1){
                    update(value, ConstantLatticeElement::Top);
                }
            }
        }else if(auto binary = llvm::dyn_cast_or_null<llvm::BinaryOperator>(value)){
            if(isNotTop(target)){
                auto left = binary->getOperand(0);
                auto right = binary->getOperand(1);

                auto *leftValue = getOrCreateLattice(left);
                auto *rightValue = getOrCreateLattice(right);
                if(leftValue->isConstant() && rightValue->isConstant() ){
                    llvm::Constant *result = llvm::ConstantExpr::get(binary->getOpcode(), leftValue->constant, rightValue->constant);
                    update(value, getOrCreateLattice(result));
                }else if(leftValue->isTop() || rightValue->isTop()){
                    update(value, ConstantLatticeElement::Top);
                }
            }
        }else if(auto unary = llvm::dyn_cast_or_null<llvm::UnaryOperator>(value)){
            if(isNotTop(target)){

                auto child = unary->getOperand(0);
                auto *childElement = getOrCreateLattice(child);
                if(childElement->isConstant()){
                    llvm::Constant *result = llvm::ConstantExpr::get(unary->getOpcode(), childElement->getConstant());
                    update(value, getOrCreateLattice(result));
                }else if(childElement->isTop()){
                    update(value, ConstantLatticeElement::Top);
                }
            }
        }else if(auto cast = llvm::dyn_cast_or_null<llvm::CastInst>(value)){
            if(isNotTop(target)){
                auto child = cast->getOperand(0);
                auto *childElement = getOrCreateLattice(child);
                if(childElement->isConstant()){
                    llvm::Constant *result =  llvm::ConstantExpr::getCast(cast->getOpcode(), childElement->getConstant(), cast->getDestTy() );
                    update(value, getOrCreateLattice(result));
                }else if(childElement->isTop()){
                    update(value, ConstantLatticeElement::Top);
                }
            }
        }else{
            if(isNotTop(target)){
                update(value, ConstantLatticeElement::Top);
            }
        }
        return target;
    }

    void debug(llvm::Value *value){
        std::string result;
        llvm::raw_string_ostream stream(result);
        value->print(stream, true);
        std::cout << result << std::endl;
    }
};
