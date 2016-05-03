#ifndef C_COMPASSFILE_H
#define C_COMPASSFILE_H

#include <QFile>
#include <QStringList>

#define CURRENT_DATA_VERSION    9

#define MAX_LINE_LENGTH    1024

#define SEPARATOR QString("#============================================================================#")

class Data
{
public:
    typedef enum OutputOptions {
        OutputAll,
        OutputNonDefault,
        OutputNonZero
    }Output;

    typedef enum OutputDataConversionType {
        None,
        SumValues,
        AverageValues,
        DamDay,
        DamNight
    }Conversion;

    typedef enum OutputDataType {
        Integer,
        Float,
        Fixed,
        Precise,
        Scientific
    }Type;

};

class CompassFile : public QFile
{
    Q_OBJECT
public:
    explicit CompassFile(QObject *parent = 0);
    CompassFile (const QString &name, QObject *parent = 0);
    ~CompassFile ();
    void setup ();

    bool parse ();
    bool readHeader ();
    bool readInfo ();
    bool readData ();

    void writeHeader ();
    void writeInfo (QString notes = QString(""));
    void writeData ();

    QStringList *header;
    int data_version;
    QString *creator;
    QString *created_date;
    QString *modifier;
    QString *modified_date;
    QString *notes;
    int      line;        /**< Current line number of input */
    QStringList *tokens;  /**< Current line elements (tokens) */
    Data::Output outputoption;


    void skipLine ();  /**< skip rest of current line */
    void skipAllNumbers ();  /**< skip to next non-numeric token */
    void skipToEnd (); /**< skip to next 'end' statement */

    /** checks for type and optional name in line after 'end' statement */
    bool checkEnd (QString type, QString name = QString(""));
    bool isEOL () {return tokens->isEmpty();}     /**< is current token string empty? */
    bool isEOF (QString token);    /**< is it the end of file marker ('EOF')? */
    bool isNa (QString token);    /**< is it string 'na'? */
    bool isInt (QString token);   /**< can convert string to int value? */
    bool isFloat (QString token); /**< can convert string to float value? */
    bool readInt (int &val);    /**< read an integer value */
    bool readUnsigned (unsigned &val);    /**< read an unsigned integer value */
    bool readFloatOrNa (QString &na, float &val); /**< read float or 'na' */
    bool readFloatArray (float *farray); /**< read values to end or next token */
    bool readIntArray (int *iarray);  /**< read values to end or next token */
    bool readString (QString &string); /**< read to end of current line */

    void writeNewline ();
    void writeSpace ();
    void writeIndent (int indent);
    void writeString (int indent, QString keyword, QString option1, QString option2 = QString (""));
    void writeStringNR (int indent, QString keyword, QString option1 = QString (""));
    void writeFloatOrNa (float val, Data::Type dtype = Data::Integer);
    void writeFloat (double val, Data::Type dtype);
    void writeInt (int val);
    void writeFloatArray (int indent, float arry[], int size, Data::Conversion ctype,
                          Data::Type dtype, float *defaultval);
    void writeIntArray (int indent, int arry[], int size, Data::Conversion ctype, int *defaultval);
    int convertInt (int val, Data::Conversion ctype);
    float convertFloat (float val, Data::Conversion ctype);

signals:
    void eof (bool); /**< signals end of file reached */

public slots:
    QString getToken (); /**< next space delimited string from file */
    QString popToken (); /**< check token for data and return it */
    void pushToken (QString token); /**< puts token back (to reprocesses) */

    void printEOF (QString data = QString (""));
    void printError (QString errmsg);
    void printMessage (QString msg);
    void printFileLine ();
};

#endif // C_COMPASSFILE_H
