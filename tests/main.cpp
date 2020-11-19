//
// Created by Christopher Jung on 19.11.20.
//

#include <iostream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "FirstTest.h"


int main(int argc, char** argv){
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}

