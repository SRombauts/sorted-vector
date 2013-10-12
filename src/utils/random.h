/**
 * @file    random.h
 * @ingroup Utils
 * @brief   Random utility functions.
 *
 * Copyright (c) 2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 *
 * @todo Add a static initializer rand(time(0))
 * @todo Add a simple Rand(modulo) method
 */
#pragma once

#include <cstddef>  // size_t

namespace Utils {

/**
 * @brief Random utility functions.
 * @ingroup Utils
 */
class Random {
public:
    /**
     * @brief Generate a printable alphanumeric character.
     */
    static char GenChar();

    /**
     * @brief Generate a printable alphanumeric string.
     */
    static void GenString(char* str, size_t len);
};

} // namespace Utils

