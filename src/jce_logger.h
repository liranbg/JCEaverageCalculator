#ifndef JCE_LOGGER_H
#define JCE_LOGGER_H

#include <QMessageLogContext>
#include <QString>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

#define LOG_FILE_NAME "J_M_Log.log"

class jce_logger
{
public:
    static void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

};

#endif // JCE_LOGGER_H
