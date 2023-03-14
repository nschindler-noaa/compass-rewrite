#ifndef CMPRIVER_H
#define CMPRIVER_H

#include "cmpdam.h"
#include "cmpreach.h"
#include "cmpheadwater.h"
#include "cmpriversegment.h"


#include <QObject>

class cmpRiverSystem;

class cmpRiver : public QObject
{
    Q_OBJECT
public:
    explicit cmpRiver(cmpRiverSystem *parent);
    cmpRiver (QString name, cmpRiverSystem *parent = nullptr);
    ~cmpRiver () override;
    void setup(QString name, cmpRiverSystem *parent);

    QString &getName();
    void setName(QString &value);

    float getFlowMax() const;
    void setFlowMax(float value);

    float getFlowMin() const;
    void setFlowMin(float value);

    QList<cmpRiverSegment *> getSegments() const;
    void setSegments(const QList<cmpRiverSegment *> &value);
    cmpRiverSegment *getSegment(int index) const;
    void addSegment(cmpRiverSegment *value);
    int getNumSegments();

    bool parseDesc(cmpFile *descfile);
    bool outputDesc(cmpFile *descfile);
    bool outputData(cmpFile *cfile, bool outputAll);

private:
    cmpRiverSystem *rs;
    QString name;
    float flowMax;
    float flowMin;
    QList<cmpRiverSegment *> segments;

};

#endif // CMPRIVER_H
