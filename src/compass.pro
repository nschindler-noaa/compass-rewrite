QT       += widgets gui core #charts

TARGET = compass
CONFIG   += console staticlib create_prl link_prl
CONFIG   -= app_bundle

TEMPLATE = app


HEADERS += \
    cmpbasin.h \
    cmpcommandsettings.h \
    cmpconsole.h \
    cmpdam.h \
    cmpdamspecies.h \
    cmpdatasettings.h \
    cmpequation.h \
    cmpfile.h \
    cmpfishway.h \
    cmpgasdistribution.h \
    cmpheadwater.h \
    cmplog.h \
    cmpmainwindow.h \
    cmpmath.h \
    cmpmontecarlomulti.h \
    cmppowerhouse.h \
    cmpreach.h \
    cmpreachclass.h \
    cmprealtime.h \
    cmprelease.h \
    cmpreleasesegmentdata.h \
    cmpreleasesite.h \
    cmpriver.h \
    cmpriverpoint.h \
    cmpriversegment.h \
    cmpriversystem.h \
    cmprsw.h \
    cmpscenario.h \
    cmpsettings.h \
    cmpspecies.h \
    cmpspillway.h \
    cmpstock.h \
    cmptransport.h \
    cmptributary.h \
    cmpusersettings.h \
    parseUtil.h \
    version.h

SOURCES += \
    cmpbasin.cpp \
    cmpcommandsettings.cpp \
    cmpconsole.cpp \
    cmpdam.cpp \
    cmpdamspecies.cpp \
    cmpdatasettings.cpp \
    cmpequation.cpp \
    cmpfile.cpp \
    cmpfishway.cpp \
    cmpgasdistribution.cpp \
    cmpheadwater.cpp \
    cmplog.cpp \
    cmpmainwindow.cpp \
    cmpmath.cpp \
    cmpmontecarlomulti.cpp \
    cmppowerhouse.cpp \
    cmpreach.cpp \
    cmpreachclass.cpp \
    cmprealtime.cpp \
    cmprelease.cpp \
    cmpreleasesegmentdata.cpp \
    cmpreleasesite.cpp \
    cmpriver.cpp \
    cmpriverpoint.cpp \
    cmpriversegment.cpp \
    cmpriversystem.cpp \
    cmprsw.cpp \
    cmpscenario.cpp \
    cmpsettings.cpp \
    cmpspecies.cpp \
    cmpspillway.cpp \
    cmpstock.cpp \
    cmptransport.cpp \
    cmptributary.cpp \
    cmpusersettings.cpp \
    main.cpp \
    parseUtil.cpp

FORMS += \
    cmpmainwindow.ui

OTHER_FILES += \
    readme.txt

RESOURCES += \
    compass.qrc


