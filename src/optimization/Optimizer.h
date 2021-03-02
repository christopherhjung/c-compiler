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

#define BOTTOM 0
#define VALUE 1
#define TOP 2


#define NOT_REACHABLE 0
#define REACHABLE 1

class ConstantLatticeElement{
public:
    int state = BOTTOM;
    int modifyCounter = 0;
    llvm::Constant *constant = nullptr;
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

    std::unordered_map<llvm::Value*, ConstantLatticeElement> constant;
    std::unordered_map<llvm::BasicBlock*, ReachableLatticeElement> reachable;
    std::unordered_map<llvm::BasicBlock*, JumpingLatticeElement> targets;

    int modifyCounter = 0;
    bool updateReachable = false;

    Optimizer(llvm::LLVMContext &context) : llvm::FunctionPass(ID), context(context) {

    }

    bool runOnFunction(llvm::Function &func) override{
        auto *entryBlock = &func.getEntryBlock();

        reachable[entryBlock].state = REACHABLE;

        std::queue<llvm::BasicBlock*> queue;

        int currentModifyCounter = -1;
        while(currentModifyCounter != modifyCounter || updateReachable){
            currentModifyCounter = modifyCounter;
            updateReachable = false;

            for( auto pair : reachable ){
                if(pair.second.state == REACHABLE){
                    queue.push(pair.first);
                }
            }

            while(!queue.empty()){
                llvm::BasicBlock *bb = queue.front();
                queue.pop();

                for (auto insIter = bb->begin(), insEnd = bb->end(); insIter != insEnd; ++insIter) {
                    llvm::Instruction *ins = &*insIter;

                    getValue(ins, bb);
                }
            }
        }

        for(auto pair : constant){
            if(pair.second.state == 1){
                if(auto ins = llvm::dyn_cast_or_null<llvm::Instruction>(pair.first)){
                    for(auto &use : pair.first->uses()){
                        if(auto branch = llvm::dyn_cast_or_null<llvm::BranchInst>(use.getUser())){
                            if(auto constInt = llvm::dyn_cast_or_null<llvm::ConstantInt>(pair.second.constant) ){
                                llvm::BasicBlock *target;
                                if(constInt->getValue().getBoolValue()){
                                    target = branch->getSuccessor(0);
                                }else{
                                    target = branch->getSuccessor(1);
                                }

                                auto targetBranch = llvm::BranchInst::Create(target);
                                targetBranch->insertAfter(branch);
                                branch->eraseFromParent();
                            }
                        }
                    }

                    pair.first->replaceAllUsesWith(pair.second.constant);
                    ins->eraseFromParent();
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

        for(auto *bb : notReachable){
            bb->eraseFromParent();
        }

        return true;
    }

    int incrementModify(){
        return modifyCounter++;
    }

    void combine(ConstantLatticeElement &left, ConstantLatticeElement &right ){
        if(left.state == 0 && right.state == 1){
            setState(left, 1);
            left.constant = right.constant;
        }else if(left.state == 1 && right.state == 1){
            if(!right.constant->isElementWiseEqual(left.constant)){
                setState(left, 2);
            }
        }else{
            left.state = std::max(left.state, right.state);
        }
    }

    void setState(ConstantLatticeElement& element, int newState){
        if(element.state < newState){
            element.state = newState;
            element.modifyCounter = incrementModify();
        }
    }

    void makeReachable(llvm::BasicBlock *current, llvm::BasicBlock *target){
        if(reachable[target].state == NOT_REACHABLE){
            reachable[target].state = REACHABLE;
            updateReachable = true;
        }

        auto &ref = targets[current].targets;

        if(ref.find(target) == ref.end()){
            targets[current].targets.insert(target);
            updateReachable = true;
        }
    }

    ConstantLatticeElement &getValue(llvm::Value *value, llvm::BasicBlock* currentBlock){
        ConstantLatticeElement &target = constant[value];

        if(auto constValue = llvm::dyn_cast_or_null<llvm::Constant>(value)){
            if(target.state == 0){
                target.constant = constValue;
                setState(target, 1);
            }else if(target.state == 1){
                if(!target.constant->isElementWiseEqual(constValue)){
                    setState(target, 2);
                }
            }
        }else if(auto phiNode = llvm::dyn_cast_or_null<llvm::PHINode>(value)){
            unsigned int incomingCount = phiNode->getNumIncomingValues();
            for(unsigned int i = 0 ; i < incomingCount; i++){
                auto incomingBlock = phiNode->getIncomingBlock(i);

                auto entry = targets.find(incomingBlock);

                bool found = false;
                if(entry != targets.end()){
                    for( llvm::BasicBlock* target : entry->second.targets ){
                        if(target == currentBlock){
                            found = true;
                            break;
                        }
                    }
                }

                if(found){
                    combine(target, getValue(phiNode->getIncomingValue(i), incomingBlock));
                }
            }
        }else if(auto branch = llvm::dyn_cast_or_null<llvm::BranchInst>(value)){
            if(branch->isConditional()){
                auto condition = branch->getCondition();

                auto &conditionValue = getValue(condition, currentBlock);

                if(conditionValue.state == 1){
                    if(auto constValue = llvm::dyn_cast<llvm::ConstantInt>(conditionValue.constant)){
                        if(constValue->getValue().getBoolValue()){
                            auto trueSuccessor = branch->getSuccessor(0);
                            makeReachable(currentBlock, trueSuccessor);
                        }else{
                            auto falseSuccessor = branch->getSuccessor(1);
                            makeReachable(currentBlock, falseSuccessor);
                        }
                    }
                }else if(conditionValue.state == 2){
                    auto trueSuccessor = branch->getSuccessor(0);
                    makeReachable(currentBlock, trueSuccessor);
                    auto falseSuccessor = branch->getSuccessor(1);
                    makeReachable(currentBlock, falseSuccessor);
                }
            }else{
                auto successor = branch->getSuccessor(0);
                makeReachable(currentBlock, successor);
            }
        }else if(auto call = llvm::dyn_cast_or_null<llvm::CallInst>(value)){
            call = call;
        }else if(auto returnInst = llvm::dyn_cast_or_null<llvm::ReturnInst>(value)){
            returnInst = returnInst;
        }else if(auto cmp = llvm::dyn_cast_or_null<llvm::CmpInst>(value)){
            auto left = cmp->getOperand(0);
            auto right = cmp->getOperand(1);

            ConstantLatticeElement &leftValue = getValue(left, currentBlock);
            ConstantLatticeElement &rightValue = getValue(right, currentBlock);

            //if(leftValue.modifyCounter > target.modifyCounter || rightValue.modifyCounter > target.modifyCounter){
                if(leftValue.state == 1 && rightValue.state == 1 && target.state < 2){
                    auto result = llvm::ConstantExpr::getCompare(cmp->getPredicate(), leftValue.constant, rightValue.constant);

                    combine(target, getValue(result, currentBlock));
                }else if(leftValue.state > 1 || rightValue.state > 1){
                    setState(target, 2);
                }
            //}
        }else if(auto binary = llvm::dyn_cast_or_null<llvm::BinaryOperator>(value)){
            auto left = binary->getOperand(0);
            auto right = binary->getOperand(1);

            ConstantLatticeElement &leftValue = getValue(left, currentBlock);
            ConstantLatticeElement &rightValue = getValue(right, currentBlock);


            //if(leftValue.modifyCounter > target.modifyCounter || rightValue.modifyCounter > target.modifyCounter){
            if(leftValue.state == 1 && rightValue.state == 1 && target.state < 2){
                llvm::Constant *result = llvm::ConstantExpr::get(binary->getOpcode(), leftValue.constant, rightValue.constant);
                combine(target, getValue(result, currentBlock));
            }else if(leftValue.state > 1 || rightValue.state > 1){
                setState(target, 2);
            }
        }else if(auto unary = llvm::dyn_cast_or_null<llvm::UnaryOperator>(value)){
            auto child = unary->getOperand(0);
            ConstantLatticeElement &childElement = getValue(child, currentBlock);

            //if(childElement.modifyCounter > target.modifyCounter ){
            if(childElement.state == 1 && target.state < 2){
                llvm::Constant *result = llvm::ConstantExpr::get(unary->getOpcode(), childElement.constant);
                combine(target, getValue(result, currentBlock));
            }else if(childElement.state > 1){
                setState(target, 2);
            }
        }else if(auto cast = llvm::dyn_cast_or_null<llvm::CastInst>(value)){
            auto child = cast->getOperand(0);
            ConstantLatticeElement &childElement = getValue(child, currentBlock);

            if(childElement.state == 1 && target.state < 2){
                llvm::Constant *result =  llvm::ConstantExpr::getCast(cast->getOpcode(), childElement.constant, cast->getDestTy() );
                combine(target, getValue(result, currentBlock));
            }else if(childElement.state > 1){
                setState(target, 2);
            }

            if(target.state != 2){
                setState(target, 2);
            }
        }else{
            if(target.state != 2){
                setState(target, 2);
            }
            //debug(value);
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
