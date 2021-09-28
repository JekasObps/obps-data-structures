#include "gtest/gtest.h"
#include <iostream>

#include "thread_pool.hpp"

using namespace obps;

class TestThreadPool : public ::testing::Test
{
protected:

};

enum STATUS{A, B, C};

TEST_F(TestThreadPool, Test)
{
    ThreadPool<STATUS, STATUS::A, STATUS::B, STATUS::C> pool;
    pool.RunTask<int>([](int i) { 
        std::cout << i << std::endl;
        return STATUS::A;}, 53);

    pool.ShutDown();
}