#ifndef C_LOG_H
#define C_LOG_H

#include <QTemporaryFile>
#include <QString>
#include <QStringList>

/** A simple outlog utility. This adds outlog messages to a outlog file that may be saved
 *  and displays selected messages in the console. */
class Log
{
public:
    explicit Log (QObject *parent = NULL);
    Log (QString filename = QString (""), QObject *parent = NULL);
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

