QT += core gui widgets

CONFIG += c++11

TARGET = StudentRecordSystemQt
TEMPLATE = app

SOURCES += \
    main_qt.cpp \
    StudentSystemQt.cpp \
    Student.cpp \
    StudentManager.cpp

HEADERS += \
    StudentSystemQt.h \
    Student.h \
    StudentManager.h

