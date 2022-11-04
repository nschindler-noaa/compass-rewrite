#ifndef CMPLOG_H
#define CMPLOG_H

#include <QTemporaryFile>
#include <QString>
#include <QStringList>

/** A simple output log utility. This adds messages to a log file that may be saved
 *  and displays selected messages in the console. */

class cmpLog
{
public:
    cmpLog(QString filename = QString());
    ~cmpLog();

    void deleteLogFile ();

    enum MessageType {
        Error,          /**< Error message. Always logged */
        Warning,        /**< Warning message. */
        Message,        /**< General message. */
        RawDump,        /**< Raw data. */
        Debug,          /**< Debug message. */
        Fatal,          /**< Fatal error. Always logged. */
        Force           /**< Force message. Always logged */
    };

    /** Add a message to the log file */
    void add (MessageType id, QString msg);

    static cmpLog *outlog;

    static cmpLog *instance();

protected:
    void setup();
    void setLogFile (QString filename = QString ());
    void addToDisplay (QString text);
    void display (QString text);

    QTemporaryFile *logFile;
    QString logFileName;

    QString lastLine;
    int repCount;

    bool saved;

    bool message;
    bool warning;
    bool rawdump;
    bool debug;

    void destroy ();

    QStringList prefix;

public:
    void setMessage (bool);
    void setWarning (bool);
    void setRawDump (bool);
    void setDebug (bool);
    void clear ();

};


#endif // CMPLOG_H
