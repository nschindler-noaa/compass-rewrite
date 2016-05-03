#ifndef C_DATACONVERSION_H
#define C_DATACONVERSION_H

#include "CompassFile.h"

/** \brief How to treat values before saving to an array.
 *
 *  Values are either not converted - saved to the array as read, or
 *  spread over a number of array items by various means.
 */
typedef enum array_conversion_type {
        None, /**< Save each number as-is. */
        Duplicate,     /**< Duplicates each number over a number of array items */
        Space,   /**< Spaces each number by a number of 0.0 array items */
        Half,    /**< Divide the number by 2 and assign to a number of array items */
        Proportion     /**< Divide by the number of array items and assign to each array item */
} DataConversion;

/** Assign a float value to an array, applying a data conversion in the process */
void assign_float_array (float farray[], int index,
            float value, DataConversion conversion, unsigned mult);
/** Assign an int value to an array, applying a data conversion in the process */
void assign_int_array (int iarray[], int index,
            int value, DataConversion conversion, unsigned mult);

/** Read an int array from a file, applying a data conversion to the values */
bool read_int_array (CompassFile *infile, int iarray[],
            int max_elem, DataConversion conversion,
            unsigned mult, char *prompt);
/** Read a float array from a file, applying a data conversion to the values */
bool read_float_array (CompassFile *infile, float array[],
            int max_elem, DataConversion conversion,
            unsigned mult, QString prompt);

#endif // C_DATACONVERSION_H
