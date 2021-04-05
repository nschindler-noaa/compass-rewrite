#ifndef C_DEFINITIONS_H
#define C_DEFINITIONS_H

/** Defines for time steps */

#define DAYS_IN_SEASON       (366) /**< Number of days in year */
#define DAM_SLICES_PER_DAY     (2) /**< Number of time slices per day at a dam */
#define STEPS_PER_DAY   DAM_SLICES_PER_DAY /**< Same as DAM_SLICES_PER_DAY */
#define DAM_SLICES_IN_SEASON (732) /**< DAM_SLICES_PER_DAY * DAYS_IN_SEASON */

#define STEPS_IN_SEASON      (732) /**< Same as DAM_SLICES_IN_SEASON for reach segments */

#define STEP_DAY(step)   ((int)(step / STEPS_PER_DAY))

/** Define (or undefine) REALTIME */
#define REALTIME

/** Define (or undefine) ROUTE_TRACKING */
#define ROUTE_TRACKING

/** Define (or undefine) gas propogation calculations */
#undef GAS_CALC
/** Define (or undefine) predation calculations */
#undef PRED_CALC
/** Define (or undefine) growth calculations  */
#undef GROWTH_CALC

/** Defines for current build */
#define JUVENILE
#undef ADULT

#define DEBUG true
#define DEBUG_INPUT true

#define PRECISION .000001

bool floatIsEqual (float val1, float val2);
bool floatIsNotEqual (float val1, float val2);
bool doubleIsEqual (double val1, double val2);

#endif // C_DEFINITIONS_H
