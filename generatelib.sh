#!/bin/sh
gcc -m64 -shared -Wl,-soname,libcalibtagfinder.so.1 CalibTagFinder.o  ./PolygonApprox.o    -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_viz -lopencv_core -lpthread    -o libcalibtagfinder.so.1 

ln -s libcalibtagfinder.so.1 libcalibtagfinder.so

#cp...
