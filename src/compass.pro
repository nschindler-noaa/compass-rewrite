QT       += widgets gui core charts

TARGET = compass
CONFIG   += console staticlib create_prl link_prl
CONFIG   -= app_bundle

TEMPLATE = app


HEADERS += \
    cmpbasin.h \
    cmpdam.h \
    cmpdamspecies.h \
    cmpequation.h \
    cmpequationdialog.h \
    cmpfile.h \
    cmpfishway.h \
    cmpheadwater.h \
    cmplog.h \
    cmpmath.h \
    cmpmontecarlomulti.h \
    cmppowerhouse.h \
    cmpreach.h \
    cmprealtime.h \
    cmprelease.h \
    cmpreleasesegmentdata.h \
    cmpreleasesite.h \
    cmpriver.h \
    cmpriverpoint.h \
    cmpriversegment.h \
    cmpriversystem.h \
    cmprsw.h \
    cmprunsettings.h \
    cmpsettings.h \
    cmpspecies.h \
    cmpspillway.h \
    cmpstock.h \
    cmptransport.h \
    cmptributary.h \
    cmpusersettings.h \
    help.h \
    CompassConsole.h \
    CompassGui.h \
    Log.h \
    transporttool.h \
    version.h \
    LogWidget.h \
    settings.h \
    HelpDialog.h \
    GraphDialog.h \
    ScheduleDialog.h \
    RiverSystem.h \
    FileManager.h \
    ScenarioManager.h \
    PowerHouse.h \
    Dam.h \
    Reach.h \
    Headwater.h \
    Period.h \
    Species.h \
    Stock.h \
    definitions.h \
    Equation.h \
    parseUtil.h \
    writeUtil.h \
    Transport.h \
    Release.h \
    dataConversion.h \
    MapWindow.h \
    IOWindow.h \
    information.h \
    Scenario.h \
    Results.h \
    equationdialog.h \
    releasetool.h \
    mainwindow.h \
    mapwidget.h \
    commondialog.h \
    releasedialog.h \
    damstructures.h

SOURCES += \
    cmpbasin.cpp \
    cmpdam.cpp \
    cmpdamspecies.cpp \
    cmpequation.cpp \
    cmpequationdialog.cpp \
    cmpfile.cpp \
    cmpfishway.cpp \
    cmpheadwater.cpp \
    cmplog.cpp \
    cmpmath.cpp \
    cmpmontecarlomulti.cpp \
    cmppowerhouse.cpp \
    cmpreach.cpp \
    cmprealtime.cpp \
    cmprelease.cpp \
    cmpreleasesegmentdata.cpp \
    cmpreleasesite.cpp \
    cmpriver.cpp \
    cmpriverpoint.cpp \
    cmpriversegment.cpp \
    cmpriversystem.cpp \
    cmprsw.cpp \
    cmprunsettings.cpp \
    cmpsettings.cpp \
    cmpspecies.cpp \
    cmpspillway.cpp \
    cmpstock.cpp \
    cmptransport.cpp \
    cmptributary.cpp \
    cmpusersettings.cpp \
    main.cpp \
    help.cpp \
    CompassConsole.cpp \
    CompassGui.cpp \
    Log.cpp \
    LogWidget.cpp \
    settings.cpp \
    HelpDialog.cpp \
    GraphDialog.cpp \
    ScheduleDialog.cpp \
    RiverSystem.cpp \
    FileManager.cpp \
    ScenarioManager.cpp \
    PowerHouse.cpp \
    Dam.cpp \
    Reach.cpp \
    Headwater.cpp \
    Period.cpp \
    Species.cpp \
    Stock.cpp \
    Equation.cpp \
    parseUtil.cpp \
    transporttool.cpp \
    writeUtil.cpp \
    Transport.cpp \
    Release.cpp \
    MapWindow.cpp \
    IOWindow.cpp \
    Scenario.cpp \
    Results.cpp \
    equationdialog.cpp \
    releasetool.cpp \
    mainwindow.cpp \
    mapwidget.cpp \
    commondialog.cpp \
    releasedialog.cpp \
    damstructures.cpp \
    dataconversion.cpp \
    definitions.cpp

FORMS += \
    CompassGui.ui \
    LogWidget.ui \
    HelpDialog.ui \
    GraphDialog.ui \
    ScheduleDialog.ui \
    MapWindow.ui \
    IOWindow.ui \
    cmpequationdialog.ui \
    equationdialog.ui \
    releasetool.ui \
    mainwindow.ui \
    mapwidget.ui \
    CommonDialog.ui \
    transporttool.ui

OTHER_FILES += \
    readme.txt

RESOURCES += \
    compass.qrc


