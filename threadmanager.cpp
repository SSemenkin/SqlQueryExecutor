#include "threadmanager.h"
#include <QThread>
#include <QSharedPointer>

ThreadManager* ThreadManager::s_instance {nullptr};

ThreadManager *ThreadManager::instance()
{
    static ThreadManager a;
    return &a;
}

std::shared_ptr<QThread> ThreadManager::create()
{
    std::shared_ptr<QThread> thread = std::make_shared<QThread>();
    m_threads.push_back(thread);
    emit threadCountChanged(m_threads.count());
    return thread;
}

bool ThreadManager::destroy(QThread *thread)
{
    for (int i = 0; i < m_threads.size(); ++i) {
        if (thread == m_threads.at(i).get()) {
            m_threads.removeAt(i);
            emit threadCountChanged(m_threads.count());
            return true;
        }
    }
    return false;
}

ThreadManager::ThreadManager()
{
    Q_ASSERT(s_instance == nullptr && "ThreadManager already exists.");
    s_instance = this;
}
