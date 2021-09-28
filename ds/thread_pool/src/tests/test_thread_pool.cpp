#include "gtest/gtest.h"
#include <iostream>

#include "thread_pool.hpp"

using namespace obps;

class TestThreadPool : public ::testing::Test
{
protected:

};

enum STATUS{RUNNING, FINISHED, ABORT};

TEST_F(TestThreadPool, Test)
{
    ThreadPool<STATUS, RUNNING, FINISHED, ABORT> pool;
    pool.RunTask<int>([](int i) { 
        std::cout << i << std::endl;
        return FINISHED;}, 53);

    pool.ShutDown();
}