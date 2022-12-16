QT += testlib
QT -= gui

TARGET = tst_statistics

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
        ../Game/statistics.hh

HEADERS += \
        ../Course/CourseLib/interfaces/istatistics.hh

SOURCES +=  tst_statistics.cpp \
        ../Game/statistics.cpp

INCLUDEPATH += \
        ../Game/

INCLUDEPATH += \
    $$PWD/../Course/CourseLib

DEPENDPATH += \
    $$PWD/../Course/CourseLib
