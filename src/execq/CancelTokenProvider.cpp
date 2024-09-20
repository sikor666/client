#include "CancelTokenProvider.h"

execq::impl::CancelToken execq::impl::CancelTokenProvider::token()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_currentToken;
}

void execq::impl::CancelTokenProvider::cancel()
{
    cancelAndRenew(false);
}

void execq::impl::CancelTokenProvider::cancelAndRenew()
{
    cancelAndRenew(true);
}

void execq::impl::CancelTokenProvider::cancelAndRenew(const bool renew)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_currentToken)
    {
        *m_currentToken = true;
    }
    if (renew)
    {
        m_currentToken = std::make_shared<std::atomic_bool>(false);
    }
}
