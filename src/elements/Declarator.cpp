//
// Created by chris on 21.01.21.
//

#include "Declarator.h"

void Declarator::dump(PrettyPrinter &stream) {

    for(uint32_t i = 0 ; i < pointer ; i++){
        stream << "(*";
    }
    if(directDeclarator != nullptr){
        directDeclarator->dump(stream);
    }
    for(uint32_t i = 0 ; i < pointer ; i++){
        stream << ")";
    }
}
