#ifndef C_BASIN_H
#define C_BASIN_H

#include "definitions.h"

/** \class basin
 * \brief A structure to represent a storage basin associated with a dam.
 */
class Basin
{
public:
    Basin();

    float volume[DAYS_IN_SEASON]; /**< Per day storage basin volumes */
    float min_volume;         /**< Minimum allowable volume */
    float max_volume;         /**< Maximum allowable volume */

};

#endif // C_BASIN_H
