//
// Created by chris on 21.01.21.
//

#include "Operator.h"

void Operator::dump(PrettyPrinter& printer) override{
    switch(id){
        case ARROW: printer << "->"; break;
        case DOT: printer << "."; break;

        case STAR: printer << "*"; break;
        case PLUS: printer << "+"; break;
        case MINUS: printer << "-"; break;
        case LESS: printer << "<"; break;
        case LESS_EQUAL: printer << "<="; break;
        case GREATER: printer << ">"; break;
        case GREATER_EQUAL: printer << ">="; break;
        case LEFT_SHIFT: printer << "<<"; break;
        case RIGHT_SHIFT: printer << ">>"; break;
        case EQUAL: printer << "=="; break;
        case NOT_EQUAL: printer << "!="; break;
        case AND_AND: printer << "&&"; break;
        case OR_OR: printer << "||"; break;
        case ASSIGN: printer << "="; break;

        case SIZEOF: printer << "sizeof "; break;
        case AND: printer << "&"; break;
        case NOT: printer << "!"; break;
    }
}