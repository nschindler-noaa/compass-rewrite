#ifndef C_FILEMANAGER_H
#define C_FILEMANAGER_H

#include <QObject>
#include <QString>
#include "CompassFile.h"
#include "parseUtil.h"
#include "writeUtil.h"

#include "RiverSystem.h"
#include "Scenario.h"
#include "settings.h"

struct file_info {
    QString fullname;
    QString filename;
    QString path;
    QStringList header;
    QString creator;
    QString created_date;
    QString modifier;
    QString modified_date;
    QString notes;
};


class FileManager : public QObject
{
    Q_OBJECT
public:
    explicit FileManager(QObject *parent = 0);

    QStringList filenames;
    QList <CompassFile *> files;

signals:
    void readingFile ();
    void fileRead ();
    void writingFile ();
    void fileWritten ();

public slots:
    bool fileExists (QString &filename, QString &path);
    bool findFile (QString filename, QString path, Settings *&set);
    bool parseHeader (QString filename, struct file_info *fi);
    bool parseFileinfo (QString filename, struct file_info *fi);
    bool readRiverDescFile (Scenario *scn, Settings *set);
    bool readRiverDescFile (RiverSystem *rs, QString filename);
    bool readFile (RiverSystem *rs, QString filename = QString("base.dat"));
//    bool readFile (QString filename = QString("base.dat"));
//    bool readFiles ();
    bool readFiles (Scenario *scn, Settings *set);
    bool readMVFiles (RiverSystem *rs, Settings *settings);
    bool readMultiFiles (RiverSystem *rs, Settings *settings);
    bool readRTFiles (RiverSystem *rs, Settings *settings);
    bool writeSummary (Results *result, QString filename = QString("summary.dat"));
    bool writeFile (Scenario *scn, QString filename = QString(""));
    bool writeFiles (Scenario *scn, Settings *settings);
    bool outputText (QString text);
    bool saveData (Scenario *scn, QString filename);
    bool isCompassFile (QString filename);
    bool isRivDescFile (QString filename);
};

#endif // C_FILEMANAGER_H
