/**
 * @file    time_test.cpp
 * @ingroup sorted-vector
 * @brief   Simple time tools.
 *
 * Copyright (c) 2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "time.h"

#include <limits>

#include "gtest/gtest.h"

TEST(Time, diff) {
    EXPECT_EQ(0,            Time::diff(0, 0));
    EXPECT_EQ(123,          Time::diff(0, 123));
    EXPECT_EQ((time_t)-123, Time::diff(123, 0));
    EXPECT_EQ(0,            Time::diff(-1, -1));
}
