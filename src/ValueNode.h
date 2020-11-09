//
// Created by Christopher Jung on 2020-11-06.
//

#ifndef C4_VALUENODE_H
#define C4_VALUENODE_H

#include "TreeNode.h"

class ValueNode :public TreeNode {

public:
    ValueNode(char cha ){
        this->cha = cha;
    }
    char cha;
};


#endif //C4_VALUENODE_H
