#ifndef C_LOG_H
#define C_LOG_H

#include <QTemporaryFile>
#include <QString>
#include <QStringList>

/** A simple log utility. This adds log messages to a log file that may be saved
 *  and displays selected messages in the console. */
class Log
{
public:
    explicit Log (QObject *parent = 0);
    Log (QString filename = QString (""), QObject *parent = 0);
    ~Log();

    static Log *instance (QObject *parent = 0);
    static Log *instance (QString filename, QObject *parent = 0);

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

    void add (MessageType id, QString msg);

//    static Log *log;

protected:
    void setup();
    void setLogFile (QString filename = QString (""));
    void addToDisplay (QString text);
    virtual void display (QString text);

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



#endif // C_LOG_H

