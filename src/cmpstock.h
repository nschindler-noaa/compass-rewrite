#ifndef CMPSTOCK_H
#define CMPSTOCK_H

#include "cmpspecies.h"
#include "cmpcommandsettings.h"
#include "cmpequation.h"
#include "cmpmontecarlomulti.h"
#include "cmpreachclass.h"
#include "cmpfile.h"

#include <QStringList>

/**
 * \class cmpStock
 * A cmpStock is a cmpSpecies that includes data for a specific stock.
 * This includes a list of cmpReachClass that indicate behavior of the
 * stock in groupings of reaches.
 *
 * A release includes a name of its stock.
 *
 */


class cmpStock : public cmpSpecies
{
public:
    cmpStock();

    void writeData(cmpFile *outfile, int indent, bool outputAll);

private:

};

#endif // CMPSTOCK_H
