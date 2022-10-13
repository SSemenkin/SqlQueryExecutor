#include "sqlqueryexecutor.h"
#include <QThread>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

SqlQueryExecutor::SqlQueryExecutor(const QSqlDatabase &database,
                             const QString &query, SqlQueryExecutor::ExecutionPolicy policy) :
    m_query(query)
  , m_connectionName(database.connectionName())
  , m_policy(policy)
{
    switch(m_policy)
    {
        case SqlQueryExecutor::ExecutionPolicy::Async:
        {
            moveToThread(new QThread(this));
            connect(thread(), &QThread::started, this, &SqlQueryExecutor::execQuery);
            thread()->start();
            break;
        }
        case SqlQueryExecutor::ExecutionPolicy::Sync:
        {
            execQuery();
            break;
        }
    }
}

SqlQueryExecutor::~SqlQueryExecutor()
{
    if (m_policy == ExecutionPolicy::Async && thread()->isRunning()) {
        thread()->quit();
        thread()->wait();
    }
    QSqlDatabase::removeDatabase(m_connectionName);
}

QString SqlQueryExecutor::query() const
{
    return m_query;
}

QSqlDatabase SqlQueryExecutor::database() const
{
    return QSqlDatabase::database(m_connectionName);
}

void SqlQueryExecutor::execQuery()
{
    QSqlDatabase database = QSqlDatabase::cloneDatabase(m_connectionName,
                                                        m_connectionName = getAvailableConnectionName());

    if (database.open()) {
        QSqlQuery query(database);
        if (query.exec(m_query)) {
            SqlQueryExecutor::QueryResult result;

            while(query.next()) {
                result.data.push_back({});
                result.data.last().reserve(query.record().count());

                if (result.headers.isEmpty()) {
                    for (int i = 0; i < query.record().count(); ++i) {
                        result.headers << query.record().fieldName(i);
                    }
                }

                for (int i = 0; i < query.record().count(); ++i) {
                    result.data.last().push_back(query.value(i).toString());
                }
            }
            emit finished(result);
        } else {
            emit failed(query.lastError().text());
        }
    } else {
        emit failed(database.lastError().text());
    }
    thread()->quit();
}

QString SqlQueryExecutor::getAvailableConnectionName() const
{
    QString s = m_connectionName;
    while(QSqlDatabase::contains(s)) {
        s += QString::number(rand() % 10);
    }
    return s;
}
