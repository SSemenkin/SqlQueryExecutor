#ifndef SQLQUERYEXECUTOR_H
#define SQLQUERYEXECUTOR_H
#include "SqlQueryExecutor_global.h"

#include <QObject>
#include <QSqlDatabase>

class SQLQUERYEXECUTOR_EXPORT SqlQueryExecutor final : public QObject
{
    Q_OBJECT
public:
    enum class  ExecutionPolicy
    {
        Sync,
        Async
    };

    explicit SqlQueryExecutor(const QSqlDatabase &database, const QString &query,
                              SqlQueryExecutor::ExecutionPolicy policy = SqlQueryExecutor::ExecutionPolicy::Async);
    virtual ~SqlQueryExecutor();
    struct QueryResult
    {
        QVector<QString> headers;
        QVector<QVector<QString>> data;

        bool operator ==(const QueryResult &other) const noexcept {
            return headers == other.headers && data == other.data;
        }

        bool operator !=(const QueryResult &other) const noexcept {
            return !operator==(other);
        }
    };

    [[nodiscard]] QString query() const;
    [[nodiscard]] QSqlDatabase database() const;

signals:
    void finished(const SqlQueryExecutor::QueryResult &result);
    void failed(const QString &description);
protected:
    void execQuery();
    [[nodiscard]] QueryResult prepareResult(QSqlQuery &query) const;

    QString getAvailableConnectionName() const;
protected:
    QString m_query;
    QString m_connectionName;
    ExecutionPolicy m_policy;
};

#endif // SQLQUERYEXECUTOR_H
