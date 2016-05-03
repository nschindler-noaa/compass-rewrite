#ifndef COMPASS_FILE_H
#define COMPASS_FILE_H

#include <QString>
#include <QFile>

#include "Log.h"

#define CURRENT_DATA_VERSION    9

class CompassFile public QFile
{
public:
    CompassFile (QString fname = QString (""));
    ~CompassFile ();

    bool readHeader ();
    bool readInfo ();

    QString nextToken ();

    void writeHeader ();
    void writeInfo ();

    QStringList *header;
    int data_version;
    QString *creator;
    QString *created_date;
    QString *modifier;
    QString *modified_date;
    QString *notes;
    int      line;        /**< Current line of input */
    QStringList *tokens;  /**< Current list of tokens */


};

struct compass_file
{
    QFile *file;
    QString fullname;
    QString filename;
    QString path;
};

typedef struct compass_file COMPASS_FILE;

COMPASS_FILE *new_compass_file (QString filename);

bool read_header (COMPASS_FILE *);
bool read_fileinfo (COMPASS_FILE *);

QString next_token (COMPASS_FILE *);

void delete_compass_file (COMPASS_FILE *);

#endif // COMPASS_FILE_H
