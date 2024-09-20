#include "ExecutionStream.h"

execq::impl::ExecutionStream::ExecutionStream(std::shared_ptr<IExecutionPool> executionPool,
    const IThreadWorkerFactory & workerFactory, std::function<void(const std::atomic_bool & isCanceled)> executee)
    : m_executionPool(executionPool)
    , m_executee(std::move(executee))
    , m_additionalWorker(workerFactory.createWorker(*this))
{
    m_executionPool->addProvider(*this);
}

execq::impl::ExecutionStream::~ExecutionStream()
{
    stop();
    waitPendingTasks();
    m_executionPool->removeProvider(*this);
}

// IExecutionStream

void execq::impl::ExecutionStream::start()
{
    m_stopped = false;
    m_executionPool->notifyAllWorkers();
    m_additionalWorker->notifyWorker();
}

void execq::impl::ExecutionStream::stop()
{
    m_stopped = true;
}

// IThreadWorkerPoolTaskProvider

execq::impl::Task execq::impl::ExecutionStream::nextTask()
{
    if (m_stopped)
    {
        return Task();
    }

    m_tasksRunningCount++;
    return Task([&] {
        m_executee(m_stopped);
        m_tasksRunningCount--;

        if (!m_tasksRunningCount)
        {
            m_taskCompleteCondition.notify_all();
        }
    });
}

// Private

void execq::impl::ExecutionStream::waitPendingTasks()
{
    std::unique_lock<std::mutex> lock(m_taskCompleteMutex);
    while (m_tasksRunningCount > 0)
    {
        m_taskCompleteCondition.wait(lock);
    }
}
