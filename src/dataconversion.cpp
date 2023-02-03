#include "dataconversion.h"

bool assignFloatList (QList<float> &inlist, QList<float> &outlist, Data::DataConversion convert, int mult)
{
    bool okay = true;
    int incount = inlist.count();
    int outcount = outlist.count();

    if (incount == outcount && mult == 1)
    {
        outlist = inlist;
    }
    else if (incount < outcount && (incount * mult) == outcount)
    {
        okay = convertInputFloatList(inlist, outlist, convert, mult);
    }
    else if (outcount < incount && (outcount * mult) == incount)
    {
       okay = convertOutputFloatList(inlist, outlist, convert, mult);
    }
    else {
        // problem with size of lists or value of mult
        okay = false;
    }
    return okay;
}

bool convertInputFloatList (QList<float> &list1, QList<float> &list2, Data::DataConversion convert, int mult)
{
    bool okay = true;
    int count1 = list1.count();
    int count2 = list2.count();
    float value;
    int index = 0;
    int i, j;

    switch (convert)
    {
    default:
    case Data::None:
        if (count1 != count2)
            break;
        list2 = list1;
        break;

    case Data::Duplicate:
        for (i = 0; i < count1; i++)
        {
            value = list1.at(i);
            index = (i * mult);
            for (j = 0; j < mult; j++)
            {
                list2[index + j] = value;
            }
        }
        break;

    case Data::Space:
        for (i = 0; i < count1; i++)
        {
            value = list1.at(i);
            index = (i * mult);
            list2[index] = value;
            for (j = 1; j < mult; j++)
                list2[index + j] = 0.0;
        }
        break;

    case Data::Proportion:
        for (i = 0; i < count1; i++)
        {
            value = list1.at(i);
            index = (i * mult);
            value /= mult;
            for (j = 0; j < mult; j++)
                list2[index + j] = value;
        }
        break;

    case Data::Half:
        for (i = 0; i < count1; i++)
        {
            value = list1.at(i);
            index = (i * mult);
            for (j = 0; j < mult; j++)
                list2[index + j] = value / 2.0;
        }
        break;
    }

    return okay;
}

bool convertOutputFloatList (QList<float> &list1, QList<float> &list2, Data::DataConversion convert, int mult)
{
    bool okay = true;
    int count1 = list1.count();
    int count2 = list2.count();
    float value;
    int index = 0;
    int i, j;
    int count;

    switch (convert)
    {
    default:
    case Data::None:
        if (count1 != count2)
            break;
        list2 = list1;
        break;

    case Data::Sum:
        for (i = 0; i < count2; i++)
        {
            index = (i * mult) + i;
            value = 0;
            count = 0;
        for (j = 0; j < mult; j++)
            value += list1.at(index + j);
        list2[index] = value;
        }
        break;

    case Data::Average:
        for (i = 0; i < count2; i++)
        {
            index = (i * mult) + i;
            value = 0;
            count = 0;
        for (j = 0; j < mult; j++)
            value += list1.at(index + j);
        value /= mult;
        list2[index] = value;
        }
        break;

    case Data::AveragePositive:
        for (i = 0; i < count2; i++)
        {
            index = (i * mult) + i;
            value = 0;
            count = 0;
        for (j = 0; j < mult; j++)
        {
            if (list1.at(index + j) > 0.0)
            {
                value += list1.at(index + j);
                count ++;
            }
        }
        value /= count;
        list2[index] = value;
        }
        break;

    case Data::AverageNonZero:
        for (i = 0; i < count2; i++)
        {
            index = (i * mult) + i;
            value = 0;
            count = 0;
        for (j = 0; j < mult; j++)
        {
            if (list1.at(index + j) > 0.0 || list1.at(index + j) < 0.0)
            {
                value += list1.at(index + j);
                count ++;
            }
        }
        value /= count;
        list2[index] = value;
        }
        break;

    case Data::DamDay:
    case Data::DamNight:
        break;
    }
    return okay;
}

bool assignInputIntList (QList<int> &list1, QList<int> &list2, Data::DataConversion convert, int mult)
{
    bool okay = true;
    int count1 = list1.count();
    int count2 = list2.count();

    if (count1 == count2 && mult == 1)
    {
        for (int i = 0; i < count1; i++)
            list2[i] = list1.at(i);
    }
    else if (count1 < count2 && (count1 * mult) == count2)
    {
        okay = convertInputIntList(list1, list2, convert, mult);
    }
    else if (count2 < count1 && (count2 * mult) == count1)
    {
        okay = convertInputIntList(list2, list1, convert, mult);
    }
    else {
        // problem with size of lists or value of mult
        okay = false;
    }
    return okay;
}

bool convertInputIntList(QList<int> &list1, QList<int> &list2, Data::DataConversion convert, int mult)
{
    bool okay = true;
    int count1 = list1.count();
    int count2 = list2.count();
    int value;
    int index = 0;
    int i, j;

    switch (convert)
    {
    default:
    case Data::None:
        if (count1 != count2)
            break;
        list2 = list1;
        break;

    case Data::Proportion:
        for (i = 0; i < count1; i++)
        {
            value = list1.at(i);
            index = (i * mult);
            value /= mult;
            for (j = 0; j < mult; j++)
                list2[index + j] = value;
        }
        break;

    case Data::Duplicate:
        for (i = 0; i < count1; i++)
        {
            value = list1.at(i);
            index = (i * mult);
            for (j = 0; j < mult; j++)
            {
                list2[index + j] = value;
            }
        }
        break;

    case Data::Space:
        for (i = 0; i < count1; i++)
        {
            value = list1.at(i);
            index = (i * mult);
            list2[index] = value;
            for (j = 1; j < mult; j++)
                list2[index + j] = 0;
        }
        break;

    case Data::Half:
        for (i = 0; i < count1; i++)
        {
            value = list1.at(i);
            index = (i * mult);
            for (j = 0; j < mult; j++)
                list2[index + j] = static_cast<int>(value / 2);
        }
        break;
    }

    return okay;
}

bool convertOutputIntList (QList<int> &list1, QList<int> &list2, Data::DataConversion convert, int mult)
{
    bool okay = true;
    int count1 = list1.count();
    int count2 = list2.count();
    int value;
    int index = 0;
    int i, j;
    int count;

    switch (convert)
    {
    default:
    case Data::None:
        if (count1 != count2)
            break;
        list2 = list1;
        break;

    case Data::Sum:
        for (i = 0; i < count2; i++)
        {
            index = (i * mult) + i;
            value = 0;
            count = 0;
        for (j = 0; j < mult; j++)
            value += list1.at(index + j);
        list2[index] = value;
        }
        break;

    case Data::Average:
        for (i = 0; i < count2; i++)
        {
            index = (i * mult) + i;
            value = 0;
            count = 0;
        for (j = 0; j < mult; j++)
            value += list1.at(index + j);
        value /= mult;
        list2[index] = value;
        }
        break;

    case Data::AveragePositive:
        for (i = 0; i < count2; i++)
        {
            index = (i * mult) + i;
            value = 0;
            count = 0;
        for (j = 0; j < mult; j++)
        {
            if (list1.at(index + j) > 0.0)
            {
                value += list1.at(index + j);
                count ++;
            }
        }
        value /= count;
        list2[index] = value;
        }
        break;

    case Data::AverageNonZero:
        for (i = 0; i < count2; i++)
        {
            index = (i * mult) + i;
            value = 0;
            count = 0;
        for (j = 0; j < mult; j++)
        {
            if (list1.at(index + j) > 0.0 || list1.at(index + j) < 0.0)
            {
                value += list1.at(index + j);
                count ++;
            }
        }
        value /= count;
        list2[index] = value;
        }
        break;

    case Data::DamDay:
        for (i = 0; i < count2; i++)
        {
            index = (i * mult) + i;
            value = 0;
            count = 0;
        }
        break;
    case Data::DamNight:
        for (i = 0; i < count2; i++)
        {
            index = (i * mult) + i;
            value = 0;
            count = 0;
        }
        break;
    }
    return okay;
}

