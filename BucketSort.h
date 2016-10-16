//
// Created by Chenyu on 14/10/2016.
//

#ifndef ASS5_BUCKETSORT_H
#define ASS5_BUCKETSORT_H

#include <vector>

struct BucketSort {
    // vector of numbers
    std::vector<unsigned int> numbersToSort;
    void sort(unsigned int numCores);
    //
    void preprocess(std::vector<std::vector<unsigned int>> &buckets, std::vector<bool> &flag);
    void compute(std::vector<std::vector<unsigned int>> &bucket, std::vector<bool> &flag);
    std::vector<unsigned int> bucketsort(std::vector<unsigned int> bucket, unsigned int position);

};



#endif //ASS5_BUCKETSORT_H
