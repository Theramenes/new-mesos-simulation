#pragma once
#include <iostream>
#include <math.h>
#include <algorithm>
#include <random>

#include "resource.hpp"

using namespace std;

namespace util
{
    template<typename T>
    T getAverageRandom(T range_from, T range_to) {
        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_int_distribution<T> distr(range_from, range_to);
        return distr(generator);
    }

    ResourceRequestModel getWeightedRandomResourceModel(RMtoINt weights)
    {
        int weightSum = 0;
        RMtoINt::iterator it;
        for (it = weights.begin(); it != weights.end(); it++)
        {
            weightSum += it->second;
        }
        int random = getAverageRandom(0, weightSum);
        ResourceRequestModel rm;

        for (it = weights.begin(); it != weights.end(); it++)
        {
            if(random < it->second)
            {
                rm = it->first;
                break;
            }
            random -= it->second;
        }
        return rm;
    }

    template<typename T>
    void shuffleVector(vector<T> &vec)
    {
        auto rng = std::default_random_engine {};
        shuffle(std::begin(vec), std::end(vec), rng);
    }

    // void averageRandom()
    // {

    // }

    // template<typename T> struct map_init_helper
    // {
    //     T& data;
    //     map_init_helper(T& d) : data(d) {}
    //     map_init_helper& operator() (typename T::key_type const& key, typename T::mapped_type const& value)
    //     {
    //         data[key] = value;
    //         return *this;
    //     }
    // };

    // template<typename T> map_init_helper<T> map_init(T& item)
    // {
    //     return map_init_helper<T>(item);
    // }
} // namespace util
