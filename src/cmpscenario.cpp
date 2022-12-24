#include "cmpscenario.h"

cmpScenario::cmpScenario(QObject *parent) : QObject(parent)
{
    settings = nullptr;
    system = nullptr;
    postRivMethods = new QStringList();
    postRivMethodNames = new QStringList();
}

cmpScenario::cmpScenario(cmpSettings *settings, QObject *parent) : QObject(parent)
{
    setSettings(settings);
    system = nullptr;
    postRivMethods = new QStringList();
    postRivMethodNames = new QStringList();
}

cmpScenario::~cmpScenario()
{
    delete postRivMethods;
    delete postRivMethodNames;
}

void cmpScenario::setSettings(cmpSettings *sets)
{
    settings = sets;
}

cmpSettings *cmpScenario::getSettings()
{
    return settings;
}

void cmpScenario::setSystem(cmpRiverSystem *sys)
{
    system = sys;
}

cmpRiverSystem *cmpScenario::getSystem()
{
    return system;
}

void cmpScenario::readDescriptionFile()
{
    cmpFile *rDesc = new cmpFile(settings->getCommandSettings()->getRivDesc());
    if (rDesc->open(QIODevice::ReadOnly))
    {
        rDesc->readHeader();
        system->parseDesc(rDesc);
        system->construct();
    }
    else
    {
        emit canceled();
    }
}


// read in data files
void cmpScenario::readDataFiles()
{
    QStringList files = settings->getCommandSettings()->getFileNameList();
    cmpFile datafile;
    system->resetData();
    for (int i = 0, total = files.count(); i < total; i++)
    {
        datafile.setFileName(files.at(i));
        if (datafile.open(QIODevice::ReadOnly))
        {
            datafile.readHeader();
            datafile.readInfo();
            system->parseData(&datafile);
        }
    }
}

void refresh_data(){

}

// release stuff
int cmpScenario::getNumberReleases()
{
    return releases.count();
}

cmpRelease *cmpScenario::getRelease(int index)
{
    return releases.at(index);
}

void cmpScenario::deleteReleases()
{
    while (releases.count() > 1)
    {
        cmpRelease *rls = releases.takeLast();
        delete rls;
    }
}

// run the scenario
void cmpScenario::run()
{

}

void cmpScenario::runScenario()
{

}

void cmpScenario::runMonteCarlo()
{

}

void cmpScenario::runMonteCarloMV()
{

}

void cmpScenario::runRealTime()
{

}

// write any output files
void cmpScenario::outputData(QString filename, bool outputAll)
{
    cmpFile *outfile;
    if (fileNames->contains(filename, Qt::CaseInsensitive))
    {
        int index = fileNames->indexOf(filename);
        outfile = files.at(index);
    }
    else
    {
        outfile = new cmpFile(filename, this);
    }

    if (outfile->open(QIODevice::WriteOnly))
    {
        // Select what type of file to write by the filename extension.

        // Write a river description file.
        if (filename.endsWith(".desc", Qt::CaseInsensitive))
            writeDescData(outfile);

        // Write a control file which has includes for the other file types.
        //  Also, force writes of all the other file types.
        else if (filename.endsWith(".ctrl", Qt::CaseInsensitive))
        {
            QStringList *outfiles = writeCtrlData(outfile);
            for (int i = 0, total = outfiles->count(); i < total; i++)
                outputData(outfiles->at(i), outputAll);
        }
        // Write a river environment data file.
        else if (filename.endsWith(".riv", Qt::CaseInsensitive))
        {
            outfile->writeHeader(settings, "River Environment");
            outfile->writeInfo();
            writeDataSettings(outfile, outputAll);
            outfile->writeNewline();
            writeRiverYrData(outfile, outputAll);
        }
        // Write a dam operations data file.
        else if (filename.endsWith(".ops", Qt::CaseInsensitive))
        {
            outfile->writeHeader(settings, "Operations");
            outfile->writeInfo();
            writeDataSettings(outfile, outputAll);
            outfile->writeNewline();
            writeDamOpsData(outfile, outputAll);
        }
        // Write a calibration data file.
        else if (filename.endsWith(".clb", Qt::CaseInsensitive))
        {
            outfile->writeHeader(settings, "Calibration");
            outfile->writeInfo();
            writeCalibData(outfile, outputAll);
        }
        // Write a release data file.
        else if (filename.endsWith(".rls", Qt::CaseInsensitive))
        {
            outfile->writeHeader(settings, "Release");
            outfile->writeInfo();
            writeReleaseData(outfile, outputAll);
        }
        // Write a post bonneville data file.
        else if (filename.endsWith(".pbn", Qt::CaseInsensitive))
        {
            outfile->writeHeader(settings, "Post-River");
            outfile->writeInfo();
            writePostRivData(outfile, outputAll);
        }
        // Write an etc data file.
        else if (filename.endsWith(".etc", Qt::CaseInsensitive))
        {
            outfile->writeHeader(settings, "Configuration");
            outfile->writeInfo();
            writeConfigData(outfile, outputAll);
        }
        // Or, write out all data if extension not one of above, i.e.
        // scenario file (.scn or .dat, usually).
        else if (filename.endsWith(".scn", Qt::CaseInsensitive) || filename.endsWith("dat", Qt::CaseInsensitive))
        {
            outfile->writeHeader(settings, "Scenario");
            outfile->writeInfo();
            writeScnData(outfile, outputAll);
        }
        outfile->close();
    }
}

void cmpScenario::writeDescData(cmpFile *outfile)
{
    outfile->writeHeader(settings, "River Description");
    system->outputDesc(outfile);
}

void cmpScenario::writeDataSettings(cmpFile *outfile, bool outputAll)
{
    if (outputAll)
    {
    outfile->writeString(0, "migration", settings->getDataSettings()->getMigration()? "juvenile": "adult");
    outfile->writeValue(0, "days_in_season", settings->getDataSettings()->getNumDaysInSeason());
    outfile->writeValue(0, "night_start", settings->getDataSettings()->getNightStart());
    outfile->writeValue(0, "day_start", settings->getDataSettings()->getDayStart());
    outfile->writeValue(0, "time_steps_per_day", settings->getDataSettings()->getTimeStepsPerDay());
    outfile->writeValue(0, "dam_slices_per_day", settings->getDataSettings()->getDamSlicesPerDay());
    outfile->writeString(0, "compute_gas", settings->getDataSettings()->getCalcGas()? "On": "Off");
    outfile->writeString(0, "compute_growth", settings->getDataSettings()->getCalcGas()? "On": "Off");
    outfile->writeString(0, "compute_turbidity", settings->getDataSettings()->getCalcGas()? "On": "Off");
    }
    else
    {
        if (!settings->getDataSettings()->getMigration())
            outfile->writeString(0, "migration", "adult");
        outfile->writeValue(0, "days_in_season", settings->getDataSettings()->getNumDaysInSeason(), 366);
        outfile->writeValue(0, "night_start", settings->getDataSettings()->getNightStart(), 2000);
        outfile->writeValue(0, "day_start", settings->getDataSettings()->getDayStart(), 600);
        outfile->writeValue(0, "time_steps_per_day", settings->getDataSettings()->getTimeStepsPerDay(), 2);
        outfile->writeValue(0, "dam_slices_per_day", settings->getDataSettings()->getDamSlicesPerDay(), 2);
        if (settings->getDataSettings()->getCalcGas())
            outfile->writeString(0, "compute_gas", "On");
        if (settings->getDataSettings()->getCalcGas())
        outfile->writeString(0, "compute_growth", "On");
        if (settings->getDataSettings()->getCalcGas())
        outfile->writeString(0, "compute_turbidity", "On");
    }
}

void cmpScenario::writeReachClassNames(cmpFile *outfile)
{
    QStringList names(system->getReachClassNames());
    int total = names.count();
    outfile->writeValue(0, "num_reach_classes", total);
    for (int i = 0; i < total; i++)
        outfile->writeString(1, "reach_class_name", QString::number(i), names.at(i));
}

void cmpScenario::writeAllSpecies(cmpFile *outfile, bool outputAll)
{
    system->outputAllSpecies(outfile, outputAll);
}

void cmpScenario::writeAllStocks(cmpFile *outfile, bool outputAll)
{
    system->outputAllStocks(outfile, outputAll);
}


void cmpScenario::writeRunSettings(cmpFile *outfile, bool outputAll)
{
    outfile->writeValue(0, "gas_dissp_exp", settings->getDataSettings()->getGasDisspExp());
    outfile->writeValue(0, "hw_flow_prop", settings->getDataSettings()->getHwFlowProp());

    outfile->writeNewline();
    settings->getDataSettings()->getFreeFlowEqn()->writeData(outfile, 0, outputAll);
    outfile->writeNewline();

    outfile->writeValue(0, "ufree_max", settings->getDataSettings()->getFreeFlowMax()); // 8.0);
    outfile->writeValue(0, "prey_energy_density", settings->getDataSettings()->getPreyEnergyDensity());// 5400.000000
    outfile->writeValue(0, "length_weight_b0", settings->getDataSettings()->getLengthWeightB0());// -11.580000
    outfile->writeValue(0, "length_weight_b1", settings->getDataSettings()->getLengthWeightB1()); // 3.033000
    outfile->writeValue(0, "fork_threshold", settings->getDataSettings()->getForkThreshold());// 0.100000
    outfile->writeString(0, "water_travel_upper_segment",  settings->getDataSettings()->getWaterTravelUpperSegment(), "Little_Goose_Pool");
    outfile->writeString(0, "water_travel_lower_segment", settings->getDataSettings()->getWaterTravelUpperSegment(),  "Estuary");
    outfile->writeValue(0, "water_travel_first_day", settings->getDataSettings()->getWaterTravelFirstDay());// 1
    outfile->writeValue(0, "water_travel_last_day", settings->getDataSettings()->getWaterTravelLastDay());// 365
    outfile->writeValue(0, "min_migration_rate", settings->getDataSettings()->getMigrationRateMin());// 1.000000
    outfile->writeString(0, "suppress_variation", settings->getDataSettings()->getSuppressVariation()? "On": "Off");
    outfile->writeString(0, "pred_vol_interaction", settings->getDataSettings()->getPredVolInteraction()? "On": "Off");
    outfile->writeString(0, "age_dependent_fge", settings->getDataSettings()->getAgeDependentFge()? "On": "Off");
    outfile->writeString(0, "truncate_travel_vect", settings->getDataSettings()->getTruncateTravelVect()? "On": "Off");
    outfile->writeString(0, "truncate_res_survival", settings->getDataSettings()->getTruncateResSurvival()? "On": "Off");
    outfile->writeValue(0, "compute_mu_method", settings->getDataSettings()->getComputeMuMethod());// 0
    outfile->writeValue(0, "year_traveltime_indicator", settings->getDataSettings()->getYearTraveltimeIndicator());// 0

    outfile->writeString(0, "mortality_class", settings->getDataSettings()->getMortClassString());//custom
}

QStringList *cmpScenario::writeCtrlData(cmpFile *outfile)
{
    QString file = outfile->fileName().replace(".ctrl", ".riv", Qt::CaseInsensitive);
    outputFiles->clear();
    outputFiles->append(file);
    outputFiles->append(file.replace(".riv", ".ops", Qt::CaseInsensitive));
    outputFiles->append(file.replace(".ops", ".clb", Qt::CaseInsensitive));
    outputFiles->append(file.replace(".clb", ".rls", Qt::CaseInsensitive));
    outputFiles->append(file.replace(".rls", ".pbn", Qt::CaseInsensitive));
    outputFiles->append(file.replace(".pbn", ".etc", Qt::CaseInsensitive));
    outfile->writeHeader(settings, "Control");
    outfile->writeInfo();
    for (int i = 0; i < 6; i++)
        outfile->writeString(0, QString("include"), outputFiles->at(i));
    outfile->writeNewline();

    return outputFiles;
}

void cmpScenario::writeRiverYrData(cmpFile *outfile, bool outputAll)
{
    system->outputRiverYrData(outfile, outputAll);
}

void cmpScenario::writeDamOpsData(cmpFile *outfile, bool outputAll)
{
    system->outputDamOpsData(outfile, outputAll);
}

void cmpScenario::writeCalibData(cmpFile *outfile, bool outputAll)
{
}

void cmpScenario::writeReleaseData(cmpFile *outfile, bool outputAll)
{
    for (int i = 0, total = releases.count(); i < total; i++)
    {
        releases.at(i)->writeData(outfile, outputAll);
        outfile->writeNewline();
    }
}

void cmpScenario::writePostRivData(cmpFile *outfile, bool outputAll)
{
    system->outputPostRiverData(outfile, outputAll);
}

void cmpScenario::writeConfigData(cmpFile *outfile, bool outputAll)
{

}

void cmpScenario::writeScnData(cmpFile *outfile, bool outputAll)
{
    writeDataSettings(outfile, outputAll);
    outfile->writeNewline();
    writeReachClassNames(outfile);
    outfile->writeNewline();
    writeAllSpecies(outfile, outputAll);
    outfile->writeNewline();
    writeAllStocks(outfile, outputAll);
    outfile->writeNewline();
    writePostRivData(outfile, outputAll);
    outfile->writeNewline();
    writeRiverYrData(outfile, outputAll);
    outfile->writeNewline();
    writeDamOpsData(outfile, outputAll);
    outfile->writeNewline();
    writeRunSettings(outfile, outputAll);
    outfile->writeNewline();
    writeReleaseData(outfile, outputAll);
    outfile->writeNewline();
}


QStringList *cmpScenario::getPostRivMethods() const
{
    return postRivMethods;
}

void cmpScenario::setPostRivMethods()
{
    postRivMethods->append ("sar_vs_date");
    postRivMethods->append ("latent_mortality");
    postRivMethods->append ("constant_d");
    postRivMethods->append ("s3_vs_wtt");
    postRivMethodNames->append ("SAR vs. Date");
    postRivMethodNames->append ("Latent Mortality");
    postRivMethodNames->append ("Constant D");
    postRivMethodNames->append ("S3 vs Water Travel Time");
}
