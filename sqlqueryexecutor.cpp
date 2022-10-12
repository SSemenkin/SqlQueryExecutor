#include "sqlqueryexecutor.h"
#include <QThread>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

SqlQueryExecutor::SqlQueryExecutor(const QSqlDatabase &database,
                             const QString &query) :
    m_query(query)
  , m_sourceConnectionName(database.connectionName())
{
    moveToThread(ThreadManager::instance()->create().get());
    connect(thread(), &QThread::started, this, &SqlQueryExecutor::execQuery);
    thread()->start();
}

SqlQueryExecutor::~SqlQueryExecutor()
{
    if (thread()->isRunning()) {
        thread()->quit();
        thread()->wait();
    }
    QSqlDatabase::removeDatabase(m_connectionName);
    bool result = ThreadManager::instance()->destroy(thread());
    Q_ASSERT(result && "Thread is not deleted.");
}

void SqlQueryExecutor::execQuery()
{
    m_connectionName = getAvailableConnectionName();
    QSqlDatabase database = QSqlDatabase::cloneDatabase(m_sourceConnectionName, m_connectionName);
    if (database.open()) {
        QSqlQuery query(database);
        if (query.exec(m_query)) {

            SqlQueryExecutor::QueryResult result;

            while(query.next()) {
                result.data.push_back({});
                result.data.last().reserve(query.record().count());

                if (result.headers.isEmpty()) {
                    for (int i = 0; i < query.record().count(); ++i) {
                        result.headers << query.value(i).toString();
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
    emit espired();
}

QString SqlQueryExecutor::getAvailableConnectionName() const
{
    QString s = m_sourceConnectionName;
    while(QSqlDatabase::contains(s)) {
        s += QString::number(rand() % 10);
    }
    return s;
}
