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

#include "utils/measure.h"
#include "utils/random.h"

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
void generateValues(const size_t aSize, const Order aOrder, std::vector<size_t>& aValues) {
    size_t value = (size_t)-1;

    aValues.reserve(aSize);
    if (eOrderReverse != aOrder) {
        for (size_t elt = 0;
                    elt < aSize;
                    ++elt) {
            if (eOrderRandom == aOrder) {
                value = (size_t)Utils::Random::gen(5 * aSize);
            } else { // eOrderForward
                value += (size_t)(Utils::Random::gen(1, 10));
            }
            aValues[elt] = value;
        }
    } else { // eOrderReverse
        for (size_t elt = aSize;
                    elt > 0;
                    --elt) {
            value += (size_t)(Utils::Random::gen(1, 10));
            aValues[elt - 1] = value;
        }
    }

    /** TODO(SRombauts)
    for (size_t elt = 0; elt < aSize; ++elt) {
        printf("%u, ", aValues[elt]);
    }
    printf("\n");
    */
}

/**
 * @brief Data index by Id
 */
class Data {
public:
    Data() :
        mId(0) {
    }
    explicit Data(const size_t aId) :
        mId(aId) {
    }
    inline size_t getId() const {
        return mId;
    }
    bool operator==(const Data& aData) const {
        return (aData.getId() == mId);
    }
private:
    size_t  mId;
};

/**
 * @brief Run each test thousands times
 */
template<typename T>
void runTest(const size_t aSize, const Order aOrder, Results& aResults) {
    using Utils::Measure;

    for (size_t idxTest = 0;
                idxTest < 10000;
                ++idxTest) {
        // Pre-generate random values for the following tests
        std::vector<size_t> values(aSize);
        generateValues(aSize, aOrder, values);

        T   container;
        ////////////////////////////////////////////////////////////////////////
        // Filling the container with the pre-generated values:
        {
            Measure measure;
            for (size_t idxValue = 0;
                        idxValue < aSize;
                        ++idxValue) {
                container.push_back(Data(values[idxValue]));
            }
            time_t  deltaUs = measure.diff();
            aResults.fillingUs.push_back(deltaUs);
        }

        ////////////////////////////////////////////////////////////////////////
        // Copying the filled container:
        {
            Measure measure;
            T       container2;
            container2 = container;
            time_t  deltaUs = measure.diff();
            aResults.copyingUs.push_back(deltaUs);
        }

        ////////////////////////////////////////////////////////////////////////
        // Finding the pre-generated values in the container:
        {
            Measure measure;
            for (size_t nbFind = 0;
                        nbFind < 1000;
                        ++nbFind) {
                typename T::iterator iValue = std::find(container.begin(),
                                                        container.end(),
                                                        container[nbFind%aSize]);
            }
            time_t  deltaUs = measure.diff();
            aResults.findingUs.push_back(deltaUs);
        }
    }
}

template<typename T>
class Vector : public std::vector<T> {
};

/*
template<typename T>
class Vector {
public:
//  typedef std::vector<typename T>::iterator  iterator;
    typedef std::vector<T>::iterator  iterator;

    typename std::vector<T>::iterator begin() {
        return mVector.begin();
    }
    typename std::vector<T>::iterator& end() {
        return mVector.end();
    }
    void push_back(const T& aVal) {
        return mVector.push_back(aVal);
    }
private:
    std::vector<T>  mVector;
};
*/

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
            runTest<Vector<Data> >(size, order, results);
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
