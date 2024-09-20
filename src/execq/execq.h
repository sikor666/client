#pragma once

#include "execq/ExecutionQueue.h"

#include <atomic>
#include <functional>
#include <memory>

namespace execq {
/**
 * @class IExecutionPool
 * @brief ThreadPool-like object that provides context for task execution.
 */
class IExecutionPool;

/**
 * @brief Creates pool with hardware-optimal number of threads.
 * @discussion Usually you want to create single instance of IExecutionPool for multiple IExecutionQueue/IExecutionStream
 * to achive best performance.
 */
std::shared_ptr<IExecutionPool> CreateExecutionPool();

/**
 * @brief Creates concurrent queue with specific processing function.
 * @discussion All objects pushed into this queue will be processed on either one of pool threads or on the queue-specific
 * thread.
 * @discussion Tasks in the queue run concurrently on available threads.
 * @discussion Queue is not designed to execute long-term tasks like waiting some event etc.
 * For such purposes use separate thread or serial queue without execution pool.
 */
template <typename R, typename T>
std::unique_ptr<IExecutionQueue<R(T)>> CreateConcurrentExecutionQueue(std::shared_ptr<IExecutionPool> executionPool,
    std::function<R(const std::atomic_bool & isCanceled, T && object)> executor)
{
    return std::unique_ptr<impl::ExecutionQueue<R, T>>(new impl::ExecutionQueue<R, T>(
        false, executionPool, *impl::IThreadWorkerFactory::defaultFactory(), std::move(executor)));
}

} // namespace execq
