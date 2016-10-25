#include "BucketSort.h"

#include <algorithm>
#include <cmath>
#include <vector>
#include <iostream>
#include <thread>
bool aLessB(const unsigned int& x, const unsigned int& y, unsigned int pow) {

    if (x == y) return false; // if the two numbers are the same then one is not less than the other
    unsigned int a = x;
    unsigned int b = y;

    // work out the digit we are currently comparing on.
    if (pow == 0) {
        while (a / 10 > 0) {
            a = a / 10;
        }
        while (b / 10 > 0) {
            b = b / 10;
        }
    } else {
        while (a / 10 >= std::pow(10,pow)) {
            a = a / 10;
        }
        while (b / 10 >= std::pow(10,pow)) {
            b = b / 10;
        }
    }

    if (a == b)
        return aLessB(x,y,pow + 1);  // recurse if this digit is the same
    else
        return a < b;
}

// TODO: replace this with a parallel version. 
void BucketSort::sort(unsigned int numCores) {

    // save bucket and flag indicate if current is used by other thread
    std::vector<std::vector<unsigned int>> bucket(10);
    std::vector<bool> flag(10);
    
    // pre processing, diveded all  number into ten buckets
    preprocess(bucket, flag);

    // if we can use more than one thread, dynamically assign buckets to thread that available
    if(numCores != 1) {
        std::vector<std::thread> thread(numCores - 1);
        for (unsigned int i = 0; i < numCores - 1; ++i) {
            thread[i] = std::thread(&BucketSort::compute, this, std::ref(bucket), std::ref(flag));
        }
        for (unsigned int i = 0; i < numCores - 1; ++i) {
            thread[i].join();
        }
    }
    // if we can only use one thread
    else {
        compute(bucket, flag);
    }
    numbersToSort.clear();
    // rebuild number_to_sort
    for (unsigned int i = 0; i < 10; ++i) {
        for (unsigned int j = 0; j < bucket[i].size() ; ++j) {
            numbersToSort.push_back(bucket[i][j]);
        }
    }
}

/**
 *  pre-process data, divide it into 10 bucket
 **/
void BucketSort::preprocess(std::vector<std::vector<unsigned int>> &buckets, std::vector<bool> &flag) {
    // sort all the number into bucket
    for (auto ele = numbersToSort.begin(); ele != numbersToSort.end(); ++ele) {
        auto left_most = *ele;
        while (left_most / 10 > 0) {
            left_most = left_most / 10;
        }
        buckets[left_most].push_back(*ele);
    }
    // mark all the bucket
    for (unsigned int i = 0; i < 10; ++i) {
        flag[i] = false;
    }
}


/**
 *  Each thread can sort current sub-bucket if current sub-bucket was not be modified before
 *  The status of curret sub-bucket is memorised by std::vector<bool> flag, if current one was be modified,
 *  set to 1, otherwies do sorting.
 **/
void  BucketSort::compute(std::vector<std::vector<unsigned int>> &bucket, std::vector<bool> &flag) {
    for (unsigned int i = 0; i < flag.size(); ++i) {
        if(flag[i] == 1) {
            continue;
        } else {
            flag[i] = 1;
            if(bucket[i].size() > 1) {
                bucket[i] = bucketsort(bucket[i], 1);
            }

        }
    }
}

/**
 *  recursive sort next digit (divide and conquer)
 **/
std::vector<unsigned int> BucketSort::bucketsort(std::vector<unsigned int> bucket, unsigned int position) {
    // including the rightmost digit which end with (-1)
    std::vector<std::vector<unsigned int>> buckets(11);
    std::vector<unsigned int> result;
    // divide the number into ten buckets
    for (unsigned int i = 0; i < bucket.size(); ++i) {
        unsigned int tmp = bucket[i];
        int count = 0;
        while (tmp/10 >= std::pow(10, position)){
            tmp = tmp/10;
            ++count;
        }
        if(count == 0) {
            buckets[10].push_back(bucket[i]);
        } else {
            tmp = tmp%10;
            buckets[tmp].push_back(bucket[i]);
        }
    }
    // conquer the smallest bucket first (which end with '-1')
    for (unsigned int k = 0; k <buckets[10].size() ; ++k) {
        result.push_back(buckets[10][k]);
    }
    // conquer rest number
    for (unsigned int j = 0; j < 10; ++j) {
        if(buckets[j].size() > 1) {
            buckets[j] = BucketSort::bucketsort(buckets[j], position + 1);
        }
        for (unsigned int i = 0; i < buckets[j].size(); ++i) {
            result.push_back(buckets[j][i]);
        }
    }
    return result;

}
