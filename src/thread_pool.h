#pragma once

#ifndef THREAD_POOL_HEADER
#define THREAD_POOL_HEADER

#include <condition_variable>
#include <mutex>
#include <thread>
#include <unordered_map>

class ThreadPool
{
  private:
    std::unordered_map<std::thread::id, std::thread> mThreads;

    unsigned int mLimit;

    mutable std::mutex mThreadsMutex;

    mutable std::condition_variable mCond;

  public:
    explicit ThreadPool(unsigned int limit)
      : mLimit(limit)
    {}

    template<typename Callable, typename... Args>
    void add(Callable&& f, Args&&... args)
    {
        std::unique_lock<std::mutex> lock(mThreadsMutex);

        mCond.wait(lock, [&] { return mThreads.size() < mLimit; });

        std::thread thread = std::thread(f, args...);
        
        mThreads[thread.get_id()] = std::move(thread);
    }
    void erase()
    {
        std::unique_lock<std::mutex> lock(mThreadsMutex);

        const auto threadId = std::this_thread::get_id();

        if (mThreads.find(threadId) != mThreads.end()) {
            
            mThreads[threadId].detach();
            mThreads.erase(threadId);
        }
        mCond.notify_one();
    }

    void Terminate()
    {
        std::unique_lock<std::mutex> lock(mThreadsMutex);
        mThreads.clear();
    }
};

#endif