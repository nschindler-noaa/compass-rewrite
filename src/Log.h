#ifndef C_LOG_H
#define C_LOG_H

#include <QTemporaryFile>
#include <QString>
#include <QStringList>

/** A simple output log utility. This adds messages to a log file that may be saved
 *  and displays selected messages in the console. */
class Log
{
public:
    explicit Log (QObject *parent = nullptr);
    Log (QString filename = QString (""), QObject *parent = nullptr);
    ~Log();

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

    static Log *outlog;

    static Log *instance();

protected:
    void setup();
    void setLogFile (QString filename = QString (""));
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



#endif // C_LOG_H

