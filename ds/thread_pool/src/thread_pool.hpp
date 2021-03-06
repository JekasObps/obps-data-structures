#pragma once

#include <forward_list>
#include <vector>
#include <thread>
#include <future>
#include <functional>

#include <mutex>
#include <iostream>

#include "spinlock.hpp"

namespace obps
{

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
    void RunTask( std::function<ThreadRet(Args...)> user_thread_func, // user task
        Args ...args // user arguments to pass into the task
    ) noexcept;

    // stop all tasks and wait for them
    void ShutDown() noexcept;

private:
    PoolState m_State;

    template <typename ...Args>
    void PoolThread(std::function<ThreadRet(Args...)>, std::promise<ThreadRet>&, std::future<ThreadRet>&, Args...);

    std::forward_list<std::tuple<std::promise<ThreadRet>, std::future<ThreadRet>, std::thread>> m_Tasks;
    spinlock m_PoolLock;    // prevents race-condition when running tasks and on shutdown
};


template <typename ThreadRet, ThreadRet running, ThreadRet finished, ThreadRet aborted>
template <typename ...Args> 
void ThreadPool<ThreadRet, running, finished, aborted>::RunTask(
    std::function<ThreadRet(Args...)> user_thread_func, Args ...args) noexcept 
{
    std::lock_guard<spinlock> lock(m_PoolLock);
    
    std::promise<ThreadRet> promise;
    auto future = promise.get_future();

    m_Tasks.push_front(std::make_tuple(std::move(promise), std::move(future), std::thread()));
    auto &[p, f, thread] = m_Tasks.front();

    // starting thread
    thread = std::thread(
        &ThreadPool::PoolThread<Args...>, this, 
        user_thread_func, 
        std::ref(p), 
        std::ref(f), 
        std::forward<Args>(args)...
    );
}

template <typename ThreadRet, ThreadRet running, ThreadRet finished, ThreadRet aborted>
template <typename ...Args> 
void ThreadPool<ThreadRet, running, finished, aborted>::PoolThread( 
    std::function<ThreadRet(Args...)> user_thread_func,
    std::promise<ThreadRet>& promise, 
    std::future<ThreadRet>& future,
    Args ...args)
{
    auto return_exception = [&promise](const std::exception& e){
        try {
            promise.set_exception(std::make_exception_ptr(e));
        } 
        catch(...) {
            std::cerr << "Exception was thrown on promise::set_exception:\n";
        }
    };

    for(;;) {
        ThreadRet status;
        do { // wait until thread completes it's work
            try {
                status = user_thread_func(args...);
            }
            catch (const std::exception& e) {
                std::cerr << "Exception on thread shutdown:\n" << e.what();
                return_exception(e);
                return;
            }
        } while(status == running);

        // return status and exit thread function
        promise.set_value_at_thread_exit(status);
        return;
    } // for(;;)

    promise.set_value_at_thread_exit(finished);
}


template <typename ThreadRet, ThreadRet running, ThreadRet completed, ThreadRet aborted>
void ThreadPool<ThreadRet, running, completed, aborted>::ShutDown() noexcept
{
    std::lock_guard<spinlock> lock(m_PoolLock);
    // state propagates to all threads
    m_State = PoolState::SHUTDOWN;
    
    for (auto current = m_Tasks.begin(); current != m_Tasks.end(); )
    {
        auto& [_, future, thread] = *current;

        future.get(); // TODO: process
        thread.join();

        current = std::next(current);
        m_Tasks.pop_front();
    }
}

} // namespace obps