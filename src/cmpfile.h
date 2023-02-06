#ifndef CMPFILE_H
#define CMPFILE_H

#include "cmpsettings.h"
#include "dataConversion.h"

#include <QFile>
#include <QStringList>

#define CURRENT_DATA_VERSION  15

#define MAX_LINE_LENGTH    1024

#define HEADER_BORDER "#============================================================================#"


class cmpFile : public QFile
{
    Q_OBJECT
public:
    explicit cmpFile(QObject *parent = nullptr);
    cmpFile (const QString &name, QObject *parent = nullptr);
    ~cmpFile () override;
    void setup ();

    bool parse ();
    bool readHeader ();
    bool readInfo ();
//    bool readData ();

//    void write (bool outputAll);
    void writeHeader (cmpSettings *sets, QString type);
    void writeInfo (QString notes = QString(""));
//    void writeData ();

    void skipLine ();  /**< skip rest of current line */
    void skipAllNumbers ();  /**< skip to next non-numeric token */
    void skipToEnd (); /**< skip to next 'end' statement */

    /** checks for type and optional name in line after 'end' statement */
    bool checkEnd (QString type, QString name = QString(""));
    bool isEOL () {return tokens->isEmpty();}     /**< is current token string empty? */
    bool isEOF (QString token);    /**< is it the end of file marker ('EOF')? */
    bool isNa (QString token);    /**< is the string 'na'? */
    bool isInt (QString token);   /**< can convert string to int value? */
    bool isFloat (QString token); /**< can convert string to float value? */
    bool readInt (int &val);      /**< read an integer value */
    bool readUnsigned (unsigned &val);    /**< read an unsigned integer value */
    bool readFloatOrNa (QString &na, float &val); /**< read float or 'na' */
    bool readTitledValue(QString &string, float &value);
    bool readFloatArray (float *farray, int size, Data::DataConversion convert, unsigned mult, QString prompt); /**< read values to end or next token */
    bool readFloatArray (QList<float> &farray, int outSize, Data::DataConversion convert, unsigned mult, QString prompt);
    bool readIntArray (int *iarray, int size, Data::DataConversion convert, unsigned mult, QString prompt);  /**< read values to end or next token */
    bool readIntArray (QList<int> &iarray, int outSize, Data::DataConversion convert, unsigned mult, QString prompt);
    bool readString (QString &string); /**< read to end of current line */
    QStringList * splitString (QString &string); /**< divide string into component words/tokens */

    bool readFloatList (QList<float> &flist, int outSize,
                        Data::DataConversion convert, unsigned mult, QString prompt);
    bool readIntList (QList<int> &ilist, int outSize,
                        Data::DataConversion convert, unsigned mult, QString prompt);
//    bool input_float_array (PARSER_FILE *infile, int inSize, float *farray,
//                            int outSize, aconv conversion,
//                            unsigned mult, QString prompt);
//
//    void assign_fInArray_fOutArray (float *in_array, int in_size,
//                                    float *out_array, int out_size,
//                                    int mult, aconv conversion);

    QString formatValue(float value, Data::Type dtype);
    QString formatValue(double value, Data::Type dtype);
    void writeNewline ();
    void writeSpace ();
    void writeBorder ();
    void writeIndent (int indent);
    void writeValue (int indent, QString keyword, double value, Data::Type dtype, double defaultValue = 100000);
    void writeValue (int indent, QString keyword, float value, Data::Type dtype, float defaultValue = 100000);
    void writeValue (int indent, QString keyword, int value, int defaultValue = 100000);
    void writeNumberedValue(int indent, QString keyword, int index, int value, int defaultVal = 100000);
    void writeNumberedValue(int indent, QString keyword, int index, float value, Data::Type dtype, float defaultVal = 100000);
    void writeTitledValue(int indent, QString keyword, QString title, int value, int defaultVal = 100000);
    void writeTitledValue(int indent, QString keyword, QString title, float value, Data::Type dtype, float defaultVal = 100000);
    void writeString (int indent, QString keyword, QString option1 = QString(), QString option2 = QString ());
    void writeStringNR (int indent, QString keyword, QString option1 = QString ());
    void writeFloatOrNa (float val, Data::Type dtype = Data::Float);
    void writeFloat (double val, Data::Type dtype = Data::Float);
    void writeInt (int val);
    void writeFloatArray (int indent, QString prefix, QString name, QList<float> &arry,
                          Data::DataConversion ctype, int mult,
                          Data::Type dtype, float defaultval);
    void writeFloatArray (int indent, QString prefix, QString name, float arry[], int size,
                          Data::DataConversion ctype, int mult,
                          Data::Type dtype, float defaultval);
    void writeIntArray (int indent, QString prefix, QString name, QList<int> &arry,
                        Data::DataConversion ctype, int mult, int defaultval);
    void writeIntArray (int indent, QString prefix, QString name, int arry[], int size,
                        Data::DataConversion ctype, int mult, int defaultval);
    void writeEnd (int indent, QString keyword, QString name = QString());
    int convertInt (QList<int> arry, int index, Data::DataConversion ctype, int mult);
    bool convertIntArray (int arry[], int size, Data::DataConversion ctype, int mult);
    float convertFloat (QList<float> arry, int index, Data::DataConversion ctype, int mult);
    bool convertFloatArray (float arry[], int size, Data::DataConversion ctype, int mult);
//    int output_float_array (QFile *outfile, const QString prefix, const QString name,
//                                     int size, float *array,
//                                     oconv conversion, unsigned mult,
//                                     enum FloatType output_type, float *default_value);

    int getDataVersion() const;
    void setDataVersion(int newDataVersion);

    void setSettings(cmpSettings *newSettings);

signals:
    void eof (bool);      /**< signals end of file reached */

public slots:
    QString getToken ();  /**< next space delimited string from file */
    QString popToken ();  /**< check token for data and return it */
    void pushToken (QString token); /**< puts token back (to reprocesses) */
//    void obsoleteToken (QString token, QString name); /**< token no longer used, message printed */
//    void unknownToken (QString token, QString name); /**< token not recognized, message printed */
    void obsoleteToken (QString obsToken, QString newToken = QString());
    void unknownToken (QString obsToken, QString segment = QString());
    void incorrectValue (int val, QString name = QString());
    void incorrectValue (float val, QString name = QString());

    void printEOF (QString data = QString (""));
    void printError (QString errmsg);
    void printMessage (QString msg);
    QString getFileLine ();

protected:
    cmpSettings *settings;
    // File information
    QStringList *header;  /**< File header lines */
    int      dataVersion; /**< Data version of this file */
    QString *creator;     /**< Originator of data file */
    QString *createdDate; /**< Date of creation (may be different from OS date) */
    QString *modifier;    /**< Last modifier of the data */
    QString *modifiedDate;/**< Date of last modification */
    QStringList *notes;   /**< Information about this data (one or more lines) */
    // Current line
    int      lineNum;     /**< Current line number of input */
    QStringList *tokens;  /**< Current line elements (tokens) */
    QString returnString;
};


#endif // CMPFILE_H
