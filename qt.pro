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

SOURCES += main.cpp CalibTagFinder.cpp
HEADERS += CalibTagFinder.h
DISTFILES += pictures.txt

#CONFIG+=OPENCV2411LOCAL
CONFIG+=OPENCV31SYSTEM

OPENCV2411LOCAL {
OPENCV_PATH =/media/HD500GO/saveHDDgarossos/developpement/openCV/opencv-2.4.11/
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
LIBS+= -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_calib3d
}

OPENCV31SYSTEM {
OPENCV_PATH =/usr/local/
INCLUDEPATH += $${OPENCV_PATH}/include/

#LIBS +=  -L$${OPENCV_PATH}/lib
#LIBS+= -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_calib3d -lopencv_imgcodecs -lopencv_videoio

#LIBS+= `pkg-config --libs opencv`
#foireux car pkg-config  ajoute -lippicv qui n'est pas installé en pratique
LIBS+=-L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_viz -lopencv_core


}

INCLUDEPATH += /usr/include/
INCLUDEPATH += ./
MAKE_LIBDIR     += /usr/local/lib/


#INCLUDEPATH += /usr/local/include
#INCLUDEPATH += /usr/local/include/opencv2
#INCLUDEPATH += /home/bvandepo/Bureau/developpement/openCV/opencv-2.4.9/include/opencv
#INCLUDEPATH += /home/bvandepo/Bureau/developpement/openCV/opencv-2.4.9/include/opencv2
#QMAKE_LIBDIR     += /usr/local/MATLAB/R2014a/bin/glnxa64/
#LIBS+= -L/usr/local/lib/ -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_calib3d
#OPENCV_PATH =/usr/local/

