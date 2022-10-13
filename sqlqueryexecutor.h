#ifndef SQLQUERYEXECUTOR_H
#define SQLQUERYEXECUTOR_H
#include "SqlQueryExecutor_global.h"

#include <QObject>
#include <QSqlDatabase>

class SQLQUERYEXECUTOR_EXPORT SqlQueryExecutor : public QObject
{
    Q_OBJECT
public:
    explicit SqlQueryExecutor(const QSqlDatabase &database, const QString &query);
    virtual ~SqlQueryExecutor();
    struct QueryResult
    {
        QVector<QString> headers;
        QVector<QVector<QString>> data;
    };


    [[nodiscard]] QString query() const;
    [[nodiscard]] QSqlDatabase database() const;

signals:
    void finished(const SqlQueryExecutor::QueryResult &result);
    void failed(const QString &description);
protected:
    virtual void execQuery();
    virtual QString getAvailableConnectionName() const;
protected:
    QString m_query;
    QString m_connectionName;
};

#endif // SQLQUERYEXECUTOR_H
