/**
 * @file    time.h
 * @ingroup Utils
 * @brief   Simple time tools.
 *
 * Copyright (c) 2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#pragma once

#include <time.h>

namespace Utils {

/**
 * @brief   Useful time measurements
 * @ingroup Utils
 */
class Time {
public:
    /**
     * @brief Get tick in microseconds
     */
    static time_t getTickMs();
    /**
     * @brief Get tick in milliseconds
     */
    static time_t getTickUs();
    /**
     * @brief Calculate difference between consecutive ticks
     */
    static time_t diff(const time_t aStartTime, const time_t aEndTime);
};

} // namespace Utils
