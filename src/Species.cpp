#include "Species.h"

Species::Species (QString nm)
{
    if (nm.isEmpty ())
        name = new QString ("Generic");
    else
        name = new QString(nm);


}

Species::~Species ()
{
    delete name;
}
