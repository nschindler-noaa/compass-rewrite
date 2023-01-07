#include "cmpstock.h"

#include "cmpdatasettings.h"

cmpStock::cmpStock() : cmpSpecies()
{}


void cmpStock::writeData(cmpFile *outfile, int indent, bool outputAll)
{
    outfile->writeString(indent, "stock", name);
    writeReachClassData(outfile, indent+1, outputAll);
    outfile->writeNewline();
    writeSpeciesData(outfile, indent+1, outputAll);
    outfile->writeEnd(indent, "stock", name);
}

