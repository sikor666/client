#pragma once

#include "execq/ExecutionQueue.h"

namespace execq {
namespace details {
template <typename R> void ExecuteQueueTask(const std::atomic_bool & isCanceled, QueueTask<R> && task)
{
    if (task.valid())
    {
        task(isCanceled);
    }
}
} // namespace details
} // namespace execq

template <typename R, typename T>
std::unique_ptr<execq::IExecutionQueue<R(T)>> execq::CreateConcurrentExecutionQueue(
    std::shared_ptr<IExecutionPool> executionPool,
    std::function<R(const std::atomic_bool & isCanceled, T && object)> executor)
{
    return std::unique_ptr<impl::ExecutionQueue<R, T>>(new impl::ExecutionQueue<R, T>(
        false, executionPool, *impl::IThreadWorkerFactory::defaultFactory(), std::move(executor)));
}

template <typename R, typename T>
std::unique_ptr<execq::IExecutionQueue<R(T)>> execq::CreateSerialExecutionQueue(
    std::shared_ptr<IExecutionPool> executionPool,
    std::function<R(const std::atomic_bool & isCanceled, T && object)> executor)
{
    return std::unique_ptr<impl::ExecutionQueue<R, T>>(new impl::ExecutionQueue<R, T>(
        false, executionPool, *impl::IThreadWorkerFactory::defaultFactory(), std::move(executor)));
}

template <typename R, typename T>
std::unique_ptr<execq::IExecutionQueue<R(T)>> execq::CreateSerialExecutionQueue(
    std::function<R(const std::atomic_bool & isCanceled, T && object)> executor)
{
    return std::unique_ptr<impl::ExecutionQueue<R, T>>(
        new impl::ExecutionQueue<R, T>(true, nullptr, *impl::IThreadWorkerFactory::defaultFactory(), std::move(executor)));
}

template <typename R>
std::unique_ptr<execq::IExecutionQueue<void(execq::QueueTask<R>)>> execq::CreateConcurrentTaskExecutionQueue(
    std::shared_ptr<IExecutionPool> executionPool)
{
    return CreateConcurrentExecutionQueue<void, QueueTask<R>>(executionPool, &details::ExecuteQueueTask<R>);
}

template <typename R>
std::unique_ptr<execq::IExecutionQueue<void(execq::QueueTask<R>)>> execq::CreateSerialTaskExecutionQueue(
    std::shared_ptr<IExecutionPool> executionPool)
{
    return CreateSerialExecutionQueue<void, QueueTask<R>>(executionPool, &details::ExecuteQueueTask<R>);
}

template <typename R>
std::unique_ptr<execq::IExecutionQueue<void(execq::QueueTask<R>)>> execq::CreateSerialTaskExecutionQueue()
{
    return CreateSerialExecutionQueue<void, QueueTask<R>>(&details::ExecuteQueueTask<R>);
}
