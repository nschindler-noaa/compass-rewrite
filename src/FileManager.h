#ifndef C_FILEMANAGER_H
#define C_FILEMANAGER_H

#include <QObject>
#include <QStringList>
#include <QFile>

#include "CompassFile.h"
#include "parseUtil.h"
#include "writeUtil.h"

#include "RiverSystem.h"
#include "Scenario.h"
#include "settings.h"

enum TYPE {
    DESC,     // River description file
    DAT,      // Scenario file or simple data file
    SCN,      // Scenario file
    OPS,      // Dam operations file
    RIV,      // River year data
    RLS,      // Fish release(s)
    FLOW,     // Dam flow - obsolete, will not write
    SPLL,     // Dam spill - obsolete, will not write
    DAM,
    SPILL,    // Dam spill - ibid
    PBN,      // Post Bonneville file - ocean effects
    CTRL,     // Input file combining 2 or more files
    TRANS,    // Dam transport file
    CLB,      // Calibration file
    ETC,      // extras
    NONE
};

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
    explicit FileManager(QObject *parent = nullptr);

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
    bool writeRiverDescFile (RiverSystem *rs, Settings *settings, QString filename = QString(""));
    bool outputRiver(River *river, CompassFile *outfile);
    bool outputSegment(RiverSegment *rseg, CompassFile *outfile);
    bool writeFile (Scenario *scn, QString filename = QString(""));
    bool writeFiles (Scenario *scn, Settings *settings);
    bool writeFileHeader(QFile *outfile, Settings *settings);
    void writeSeparator (QFile *outfile);
    bool outputText (QString text);
    bool saveData (Scenario *scn, QString filename);
    bool isCompassFile (QString filename);
    bool isRivDescFile (QString filename);
    TYPE getType(QString filename);
    void addSpaces(QString &line, int col);
};

#endif // C_FILEMANAGER_H
