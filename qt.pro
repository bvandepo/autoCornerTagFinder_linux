#-------------------------------------------------
#
# Project created by QtCreator 2016-05-25T20:53:02
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = FindCorners.exe
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = ./
OBJECTS_DIR = ./
INCLUDEPATH += /usr/include/eigen3/
INCLUDEPATH += ./
INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/opencv2
MAKE_LIBDIR     += /usr/local/lib/


#INCLUDEPATH += /home/bvandepo/Bureau/developpement/openCV/opencv-2.4.9/include/opencv
#INCLUDEPATH += /home/bvandepo/Bureau/developpement/openCV/opencv-2.4.9/include/opencv2
#QMAKE_LIBDIR     += /usr/local/MATLAB/R2014a/bin/glnxa64/


LIBS+= -L/usr/local/lib/ -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_calib3d

SOURCES +=   main.cpp cvcalibinit3.cpp
HEADERS += cvcalibinit3.h

