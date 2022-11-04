#ifndef CMPFILE_H
#define CMPFILE_H

#include <QFile>
#include <QStringList>

#define CURRENT_DATA_VERSION    12

#define MAX_LINE_LENGTH    1024

#define COMMENT_SEPARATOR "#============================================================================#"

#include "dataConversion.h"

class cmpFile : public QFile
{
    Q_OBJECT
public:
    explicit cmpFile(QObject *parent = nullptr);
    cmpFile (const QString &name, QObject *parent = nullptr);
    ~cmpFile ();
    void setup ();

    bool parse ();
    bool readHeader ();
    bool readInfo ();
//    bool readData ();

    void writeHeader ();
    void writeInfo (QString notes = QString(""));
//    void writeData ();

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
    bool readFloatArray (QList<float> farray);
    bool readIntArray (int *iarray);  /**< read values to end or next token */
    bool readIntArray (QList<int> iarray);
    bool readString (QString &string); /**< read to end of current line */
    QStringList * splitString (QString &string); /**< divide string into component words/tokens */

    bool readFloatList (QList<float> &flist, int outSize, Data::DataConversion convert, unsigned mult, QString prompt);
    bool readIntList (QList<float> &ilist, int outSize, Data::DataConversion convert, unsigned mult, QString prompt);
//    bool input_float_array (PARSER_FILE *infile, int inSize, float *farray,
//                            int outSize, aconv conversion,
//                            unsigned mult, QString prompt);
//
//    void assign_fInArray_fOutArray (float *in_array, int in_size,
//                                    float *out_array, int out_size,
//                                    int mult, aconv conversion);

    void writeNewline ();
    void writeSpace ();
    void writeSeparator ();
    void writeIndent (int indent);
    void writeValue (int indent, QString keyword, float value, float *defaultValue = nullptr);
    void writeValue (int indent, QString keyword, int value, int *defaultValue = nullptr);
    void writeNumberedValue(int indent, QString keyword, int index, int value, int *defaultVal = nullptr);
    void writeNumberedValue(int indent, QString keyword, int index, float value, float *defaultVal = nullptr);
    void writeString (int indent, QString keyword, QString option1 = QString(""), QString option2 = QString (""));
    void writeStringNR (int indent, QString keyword, QString option1 = QString (""));
    void writeFloatOrNa (float val, Data::Type dtype = Data::Float);
    void writeFloat (double val, Data::Type dtype);
    void writeInt (int val);
    void writeFloatArray (int indent, float arry[], int size, Data::OutputConversion ctype,
                          Data::Type dtype, float *defaultval);
    void writeIntArray (int indent, int arry[], int size, Data::OutputConversion ctype,
                        int *defaultval);
    int convertInt (int val, Data::OutputConversion ctype);
    float convertFloat (float val, Data::OutputConversion ctype);
//    int output_float_array (QFile *outfile, const QString prefix, const QString name,
//                                     int size, float *array,
//                                     oconv conversion, unsigned mult,
//                                     enum FloatType output_type, float *default_value);

signals:
    void eof (bool); /**< signals end of file reached */

public slots:
    QString getToken (); /**< next space delimited string from file */
    QString popToken (); /**< check token for data and return it */
    void pushToken (QString token); /**< puts token back (to reprocesses) */

    void printEOF (QString data = QString (""));
    void printError (QString errmsg);
    void printMessage (QString msg);
    QString getFileLine ();
    void handle_obsolete_token (QString obs_token, QString new_token = QString());
    void handle_unknown_token (QString obs_token);

protected:
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

};


#endif // CMPFILE_H
