/**
 * @file    random.cpp
 * @ingroup Utils
 * @brief   Random utility functions.
 *
 * Copyright (c) 2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "utils/random.h"

#include <stdio.h>

#include <cstdlib>  // rand(), srand()
#include <ctime>    // time()

namespace Utils {

// Generate an integer between 0 and aMax (<= RAND_MAX).
unsigned long Random::gen(unsigned long aMax) {
    return (std::rand() % (aMax + 1));
}

// Generate a printable alphanumeric character.
char Random::genChar() {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    return alphanum[Random::gen(sizeof(alphanum) - 1)];
}

// Generate a printable alphanumeric string.
void Random::genString(char* str, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        str[i] = Random::genChar();
    }

    str[len] = '\0';
}

// Initialize the random generator with the current time
Random::Init::Init() {
    std::srand(std::time(0));
}

// Static initialization of the random generator, calling Init() before main()
static Random::Init _mInit;


} // namespace Utils
