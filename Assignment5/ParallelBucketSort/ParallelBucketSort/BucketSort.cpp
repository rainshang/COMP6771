//
//  BucketSort.cpp
//  ParallelBucketSort
//
//  Created by Ethan Xu on 28/10/17.
//  Copyright © 2017 Ethan Xu. All rights reserved.
//

#include "BucketSort.h"
#include <cmath>
#include <thread>
#include <algorithm>

struct BucketTask
{
    unsigned bucketIndex;
    std::vector<unsigned> numbersInBucket;

    bool isRunning;
    bool isDone;

    BucketTask() {}

    BucketTask(unsigned bucketIndex)
    {
        this->bucketIndex = bucketIndex;
        isRunning = false;
        isDone = false;
    }

    void run(std::vector<unsigned int> &numbersToSort)
    {
        isRunning = true;
        isDone = false;

        for (auto a : numbersToSort)
        {
            unsigned bits = getBits(a); // get the bits of each number
            unsigned firstNum = a / std::pow(10, bits - 1);
            if (firstNum == bucketIndex)
            {
                numbersInBucket.push_back(a);
            }
        }

        // in each bucket, do the sort
        std::sort(numbersInBucket.begin(), numbersInBucket.end(), [this](const unsigned &x, const unsigned &y) {
            return aLessB(x, y, 0);
        });

        isRunning = false;
        isDone = true;
    }

    /**
     *return the bits of i, e.g. i = 1000, then return 4
     */
    unsigned getBits(unsigned i)
    {
        unsigned bits = 1; //at least, 1 bit
        unsigned tempMaxValue = i;
        while (tempMaxValue >= 10)
        {
            ++bits;
            tempMaxValue /= 10;
        }
        return bits;
    }

    bool aLessB(const unsigned int &x, const unsigned int &y, unsigned int pow)
    {
        if (x == y)
        {
            return false; // if the two numbers are the same then one is not less than the other
        }
        unsigned int a = x;
        unsigned int b = y;
        // work out the digit we are currently comparing on.
        if (pow == 0)
        {
            while (a / 10 > 0)
            {
                a = a / 10;
            }
            while (b / 10 > 0)
            {
                b = b / 10;
            }
        }
        else
        {
            while (a / 10 >= (unsigned int)std::round(std::pow(10, pow)))
            {
                a = a / 10;
            }
            while (b / 10 >= (unsigned int)std::round(std::pow(10, pow)))
            {
                b = b / 10;
            }
        }

        if (a == b)
            return aLessB(x, y, pow + 1); // recurse if this digit is the same
        else
            return a < b;
    }

    void move2MainBucket(std::vector<unsigned int> &numbersToSort)
    {
        for (auto a : numbersInBucket)
        {
            numbersToSort.push_back(a);
        }
        numbersInBucket.clear();
        isRunning = false;
        isDone = false;
    }
};

void BucketSort::sort(unsigned int numCores)
{
    const unsigned BUCKET_NUM = 9; //for MSD (Most Significant Digit) 1~9
    BucketTask BUCKETS[BUCKET_NUM];
    const unsigned TASK_NUM = numCores - 1; // main thread + task threads = numCores

    // init 9 buckets
    for (unsigned i = 0; i < BUCKET_NUM; ++i)
    {
        BUCKETS[i] = BucketTask{i + 1};
    }

    // main loop, to manage task threads
    while (true)
    {
        unsigned isRunningNum = 0;
        unsigned isDoneNum = 0;

        for (unsigned i = 0; i < BUCKET_NUM; ++i)
        {
            if (BUCKETS[i].isRunning)
            {
                ++isRunningNum;
            }

            if (BUCKETS[i].isDone)
            {
                ++isDoneNum;
            }
        }

        if (isRunningNum < TASK_NUM) // can run more thread
        {
            unsigned validThreadNum = TASK_NUM - isRunningNum;
            if (isDoneNum < BUCKET_NUM) // some buckets are still waiting to start
            {
                for (unsigned i = 0; i < BUCKET_NUM; ++i)
                {
                    if (!BUCKETS[i].isRunning && !BUCKETS[i].isDone && validThreadNum > 0)
                    {
                        std::thread{&BucketTask::run, &BUCKETS[i], std::ref(numbersToSort)}.detach(); // to send the references of BUCKETS and numbersToSort to the space of new thread; make the thread run independently
                        --validThreadNum;
                    }
                }
            }
            else // all done
            {
                numbersToSort.clear(); // clear main bucket
                for (unsigned i = 0; i < BUCKET_NUM; ++i)
                {
                    BUCKETS[i].move2MainBucket(numbersToSort);
                }
                break;
            }
        }
    }
}
