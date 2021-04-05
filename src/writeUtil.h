#ifndef C_WRITEUTIL_H
#define C_WRITEUTIL_H
#include <QStringList>
#include <QFile>

#include "CompassFile.h"

bool write_file (QString filename);
bool write_header (QString filename);
bool write_scn (QString filename);
bool write_riv (QString filename);
bool write_ops (QString filename);
bool write_clb (QString filename);
bool write_pbn (QString filename);
bool write_rls (QString filename);
bool write_ctrl (QString filename);
bool writeFileComments (CompassFile *file, QStringList *comments);
bool outputEnd (CompassFile *outfile, int tab, QString type, QString name = QString(""));

bool output_string (QFile *outfile, int tab_level, QString text, bool echo);
bool output_float (QFile *outfile, int tab_level, float value, int precision, bool echo);
bool output_int (QFile *outfile, int tab_level, int value, bool echo);

bool newline (QFile *outfile);

#endif // C_WRITEUTIL_H
