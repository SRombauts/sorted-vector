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
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <numeric>

#include "base/time.h"

/**
 * @brief Store all benchmarks results
 */
struct Results {
    std::vector<time_t> fillingUs;
    std::vector<time_t> copyingUs;
    std::vector<time_t> findingUs;
    std::vector<time_t> insertingUs;
    std::vector<time_t> removingUs;
};

/**
 * @enum Orders in which containers are filled
*/
enum Order {
    eOrderRandom,   ///< Filled with random integers
    eOrderForward,  ///< Filled with ordered random integers
    eOrderReverse   ///< Filled with reverse ordered random integers
};

/**
 * @brief Convert an Order value to its textual description
 */
const char* OrderToString(const Order aOrder) {
    const char* pOrderString = NULL;

    switch (aOrder) {
        case eOrderRandom:  pOrderString = "random";    break;
        case eOrderForward: pOrderString = "forward";   break;
        case eOrderReverse: pOrderString = "reverse";   break;
        default:            pOrderString = "?";
    }

    return pOrderString;
}

/**
 * @brief Generate values in the order specified for the test
 */
template<typename TVal>
void generateValues(const size_t aSize, const Order aOrder, std::vector<TVal>& aValues) {
    TVal value;
    if (eOrderReverse != aOrder) {
        value = (TVal)0;
    } else {
        value = (TVal)aSize;
    }
    for (size_t elt = 0;
                elt < aSize;
                ++elt) {
        switch (aOrder) {
            case eOrderRandom:
                value = (TVal)(rand()%aSize);
                break;
            case eOrderForward:
                ++value;
                break;
            case eOrderReverse:
                --value;
                break;
        }
        aValues.push_back(value);
    }
}

/**
 * @brief Run each test thousands times
 */
template<typename T>
void runTest(const size_t aSize, const Order aOrder, Results& aResults) {
    for (size_t idxTest = 0;
                idxTest < 10000;
                ++idxTest) {
        // Generate values for the test
        std::vector<typename T::value_type> values(aSize);
        generateValues<typename T::value_type>(aSize, aOrder, values);

        T   container;
        ////////////////////////////////////////////////////////////////////////
        // Filling :
        {
            time_t  startUs = Time::getTickUs();
            for (size_t idxValue = 0;
                        idxValue < aSize;
                        ++idxValue) {
                container.push_back(values[idxValue]);
            }
            time_t  endUs   = Time::getTickUs();
            time_t  deltaUs = Time::diff(startUs, endUs);
            aResults.fillingUs.push_back(deltaUs);
        }

        ////////////////////////////////////////////////////////////////////////
        // Copying :
        {
            time_t  startUs = Time::getTickUs();
            T       container2;
            container2 = container;
            time_t  endUs   = Time::getTickUs();
            time_t  deltaUs = Time::diff(startUs, endUs);
            aResults.copyingUs.push_back(deltaUs);
        }

        ////////////////////////////////////////////////////////////////////////
        // Finding :
        {
            time_t  startUs = Time::getTickUs();
            for (size_t nbFind = 0;
                        nbFind < 1000;
                        ++nbFind) {
                typename T::iterator iValue = std::find(container.begin(), container.end(), (typename T::value_type)(rand()%aSize));
            }
            time_t  endUs   = Time::getTickUs();
            time_t  deltaUs = Time::diff(startUs, endUs);
            aResults.findingUs.push_back(deltaUs);
        }
    }
}

/**
 * @brief Run various benchmarks on different containers
 *
 * (C++11, Boost) * (sizes: 25, 100, 250) * (random, forward, reverse) * (10000 run) :
 * - filling
 * - full copy
 * - 1000 random find
 * - (futur) 10 random insertion
 * - (futur) 10 random removal
*/
int main() {
    srand(time(0));

    Order tabOrder[] = {eOrderRandom, eOrderForward, eOrderReverse};
    size_t tabSizes[] = {25, 100, 250};

    for (size_t idxSize = 0;
                idxSize < sizeof(tabSizes)/sizeof(tabSizes[0]);
                ++idxSize) {
        size_t size = tabSizes[idxSize];
        for (size_t idxOrder = 0;
                    idxOrder < sizeof(tabOrder)/sizeof(tabOrder[0]);
                    ++idxOrder) {
            Order order = tabOrder[idxOrder];
            printf("%i %s; ", size, OrderToString(order));

            Results results;
            runTest<std::vector<size_t> >(size, order, results);
            time_t  fillingMs   = std::accumulate(results.fillingUs.begin(), results.fillingUs.end(), 0)/1000;
            time_t  copyingMs   = std::accumulate(results.copyingUs.begin(), results.copyingUs.end(), 0)/1000;
            time_t  findingMs   = std::accumulate(results.findingUs.begin(), results.findingUs.end(), 0)/1000;
            time_t  insertingMs = std::accumulate(results.insertingUs.begin(), results.insertingUs.end(), 0)/1000;
            time_t  removingMs  = std::accumulate(results.removingUs.begin(), results.removingUs.end(), 0)/1000;
            printf("%lu;%lu;%lu;%lu;%lu;\n", fillingMs, copyingMs, findingMs, insertingMs, removingMs);
        }
    }

    return 0;
}
