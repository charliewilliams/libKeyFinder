#*************************************************************************
#
# Copyright 2011-2014 Ibrahim Sha'ath
#
# This file is part of LibKeyFinder.
#
# LibKeyFinder is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# LibKeyFinder is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with LibKeyFinder.  If not, see <http://www.gnu.org/licenses/>.
#
#*************************************************************************

cache()

QT -= gui
CONFIG -= qt

TARGET = keyfinder
TEMPLATE = lib

VERSION = 2.0.0

DEFINES += LIBKEYFINDER_LIBRARY

HEADERS += \
    audiodata.h \
    binode.h \
    chromagram.h \
    chromatransform.h \
    chromatransformfactory.h \
    constants.h \
    exception.h \
    fftadapter.h \
    keyclassifier.h \
    keyfinder.h \
    lowpassfilter.h \
    lowpassfilterfactory.h \
    spectrumanalyser.h \
    temporalwindowfactory.h \
    toneprofiles.h \
    windowfunctions.h \
    workspace.h

SOURCES += \
    audiodata.cpp \
    chromagram.cpp \
    chromatransform.cpp \
    chromatransformfactory.cpp \
    fftadapter.cpp \
    keyclassifier.cpp \
    keyfinder.cpp \
    lowpassfilter.cpp \
    lowpassfilterfactory.cpp \
    spectrumanalyser.cpp \
    temporalwindowfactory.cpp \
    toneprofiles.cpp \
    windowfunctions.cpp \
    workspace.cpp \
    constants.cpp

OTHER_FILES += README

macx{
  QMAKE_MAC_SDK = macosx10.9
  DEPENDPATH += /usr/local/lib
  INCLUDEPATH += /usr/local/include
  CONFIG -= ppc ppc64
  CONFIG += x86 x86_64
# installs
  QMAKE_LFLAGS_SONAME  = -Wl,-install_name,/usr/local/lib/
  headers.path = /usr/local/include/$$TARGET
  headers.files = $$HEADERS
  INSTALLS += headers
}

unix|macx{
  LIBS += -lfftw3 -lboost_system -lboost_thread
}

win32{
  INCLUDEPATH += C:/minGW32/local/include
  DEPENDPATH += C:/minGW32/local/bin
  LIBS += -LC:/minGW32/local/bin
  LIBS += -LC:/minGW32/local/lib
  LIBS += -lfftw3-3 -lboost_system-48-mt-1_52 -lboost_thread-48-mt-1_52
}

# the rest auto generated by Qt Creator...

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE1558240
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = LibKeyFinder.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}
