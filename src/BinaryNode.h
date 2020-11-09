//
// Created by Christopher Jung on 2020-11-06.
//


#ifndef C4_BINARYNODE_H
#define C4_BINARYNODE_H

#include "TreeNode.h"

class BinaryNode : public TreeNode{

public:
    BinaryNode(int type, TreeNode* left, TreeNode* right){
        this->type = type;
        this->left = left;
        this->right = right;
    }

    int type;
    TreeNode* left;
    TreeNode* right;
};


#endif //C4_BINARYNODE_H
