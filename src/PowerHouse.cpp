#include "PowerHouse.h"

PowerHouse::PowerHouse(int num)
{
    number = num;
    capacity = 0.0;
    schedule = NULL;
    for (int i = 0; i < DAM_SLICES_IN_SEASON; i++)
         flow_fraction[i] = 0.0;
}
