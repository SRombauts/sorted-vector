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
#include <vector>
#include <algorithm>
#include <numeric>

#include "utils/random.h"
#include "utils/time.h"

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
 * @brief Orders in which containers are filled
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
        default:            pOrderString = "?";         break;
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
        value = (TVal)-1;
    } else {
        value = (TVal)aSize;
    }
    for (size_t elt = 0;
                elt < aSize;
                ++elt) {
        switch (aOrder) {
            case eOrderRandom:
                value = (TVal)Utils::Random::gen(aSize - 1);
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
    using Utils::Time;

    for (size_t idxTest = 0;
                idxTest < 1000;
                ++idxTest) {
        // Pre-generate random values for the following tests
        std::vector<typename T::value_type> values(aSize);
        generateValues<typename T::value_type>(aSize, aOrder, values);

        T   container;
        ////////////////////////////////////////////////////////////////////////
        // Filling the container with the pre-generated values:
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
        // Copying the filled container:
        {
            time_t  startUs = Time::getTickUs();
            T       container2;
            container2 = container;
            time_t  endUs   = Time::getTickUs();
            time_t  deltaUs = Time::diff(startUs, endUs);
            aResults.copyingUs.push_back(deltaUs);
        }

        ////////////////////////////////////////////////////////////////////////
        // Finding the pre-generated values in the container:
        {
            time_t  startUs = Time::getTickUs();
            for (size_t nbFind = 0;
                        nbFind < 1000;
                        ++nbFind) {
                typename T::iterator iValue = std::find(container.begin(),
                                                        container.end(),
                                                        container[nbFind%aSize]);
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
 * - (future) 10 random insertion
 * - (future) 10 random removal
*/
int main() {
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
