//
// Created by Christopher Jung on 21.12.20.
//

#pragma once

#include "SimpleObjects.h"


class Visitor {

    void enter(LabeledStatement* labeledStatement){

    }

    void enter(If* labeledStatement){

    }

    void enter(While* labeledStatement){

    }

    void enter(GoTo* labeledStatement){

    }

    void enter(Continue* labeledStatement){

    }

    void enter(Break* labeledStatement){

    }

    void enter(Block* labeledStatement){

    }

    void enter(Declaration* labeledStatement){

    }

    void enter(Expression* labeledStatement){

    }

};
