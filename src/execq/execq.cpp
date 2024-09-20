#include "execq.h"
#include "ExecutionStream.h"

namespace {
uint32_t GetOptimalThreadCount()
{

    const uint32_t defaultThreadCount = 4;
    const uint32_t hardwareThreadCount = std::thread::hardware_concurrency();

    return hardwareThreadCount ? hardwareThreadCount : defaultThreadCount;
}

std::shared_ptr<execq::IExecutionPool> CreateDefaultExecutionPool(const uint32_t threadCount)
{
    return std::make_shared<execq::impl::ExecutionPool>(threadCount, *execq::impl::IThreadWorkerFactory::defaultFactory());
}
} // namespace

std::shared_ptr<execq::IExecutionPool> execq::CreateExecutionPool()
{
    return CreateDefaultExecutionPool(GetOptimalThreadCount());
}

std::shared_ptr<execq::IExecutionPool> execq::CreateExecutionPool(const uint32_t threadCount)
{
    if (!threadCount)
    {
        throw std::runtime_error("Failed to create IExecutionPool: thread count could not be zero.");
    }
    else if (threadCount == 1)
    {
        throw std::runtime_error(
            "Failed to create IExecutionPool: for single-thread execution use pool-independent serial queue.");
    }

    return CreateDefaultExecutionPool(threadCount);
}

std::unique_ptr<execq::IExecutionStream> execq::CreateExecutionStream(
    std::shared_ptr<IExecutionPool> executionPool, std::function<void(const std::atomic_bool & isCanceled)> executee)
{
    return std::unique_ptr<impl::ExecutionStream>(
        new impl::ExecutionStream(executionPool, *impl::IThreadWorkerFactory::defaultFactory(), std::move(executee)));
}
