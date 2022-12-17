#include "cmpstock.h"

#include "cmpdatasettings.h"
/*
cmpStock::cmpStock()
{
    name = QString ("generic");
    inriverReturnEqn = new cmpEquation();
    transportReturnEqn = new cmpEquation();
    reachClasses.append(new cmpReachClass("0"));
    allocate(732, 1);

}

cmpStock::~cmpStock()
{
    delete inriverReturnEqn;
    delete transportReturnEqn;
    while (reachClasses.count() > 0)
        delete reachClasses.takeLast();
}

void cmpStock::setDefaults()
{
    for (int i = 0; i < reachClasses.count(); i++)
        reachClasses.at(i)->setDefaults();

}

bool cmpStock::parseData(cmpFile *cfile)
{
    bool okay = true, end = false;
    QString token ("");

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            cfile->printEOF("Stock data.");
            okay = false;
        }
        else if (token.compare("end", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->checkEnd("reach", name);
            end = true;
        }
        else
        {
            cfile->unknownToken(token, name);
        }
    }

    return okay;
}
*/
