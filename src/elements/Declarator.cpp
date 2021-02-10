//
// Created by chris on 21.01.21.
//

#include "Declarator.h"
#include "../parser/PrettyPrinter.h"

void Declarator::dump(PrettyPrinter &stream) {

    for (int i = 0; i < pointer; i++) {
        stream << "(*";
    }
    if (directDeclarator != nullptr) {
        directDeclarator->dump(stream);
    }
    for (int i = 0; i < pointer; i++) {
        stream << ")";
    }
}
