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




#INCLUDEPATH += /home/bvandepo/Bureau/developpement/openCV/opencv-2.4.9/include/opencv
#INCLUDEPATH += /home/bvandepo/Bureau/developpement/openCV/opencv-2.4.9/include/opencv2
#QMAKE_LIBDIR     += /usr/local/MATLAB/R2014a/bin/glnxa64/


#LIBS+= -L/usr/local/lib/ -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_calib3d
LIBS+= -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_calib3d

SOURCES +=   main.cpp cvcalibinit3.cpp
HEADERS += cvcalibinit3.h



OPENCV_PATH =/media/HD500GO/saveHDDgarossos/developpement/openCV/opencv-2.4.11/
#OPENCV_PATH =/usr/local/


INCLUDEPATH += $${OPENCV_PATH}/include/
INCLUDEPATH += $${OPENCV_PATH}/include/opencv2/
INCLUDEPATH += $${OPENCV_PATH}/modules/highgui/include/
INCLUDEPATH += $${OPENCV_PATH}/modules/core/include/
INCLUDEPATH += $${OPENCV_PATH}/modules/hal/include/
INCLUDEPATH += $${OPENCV_PATH}/modules/imgproc/include/
INCLUDEPATH += $${OPENCV_PATH}/modules/imgcodecs/include/
INCLUDEPATH += $${OPENCV_PATH}/modules/videoio/include/
INCLUDEPATH += $${OPENCV_PATH}/modules/core/
INCLUDEPATH += $${OPENCV_PATH}/modules/contrib/include/
INCLUDEPATH += $${OPENCV_PATH}/modules/features2d/include/
INCLUDEPATH += $${OPENCV_PATH}/modules/objdetect/include/
INCLUDEPATH += $${OPENCV_PATH}/modules/calib3d/include/

LIBS +=  -L$${OPENCV_PATH}/build/lib

INCLUDEPATH += /usr/include/eigen3/
INCLUDEPATH += ./
#INCLUDEPATH += /usr/local/include
#INCLUDEPATH += /usr/local/include/opencv2
MAKE_LIBDIR     += /usr/local/lib/
