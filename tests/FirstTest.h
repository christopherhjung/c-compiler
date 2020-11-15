//
// Created by Christopher Jung on 15.11.20.
//

#pragma once

#include <tests/Base.hh>

void testBasic()
{
    // do some nice calculation; store the results in `foo` and `bar`,
    // respectively

    ALEPH_ASSERT_THROW( foo != bar );
    ALEPH_ASSERT_EQUAL( foo, 2.0 );
    ALEPH_ASSERT_EQUAL( bar, 1.0 );
}

void testAdvanced()
{
    // a more advanced test
}

int main(int, char**)
{
    testBasic();
    testAdvanced();
}