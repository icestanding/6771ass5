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
//    std::sort(numbersToSort.begin(),numbersToSort.end(), [](const unsigned int& x, const unsigned int& y){
//        return aLessB(x,y,0);} );

    // save bucket and flag indicate if current is used by other thread
    std::vector<std::vector<unsigned int>> bucket(10);
    std::vector<bool> flag(10);
    // pre processing
    preprocess(bucket, flag);
//    for (int i = 0; i < bucket[1].size(); ++i) {
//        std::cout<<bucket[1][i]<<std::endl;
//    }
    // if we can use more than one thread
    if(numCores != 1) {
        std::vector<std::thread> thread(numCores - 1);
        for (int i = 0; i < numCores - 1; ++i) {
            thread[i] = std::thread(&BucketSort::compute, this, std::ref(bucket), std::ref(flag));
        }
//        compute(bucket, flag);
        for (int i = 0; i < numCores - 1; ++i) {
            thread[i].join();
        }

    }// if we can only use one thread
    else {
        compute(bucket, flag);
    }
    numbersToSort.clear();
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < bucket[i].size() ; ++j) {
            numbersToSort.push_back(bucket[i][j]);
        }
    }


}


void BucketSort::preprocess(std::vector<std::vector<unsigned int>> &buckets, std::vector<bool> &flag) {
    for (auto ele = numbersToSort.begin(); ele != numbersToSort.end(); ++ele) {
        auto left_most = *ele;
        while (left_most / 10 > 0) {
            left_most = left_most / 10;
        }

        buckets[left_most].push_back(*ele);
    }
    for (unsigned int i = 0; i < 10; ++i) {
        flag[i] = false;
    }
}

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
std::vector<unsigned int> BucketSort::bucketsort(std::vector<unsigned int> bucket, unsigned int position) {

    // including the rightmost
    std::vector<std::vector<unsigned int>> buckets(11);
    std::vector<unsigned int> result;


    // divede the number into ten bucket
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
//            tmp = tmp - std::pow(10, position + count - 1);
            tmp = tmp%10;
            buckets[tmp].push_back(bucket[i]);
        }
    }
    // insert right
    for (unsigned int k = 0; k <buckets[10].size() ; ++k) {
        result.push_back(buckets[10][k]);
    }
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
