#include "ExecutionPool.h"

execq::impl::ExecutionPool::ExecutionPool(const uint32_t threadCount, const IThreadWorkerFactory & workerFactory)
{
    for (uint32_t i = 0; i < threadCount; i++)
    {
        m_workers.emplace_back(workerFactory.createWorker(m_providerGroup));
    }
}

void execq::impl::ExecutionPool::addProvider(ITaskProvider & provider)
{
    m_providerGroup.addProvider(provider);
}

void execq::impl::ExecutionPool::removeProvider(ITaskProvider & provider)
{
    m_providerGroup.removeProvider(provider);
}

bool execq::impl::ExecutionPool::notifyOneWorker()
{
    return details::NotifyWorkers(m_workers, true);
}

void execq::impl::ExecutionPool::notifyAllWorkers()
{
    details::NotifyWorkers(m_workers, false);
}

// Details

bool execq::impl::details::NotifyWorkers(const std::vector<std::unique_ptr<IThreadWorker>> & workers, const bool single)
{
    bool notified = false;
    for (const auto & worker : workers)
    {
        notified |= worker->notifyWorker();
        if (notified && single)
        {
            return true;
        }
    }

    return notified;
}
