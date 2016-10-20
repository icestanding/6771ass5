#include <iostream>
#include <random>
#include <thread>
#include "BucketSort.h"

// Sorting by least significant digit

// normal redix sort
int main() {

//    unsigned int num = 1;
//    auto lmada  =  [] () {
//        std::cout<<"this is "<<std::endl;
//    };
//    std::vector<std::thread> num_thread;
//    for (unsigned int i = 0; i < num; ++i) {
//        num_thread.push_back(std::thread{lmada});
//    }
//    lmada();
//    for (unsigned int i = 0; i < num; ++i) {
//        num_thread[i].join();
//    }
//    std::cout<<"finish";  10000000
    unsigned int totalNumbers =	10000000;
//    unsigned int printIndex =	259000;

    // use totalNumbers required as the seed for the random
    // number generator.
    std::mt19937 mt(totalNumbers);
    std::uniform_int_distribution<unsigned int> dist(1, std::numeric_limits<unsigned int>::max());

    // create a sort object
    BucketSort pbs;

    // insert random numbers into the sort object
    for (unsigned int i=0; i < totalNumbers; ++i) {
        pbs.numbersToSort.push_back(dist(mt));
//        std::cout<<pbs.numbersToSort[i]<<" ";
    }

    // call sort giving the number of cores available.
    const unsigned int numCores = std::thread::hardware_concurrency();
    std::cout<<"numCores:"<<numCores<<std::endl;

    pbs.sort(numCores);
    std::cout<<"down\n";
//    for (unsigned int i=0; i < totalNumbers; ++i) {
//        std::cout<<pbs.numbersToSort[i]<<" ";
//    }



}