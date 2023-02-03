#include "cmpfile.h"
#include "cmplog.h"
#include "cmpmath.h"
//#include "definitions.h"

#include <cstdio>
#include <iostream>

#include <QDateTime>

using namespace std;

cmpFile::cmpFile(QObject *parent) :
    QFile(parent)
{
    setup ();
}

cmpFile::cmpFile (const QString &name, QObject *parent) :
    QFile (name, parent)
{
    setup ();
}

void cmpFile::setup ()
{
    header = new QStringList ();
    dataVersion = CURRENT_DATA_VERSION;
    creator = new QString();
    createdDate = new QString();
    modifier = new QString();
    modifiedDate = new QString();
    notes = new QStringList();
    lineNum = 0;
    tokens = new QStringList ();
//    outputoption = Data::OutputAll;
}

cmpFile::~cmpFile ()
{
    delete header;
    delete creator;
    delete createdDate;
    delete modifier;
    delete modifiedDate;
    delete notes;
    delete tokens;
//    ~QFile();
}

bool cmpFile::parse ()
{
    bool okay = this->open (QIODevice::ReadOnly);

    if (okay) okay = readHeader ();
    if (okay) okay = readInfo ();
//    if (okay) okay = readData ();

    close ();
    return okay;
}

bool cmpFile::readHeader ()
{
    bool okay = true, end = false;
    QString token (""), val("");
    seek(0);

    while (okay && !end)
    {
        token = popToken ();
        if (token.contains ("EOF", Qt::CaseInsensitive))
        {
            printEOF ("Header information");
            okay = false;
        }
        else if (token.contains ("#====="))
        {
            skipLine ();
        }
        else if (token.startsWith ("#"))
        {
            val = popToken();
            if (!isFloat(val))
                val.prepend("   ");
            val.prepend("# ");
            pushToken (val);
            okay = readString (token);
            header->append (token);
        }
        else
        {
            pushToken (token);
            end = true;
        }
    }

    return okay;
}

bool cmpFile::readInfo ()
{
    bool okay = true, end = false;
    while (okay && !end)
    {
        QString token = popToken ();
        if (token.contains ("EOF"))
        {
            printEOF ("File information");
            okay = false;
            end = true;
        }
        else if (token.contains ("version"))
        {
            okay = readInt (dataVersion);
        }
        else if (token.contains ("file_creation_date"))
        {
            okay = readString (token);
            if (okay)
            {
                delete createdDate;
                createdDate = new QString (token);
            }
        }
        else if (token.contains ("file_creator"))
        {
            okay = readString (token);
            if (okay)
            {
                delete creator;
                creator = new QString (token);
            }
        }
        else if (token.contains ("file_modification_date"))
        {
            okay = readString (token);
            if (okay)
            {
                delete modifiedDate;
                modifiedDate = new QString(token);
            }
        }
        else if (token.contains ("file_modifier"))
        {
            okay = readString (token);
            if (okay)
            {
                delete modifier;
                modifier = new QString (token);
            }
        }
        else if (token.contains ("note"))
        {
            okay = readString (token);
            if (okay)
            {
                delete notes;
                notes->append(QString (token));
            }
            end = true;
        }
        else
        {
            pushToken (token);
            end = true;
        }
    }
    if (dataVersion < 13)
        cmpLog::outlog->add (cmpLog::Debug, QString ("Old data version %1").arg(dataVersion));
    return okay;
}

//bool CompassFile::readData ()
//{
//    bool okay = true;
//    return okay;
//}

//void CompassFile::writeData ()
//{
//    if (open (QIODevice::WriteOnly))
//    {
//        writeHeader ();
//        writeInfo ();
//        if (fileName ().endsWith ("rls", Qt::CaseInsensitive))
//        {
//            writeReleaseData (this);
//        }
//
//    }
//    else
//    {
//        cmpLog::outlog->add (cmpLog::Error, QString("Could not write to file: %1!")
//                               .arg (fileName ()));
//    }
//}

void cmpFile::writeHeader (cmpSettings *sets, QString type)
{
    QString sep (HEADER_BORDER);
    QString os, user, date, line, space;
    int spaces;

    if (isOpen())
    {
        writeString (0, sep);
        if (header->isEmpty())
        {
            os = QString("COMPASS ver %1 #").arg(sets->getAppVersion());
            line = QString (QString("# COMPASS %1 File").arg(type));
            spaces = line.count() + os.count();
            space.fill(' ', 78-spaces);
            line.append(space);
            line.append(os);
            writeString(0, line);
            space.clear();
            space.fill(' ', 76);
            line = QString("#%1#").arg(space);
            writeString(0, line);
            user = sets->getUserSettings()->getUserName();
            if (user.isEmpty())
                user = QString("# Written on ");
            else
                user = QString("# Written by %1 on ").arg(user);
            date = QDate::currentDate().toString("dd MMM yyyy");
            spaces = user.count() + date.count();
            space.clear();
            space.fill(' ', 77-spaces);
            line = QString("%1%2%3#").arg(user, date, space);
            writeString(0, line);
        }
        else
        {
            for (int i = 0, total = header->count(); i < total; i++)
            {
                writeString (0, header->at (i));
            }
        }
        writeString (0, sep);
    }
}

void cmpFile::writeInfo (QString newnotes)
{
    QString version (QString::number(dataVersion));
    open (QIODevice::WriteOnly);
    writeString (0, "version", version);
    if (!creator->isEmpty())
        writeString (0, "creator", *creator);
    if (!createdDate->isEmpty())
        writeString (0, "createdDate", *createdDate);
    if (!modifier->isEmpty())
        writeString (0, "modifier", *modifier);
    if (!modifiedDate->isEmpty())
        writeString (0, "modifiedDate", *modifiedDate);
    if (!notes->isEmpty())
    {
        QStringList notetokens(newnotes.split('\n'));
        for (int i = 0, total = notetokens.count(); i < total; i++)
            writeString (0, "notes", notetokens.at(i));
        for (int i = 0, total = notes->count(); i < total; i++)
            writeString (0, "notes", notes->at(i));
    }
}

//bool CompassFile::read
bool cmpFile::readString (QString &string)
{
    bool okay = true;
    string.clear ();
    if (tokens->count () > 0)
    {
        QString token (tokens->takeFirst());
        token.remove('\t'); token.remove(' ');
        string.append (token);
        while (tokens->count() > 0)
        {
            token = QString(tokens->takeFirst());

            if (token.length() > 0)
            {
                string.append (' ');
                string.append (token);
            }
        }
        string = string.simplified();
    }
    else
    {
        okay = false;
    }
    return okay;
}

QStringList *cmpFile::splitString(QString &string)
{
    QString newstring(string.replace('\t',' '));
    tokens->clear();
    tokens->append(newstring.split(' ', QString::SkipEmptyParts));
    return tokens;
}

QString cmpFile::getToken ()
{
    while (tokens->isEmpty ())
    {
        QString rline ("");
        while (rline.length () < 2)
        {
            rline.clear ();
            if (this->atEnd ())
            {
                rline.append ("EOF");
            }
            else
            {
                rline.append (this->readLine (MAX_LINE_LENGTH));
                rline.remove('\n');
                rline.remove('\r');
            }
            lineNum++;
        }
        splitString(rline);
//        readString (rline);
//        delete tokens;
//        tokens = new QStringList (rline.split(' ', QString::SkipEmptyParts));
    }

    return tokens->takeFirst ();
}

QString cmpFile::popToken ()
{
    QString token ("");
    while (token.isEmpty())
    {
        token.append (getToken ());
        if (token.compare(" ") == 0)
        {
            token.clear();
        }
        else if (token.compare("\t") == 0)
        {
            token.clear();
        }
    /*    if (token.length () < 2)
        {
            QRegExp rx ("[a-z] | [A-Z] | [d] | [#]");
            token.contains (rx);
            if (rx.isEmpty())
                token = getToken ();
        }*/
    }
#ifdef DEBUG_INPUT
    qDebug("size of token %s is %u", token.toUtf8().data(), static_cast<unsigned>(token.length()));
#endif
    return token;
}

void cmpFile::pushToken (QString token)
{
    tokens->prepend (" ");
    tokens->prepend (token);
}

void cmpFile::skipLine ()
{
    tokens->clear ();
}

void cmpFile::skipAllNumbers()
{
    bool end = false;
    QString token;
    skipLine ();
    while (!end)
    {
        token = popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            printEOF("skipping numbers");
        }
        else if (isFloat (token))
        {
            skipLine ();
        }
        else
        {
            pushToken (token);
            end = true;
        }
    }
}

void cmpFile::skipToEnd ()
{
    QString token;
    tokens->clear ();
    token = popToken ();
    while (!token.contains ("eof", Qt::CaseInsensitive))
    {
        if (token.contains ("end", Qt::CaseInsensitive))
        {
            pushToken (token);
            break;
        }
        skipLine ();
    }
}

bool cmpFile::checkEnd (QString type, QString name)
{
    QString line, namestr;
    bool okay = readString(line);
    QStringList tokens = line.split('(', QString::SkipEmptyParts);
    if (okay && tokens.count() > 1)
    {
        if (!tokens.takeFirst().contains(type))
        {
            QString msg (QString("{end} statement does not include type '%1'").arg (type));
            printMessage (msg);
        }
    }
    tokens = tokens[0].split(')', QString::SkipEmptyParts);
    if (!tokens.isEmpty())
    {
        namestr = tokens[0].simplified().replace(' ', '_');
        if(!name.isEmpty() && !name.contains(namestr))
        {
            QString msg (QString("{end} statement name '%1' does not match expected '%2'")
                         .arg (namestr, name));
            printMessage (msg);
        }
    }
    return okay;
}

bool cmpFile::isNa (QString token)
{
    return (token.compare ("na", Qt::CaseInsensitive) == 0);
}

bool cmpFile::isEOF (QString token)
{
    return (token.compare ("eof", Qt::CaseInsensitive) == 0);
}

bool cmpFile::isInt (QString token)
{
    bool okay = true;
    token.toInt (&okay);
    return okay;
}

bool cmpFile::isFloat (QString token)
{
    bool okay = true;
    token.toFloat (&okay);
    return okay;
}

bool cmpFile::readFloatOrNa (QString &na, float &val)
{
    bool okay = true;
    na.clear ();
    val = -1.0;
    na.append (popToken ());
    if (isEOF (na))
        okay = false;
    else if (isNa (na))
        okay = false;
    else
        val = na.toFloat (&okay);

    return okay;
}

bool cmpFile::readTitledValue(QString &string, float &value)
{
    bool okay = true;
    QString na = QString();
    string.clear();
    value = -1.0;
    string.append(popToken());
    okay = readFloatOrNa(na, value);
    return okay;
}

bool cmpFile::readInt (int &val)
{
    bool okay = true;
    QString token = popToken ();
    val = -1;

    if (isEOF (token))
    {
        okay = false;
        printEOF ("integer value");
    }
    else
    {
        val = token.toInt (&okay);
    }

    return okay;
}
bool cmpFile::readUnsigned (unsigned &val)
{
    bool okay = true;
    QString token = popToken ();
    val = -1;

    if (isEOF (token))
    {
        okay = false;
        printEOF ("integer value");
    }
    else
    {
        val = token.toUInt(&okay);
    }

    return okay;
}

bool cmpFile::readFloatArray (float *farray, int size, Data::DataConversion convert, unsigned mult, QString prompt)
{
    bool okay = true, end = false, error = false;
    QString token;
    QStringList tokens;
    int cur_elem = 0;
    float value = 0;
    int start_elem = 0, end_elem = 0;
    short flag = 1;

    // initialize array
    for (int i = 0; i < size * mult; i++)
        farray[i] = 0;

    while (!end && okay)
    {
        token = popToken ();

        if (token.contains ('['))
        {
            token = token.section ('[', -1);
            token = token.section (']', 0, 0);
            tokens = token.split(':');
            if (token.count() == 2)
            {
                start_elem = tokens.at(0).toInt(&okay);
                if (okay)
                    end_elem = tokens.at(1).toInt(&okay);
                else
                    end_elem = start_elem;
            }
            else if (token.contains ('*'))
            {
                start_elem = 0;
                end_elem = size - 1;
            }
            else
            {
                start_elem = token.toInt(&okay);
                end_elem = start_elem;
            }
            if (okay)
            {
                token = popToken ();
                if (isInt (token))
                {
                    value = token.toFloat(&okay);
                    for (cur_elem = start_elem; cur_elem <= end_elem; cur_elem++)
                    {
                        farray[cur_elem] = value; //convertFloat(farray, cur_elem, convert, mult);
//						assign_int_array (iarray, cur_elem, number, convert, mult);
                    }
                }
                else
                {
                    pushToken (token);
                }
            }
            else
            {
                printError ("parsing integer array");
            }
            if (cur_elem >= size)
                end = true;
        }
        else
        {
            if (isInt (token))
            {
                value = token.toFloat(&okay);
                farray[cur_elem] = value;
            }
            else
            {
                pushToken (token);
                end = true;
            }
        }
    }
    return end;
}

bool cmpFile::readFloatArray(QList<float> &farray, int size, Data::DataConversion convert, unsigned mult, QString prompt)
{
    bool okay = true, end = false, error = false;
    QString token;
    QStringList tokens;
    int cur_elem = 0;
    float value = 0;
    int start_elem = 0, end_elem = 0;
    short flag = 1;

    // initialize array
    int count = farray.count();
    for (int i = 0; i < count; i++)
        farray[i] = 0;
    while (count < size)
    {
        farray.append(0);
        count++;
    }
    while (count > size)
    {
        farray.takeLast();
        count--;
    }

    while (!end && okay)
    {
        token = popToken ();

        if (token.contains ('['))
        {
            token = token.section ('[', -1);
            token = token.section (']', 0, 0);
            tokens = token.split(':');
            if (token.count() == 2)
            {
                start_elem = tokens.at(0).toInt(&okay);
                if (okay)
                    end_elem = tokens.at(1).toInt(&okay);
                else
                    end_elem = start_elem;
            }
            else if (token.contains ('*'))
            {
                start_elem = 0;
                end_elem = size - 1;
            }
            else
            {
                start_elem = token.toInt(&okay);
                end_elem = start_elem;
            }
            if (okay)
            {
                token = popToken ();
                if (isFloat (token))
                {
                    value = token.toFloat(&okay);
                    for (cur_elem = start_elem; cur_elem <= end_elem; cur_elem++)
                    {
                        farray[cur_elem] = convertFloat(farray, cur_elem, convert, mult);
//						assign_int_array (iarray, cur_elem, number, convert, mult);
                    }
                }
                else
                {
                    pushToken (token);
                }
            }
            else
            {
                printError ("parsing integer array");
            }
            if (cur_elem >= size)
                end = true;
        }
        else
        {
            if (isFloat (token))
            {
                value = token.toFloat(&okay);
                farray[cur_elem] = value;
            }
            else
            {
                pushToken (token);
                end = true;
            }
        }
    }
    return end;
}

bool cmpFile::readIntArray (int *iarray, int size, Data::DataConversion convert, unsigned mult, QString prompt)
{
    bool okay = true, end = false, error = false;
    QString token;
    QStringList tokens;
    int cur_elem = 0;
    int number = 0;
    int start_elem = 0, end_elem = 0;
    short flag = 1;

    // initialize array
    for (int i = 0; i < size * mult; i++)
        iarray[i] = 0;

    while (!end && okay)
    {
        token = popToken ();

        if (token.contains ('['))
        {
            token = token.section ('[', -1);
            token = token.section (']', 0, 0);
            tokens = token.split(':');
            if (token.count() == 2)
            {
                start_elem = tokens.at(0).toInt(&okay);
                if (okay)
                    end_elem = tokens.at(1).toInt(&okay);
                else
                    end_elem = start_elem;
            }
            else if (token.contains ('*'))
            {
                start_elem = 0;
                end_elem = size - 1;
            }
            else
            {
                start_elem = token.toInt(&okay);
                end_elem = start_elem;
            }
            if (okay)
            {
                token = popToken ();
                if (isInt (token))
                {
                    number = token.toInt(&okay);
                    for (cur_elem = start_elem; cur_elem <= end_elem; cur_elem++)
                    {
                        iarray[cur_elem] = number; //convertInt(iarray, cur_elem, convert, mult);
//						assign_int_array (iarray, cur_elem, number, convert, mult);
                    }
                }
                else
                {
                    pushToken (token);
                }
            }
            else
            {
                printError ("parsing integer array");
            }
            if (cur_elem >= size)
                end = true;
        }
        else
        {
            if (isInt (token))
            {
                number = token.toInt(&okay);
                iarray[cur_elem] = number;
            }
            else
            {
                pushToken (token);
                end = true;
            }
        }
    }

    return end;
}

bool cmpFile::readIntArray(QList<int> &iarray, int size, Data::DataConversion convert, unsigned mult, QString prompt)
{
    bool okay = true, end = false, error = false;
    QString token;
    QStringList tokens;
    int cur_elem = 0;
    int number = 0;
    int start_elem = 0, end_elem = 0;
    short flag = 1;

    // initialize array
    for (int i = 0; i < size * mult; i++)
        iarray[i] = 0;

    while (!end && okay)
    {
        token = popToken ();

        if (token.contains ('['))
        {
            token = token.section ('[', -1);
            token = token.section (']', 0, 0);
            tokens = token.split(':');
            if (token.count() == 2)
            {
                start_elem = tokens.at(0).toInt(&okay);
                if (okay)
                    end_elem = tokens.at(1).toInt(&okay);
                else
                    end_elem = start_elem;
            }
            else if (token.contains ('*'))
            {
                start_elem = 0;
                end_elem = size - 1;
            }
            else
            {
                start_elem = token.toInt(&okay);
                end_elem = start_elem;
            }
            if (okay)
            {
                token = popToken ();
                if (isInt (token))
                {
                    number = token.toInt(&okay);
                    for (cur_elem = start_elem; cur_elem <= end_elem; cur_elem++)
                    {
                        iarray[cur_elem] = convertInt(iarray, cur_elem, convert, mult);
//						assign_int_array (iarray, cur_elem, number, convert, mult);
                    }
                }
                else
                {
                    pushToken (token);
                }
            }
            else
            {
                printError ("parsing integer array");
            }
            if (cur_elem >= size)
                end = true;
        }
        else
        {
            if (isInt (token))
            {
                number = token.toInt(&okay);
                iarray[cur_elem] = number;
            }
            else
            {
                pushToken (token);
                end = true;
            }
        }
    }
    return end;
}

bool cmpFile::readIntList(QList<int> &intlist, int size, Data::DataConversion convert, unsigned mult, QString prompt)
{
    bool okay = true;
    QString token;
//    QString NA("");
    int value = 0;
    int total = intlist.count();

    for (int i = 0; i < total; i++)
        intlist[i] = 0;

    for (int i = 0; i < total; i++)
    {
        if (!okay)
        {
            qWarning("%s %s", "error reading int array for", prompt.toUtf8().data());
            break;
        }
        token = popToken();
        okay = readInt(value);
        intlist[i] = value;
    }
    return okay;
}

bool cmpFile::readFloatList(QList<float> &fltlist, int inSize, Data::DataConversion convert, unsigned mult, QString prompt)
{
    bool okay = true;
    QString NA ("");

    for (int i = 0, total = fltlist.count(); i < total; i++)
    {
        if (!okay)
        {
            printError(QString("error reading float array."));
            break;
        }
        NA.clear();
        okay = readFloatOrNa(NA, fltlist[i]);
    }

    return okay;
}

void cmpFile::writeNewline ()
{
    write ("\n", 1);
}

void cmpFile::writeSpace ()
{
    write (" ", 1);
}

void cmpFile::writeBorder()
{
    write(HEADER_BORDER, 78);
}

void cmpFile::writeIndent (int indent)
{
    for (int i = 0; i < indent; i++)
        write ("  ", 2);
}

void cmpFile::writeValue(int indent, QString keyword, double value, double defaultValue)
{
    if (floatIsNotEqual(value, defaultValue))
    {
        QString valueString(QString::number(value));
        writeString(indent, keyword, valueString);
    }
}

void cmpFile::writeValue(int indent, QString keyword, float value, float defaultValue)
{
    if (floatIsNotEqual(value, defaultValue))
    {
        QString valueString(QString::number(value, 'f', 2));
        writeString(indent, keyword, valueString);
    }
}

void cmpFile::writeValue(int indent, QString keyword, int value, int defaultValue)
{
    if (value != defaultValue)
    {
        QString valueString(QString::number(value));
        writeString(indent, keyword, valueString);
    }
}

void cmpFile::writeNumberedValue(int indent, QString keyword, int index, int value, int defaultVal)
{
    if (value != defaultVal)
    {
        QString num(QString::number(index));
        QString val(QString::number(value));
        writeString(indent, keyword, num, val);
    }
}

void cmpFile::writeNumberedValue(int indent, QString keyword, int index, float value, float defaultVal)
{
    if (floatIsNotEqual(value, defaultVal))
    {
        QString num(QString::number(index));
        QString val(QString::number(value));
        writeString(indent, keyword, num, val);
    }
}

void cmpFile::writeTitledValue(int indent, QString keyword, QString title, int value, int defaultVal)
{
    if (value != defaultVal)
    {
        QString val(QString::number(value));
        writeString(indent, keyword, title, val);
    }
}

void cmpFile::writeTitledValue(int indent, QString keyword, QString title, float value, float defaultVal)
{
    if (floatIsNotEqual(value, defaultVal))
    {
        QString val(QString::number(value));
        writeString(indent, keyword, title, val);
    }
}

void cmpFile::writeString (int indent, QString keyword, QString option1, QString option2)
{
    writeIndent(indent);
    write (keyword.toUtf8());
    if (!option1.isEmpty())
    {
        writeSpace();
        write (option1.toUtf8());

        if (!option2.isEmpty())
        {
            writeSpace();
            write (option2.toUtf8());
        }
    }
    writeNewline();
}

void cmpFile::writeStringNR (int indent, QString keyword, QString option1)
{
    writeIndent(indent);
    write (keyword.toUtf8());
    if (!option1.isEmpty())
    {
        writeSpace();
        write (option1.toUtf8());
    }
}

void cmpFile::writeFloatOrNa (float val, Data::Type dtype)
{
    if (val == -1)
        write (QString("na").toUtf8());
    else
        writeFloat (val, dtype);
}

void cmpFile::writeFloat (double val, Data::Type dtype)
{
    switch (dtype)
    {
    case Data::Integer:
        write (QString::number(val, 'f', 0).toUtf8());
//        writeInt(static_cast<int>(val+.5));
        break;

    case Data::Float:
        write (QString::number(val, 'f', 2).toUtf8());
        break;

    case Data::Fixed:
        write (QString::number(val, 'f', 6).toUtf8());
        break;

    case Data::Scientific:
        write (QString::number(val, 'f', 6).toUtf8());
        break;

    case Data::Precise:
        write (QString::number(val, 'f', 12).toUtf8());
        break;
    }
}

void cmpFile::writeInt (int val)
{
    write (QString::number(val).toUtf8());
}

int cmpFile::convertInt(QList<int> arry, int index, Data::DataConversion ctype, int mult)
{
    int retval = 0;
    int end = 0;
    int total = 0;
    int count = 0;

    switch (ctype)
    {
    case Data::Sum:
        end = index + mult;
        for (int i = index; i < end; i++)
            retval += arry[i];
        break;
    case Data::Average:
        end = index + mult;
        total = 0;
        count = 0;
        for (int i = index; i < end; i++)
        {
            total += arry[i];
            count ++;
        }
        retval = total / count;
        break;
    case Data::AverageNonZero:
        end = index + mult;
        total = 0;
        count = 0;
        for (int i = index; i < end; i++)
        {
            if (arry[i] != 0)
            {
                total += arry[i];
                count ++;
            }
        }
        retval = total / count;
        break;
    case Data::AveragePositive:
        end = index + mult;
        total = 0;
        count = 0;
        for (int i = index; i < end; i++)
        {
            if (arry[i] > 0)
            {
                total += arry[i];
                count ++;
            }
        }
        retval = total / count;
        break;
    case Data::DamDay:
        end = index + mult;
        total = 0;
        count = 0;
        for (int i = index; i < end; i++)
        {
            if (settings->getDataSettings()->isDay(i))
            {
                total += arry[i];
                count ++;
            }
        }
        retval = total / count;
        break;
    case Data::DamNight:
        end = index + mult;
        total = 0;
        count = 0;
        for (int i = index; i < end; i++)
        {
            if (settings->getDataSettings()->isNight(i))
            {
                total += arry[i];
                count ++;
            }
        }
        retval = total / count;
        break;
    case Data::None:
//    default:
        retval = arry[index];
        break;
   }

    return retval;
}

bool cmpFile::convertIntArray(int arry[], int size, Data::DataConversion ctype, int mult)
{
    bool okay = true;
    int value = 0;
    int total = 0;
    int count = 0;

    switch (ctype)
    {
    case Data::Sum:
        for (int i = 0; i < size; i += mult)
        {
            value = 0;
            for (int j = 0; j < mult; j++)
                value += arry[j];
            for (int j = 0; j < mult; j++)
                arry[j] = value;
        }
        break;
    case Data::Average:
        for (int i = 0; i < size; i += mult)
        {
            total = 0;
            count = 0;
            for (int j = 0; j < mult; j++)
            {
                total += arry[j];
                count ++;
            }
            value = total / count;
            for (int j = 0; j < mult; j++)
                arry[j] = value;
        }
        break;
    case Data::AverageNonZero:
        for (int i = 0; i < size; i += mult)
        {
            total = 0;
            count = 0;
            for (int j = 0; j < mult; j++)
            {
                if (arry[i] != 0)
                {
                    total += arry[i];
                    count ++;
                }
            }
            value = total / count;
            for (int j = 0; j < mult; j++)
                arry[j] = value;
        }
        break;
    case Data::AveragePositive:
        for (int i = 0; i < size; i += mult)
        {
            total = 0;
            count = 0;
            for (int j = 0; j < mult; j++)
            {
                if (arry[i] > 0)
                {
                    total += arry[i];
                    count ++;
                }
            }
            value = total / count;
            for (int j = 0; j < mult; j++)
                arry[j] = value;
        }
        break;
    case Data::DamDay:
        for (int i = 0; i < size; i += mult)
        {
            total = 0;
            count = 0;
            for (int j = 0; j < mult; j++)
            {
                if (settings->getDataSettings()->isDay(i))
                {
                    total += arry[i];
                    count ++;
                }
            }
            value = total / count;
            for (int j = 0; j < mult; j++)
                arry[j] = value;
        }
        break;
    case Data::DamNight:
        for (int i = 0; i < size; i += mult)
        {
            total = 0;
            count = 0;
            for (int j = 0; j < mult; j++)
            {
                if (settings->getDataSettings()->isNight(i))
                {
                    total += arry[i];
                    count ++;
                }
            }
            value = total / count;
            for (int j = 0; j < mult; j++)
                arry[j] = value;
        }
        break;
    case Data::None:
        break;
    default:
        okay = false;
        break;
   }

    return okay;
}

void cmpFile::writeEnd(int indent, QString keyword, QString name)
{
    writeIndent(indent);
    write ("end ");
    write (keyword.toUtf8());
    if (!name.isEmpty())
    {
        write (" (");
        write (name.toUtf8());
        write (")");
    }
    else
    {
        write(" ()");
    }
    writeNewline();
}

float cmpFile::convertFloat(QList<float> arry, int index, Data::DataConversion ctype, int mult)
{
    float retval = 0;
    int end = 0;
    float total = 0;
    int count = 0;

    switch (ctype)
    {
    case Data::Sum:
        end = index + mult;
        for (int i = index; i < end; i++)
            retval += arry[i];
        break;
    case Data::Average:
        end = index + mult;
        total = 0;
        count = 0;
        for (int i = index; i < end; i++)
        {
            total += arry[i];
            count ++;
        }
        retval = total / count;
        break;
    case Data::AverageNonZero:
        end = index + mult;
        total = 0;
        count = 0;
        for (int i = index; i < end; i++)
        {
            if (floatIsNotEqual(arry[i], 0))
            {
                total += arry[i];
                count ++;
            }
        }
        retval = total / count;
        break;
    case Data::AveragePositive:
        end = index + mult;
        total = 0;
        count = 0;
        for (int i = index; i < end; i++)
        {
            if (arry[i] > 0)
            {
                total += arry[i];
                count ++;
            }
        }
        retval = total / count;
        break;
    case Data::DamDay:
        end = index + mult;
        total = 0;
        count = 0;
        for (int i = index; i < end; i++)
        {
            if (settings->getDataSettings()->isDay(i))
            {
                total += arry[i];
                count ++;
            }
        }
        retval = total / count;
        break;
    case Data::DamNight:
        end = index + mult;
        total = 0;
        count = 0;
        for (int i = index; i < end; i++)
        {
            if (settings->getDataSettings()->isNight(i))
            {
                total += arry[i];
                count ++;
            }
        }
        retval = total / count;
        break;
    case Data::None:
//    default:
        retval = arry[index];
        break;
   }

    return retval;
}

bool cmpFile::convertFloatArray(float arry[], int size, Data::DataConversion ctype, int mult)
{
    bool okay = true;
    float value = 0;
    float total = 0;
    int count = 0;

    switch (ctype)
    {
    case Data::Sum:
        for (int i = 0; i < size; i += mult)
        {
            value = 0;
            for (int j = 0; j < mult; j++)
                value += arry[j];
            for (int j = 0; j < mult; j++)
                arry[j] = value;
        }
        break;
    case Data::Average:
        for (int i = 0; i < size; i += mult)
        {
            total = 0;
            count = 0;
            for (int j = 0; j < mult; j++)
            {
                total += arry[j];
                count ++;
            }
            value = total / count;
            for (int j = 0; j < mult; j++)
                arry[j] = value;
        }
        break;
    case Data::AverageNonZero:
        for (int i = 0; i < size; i += mult)
        {
            total = 0;
            count = 0;
            for (int j = 0; j < mult; j++)
            {
                if (arry[i] < 0 || arry[i] > 0)
                {
                    total += arry[i];
                    count ++;
                }
            }
            value = total / count;
            for (int j = 0; j < mult; j++)
                arry[j] = value;
        }
        break;
    case Data::AveragePositive:
        for (int i = 0; i < size; i += mult)
        {
            total = 0;
            count = 0;
            for (int j = 0; j < mult; j++)
            {
                if (arry[i] > 0)
                {
                    total += arry[i];
                    count ++;
                }
            }
            value = total / count;
            for (int j = 0; j < mult; j++)
                arry[j] = value;
        }
        break;
    case Data::DamDay:
        for (int i = 0; i < size; i += mult)
        {
            total = 0;
            count = 0;
            for (int j = 0; j < mult; j++)
            {
                if (settings->getDataSettings()->isDay(i))
                {
                    total += arry[i];
                    count ++;
                }
            }
            value = total / count;
            for (int j = 0; j < mult; j++)
                arry[j] = value;
        }
        break;
    case Data::DamNight:
        for (int i = 0; i < size; i += mult)
        {
            total = 0;
            count = 0;
            for (int j = 0; j < mult; j++)
            {
                if (settings->getDataSettings()->isNight(i))
                {
                    total += arry[i];
                    count ++;
                }
            }
            value = total / count;
            for (int j = 0; j < mult; j++)
                arry[j] = value;
        }
        break;
    case Data::None:
        break;
    default:
        okay = false;
        break;
   }
    return okay;
}

void cmpFile::writeFloatArray(int indent, QString prefix, QString name, QList<float> &arry,
                      Data::DataConversion ctype, int mult,
                      Data::Type dtype, float defaultval)
{
    int total = arry.size();
    if (total > 0)
    {
        float *newarry = static_cast<float*>(calloc(total, sizeof(float)));
        for (int i = 0; i < total; i++)
            newarry[i] = arry[i];
        writeFloatArray(indent, prefix, name, newarry, total, ctype, mult, dtype, defaultval);
    }
}

void cmpFile::writeFloatArray (int indent, QString prefix, QString name, float arry[], int size,
                       Data::DataConversion ctype, int mult,
                       Data::Type dtype, float defaultval)
{
    int num_on_line = 0;
    int indent2 = indent + 2;

    if (arry == nullptr) {
        qWarning("Float array is nullptr.");
        cmpLog::outlog->add(cmpLog::Error, QString("Float array is nullptr."));
        return;
    }
    else {
        // check for all default
        bool alldef = true;
        for (int i = 0; i < size; i++) {
            if (floatIsNotEqual(arry[i], defaultval)) {
                alldef = false;
            }
        }
        if (alldef) return;
    }
    convertFloatArray(arry, size, ctype, mult);

    writeStringNR(indent, prefix, name);
    writeSpace();
    num_on_line = 0;

    int first = 0, last = 1, l;
    float firstval = 0, lastval = 0;

    for (int i = 0; i < size; i++)
    {
        first = i;
        last = i + 1;
        firstval = arry[first];
        lastval = arry[last];
        if (floatIsEqual(lastval, firstval))
        {
            if (num_on_line >= 2)
            {
                writeNewline();
                writeIndent(indent2);
                num_on_line = 0;
            }
            l = last + 1;
            while (l < size && floatIsEqual((lastval = arry[l]), firstval))
                l++;
            last = l - 1;
            if (first == 0 && l == size)
            {
                write (QString ("[*] ").toUtf8());
                writeFloat (firstval, dtype);
            }
            else if (last - first > 2)
            {
                QString range (QString ("[%1:%2] ")
                           .arg (QString::number(first), QString::number(last)));
                write (range.toUtf8());
                writeFloat (firstval, dtype);
            }
            else
            {
                writeFloat (firstval, dtype);
                writeFloat (arry[last], dtype);
            }
            num_on_line += 2;
            i = last;
        }
        else
        {
            if (num_on_line == 4)
            {
                writeNewline();
                writeIndent(indent2);
                num_on_line = 0;
            }
            writeFloat (firstval, dtype);
            num_on_line++;
        }
    }
    writeNewline();
}

void cmpFile::writeIntArray(int indent, QString prefix, QString name, QList<int> &arry,
                            Data::DataConversion ctype, int mult, int defaultval)
{
    int total = arry.size();
    if (total > 0)
    {
        int *newarry = static_cast<int*>(calloc(total, sizeof(int)));
        for (int i = 0; i < total; i++)
            newarry[i] = arry[i];
        writeIntArray(indent, prefix, name, newarry, total, ctype, mult, defaultval);
    }
}

void cmpFile::writeIntArray (int indent, QString prefix, QString name, int arry[], int size,
                             Data::DataConversion ctype, int mult, int defaultval)
{
    int num_on_line = 0;
    int indent2 = indent + 2;

    if (arry == nullptr) {
        qWarning("Float array is nullptr.");
        cmpLog::outlog->add(cmpLog::Error, QString("Float array is nullptr."));
        return;
    }
    else {
        // check for all default
        bool alldef = true;
        for (int i = 0; i < size; i++) {
            if (arry[i] != defaultval) {
                alldef = false;
            }
        }
        if (alldef) return;
    }
    convertIntArray(arry, size, ctype, mult);

    writeStringNR(indent, prefix, name);
    writeSpace();
    num_on_line = 1;

    int first = 0, last = 1, l;
    int firstval = 0, lastval = 0;

    for (int i = 0; i < size; i++)
    {
        first = i;
        last = i + 1;
        firstval = arry[first];
        lastval = arry[last];
        if (lastval == firstval)
        {
            if (num_on_line >= 2)
            {
                writeNewline();
                writeIndent(indent2);
                num_on_line = 0;
            }
            l = last + 1;
            while (l < size && (lastval = arry[l]) == firstval)
                l++;
            last = l - 1;
            if (first == 0 && l == size)
            {
                write (QString ("[*] ").toUtf8());
                writeInt (firstval);
            }
            else if (last - first > 2)
            {
                QString range (QString ("[%1:%2] ")
                           .arg (QString::number(first), QString::number(last)));
                write (range.toUtf8());
                writeInt (firstval);
            }
            else
            {
                writeInt (firstval);
                writeInt (arry[last]);
            }
            num_on_line += 2;
            i = last + 1;
        }
        else
        {
            if (num_on_line == 4)
            {
                writeNewline();
                writeIndent(indent2);
                num_on_line = 0;
            }
            writeInt (firstval);
            num_on_line++;
        }
    }
    writeNewline();
}

void cmpFile::printEOF (QString data)
{
    if (data.isEmpty ())
    {
        printMessage ("Found EOF.");
    }
    else
    {
        printError (QString ("Found EOF instead of %1."). arg (data));
    }
}


void cmpFile::printMessage (QString msg)
{
    QMessageLogContext mlc(fileName().toStdString().data(), lineNum, nullptr, nullptr);
    qInfo(msg.toStdString().data());
    cmpLog::instance()->add (cmpLog::Message, msg);
    cmpLog::instance()->add (cmpLog::Message, getFileLine());

    cout << "Info  :" << msg.toStdString();
    cout << ": " << getFileLine().toStdString() << endl;
}

void cmpFile::printError (QString errmsg)
{
    qWarning(errmsg.toStdString().data());
    cmpLog::instance()->add (cmpLog::Error, errmsg);
    cmpLog::instance()->add (cmpLog::Error, getFileLine());

    cout << "Error :" << errmsg.toStdString() << endl;
    cout << getFileLine().toStdString() << endl;
}

QString cmpFile::getFileLine ()
{
   return QString((QString("File: %1, Line: %2").arg
                   (fileName(), QString::number(lineNum))));
}

void cmpFile::setSettings(cmpSettings *newSettings)
{
    settings = newSettings;
}

int cmpFile::getDataVersion() const
{
    return dataVersion;
}

void cmpFile::setDataVersion(int newDataVersion)
{
    dataVersion = newDataVersion;
}

void cmpFile::obsoleteToken (QString obsToken, QString newToken)
{
        QString message (QString("obsolete token {%1}. ").arg(obsToken));

        if (newToken.isEmpty())
        {
                message.append ("Data discarded.\n");
        }
        else
        {
                message.append (QString("Replaced with {%1}.\n").arg(newToken));
        }
        skipLine();
        printError(message);
}

void cmpFile::unknownToken (QString token, QString segment)
{
    bool isFloat;
    QString message;
    token.toFloat (&isFloat);
    if (!segment.isEmpty())
        message = QString(" for segment %1.\n").arg(segment);
    else
        message = QString(".\n");
    if (isFloat)
    {
        message.prepend (QString ("unknown or unexpected number {%1}").arg (token));
        message.append ("\tSkipping all numbers until a non-numeric token is reached.\n");
        skipAllNumbers();
    }
    else
    {
        message.prepend (QString ("unknown or unexpected command {%1}").arg (token));
        message.append ("\tDiscarding line.\n");
        skipLine();
    }
    printError(message);
}

void cmpFile::incorrectValue (int val, QString name)
{
    QString message("Problem reading integer value ");
    message.append(QString::number(val));
    if (!name.isEmpty())
        message.append(QString(" for %1").arg(name));
    printError(message);
}
void cmpFile::incorrectValue (float val, QString name)
{
    QString message("Problem reading float value ");
    message.append(QString::number(val));
    if (!name.isEmpty())
        message.append(QString(" for %1").arg(name));
    printError(message);
}

