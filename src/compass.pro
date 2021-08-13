QT       += widgets gui core charts

TARGET = compass
CONFIG   += console staticlib create_prl link_prl
CONFIG   -= app_bundle

TEMPLATE = app


HEADERS += \
    cmpequation.h \
    cmpequationdialog.h \
    help.h \
    CompassConsole.h \
    CompassGui.h \
    Log.h \
    RiverSegment.h \
    RiverPoint.h \
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
    Tributary.h \
    Headwater.h \
    Basin.h \
    Period.h \
    Species.h \
    Stock.h \
    definitions.h \
    Equation.h \
    parseUtil.h \
    writeUtil.h \
    Transport.h \
    Release.h \
    CompassFile.h \
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
    cmpequation.cpp \
    cmpequationdialog.cpp \
    main.cpp \
    help.cpp \
    CompassConsole.cpp \
    CompassGui.cpp \
    Log.cpp \
    RiverSegment.cpp \
    RiverPoint.cpp \
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
    Tributary.cpp \
    Headwater.cpp \
    Basin.cpp \
    Period.cpp \
    Species.cpp \
    Stock.cpp \
    Equation.cpp \
    parseUtil.cpp \
    transporttool.cpp \
    writeUtil.cpp \
    Transport.cpp \
    Release.cpp \
    CompassFile.cpp \
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


