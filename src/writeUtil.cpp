#include "writeUtil.h"

bool write_file (QString filename)
{
    return true;
}

bool write_header (QString filename)
{
    return true;
}


bool write_scn (QString filename)
{
    return true;
}


bool write_riv (QString filename)
{
    return true;
}

bool write_ops (QString filename)
{
    return true;
}

bool write_clb (QString filename)
{
    return true;
}

bool write_pbn (QString filename)
{
    return true;
}

bool write_rls (QString filename)
{
    return true;
}

bool write_ctrl (QString filename)
{
    return true;
}

bool writeFileComments (CompassFile *file, QStringList *comments)
{
    bool okay = true;
    if (file->isOpen())
    {
        for (int i = 0; i < comments->count(); i++)
        {
            if (file->write(comments->at(i).toUtf8().data()) < 0)
                okay = false;
            file->write("\n");
        }
    }
    else {
        okay = false;
    }
    return okay;
}

bool outputEnd (CompassFile *outfile, int tab, QString type, QString name)
{
    bool okay = true;
    QString nameString(QString("%1 (%2)").arg(type, name));

    outfile->writeString(tab, "end", nameString);
    return okay;
}


bool output_string (QFile *outfile, int tab_level, QString text, bool echo = false)
{
    return true;
}

bool output_float (QFile *outfile, int tab_level, float value, int precision, bool echo = false)
{
    return true;
}

bool output_int (QFile *outfile, int tab_level, int value, bool echo = false)
{
    return true;
}


bool newline (QFile *outfile);
