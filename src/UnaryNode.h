//
// Created by Christopher Jung on 2020-11-06.
//

#ifndef C4_UNARYNODE_H
#define C4_UNARYNODE_H

#include "TreeNode.h"

class UnaryNode : public TreeNode{
public:
    UnaryNode(int type, TreeNode* child){
        this->type = type;
        this->child = child;
    }
    int type;
    TreeNode* child;
};


#endif //C4_UNARYNODE_H
