#!/bin/bash
#download the images for testing:
wget http://homepages.laas.fr/bvandepo/files/autoCornerTagFinder_linux/inputImages.zip
unzip inputImages.zip
rm inputImages.zip

#create empty directories
mkdir -p cToMatlab
mkdir -p outputImages
mkdir -p timer

