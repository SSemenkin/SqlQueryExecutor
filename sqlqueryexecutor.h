#ifndef SQLQUERYEXECUTOR_H
#define SQLQUERYEXECUTOR_H

#include <QObject>
#include <QSqlDatabase>

#include "threadmanager.h"

class SQLQUERYEXECUTOR_EXPORT SqlQueryExecutor : public QObject
{
    Q_OBJECT
public:
    explicit SqlQueryExecutor(const QSqlDatabase &database, const QString &query);
    struct QueryResult
    {
        QVector<QString> headers;
        QVector<QVector<QString>> data;
    };
    ~SqlQueryExecutor();
signals:
    void finished(const SqlQueryExecutor::QueryResult&);
    void failed(const QString &description);
    void espired();
protected:
    virtual void execQuery();
    virtual QString getAvailableConnectionName() const;
protected:
    QString m_query;
    QString m_sourceConnectionName;
    QString m_connectionName;
};

#endif // SQLQUERYEXECUTOR_H
