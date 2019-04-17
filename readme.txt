readme.txt

=============================================================================
COMPASS v 2.8.9
=============================================================================

This file contains information on
1. Compatibility
2. Known issues
3. History
4. Redistribution
5. Copyright

Please report any problems, inconsistencies, or improvment ideas
to Neal Schindler (Neal.Schindler(at)noaa.gov).

=============================================================================
Compatibility:
-----------------------------------------------------------------------------
This program will run on Vista and Win7, Win8 and Win10.

=============================================================================
Known Issues:
-----------------------------------------------------------------------------
1. The Help system is disabled since much of the information is incorrect.
2. When saving a file in DOS/Windows, the "Original Filename:" is not part  
   of comments.
3. IO Table values may not appear until after running a scenario.
4. Flows and Temperatures for Headwaters are not editable in GUI.
5. Red dots don't always show when a tool changes data.
6. Graphs and map do not print.
7. Copy/Paste in spreadsheet view does not work.
8. File and directory names in preprocessor must have no spaces.

=============================================================================
History:
-----------------------------------------------------------------------------
Changes for version 2.8.9 (03/2019)
-------------------------------
1. Added capability to read different number of gas values per day.

Changes for version 2.8.8 (03/2019)
-------------------------------
1. Corrected calibration output dealing with travel time.

Changes for version 2.8.7 (02/2019)
-------------------------------
1. Rsim2cmpss now records gas correctly (supersat and non-negative).

Changes for version 2.8.6 (02/2019)
-------------------------------
1. Moved location of COMPASS temporary file.
2. Corrected output of help and version in batch mode.
3. Corrected a circular include statement.
4. Changed to current dll libraries.

Changes for version 2.8.5 (02/2019)
-------------------------------
1. Rsim2cmpss can now accomodate bad data.

Changes for version 2.8.4 (02/2019)
-------------------------------
1. Fish releases output correctly.

Changes for version 2.8.3 (02/2019)
-------------------------------
1. Fixed reading of flow.archive files (datmaker.dat)
2. time_steps_per_day and dam_slices_per_day now output all the time.
3. Rsim2cmpss working.

Changes for version 2.8.2 (01/2019)
-------------------------------
1. fixed assignment of default runMode.day_start (using leading 0 converted it
   to base 8).

Changes for version 2.8.1 (11/2018):
-------------------------------
1. Rewrote is_night to return a bool (and made other necessary changes).
2. Corrected how TIME_INLINE is used.
3. Cleaned up some extra definitions of is_night().

Changes for version 2.8.0 (11/2018):
-------------------------------
1. Moved to Windows 10 and Qt 5.11 with MinGW 5.30.
2. Added variable night start and end for spill only.

Changes for version 2.7.24 (10/2018):
-------------------------------
1. Fixed error routine for having no releases.
2. Fixed error routine for including fake river segments.

Changes for version 2.7.23 (07/2018):
-------------------------------
1. Changed reach model to only use the influence of steps i to (j-1) in
   survival calculations.

Changes for version 2.7.22 (07/2018):
-------------------------------
1. Changed gas mortality equation default values to 0.

Changes for version 2.7.21 (06/2018):
-------------------------------
1. Corrected determination of zero spill.
2. Changed name of internal variable dam_spill to fish_spill_exp.

Changes for version 2.7.20 (06/2018):
-------------------------------
1. Corrected determination of spill usage for rz_model.
2. Changed float array passed for down stream dam spill.
3. Minor change to down stream dam spill determination.
4. Minor change to up stream fish spill - no longer using dam probabilities
   for top segment.

Changes for version 2.7.19 (05/2018):
-------------------------------
1. Fixed calculation of calibration values.
2. Fixed survival calculation when using spill.

Changes for version 2.7.18 (05/2018):
-------------------------------
1. Changed how zero proportion spill is calculated for efficiency.
2. Corrected EQ_MIGR12 calculation

Changes for version 2.7.17 (05/2018):
-------------------------------
1. Fixed reading of dam spill equations (long-standing bug).

Changes for version 2.7.16b (05/2018):
-------------------------------
1. Fixed bug in calculating migration when vvar = 0.
2. Corrected calculation of calib values.
3. Changed default parameter 1 for equations 66 and 67.
4. Added version number in summary.dat

Changes for version 2.7.15b (05/2018):
-------------------------------
1. Fixed bug where fish die in bypass if transport is off.
2. Fixed bug where fish die in RSW if more than one period is specified.
3. Set VVAR to print for every reach class every time.
4. Created a way to get the proportion of zero spill days over a range of days.
5. Changed cum_data calculations in rz_model to include data at step i:
   (cum_data[j] - cum_data[i-1]) / (j - i + 1)
6. Changed comments in eqns_dflt.cpp to match documentation.
7. Changed migration equations 24, 40, 41, 52, 59, and 60 to match memo
   from Dan Widener 2018-05-21.
8. Changed surival equations 66 and 67 per memo from Dan Widener 2018-05-08.

Changes for version 2.7.14b (04/2018):
-------------------------------
1. Fixed file list to reflect reality.
2. Added earlier version information to datafile_readme.txt
3. Changed some comments and some informational messages.
4. Corrected datafile version in fileIoUtils.cpp
5. Corrected initialization of runMode.
6. Added resize to Post-Bonneville page.
7. Changed "Processing release..." message to plain message instead of debug.

Changes for version 2.7.13b (12/2017):
-------------------------------
1. Fixed transport start day (again!). The GUI has problems when reading files
   that are not on the lists of files.
2. Added copyright notice in header files and separate copyright.txt

Changes for version 2.7.12 (11/2017):
-------------------------------
1. Fixed release tool in GUI.

Changes for version 2.7.11 (11/2017):
-------------------------------
1. Fixed collection start day.

Changes for version 2.7.10 (10/2017):
-------------------------------
1. Fixed collection efficiency to be 1.0 when not collecting.

Changes for version 2.7.9 (09/2017):
-------------------------------
1. Fixed input gas in reach overwriting calculated gas. (Allocated even when
   no input gas present)

Changes for version 2.7.8 (06/2017):
-------------------------------
1. Fixed another hidden array. In this case, it's the period list for the
   additional powerhouses which were not being redimensioned.

Changes for version 2.7.7 (06/2017):
-------------------------------
1. Fixed another crash on exit (stock-mb1fac).

Changes for version 2.7.7 (06/2017):
-------------------------------
1. Many internal changes which may reduce memory requirements.
2. Fixed crash on exit (stock-covmat[]-covar).

Changes for version 2.7.5b (06/2017):
-------------------------------
1. Fixed problem with travel vector maximum, it is now calculated using
   days_in_season as well as steps_per_day.
2. Upper limit for all float arrays is changed to days_in_season - 1.
3. If a transport end date is not given, it is set to the earlier of
   the end date of the transport active array, or days_in_season - 1.
4. The Release Tool is renamed to Release View Tool. Constructing a release
   is discouraged.

Changes for version 2.7.4b (06/2017):
-------------------------------
1. Fixed problem checking period lists.

Changes for version 2.7.3b (05/2017):
-------------------------------
1. Reworked dam_model.cpp internally to make it easier to understand.
   Results matched to .01 fish.

Changes for version 2.7.2b (05/2017):
-------------------------------
1. Fixed a problem with transports that failed to use the separation
   probability.

Changes for version 2.7.1b (05/2017):
-------------------------------
1. Implemented variable for days in season. This allows season to be greater
   or less than one year. Number of values in arrays must reflect this.

Changes for version 2.6.10b (01/2017):
-------------------------------
1. Corrected reading files for monte-carlo multivariate mode.
2. Created separate functions for reset_data and refresh_data.
3. Created separate function to calculate process variation or sample
   error. This is now applied after survival rather than as part of the
   process which simplifies the calculation.

Changes for version 2.6.9b (12/2016):
-------------------------------
1. Added time travel offset parameter and term to MIGR11, eqn 52, and MIGR12,
   eqn 59. Added year_timetravel_offset to runMode flags, noted as
   year_timetravel_indicator in input file (.scn or .dat).

Changes for version 2.6.8b (12/2016):
-------------------------------
1. Fixed problems with MIGR14, Eq #69. Corrected the calculation of the
   migration factor and the equation itself, and made sure it was being used.

Changes for version 2.6.7b (11/2016):
-------------------------------
1. Fixed problem with multiple releases.
2. Fixed problem in GUI with differing time steps.

Changes for version 2.6.6b (11/2016):
-------------------------------
1. Fixed real time output/crashing/memory leak.

Changes for version 2.6.5b (11/2016):
-------------------------------
1. Corrected summary output and printing an array when all values = 0.

Changes for version 2.6.4b (10/2016):
-------------------------------
1. Fixed loss of data in summary.dat.
2. Re-enabled "message repeated" message.
3. Fixed internal arrays to be dynamically allocated for steps per day.
4. Created internal functions for transport collection, unknown segment
   fish passage, and delayed mortality table calculations.

Changes for version 2.6.3b (9/2016):
-------------------------------
1. Fixed 'nan' problem when migration hits end of season/year.
2. Changed how truncate_travel_vector works: now, it aggregates < .0005 into
   one step and  > .9995 into one step. It still limits each reach vector to
   120 days, causing those fish to be censored.
3. Censored fish are now produced at the end of season whether or not the
   vector is truncated.

Changes for version 2.6.2b (9/2016):
-------------------------------
1. Fixed problem with initializing stocks and species.
2. Now reads run flags separately.
3. Dam passage output when flag set.

Changes for version 2.6.1b (8/2016):
-------------------------------
1. Changed a lot of internal arrays from static to dynamic dimensioning.
2. Changed global stocks from array to QList.
3. Implemented new initializer for run_mode (includes time steps, etc.).
4. Completed merge of different flavors of COMPASS: sixteen-steps, turbidity,
   and growth.
5. Debugged growth routine and averaging problem.

Changes for version 2.6.0b (8/2016):
-------------------------------
1. Added base16.dat to datafiles.
2. Added a warning when turning off growth due to missing values.
3. Corrected miles/day calculation in reach day 1 message.
4. Collected common code in parsing river segments.
5. Changed mortality classes to enum instead of defines.
6. Added the following run settings to the datafile .riv or .scn:
     compute_growth On/Off
     compute_turbidity On/Off
   Added the following run settings which are not yet implemented.
     time_steps_per_day x - power of 2, less or equal to dam_slices
     dam_slices_per_day x - power of 2
7. Moved compute_gas On/Off to beginning of file.
8. Changed runtime_settings from number to individual flags (they can be
   specified either way for backward compatibility): These are advanced
   options and should only be changed by someone familiar with the program.
     suppress_variation On
     pred_vol_interaction Off
     age_dependent_fge Off
     truncate_travel_vect On
     truncate_res_survival On
9. Added new migration equation, 39 "MIGR14", specifically for the
   Sacramento River. It is based on date and flow.
10. Changed datafile version to 11 for the above changes.

Changes for version 2.5.7b (6/2016):
-------------------------------
1. Added parameters for temp and temp^2 to migration equation 13 (EQ_MIGR12),
   number 59. Parameter 5 is multiplied by temp and parameter 6 is multiplied
   by temp^2.

Changes for version 2.5.6b (6/2016):
-------------------------------
1. Added a new migration calculation in rz_model. The calculation of mu can
   now be dependent on lower dam spill and sluiceway probabilities. In the
   input file,add a line 'compute_mu_method 1' to use it, default or 0 will
   compute it the old way, using upper dam spill experience.
2. Changed error check for elevation change in reaches, it no longer aborts
   after setting bad elevation delta value to 0.
3. Fixed reading of [int:int] value construct.
4. Fixed some memory leaks.
5. Fixed output of mu_values.csv and mort_values.csv when not in calibration
   mode.

Changes for version 2.5.5b (5/2016):
-------------------------------
1. Fixed treatment of point pointers - gui doesn't crash.

Changes for version 2.5.4b (5/2016):
-------------------------------
1. Fixed reading array with [*] value.

Changes for version 2.5.3b (4/2016):
-------------------------------
1. Fixed scaling for Migration Eq 12 (EQ_MIGR11) 52.
2. Replaced Macro MB1FAC_INTERNAL with a function.
3. Fixed output of summary.dat when RT_SCENARIO mode.

Changes for version 2.5.2b (3/2016):
-------------------------------
1. Added flag to realtime output to allow step-wise values.
2. Changed mode to run scenario when running realtime scenario mode.
3. Corrected values that were hard-coded instead of relying on
   STEPS_PER_DAY: Loop limits, min_migr_rate, arrival limit.

Changes for version 2.5.1b (3/2016):
-------------------------------
1. Rehosted on nsfsc github.
2. set number of steps per day and dam slices per day to 16.
3. Fixed a small issue with step_time() and made an inline version.

Changes for version 2.3.46 (3/2016):
-------------------------------
1. Fixed deleting gui Release Tool.

Changes for version 2.3.46b (3/2016):
-------------------------------
1. Fixed gui Release Tool.

Changes for version 2.3.45b (2/2016):
-------------------------------
1. Corrected default values for preprocess.
2. Changed help output for compass_pp to match current behavior.
3. New .ops, .riv, and .rls data files for 1997-2015.

Changes for version 2.3.44b (2/2016):
-------------------------------
1. Debug preprocess, added options to compass_pp.exe, compass_fm.exe, and
   compass_pp3.exe.
2. Made some changes to be compatible with Qt5.

Changes for version 2.3.43b (1/2016):
-------------------------------
1. Set default arguments for Monte Carlo mode and writing summary file.
2. Set steps per day and dam slices per day to 2.

Changes for version 2.3.42x16 (1/2016):
-------------------------------
1. Fixed output of ufree_max.
2. Fixed gui asking for river.desc file.
3. Changed all DAM_TIME_SLICE occurences to DAM_TIME_SLC.

Changes for version 2.3.41x16 (1/2016):
-------------------------------
1. Experimental only: Changed Steps per day to 16, and Dam Slices per day to 16.

Changes for version 2.3.41x (12/2015):
-------------------------------
1. Experimental only: Changed Steps per day to 8, and Dam Slices per day to 8.

Changes for version 2.3.40 (10/2015):
-------------------------------
1. Added gas to calibration values, average of (gas * #fish) per step.
2. Cleaned up unused code.

Changes for version 2.3.39 (09/2015):
-------------------------------
1. Fixed comment when flow_coefficient is missing or wrong.
2. Added day squared parameter to equation 52 (EQ_MIGR11) to
   correspond to Fisheries Model 12.

Changes for version 2.3.38 (07/2015):
-------------------------------
1. Fixed output of reach class variables with -oa option.

Changes for version 2.3.37 (07/2015):
-------------------------------
1. Fixed output of multiple reach class values.
2. Fixed zoom controls for IOTool and MapTool.
3. Added drop-down menu of dams in scheduling dialog.
4. Fixed the showGrid button.
5. Corrected verbose help output.
6. Hid unecessary icons/menu options.
7. User name is now added as creator and/or modifier of files when saved.

Changes for version 2.3.36 (06/2015):
-------------------------------
1. Fixed command line option -u.
2. Fixed crash in monte carlo mode.

Changes for version 2.3.35 (06/2015):
-------------------------------
1. Added a spill probability factor to delay equation 3 (46).

Changes for version 2.3.34 (05/2015):
-------------------------------
1. Changed how files are selected in the Multiple Scenario Tool.
2. Fixed Multiple Scenario mode.

Changes for version 2.3.33 (04/2015):
-------------------------------
1. Changed message "No releases in run" from Error to Message.
2. Corrected directory when mode is realtime monte.

Changes for version 2.3.32 (04/2015):
-------------------------------
1. Created fish and predator cumulative density vectors to match flow
   and temp vectors.

Changes for version 2.3.31 (03/2015):
-------------------------------
1. Corrected problems in gui files since switching to new IDE/compiler.
2. Corrected computation of fish and predator density in rz_model.cpp.
3. Corrected flow archive file reading.

Changes for version 2.3.30 (01/2015):
-------------------------------
1. Corrected an allocation error in reach_model.cpp
2. Changed some equation defaults for NSAT equations.

Changes for version 2.3.29 (12/2014):
-------------------------------
1. Implemented a new look-up on segment name so that a data for a segment
   named N_F_Clearwater will be assigned to North_Fork_Clearwater. A debug
   message displays segment names as they are found.

Changes for version 2.3.28 (12/2014):
-------------------------------
1. Fixed segfault on exit.
2. Changed handling of unknown or "fake" reaches. It now does not create
   a reach object at the end of the list. 
3. Improved messages about unknown reaches.

Changes for version 2.3.27 (12/2014):
-------------------------------
1. Changed computation of base gas mortality to be more like high gas
   mortality and combined the code into one file and streamlined the code.
2. Fixed code calculating Ndiff so that it is always right_gas - left_gas.
3. Changed look up exp table to actual computed values.

Changes for version 2.3.26 (12/2014):
-------------------------------
1. Fixed fraction_side in gas_out for dams in dam_model.cpp.

Changes for version 2.3.25 (11/2014):
-------------------------------
1. Fixed index for water elevation change (elev_change) in gas_mort.cpp (it
   is in days instead of steps).
2. Changed gas threshold in calculating mortality.
3. Added debug code to output dam spill fraction and reach gas values for left
   and right to assist in calibration - only active in the debug build.
4. Made changes in order to build GUI in Qt Creator (not finished).

Changes for version 2.3.24 (10/2014):
-------------------------------
1. Changed precision for velocity output. 

Changes for version 2.3.23 (10/2014):
-------------------------------
1. Changed computing reach depth for gas mortality to a per section (slug)
   basis if upper and lower depths differ. 

Changes for version 2.3.22 (10/2014):
-------------------------------
1. Changed gas effects from a mortality to a survival basis (1 - mort) since
   all fish experience it on every step. 

Changes for version 2.3.21 (10/2014):
-------------------------------
1. Changed default for reach gas theta to 0.075
2. Changed defaults for fd_dbot to 36.0 and fd_dmode to 12.0 (fish density)

Changes for version 2.3.20 (10/2014):
-------------------------------
1. Changed where defaults for reach classes are set to get rid of seg fault.

Changes for version 2.3.19 (10/2014):
-------------------------------
1. Tightened up internal code

Changes for version 2.3.18 (09/2014):
-------------------------------
1. Added command line option, -oa, to output all values to a data file
   including default values.

Changes for version 2.3.17 (09/2014):
-------------------------------
1. Changed default nsat equation for all dams (backup_nsat_eq).

Changes for version 2.3.16 (09/2014):
-------------------------------
1. Fixed selection of new custom survival equations.
2. Fixed seg fault when cleaning up model.

Changes for version 2.3.15 (09/2014):
-------------------------------
1. Fixed selection of new custom survival equations.

Changes for version 2.3.14 (08/2014):
-------------------------------
1. Fixed calculation of new custom survival equations.
2. Fixed start up of Monte Carlo mode.

Changes for version 2.3.13 (08/2014):
-------------------------------
1. Added new custom survival equations: 66 and 67.
2. Added a geometry check for reach side slope. If sides cross above 
   river bed, it will generate an error and crash.

Changes for version 2.3.12 (08/2014):
-------------------------------
1. Fixed new FGE and Spill Efficiency equations.

Changes for version 2.3.11 (08/2014):
-------------------------------
1. Fixed problem with parsing compute_gas and mortality_type flags.

Changes for version 2.3.10 (07/2014):
-------------------------------
1. Implemented new FGE and Spill Efficiency equations.

Changes for version 2.3.9 (07/2014):
-------------------------------
1. Turned on gas mortality for mortality class "custom".

For previous changes, see readme.archive file.

=============================================================================
Redistribution:
-----------------------------------------------------------------------------
The unaltered install executable may be redistributed with this notice.

=============================================================================
Copyright:
-----------------------------------------------------------------------------
This is a work of the U.S. Government and is not subject to copyright
protection in the United States. Foreign copyrights may apply.
See copyright.txt for more information.

Department of Commerce,
	NOAA Northwest Fisheries Science Center, 
		Fish Ecology Division

2019-03-26
