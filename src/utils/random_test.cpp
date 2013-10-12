/**
 * @file    random_test.cpp
 * @ingroup Utils
 * @brief   Random utility functions.
 *
 * Copyright (c) 2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "utils/random.h"

#include <limits>

#include "gtest/gtest.h"

TEST(Random, GenChar) {
    EXPECT_NE(Utils::Random::GenChar(), Utils::Random::GenChar());
}

TEST(Random, GenString) {
    #define NB_CHAR 10
    char str1[NB_CHAR+1] = "";
    char str2[NB_CHAR+1] = "";
    Utils::Random::GenString(str1, NB_CHAR);
    Utils::Random::GenString(str2, NB_CHAR);
    EXPECT_STRNE(str1, str2);
}
