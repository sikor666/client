#include <algorithm>
#include "TaskProviderList.h"

execq::impl::Task execq::impl::TaskProviderList::nextTask()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    const size_t taskProvidersCount = m_taskProviders.size();
    const auto listEndIt = m_taskProviders.end();

    for (size_t i = 0; i < taskProvidersCount; i++)
    {
        if (m_currentTaskProviderIt == listEndIt)
        {
            m_currentTaskProviderIt = m_taskProviders.begin();
        }

        ITaskProvider * const provider = *(m_currentTaskProviderIt++);
        Task task = provider->nextTask();
        if (task.valid())
        {
            return task;
        }
    }

    return Task();
}

void execq::impl::TaskProviderList::addProvider(ITaskProvider & provider)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_taskProviders.push_back(&provider);
    m_currentTaskProviderIt = m_taskProviders.begin();
}

void execq::impl::TaskProviderList::removeProvider(ITaskProvider & provider)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    const auto it = std::find(m_taskProviders.begin(), m_taskProviders.end(), &provider);
    if (it != m_taskProviders.end())
    {
        m_taskProviders.erase(it);
        m_currentTaskProviderIt = m_taskProviders.begin();
    }
}
