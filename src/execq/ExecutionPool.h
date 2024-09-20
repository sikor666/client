#pragma once

#include "execq/TaskProviderList.h"

#include <atomic>
#include <memory>
#include <vector>

namespace execq {
class IExecutionPool
{
public:
    virtual ~IExecutionPool() = default;

    virtual void addProvider(impl::ITaskProvider & provider) = 0;
    virtual void removeProvider(impl::ITaskProvider & provider) = 0;

    virtual bool notifyOneWorker() = 0;
    virtual void notifyAllWorkers() = 0;
};

namespace impl {
class ExecutionPool : public IExecutionPool
{
public:
    ExecutionPool(const uint32_t threadCount, const IThreadWorkerFactory & workerFactory);

    virtual void addProvider(ITaskProvider & provider) final;
    virtual void removeProvider(ITaskProvider & provider) final;

    virtual bool notifyOneWorker() final;
    virtual void notifyAllWorkers() final;

private:
    std::atomic_bool m_valid{true};
    TaskProviderList m_providerGroup;

    std::vector<std::unique_ptr<IThreadWorker>> m_workers;
};

namespace details {
bool NotifyWorkers(const std::vector<std::unique_ptr<IThreadWorker>> & workers, const bool single);
}
} // namespace impl
} // namespace execq
