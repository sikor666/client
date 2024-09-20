#pragma once

#include "IExecutionQueue.h"
#include "IExecutionStream.h"

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
 * @brief Creates pool with manually-specified number of threads.
 * @discussion Sometimes number of threads should be restricted or can be detected in some non-default way.
 * @param threadCount Number of threads for execution context. If number of threads less than 2, exeption will be raised.
 */
std::shared_ptr<IExecutionPool> CreateExecutionPool(const uint32_t threadCount);

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
    std::function<R(const std::atomic_bool & isCanceled, T && object)> executor);

/**
 * @brief Creates serial queue with specific processing function.
 * @discussion All objects pushed into this queue will be processed on either one of pool threads or on the queue-specific
 * thread.
 * @discussion Tasks in the queue run in serial (one-after-one) order.
 * @discussion Queue is not designed to execute long-term tasks like waiting some event etc.
 * For such purposes use separate thread or serial queue without execution pool.
 */
template <typename R, typename T>
std::unique_ptr<IExecutionQueue<R(T)>> CreateSerialExecutionQueue(std::shared_ptr<IExecutionPool> executionPool,
    std::function<R(const std::atomic_bool & isCanceled, T && object)> executor);

/**
 * @brief Creates serial queue with specific processing function.
 * @discussion All objects pushed into this queue will be processed on the queue-specific thread.
 * @discussion Tasks in the queue run in serial (one-after-one) order.
 * @discussion This queue can be used to execute long-term tasks like waiting some event etc.
 */
template <typename R, typename T>
std::unique_ptr<IExecutionQueue<R(T)>> CreateSerialExecutionQueue(
    std::function<R(const std::atomic_bool & isCanceled, T && object)> executor);

/**
 * @brief Creates execution stream with specific executee function. Stream is stopped by default.
 * @discussion When stream started, 'executee' function will be called each time when ExecutionPool have free thread.
 * @discussion Stream is not designed to execute long-term tasks like waiting some event etc.
 * For such purposes use separate thread or serial queue without execution pool.
 */
std::unique_ptr<IExecutionStream> CreateExecutionStream(
    std::shared_ptr<IExecutionPool> executionPool, std::function<void(const std::atomic_bool & isCanceled)> executee);

template <typename R> using QueueTask = std::packaged_task<R(const std::atomic_bool & isCanceled)>;

/**
 * @brief Creates concurrent queue that processes custom tasks.
 * @discussion All objects pushed into this queue will be processed on either one of pool threads or on the queue-specific
 * thread.
 * @discussion Tasks in the queue run concurrently on available threads.
 * @discussion Queue is not designed to execute long-term tasks like waiting some event etc.
 * For such purposes use separate thread or serial queue without execution pool.
 */
template <typename R = void>
std::unique_ptr<IExecutionQueue<void(QueueTask<R>)>> CreateConcurrentTaskExecutionQueue(
    std::shared_ptr<IExecutionPool> executionPool);

/**
 * @brief Creates serial queue that processes custom tasks.
 * @discussion All objects pushed into this queue will be processed on either one of pool threads or on the queue-specific
 * thread.
 * @discussion Tasks in the queue run in serial (one-after-one) order.
 * @discussion Queue is not designed to execute long-term tasks like waiting some event etc.
 * For such purposes use separate thread or serial queue without execution pool.
 */
template <typename R = void>
std::unique_ptr<IExecutionQueue<void(QueueTask<R>)>> CreateSerialTaskExecutionQueue(
    std::shared_ptr<IExecutionPool> executionPool);

/**
 * @brief Creates serial queue that processes custom tasks.
 * @discussion All objects pushed into this queue will be processed on the queue-specific thread.
 * @discussion Tasks in the queue run in serial (one-after-one) order.
 * @discussion This queue can be used to execute long-term tasks like waiting some event etc.
 */
template <typename R = void> std::unique_ptr<IExecutionQueue<void(QueueTask<R>)>> CreateSerialTaskExecutionQueue();

} // namespace execq

#include "execq/execq_private.h"
