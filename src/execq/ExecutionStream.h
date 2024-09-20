#pragma once

#include "execq/ExecutionPool.h"
#include "execq/IExecutionStream.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace execq {
namespace impl {
class ExecutionStream : public IExecutionStream, private ITaskProvider
{
public:
    ExecutionStream(std::shared_ptr<IExecutionPool> executionPool, const IThreadWorkerFactory & workerFactory,
        std::function<void(const std::atomic_bool & isCanceled)> executee);
    ~ExecutionStream();

public: // IExecutionStream
    virtual void start() final;
    virtual void stop() final;

private: // ITaskProvider
    virtual Task nextTask() final;

private:
    void waitPendingTasks();

private:
    std::atomic_bool m_stopped{true};

    std::atomic_size_t m_tasksRunningCount{0};
    std::mutex m_taskCompleteMutex;
    std::condition_variable m_taskCompleteCondition;

    const std::shared_ptr<IExecutionPool> m_executionPool;
    const std::function<void(const std::atomic_bool & shouldQuit)> m_executee;

    const std::unique_ptr<IThreadWorker> m_additionalWorker;
};
} // namespace impl
} // namespace execq
