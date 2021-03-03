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
#define TRUE 3
#define FALSE 4

#define NOT_REACHABLE 0
#define REACHABLE 1

#define NEGATIVE_INFINITY -1
#define NON_INFINITY 0
#define POSITIVE_INFINITY 1

class LatticeElement{
public:

    int modifyCounter = 0;
    virtual bool isTrue() = 0;
    virtual bool isFalse() = 0;

    virtual LatticeElement* meet(LatticeElement *right ) = 0;
    virtual LatticeElement* join(LatticeElement *right ) = 0;

    virtual bool isConstant() = 0;
    virtual llvm::Constant* getConstant() = 0;
    virtual bool isTop() = 0;
};

class RangeLatticeValue : public LatticeElement{
public:
    int infinity = NON_INFINITY;
    llvm::Constant *constant = nullptr;

    bool equals(RangeLatticeValue *other ){
        if(other->infinity == POSITIVE_INFINITY && infinity == POSITIVE_INFINITY || other->infinity == NEGATIVE_INFINITY && infinity == NEGATIVE_INFINITY){
            return true;
        }

        return constant->isElementWiseEqual(other->constant);
    }

    bool isSmaller(RangeLatticeValue &other ){
        if(other.infinity == POSITIVE_INFINITY){
            return infinity != POSITIVE_INFINITY;
        }else if(other.infinity == NEGATIVE_INFINITY){
            return false;
        }else if(infinity == NEGATIVE_INFINITY){
            return true;
        }else if(infinity == POSITIVE_INFINITY){
            return false;
        }

        return constant->isElementWiseEqual(other.constant);
    }

    bool isTop() override{
        return infinity != NON_INFINITY;
    }

    bool isFalse() override{


    }

    bool isTrue() override{
        
    }

    bool isConstant() override{
        return infinity == NON_INFINITY;
    }

    llvm::Constant * getConstant() override{
        return constant;
    }

    RangeLatticeValue * meet(LatticeElement *other) override{
        if(auto right = dynamic_cast<RangeLatticeValue*>(other)){
            RangeLatticeValue *result;
            if(infinity || right->infinity ){
                result->infinity = true;
            }else if(llvm::ConstantExpr::getICmp(llvm::CmpInst::Predicate::ICMP_SLT, right->constant, constant)->getUniqueInteger().getBoolValue()){
                result->constant = right->constant;
            }else{
                result->constant = constant;
            }

            return result;
        }

        throw "tot";
    }

    RangeLatticeValue * join(LatticeElement *other) override{
        if(auto right = dynamic_cast<RangeLatticeValue*>(other)){
            RangeLatticeValue *result;
            if(infinity || right->infinity ){
                result->infinity = true;
            }else if(llvm::ConstantExpr::getICmp(llvm::CmpInst::Predicate::ICMP_SLT, right->constant, constant)->getUniqueInteger().getBoolValue()){
                result->constant = constant;
            }else{
                result->constant = right->constant;
            }

            return result;
        }

        throw "tot";
    }
};




class IntervalLatticeElement : public LatticeElement{
public:
    int state = BOTTOM;
    RangeLatticeValue *lower = new RangeLatticeValue();
    RangeLatticeValue *upper = new RangeLatticeValue();

    bool isTop() override{
        return (lower->isTop() && upper->isTop()) || state == TOP;
    }

    bool isTrue() override{

    }

    bool isFalse() override{

    }

    bool isConstant() override{
        return state != BOTTOM && lower->equals(upper);
    }

    llvm::Constant * getConstant() override{
        return lower->constant;
    }

    IntervalLatticeElement *meet(LatticeElement *other ) override{
        if(auto right = dynamic_cast<IntervalLatticeElement*>(other)){
            auto *result = new IntervalLatticeElement();

            if(state == 0 && right->state == 1){
                result->state = VALUE;
                result->lower->constant = right->lower->constant;
                result->upper->constant = right->upper->constant;
            }else if(state == 1 && right->state == 0){
                result->state = VALUE;
                result->lower->constant = right->lower->constant;
                result->upper->constant = right->upper->constant;
            }else if(state == VALUE && right->state == VALUE){
                result->lower = lower->join(right->lower);
                result->upper = upper->meet(right->upper);
            }

            return result;
        }

        throw "tot";
    }

    IntervalLatticeElement *join(LatticeElement *other ) override{
        if(auto right = dynamic_cast<IntervalLatticeElement*>(other)){
            auto *result = new IntervalLatticeElement();

            if(state == 0 && right->state == 1){
                result->state = VALUE;
                result->lower->constant = right->lower->constant;
                result->upper->constant = right->upper->constant;
            }else if(state == 1 && right->state == 0){
                result->state = VALUE;
                result->lower->constant = right->lower->constant;
                result->upper->constant = right->upper->constant;
            }else if(state == VALUE && right->state == VALUE){
                result->lower = lower->meet(right->lower);
                result->upper = upper->join(right->upper);
            }
            return result;
        }

        throw "tot";
    }

};

static llvm::ConstantInt *trueValue = nullptr;
static llvm::ConstantInt *falseValue = nullptr;

class BoolLatticeElement : public LatticeElement{
public:
    int state = BOTTOM;

    bool isTop() override{
        return state == TOP;
    }

    bool isTrue() override{
        return state == TRUE || state == TOP;
    }

    bool isFalse() override{
        return state == FALSE || state == TOP;
    }

    bool isConstant() override{
        return state == TRUE || state == FALSE;
    }

    llvm::Constant * getConstant() override{
        if(state == TRUE){
            return trueValue;
        }else if(state == FALSE){
            return falseValue;
        }

        throw "tot";
    }

    LatticeElement * meet(LatticeElement *other) override{
        if(auto right = dynamic_cast<BoolLatticeElement*>(other)){
            auto *result = new BoolLatticeElement();

            if(state == BOTTOM || right->state == BOTTOM){
                result->state = BOTTOM;
            }else if(state == TOP && right->state == TOP){
                result->state = TOP;
            }else if(state != right->state){
                result->state = BOTTOM;
            }else if(state == right->state){
                result->state = state;
            }

            return result;
        }

        throw "tot";
    }

    LatticeElement * join(LatticeElement *other) override{
        if(auto right = dynamic_cast<BoolLatticeElement*>(other)){
            auto *result = new BoolLatticeElement();

            if(state == TOP || right->state == TOP){
                result->state = TOP;
            }else if(state == BOTTOM && right->state == BOTTOM){
                result->state = BOTTOM;
            }else if(state != right->state){
                result->state = TOP;
            }else if(state == right->state){
                result->state = state;
            }

            return result;
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

    std::unordered_map<llvm::Value*, LatticeElement*> constant;
    std::unordered_map<llvm::BasicBlock*, ReachableLatticeElement> reachable;
    std::unordered_map<llvm::BasicBlock*, JumpingLatticeElement> targets;

    int modifyCounter = 0;
    bool updateReachable = false;

    Optimizer(llvm::LLVMContext &context) : llvm::FunctionPass(ID), context(context) {
        falseValue = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), 0);
        trueValue = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), 1);
    }

    bool runOnFunction(llvm::Function &func) override{
        constant.clear();
        reachable.clear();
        targets.clear();
        updateReachable = false;
        modifyCounter = 0;

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
            if(pair.second != nullptr && pair.second->isConstant()){
                if(auto ins = llvm::dyn_cast_or_null<llvm::Instruction>(pair.first)){
                    llvm::Constant* constant = pair.second->getConstant();
                    for(auto &use : pair.first->uses()){
                        if(auto branch = llvm::dyn_cast_or_null<llvm::BranchInst>(use.getUser())){
                            if(auto constInt = llvm::dyn_cast_or_null<llvm::ConstantInt>(constant) ){
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
                    }

                    pair.first->replaceAllUsesWith(constant);
                    ins->eraseFromParent();
                }
            }
        }

        std::unordered_set<llvm::BasicBlock*> notReachable;
        for( auto &bb : func.getBasicBlockList() ){
            auto entry = reachable.find(&bb);
            if(entry == reachable.end() || entry->second.state == NOT_REACHABLE){
                notReachable.insert(&bb);
            }
        }

        for(auto *bb : notReachable){
            //std::cout << bb->getName().str() << std::endl;
            bb->eraseFromParent();
        }

        return true;
    }

    int incrementModify(){
        return modifyCounter++;
    }

    bool isNotTop(LatticeElement *element){
        return element == nullptr || !element->isTop();
    }


    void combine(LatticeElement *& left, LatticeElement *right){
        if(right == nullptr){
            throw "xxx";
        }

        if(left == nullptr){
            left = right;
        } else{
            left = left->join(right);
        }

        left->modifyCounter = incrementModify();
    }

    //void combine(LatticeElement &left, LatticeElement &right ){
    //    left = left.meet(right);
        /*if(left.state == 0 && right.state == 1){
            setState(left, VALUE);
            left.min.constant = right.min.constant;
            left.max.constant = right.max.constant;
        }else if(left.state == 1 && right.state == 1){
            if(!right.min.constant->isElementWiseEqual(left.max.constant) &&
                    !right.max.constant->isElementWiseEqual(left.max.constant)){

                if(llvm::ConstantExpr::getICmp(llvm::CmpInst::Predicate::ICMP_SLT, right.min.constant, left.min.constant)->getUniqueInteger().getBoolValue()){
                    left.min.constant = right.min.constant;
                }

                if(llvm::ConstantExpr::getICmp(llvm::CmpInst::Predicate::ICMP_SLT, left.max.constant, right.max.constant)->getUniqueInteger().getBoolValue()){
                    left.max.constant = right.max.constant;
                }
            }
        }else{
            left.state = std::max(left.state, right.state);
        }*/
    //}

    template<class T>
    T *createTop(){
        auto *element = new T();
        element->state = TOP;
        element->modifyCounter = incrementModify();
        return element;
    }

    void setState(IntervalLatticeElement *element, int newState){
        if(element->isTop()){
            throw "xxx";
        }

        if(element->state != newState ){
            element->state = newState;
            element->modifyCounter = incrementModify();
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

    bool getBool(llvm::Constant *constant){
        if(auto constInt = llvm::dyn_cast_or_null<llvm::ConstantInt>(constant)){
            return constInt->getValue().getBoolValue();
        }

        throw "eee";
    }

    LatticeElement *getValue(llvm::Value *value, llvm::BasicBlock* currentBlock){
        LatticeElement *target = constant[value];

        if(auto constValue = llvm::dyn_cast_or_null<llvm::Constant>(value)){
            if(target == nullptr){
                auto *target = new IntervalLatticeElement();
                constant[value] = target;

                //if(target->state == BOTTOM){
                    target->lower->constant = constValue;
                    target->upper->constant = constValue;
                    target->modifyCounter = incrementModify();
                    setState(target, VALUE);
                //}/*else if(target.state == VALUE){
                    //throw
                //}
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
                    LatticeElement* element = getValue(phiNode->getIncomingValue(i), incomingBlock);
                    if(target == nullptr || element->modifyCounter > target->modifyCounter){
                        combine(target, element);
                    }
                }
            }

            constant[value] = target;
        }else if(auto branch = llvm::dyn_cast_or_null<llvm::BranchInst>(value)){
            if(branch->isConditional()){
                auto condition = branch->getCondition();

                auto *conditionValue = getValue(condition, currentBlock);

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

                auto *leftValue = dynamic_cast<IntervalLatticeElement*>(getValue(left, currentBlock));
                auto *rightValue = dynamic_cast<IntervalLatticeElement*>(getValue(right, currentBlock));
                if(leftValue != nullptr && rightValue != nullptr){
                    if(target == nullptr || leftValue->modifyCounter > target->modifyCounter || rightValue->modifyCounter > target->modifyCounter){
                        if(leftValue->isConstant() && rightValue->isConstant()){

                            auto *boolElement = new BoolLatticeElement();
                            if(llvm::CmpInst::Predicate::ICMP_SLT == cmp->getPredicate()){
                                auto resultA = llvm::ConstantExpr::getCompare(llvm::CmpInst::Predicate::ICMP_SLT, leftValue->upper->constant, rightValue->lower->constant);

                                if(getBool(resultA)){
                                    boolElement->state = TRUE;
                                }else{
                                    auto resultB = llvm::ConstantExpr::getCompare(llvm::CmpInst::Predicate::ICMP_SLE, rightValue->upper->constant, leftValue->lower->constant);
                                    if(getBool(resultB)){
                                        boolElement->state = FALSE;
                                    }else{
                                        boolElement->state = TOP;
                                    }
                                }
                            }else if(llvm::CmpInst::Predicate::ICMP_EQ == cmp->getPredicate()){
                                if(leftValue->lower->equals(leftValue->upper) && leftValue->upper->equals(rightValue->lower) && rightValue->lower->equals(rightValue->upper)){
                                    boolElement->state = TRUE;
                                }else{
                                    auto resultB = llvm::ConstantExpr::getCompare(llvm::CmpInst::Predicate::ICMP_SLE, leftValue->lower->constant, leftValue->lower->constant);
                                    if(getBool(resultB)){
                                        boolElement->state = FALSE;
                                    }else{
                                        boolElement->state = TOP;
                                    }
                                }
                            }

                            boolElement->modifyCounter = incrementModify();
                            constant[value] = boolElement;
                        }else if(leftValue->state > 1 || rightValue->state > 1){
                            constant[value] = createTop<BoolLatticeElement>();
                        }
                    }
                }else{
                    throw "xxx";
                }
            }
        }else if(auto binary = llvm::dyn_cast_or_null<llvm::BinaryOperator>(value)){
            if(isNotTop(target)){
                auto left = binary->getOperand(0);
                auto right = binary->getOperand(1);

                auto *leftValue = dynamic_cast<IntervalLatticeElement*>(getValue(left, currentBlock));
                auto *rightValue = dynamic_cast<IntervalLatticeElement*>(getValue(right, currentBlock));
                if(leftValue != nullptr && rightValue != nullptr){
                    if(target == nullptr || leftValue->modifyCounter > target->modifyCounter || rightValue->modifyCounter > target->modifyCounter){
                        if(leftValue->isConstant() && rightValue->isConstant() ){
                            llvm::Constant *result = llvm::ConstantExpr::get(binary->getOpcode(), leftValue->lower->constant, rightValue->lower->constant);

                            constant[value] = getValue(result, currentBlock);
                        }else if(leftValue->isTop() || rightValue->isTop()){
                            constant[value] = createTop<IntervalLatticeElement>();
                        }
                    }
                }else{
                    throw "ooo";
                }
            }
        }else if(auto unary = llvm::dyn_cast_or_null<llvm::UnaryOperator>(value)){
            if(isNotTop(target)){

                auto child = unary->getOperand(0);
                auto *childElement = dynamic_cast<IntervalLatticeElement*>(getValue(child, currentBlock));
                if(childElement != nullptr){
                    if(target == nullptr || childElement->modifyCounter > target->modifyCounter){
                        //if(childElement.modifyCounter > target.modifyCounter ){
                        if(childElement->isConstant()){
                            llvm::Constant *result = llvm::ConstantExpr::get(unary->getOpcode(), childElement->getConstant());
                            constant[value] = getValue(result, currentBlock);
                        }else if(childElement->isTop()){
                            constant[value] = createTop<IntervalLatticeElement>();
                        }
                    }
                }else{
                    throw "ooo";
                }
            }
        }else if(auto cast = llvm::dyn_cast_or_null<llvm::CastInst>(value)){
            if(isNotTop(target)){
                auto child = unary->getOperand(0);
                auto *childElement = dynamic_cast<IntervalLatticeElement*>(getValue(child, currentBlock));
                if(childElement != nullptr){
                    if(target == nullptr || childElement->modifyCounter > target->modifyCounter){
                        //if(childElement.modifyCounter > target.modifyCounter ){
                        if(childElement->isConstant()){
                            llvm::Constant *result =  llvm::ConstantExpr::getCast(cast->getOpcode(), childElement->getConstant(), cast->getDestTy() );
                            constant[value] = getValue(result, currentBlock);
                        }else if(childElement->isTop()){
                            constant[value] = createTop<IntervalLatticeElement>();
                        }
                    }
                }else{
                    throw "ooo";
                }
            }
        }else{
            if(isNotTop(target)){
                constant[value] = createTop<IntervalLatticeElement>();
            }
        }

        return constant[value];
    }



    void debug(llvm::Value *value){
        std::string result;
        llvm::raw_string_ostream stream(result);
        value->print(stream, true);
        std::cout << result << std::endl;
    }
};
