/**
 * @file    main.cpp
 * @ingroup sorted-vector
 * @brief   Comparing various implementations of sorted STL containers.
 *
 * Copyright (c) 2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <cstdio>

#include "base/time.h"

int main() {
    time_t startUs = Time::getTickUs();
    // Do something ...
    time_t endUs   = Time::getTickUs();

    time_t deltaUs = Time::diff(startUs, endUs);

    printf("elpased time: %luus\n", deltaUs);

    return 0;
}
