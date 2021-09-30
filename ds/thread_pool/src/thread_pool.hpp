#pragma once

#include "spinlock.hpp"

#include <forward_list>
#include <vector>
#include <future>
#include <functional>
#include <mutex>
#include <iostream>

namespace obps
{

//
// This class is more of a container for threads than a pool, it does not aggregates queue.
// What it actually does: it ensures thread-safe task insertion. Runs user provided thread in a task wrapper.
// Additionally, ShutDown method provided that locks thread pool until all tasks will finish their execution.
// And there is no way to force task to exit.
//
template <
    typename ThreadRet, // user defined status enum that pool will use to indicate a state of thread execution
    ThreadRet running,  // continue looping on user_thread_func
    ThreadRet finished, // stop looping, task completed
    ThreadRet aborted   // status that indicates errorneus execution stop
> class ThreadPool final 
{
public:
    enum class PoolState {RUNNING, SHUTDOWN};

    ThreadPool() : m_State{PoolState::RUNNING} 
    {}
    
    ~ThreadPool() 
    {
        ShutDown();
    }

    template <typename ...Args> 
    void RunTask(std::function<ThreadRet(Args...)> user_thread_func, Args ...args) noexcept;

    // stop all tasks and wait for them
    void ShutDown() noexcept;

private:
    PoolState m_State;

    template <typename ...Args>
    ThreadRet PoolThread(std::function<ThreadRet(Args...)>, Args...);

    std::forward_list<std::future<ThreadRet>> m_Tasks;
    
    spinlock m_PoolLock;    // prevents race-condition when running tasks and on shutdown
};

template <typename ThreadRet, ThreadRet running, ThreadRet finished, ThreadRet aborted>
template <typename ...Args> 
void ThreadPool<ThreadRet, running, finished, aborted>::RunTask(
    std::function<ThreadRet(Args...)> user_thread_func, Args ...args) noexcept 
{
    std::lock_guard<spinlock> lock(m_PoolLock);

    m_Tasks.emplace_front(std::async(std::launch::async, [this, user_thread_func, args...]()
    {
        return PoolThread(user_thread_func, args...);
    }));
}

template <typename ThreadRet, ThreadRet running, ThreadRet finished, ThreadRet aborted>
template <typename ...Args> 
ThreadRet ThreadPool<ThreadRet, running, finished, aborted>::PoolThread( 
    std::function<ThreadRet(Args...)> user_thread_func, Args ...args)
{
    for(;;) {
        ThreadRet status;
        do { // wait until thread completes it's work
            try {
                status = user_thread_func(args...);
            }
            catch (const std::exception& e) {
                std::cerr << "Exception on thread shutdown:\n" << e.what();
                throw e;
            }
        } while(status == running);

        return status;
    } // for(;;)
}

template <typename ThreadRet, ThreadRet running, ThreadRet completed, ThreadRet aborted>
void ThreadPool<ThreadRet, running, completed, aborted>::ShutDown() noexcept
{
    std::lock_guard<spinlock> lock(m_PoolLock);
    // state propagates to all threads
    m_State = PoolState::SHUTDOWN;
    
    for (auto& current : m_Tasks)
    {
        current.get(); // TODO: process
    }

    m_Tasks.clear();
}

} // namespace obps