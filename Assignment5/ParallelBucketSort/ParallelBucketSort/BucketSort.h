//
//  BucketSort.h
//  ParallelBucketSort
//
//  Created by Ethan Xu on 28/10/17.
//  Copyright © 2017 Ethan Xu. All rights reserved.
//

#ifndef BucketSort_h
#define BucketSort_h

#include <vector>

struct BucketSort
{
    // vector of numbers
    std::vector<unsigned int> numbersToSort;

    void sort(unsigned int numCores);
};

#endif /* BucketSort_h */
