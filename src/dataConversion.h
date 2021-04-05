#ifndef C_DATACONVERSION_H
#define C_DATACONVERSION_H

//#include "CompassFile.h"
#include <QStringList>

//ACT_NO_CONV, /**< Save each number as-is. */
//ACT_DUP,     /**< Duplicates each number over a number of array items */
//ACT_SPLIT,   /**< Spaces each number by a number of 0.0 array items */
//ACT_HALF,    /**< Divide the number by 2 and assign to a number of array items */
//ACT_PROP,    /**< Divide by the number of array items and assign to each array item */
//ACT_AVG      /**< Averages number of items and places in single, smaller array */

//OCT_NO_CONV, /**< Write each number as-is. */
//OCT_SUM,
//OCT_AVERAGE,
//OCT_AVE_NON_0,
//OCT_AVE_POS,
//OCT_DAM_DAY,
//OCT_DAM_NIGHT

class Data
{
public:
    typedef enum InputDataConversionType {
        // from small to large
        Proportion = 101, /**< Divide value by multiple and assign to array. */
        Duplicate,        /**< Duplicate value and assign to array. */
        Space,            /**< Assign to every multiple value in array. */
        Half,             /**< Divide by 2 and assign to array. */
        // from large to small
        Average,          /**< Average input and assign to smaller array. */
        AveragePos,       /**< Output average of positive values only. */
        AverageNonZ,      /**< Output average of non-zero values only. */
        DamDay,           /**< Output as Dam day values. */
        DamNight,         /**< Output as Dam night values. */
        Sum               /**< Sum multiple values and assign to one */
    }DataConversion;

    typedef enum OutputOptions {
        OutputAll = 211,  /**< Output all values even if default. */
        OutputNonDefault, /**< Output values that are not defaults. */
        OutputNonZero     /**< Output values that are not zero. */
    }Output;

    typedef enum OutputDataConversionType {
        None = 200,       /**< Output values as-is. */
        SumValues,        /**< Output sum of multiple values. */
        AverageValues,    /**< Output average of multiple values. */
        AveragePositive,  /**< Output average of positive values only. */
        AverageNonZero,   /**< Output average of non-zero values only. */
        DamDayValues,     /**< Output as Dam day values. */
        DamNightValues    /**< Output as Dam night values. */
    }OutputConversion;

    typedef enum OutputDataType {
        Integer = 220,    /**< Output as an integer. */
        Float,            /**< Output with 2 decimal places. */
        Fixed,            /**< Output with 4 decimal places. */
        Scientific,       /**< Output with 6 decimal places. */
        Precise           /**< Output with 12 decimal places (double). */
    }Type;

};


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
//void assign_float_array (float farray[], int index,
//            float value, DataConversion conversion, unsigned mult);
/** Assign an int value to an array, applying a data conversion in the process */
//void assign_int_array (int iarray[], int index,
//            int value, DataConversion conversion, unsigned mult);

/** Read an int array from a file, applying a data conversion to the values */
//bool read_int_array (CompassFile *infile, int iarray[],
//            int max_elem, DataConversion conversion,
//            unsigned mult, char *prompt);
/** Read a float array from a file, applying a data conversion to the values */
//bool read_float_array (CompassFile *infile, float array[],
//            int max_elem, DataConversion conversion,
//            unsigned mult, QString prompt);

bool assignInputFloatList (QList<float> &array1, QList<float> &array2, Data::DataConversion convert, int mult);
bool convertInputFloatList (QList<float> &smList, QList<float> &lgList, Data::DataConversion convert, int mult);
bool convertOutputFloatList (QList<float> &smList, QList<float> &lgList, Data::DataConversion convert, int mult);

bool assignInputIntList (QList<int> &list1, QList<int> &list2, Data::DataConversion convert, int mult);
bool convertInputIntList (QList<int> &smList, QList<int> &lgList, Data::DataConversion convert, int mult);
bool convertOutputIntList (QList<int> &smList, QList<int> &lgList, Data::DataConversion convert, int mult);

#endif // C_DATACONVERSION_H
