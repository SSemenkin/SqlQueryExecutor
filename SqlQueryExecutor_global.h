#ifndef SQLQUERYEXECUTOR_GLOBAL_H
#define SQLQUERYEXECUTOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SQLQUERYEXECUTOR_LIBRARY)
#  define SQLQUERYEXECUTOR_EXPORT Q_DECL_EXPORT
#else
#  define SQLQUERYEXECUTOR_EXPORT Q_DECL_IMPORT
#endif

#endif // SQLQUERYEXECUTOR_GLOBAL_H
