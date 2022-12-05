#ifndef CMPDAM_H
#define CMPDAM_H

#include "cmpriversegment.h"
#include "cmpbasin.h"
#include "cmprsw.h"
#include "cmptransport.h"
#include "cmpspillway.h"
#include "cmpfishway.h"
#include "cmppowerhouse.h"
#include "cmpdamspecies.h"

class cmpRiver;

/** \class Dam
 * \brief Dam-specific segment data (i.e. the dam parameters) */
class cmpDam : public cmpRiverSegment
{
public:
    explicit cmpDam(cmpRiver *parent = nullptr);
    cmpDam (QString dname, cmpRiver *parent = nullptr);
    cmpDam (const cmpDam &rhs);
    ~cmpDam () override;
    void setup ();
    void resetData();
    void clear ();
    bool parseData (cmpFile *infile);
    bool parseToken (QString token, cmpFile *infile);

    bool parseDesc (cmpFile *descfile) override;
    void outputDesc (cmpFile *outfile) override;

    cmpSpillway *getSpillway() const;
    void setSpillway(cmpSpillway *value);

    cmpBasin *getBasin() const;
    void setBasin(cmpBasin *value);

    cmpDamSpecies *getSpecies() const;
    void setSpecies(cmpDamSpecies *spec);

    cmpPowerhouse *getPowerhouse(int index);
    const QList<cmpPowerhouse *> &getPowerhouses() const;
    void setPowerhouses(const QList<cmpPowerhouse *> &value);
    int setPowerhouse (cmpPowerhouse *house, int num = 0);
    int getNumPowerhouses ();

    Location getPhouseSide() const;
    void setPhouseSide(const Location &value);

    float getWidthTailrace() const;
    void setWidthTailrace(float value);

    float getLengthTailrace() const;
    void setLengthTailrace(float value);

    float getElevBase() const;
    void setElevBase(float value);

    float getElevForebay() const;
    void setElevForebay(float value);

    float getElevTailrace() const;
    void setElevTailrace(float value);

    float getFullHead() const;
    void setFullHead(float value);

    float getDepthForebay() const;
    void setDepthForebay(float value);

    float getDepthTailrace() const;
    void setDepthTailrace(float value);

    float getHeightBypass() const;
    void setHeightBypass(float value);

    int getCollector() const;
    void setCollector(int value);

    float getLengthBasin() const;
    void setLengthBasin(float value);

    float getSpecGrav() const;
    void setSpecGrav(float value);

    QList<float> getSpill() const;
    void setSpill(const QList<float> &value);

    QList<float> getSpillPlanned() const;
    void setSpillPlanned(const QList<float> &value);

    FloatPeriodList *getSpillPlannedDay() const;
    void setSpillPlannedDay(FloatPeriodList *value);

    FloatPeriodList *getSpillPlannedNight() const;
    void setSpillPlannedNight(FloatPeriodList *value);

    FloatPeriodList *getSpillLegacyPlanned() const;
    void setSpillLegacyPlanned(FloatPeriodList *value);

    FloatPeriodList *getSpillLegacyFish() const;
    void setSpillLegacyFish(FloatPeriodList *value);

    float getSpillMax() const;
    void setSpillMax(float value);

    QString &getSpillSideText();
    void setSpillSideText(const QString &text);
    Location getSpillSide() const;
    void setSpillSide(const Location &value);

    float getRswSpillMax() const;
    void setRswSpillMax(float value);

    cmpRSW *getRswActive() const;
    void setRswActive(cmpRSW *value);

    float getFlowProjectMin() const;
    void setFlowProjectMin(float value);

    float getFlowRiverMin() const;
    void setFlowRiverMin(float value);

    cmpTransport *getTransport() const;
    void setTransport(cmpTransport *value);

    QList<float> getDepthForebayDay() const;
    void setDepthForebayDay(const QList<float> &value);

    QList<float> getDepthTailraceDay() const;
    void setDepthTailraceDay(const QList<float> &value);

    QList<float> getDropRatioDay() const;
    void setDropRatioDay(const QList<float> &value);

    QList<float> getDropRatioDayTR() const;
    void setDropRatioDayTR(const QList<float> &value);

    QList<float> getDaylightProportion() const;
    void setDaylightProportion(const QList<float> &value);

//    int getNumFishways();
//    void setNumFishways(int num);
    cmpFishway *getFishway() const;
    void setFishway(cmpFishway *value);

private:
    /* Spillway information  */
    cmpSpillway *spillway; /**< Spillway pointer holds information about the spillway. */

    /* Storage basin info */
    cmpBasin *basin;       /**< Storage basin pointer. If no basin, this pointer is nullptr. */

    cmpFishway *fishway;   /**< Fishway pointer holds information about the fishway for adult migration only.
                            * If no fishway, this pointer is nullptr.*/

    /** Pointer to the dam species information */
    cmpDamSpecies *species;

    /** List of pointers to powerhouses. If there are no powerhouses, this pointer is nullptr. */
    QList<cmpPowerhouse *> powerhouses;
    Location phouseSide;


    /* physical characteristics; some may be computed from others,
     * depending on which were found in the river description file.
     */
    float widthTailrace; /**< Tailrace width (ft) */
    float lengthTailrace;/**< Tailrace length in ft */
    float elevBase;      /**< Dam floor elevation (ft) */
    float elevForebay;   /**< Forebay elevation (ft) */
    float elevTailrace;  /**< Tailrace elevation (ft) */
    float elevBypass;    /**< Bypass opening elevation (ft) */
    float fullHead;      /**< Full pool head */
    float depthForebay;  /**< Full pool forebay depth */
    float depthTailrace; /**< Nominal tailrace depth */

    int collector;        /**< Whether or not this dam is a collector dam.
                           * (i.e. it has PIT tag detectors and a
                           * different kind of bypass system which can
                           * produce latent effects) */

    float lengthBasin;   /**< Stilling basin length (this is part of the tailrace). */
    float specGrav;      /**< Specific gravity of roller */

    /* Spill */
    QList<float> spill;  /**< Spill fraction given at each
                          * time slice [DAM_SLICES_IN_SEASON]*/

    /** Planned spill vector is computed from planned_spill days/nights,
           then serves as input to compute_flow() [DAM_SLICES_IN_SEASON]*/
    QList<float> spillPlanned;

    FloatPeriodList *spillPlannedDay; /**< Planned spill during the day */
    FloatPeriodList *spillPlannedNight; /**< Planned spill during the night */

        /* These two variables are used to implement support for legacy tokens
         * planned_spill and fish_spill. Only planned_spill_day and
         * planned_spill_night should be used by new users and these legacy
         * tokens only set planned_spill_day and planned_spill_night. */
    FloatPeriodList *spillLegacyPlanned; /**< Planned spill during the day.
                           * This is used to implement support for legacy
                           * planned_spill tokens. */
    FloatPeriodList *spillLegacyFish; /**< Planned spill during the day. This
                           * is used to implement support for legacy
                           * fish_spill tokens. */

    float spillMax;      /**< Maximum unforced spill (kcfs) */
    Location spillSide;  /**< Side of the river of the spillway - Left bank or right bank */
    QString spillSideText;

    /* Removable spill weir */
    cmpRSW *spillWeir; /** RSW information. If there is no RSW, this is nullptr. */

    /* Flow */
//    float flowMax;       /**< For slider top-end */
    float flowProjectMin; /**< Flow project minimum is user data; used by modulators*/
    float flowRiverMin; /**< River flow minimum is calculated from river desc */
//    QList<float> flow;/**< Flow in KCFS given at each day [days_per_season]*/

    /* Transport info */
    cmpTransport *transport;/**< Pointer to transport operation information
                             * started at this dam. If not transport this is nullptr. */

    /* nsat stuff */
    cmpEquation *nsatEqn;    /**< Nitrogen supersaturation equation that
                           * determines the production of gas during day
                           * hours */
    cmpEquation *nsatNightEqn;/**< Nitrogen supersaturation equation during
                           * night hours */
    cmpEquation *nsatBackupEqn;/**< Nitrogen supersaturation equation for when
                           * the spill fraction drops out of the range
                           * in which the day and night equations
                           * reasonably model behavior */
    float k_entrain;      /**< Powerhouse side gas entrainment */

    /* actual depths, calculated from adjacent reaches, with drawdown */
    QList<float> depthForebayDay;/**< Forebay depth at each day [days_per_season]*/
    QList<float> depthTailraceDay;/**< Tailrace depth at each day [days_per_season]*/

    /* useful params, calculated during each simulation */
    QList<float> dropRatioDay;/**< Drop ratio. [days_per_season]*/
    QList<float> dropRatioDayTR;/**< Drop ratio for the tailrace.[days_per_season]*/
    QList<float> daylightProportion;/**< Proportion of
                           * the day that is light given at each dam time
                           * slice. [DAM_SLICES_IN_SEASON]*/

public slots:
    void allocateDays(int days, int slices);
    void calculateFlow () override;
    void calculateFlows ();
    void calculateTemp () override;
    void calculateTemps ();
    void calculateSpill ();
    void calculateFish() override;
    void calculateStats() override;

    void deleteSpill ();
};

#endif // CMPDAM_H
