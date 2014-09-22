#include "jce_logger.h"

/*
 * This Static method will help parsing our debug messages to a readable Log file
 *
 * timestamp - Message type - message
 *
 * Message types cam be:
 *
 *      - DEBUG
 *      - WARNING
 *      - CRITICAL
 *      - FATAL
 *
 * Logs stored in a log file. File name is Stored in the Macro in Header file
 */
void jce_logger::customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
   Q_UNUSED(context);

   QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
   QString txt = QString("%1\t").arg(dt);

   switch (type)
   {
      case QtDebugMsg:
         txt += QString("DEBUG \t\t %1").arg(msg);
         break;
      case QtWarningMsg:
         txt += QString("WARNING \t %1").arg(msg);
         break;
      case QtCriticalMsg:
         txt += QString("CRITICAL \t %1").arg(msg);
         break;
      case QtFatalMsg:
         txt += QString("FATAL \t\t %1").arg(msg);
         abort();
         break;
   }

   QFile outFile(LOG_FILE_NAME);
   outFile.open(QIODevice::WriteOnly | QIODevice::Append);

   QTextStream textStream(&outFile);
   textStream << txt << endl;
}
