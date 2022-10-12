#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <memory>

#include "SqlQueryExecutor_global.h"


class SQLQUERYEXECUTOR_EXPORT ThreadManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ThreadManager);

public:
    static ThreadManager* instance();

    std::shared_ptr<QThread> create();
    bool destroy(QThread *thread);

    int count() const;
signals:
    void threadCountChanged(int currentSize);
private:
    explicit ThreadManager();
private:
    static ThreadManager *s_instance;
    QVector<std::shared_ptr<QThread>> m_threads;
};

#endif // THREADMANAGER_H
