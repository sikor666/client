#include "ThreadWorker.h"

namespace execq {
namespace impl {
class ThreadWorker : public IThreadWorker
{
public:
    explicit ThreadWorker(ITaskProvider & provider);
    virtual ~ThreadWorker();

    virtual bool notifyWorker() final;

private:
    void threadMain();
    void shutdown();

private:
    std::atomic_bool m_shouldQuit{false};
    std::atomic_bool m_checkNextTask{false};
    std::condition_variable m_condition;
    std::mutex m_mutex;
    std::unique_ptr<std::thread> m_thread;

    ITaskProvider & m_provider;
};
} // namespace impl
} // namespace execq

std::shared_ptr<const execq::impl::IThreadWorkerFactory> execq::impl::IThreadWorkerFactory::defaultFactory()
{
    class ThreadWorkerFactory : public IThreadWorkerFactory
    {
    public:
        virtual std::unique_ptr<IThreadWorker> createWorker(ITaskProvider & provider) const final
        {
            return std::unique_ptr<IThreadWorker>(new ThreadWorker(provider));
        }
    };

    static std::shared_ptr<IThreadWorkerFactory> s_factory = std::make_shared<ThreadWorkerFactory>();
    return s_factory;
}

execq::impl::ThreadWorker::ThreadWorker(ITaskProvider & provider)
    : m_provider(provider)
{
}

execq::impl::ThreadWorker::~ThreadWorker()
{
    shutdown();
    if (m_thread && m_thread->joinable())
    {
        m_thread->join();
    }
}

bool execq::impl::ThreadWorker::notifyWorker()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_checkNextTask)
    {
        return false;
    }

    m_checkNextTask = true;
    if (!m_thread)
    {
        m_thread.reset(new std::thread(&ThreadWorker::threadMain, this));
    }

    m_condition.notify_one();

    return true;
}

void execq::impl::ThreadWorker::shutdown()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_shouldQuit = true;
    m_condition.notify_one();
}

void execq::impl::ThreadWorker::threadMain()
{
    while (true)
    {
        if (m_shouldQuit)
        {
            break;
        }

        m_checkNextTask = false;
        Task task = m_provider.nextTask();
        if (task.valid())
        {
            task();
            continue;
        }

        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_checkNextTask)
        {
            continue;
        }

        if (m_shouldQuit)
        {
            break;
        }

        m_condition.wait(lock);
    }
}
