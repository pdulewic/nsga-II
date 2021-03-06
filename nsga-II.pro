#-------------------------------------------------
#
# Project created by QtCreator 2017-05-05T14:37:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = nsga-II
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/qcustomplot.cpp \
    src/solution.cpp \
    src/nsga.cpp \
    src/range_dialog.cpp \
    src/pareto_table.cpp \
    src/pareto_dialog.cpp

HEADERS  += inc/mainwindow.h \
    inc/qcustomplot.h \
    inc/solution.h \
    inc/nsga.h \
    inc/constants.h \
    inc/range_dialog.h \
    inc/exprtk.h \
    inc/pareto_table.h \
    inc/pareto_dialog.h
