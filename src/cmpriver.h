#ifndef CMPRIVER_H
#define CMPRIVER_H

#include "cmpriversegment.h"

#include <QObject>

class cmpRiver : public QObject
{
    Q_OBJECT
public:
    explicit cmpRiver(QObject *parent = nullptr);
    cmpRiver (QString name, QObject *parent = nullptr);
    ~cmpRiver () override;

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
    bool output(cmpFile *cfile);

private:
    QString name;
    float flowMax;
    float flowMin;
    QList<cmpRiverSegment *> segments;


};

#endif // CMPRIVER_H
