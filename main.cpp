/************************************************************************************\
    This is improved variant of chessboard corner detection algorithm that
    uses a graph of connected quads. It is based on the code contributed
    by Vladimir Vezhnevets and Philip Gruebele.
    Here is the copyright notice from the original Vladimir's code:
    ===============================================================

    The algorithms developed and implemented by Vezhnevets Vldimir
    aka Dead Moroz (vvp@graphics.cs.msu.ru)
    See http://graphics.cs.msu.su/en/research/calibration/opencv.html
    for detailed information.

    Reliability additions and modifications made by Philip Gruebele.
    <a href="mailto:pgruebele@cox.net">pgruebele@cox.net</a>

    His code was adapted for use with low resolution and omnidirectional cameras
    by Martin Rufli during his Master Thesis under supervision of Davide Scaramuzza, at the ETH Zurich. Further enhancements include:
        - Increased chance of correct corner matching.
        - Corner matching over all dilation runs.

If you use this code, please cite the following articles:

1. Scaramuzza, D., Martinelli, A. and Siegwart, R. (2006), A Toolbox for Easily Calibrating Omnidirectional Cameras, Proceedings of the IEEE/RSJ International Conference on Intelligent Robots and Systems  (IROS 2006), Beijing, China, October 2006.
2. Scaramuzza, D., Martinelli, A. and Siegwart, R., (2006). "A Flexible Technique for Accurate Omnidirectional Camera Calibration and Structure from Motion", Proceedings of IEEE International Conference of Vision Systems  (ICVS'06), New York, January 5-7, 2006.
3. Rufli, M., Scaramuzza, D., and Siegwart, R. (2008), Automatic Detection of Checkerboards on Blurred and Distorted Images, Proceedings of the IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS 2008), Nice, France, September 2008.

\************************************************************************************/


// Includes
#include <cstdlib>
#include <iostream>

#include <opencv2/opencv.hpp>

#include <stdio.h>
#include <string.h>
#include <time.h>

#include <fstream>
using namespace std;
using std::ifstream;

#include "CalibTagFinder.h"


//launch with these arguments  -w 8 -h 8 -m 64 pictures.txt
/*
je configure dans Qt comme arguments au lancement:
 -w 8 -h 8 -m 64 pictures.txt

et en fait il converti le -h en --help si je demande à qt de lancer dans un terminal....

app launched via: /media/HD500GO/zodiac/matlab/fisheye/autoCornerTagFinder_linux/FindCorners.exe -w 8 8 -m 64 pictures.txt --help
Appuyez sur <ENTRÉE> pour fermer cette fenêtre...

donc il faudra changer le choix de la taille de mire de -h  vers autre chose...

  */
//===========================================================================
// MAIN LOOP 
//===========================================================================
int main( int argc, char** argv )
{

    // Initializations


    const char* input_filename		= 0;
    CvCapture* capture				= 0;
    FILE* f							= 0;
    char imagename[1024];

    int found						= -2;
    input_filename					= "pictures.txt";
    //input_filename					= "myVideo2.avi";


    cout<<"FindCorners.exe modified by BVDP"<<"\n";
    cout << __DATE__ << endl << __TIME__ << endl;
    cout<<"app launched via: ";
    for (int j=0;j<argc;j++)
        cout<<argv[j]<<" ";
    cout<<"\n";

    // Create error message file
    ofstream error("outputImages/error.txt");

    CalibTagFinder* calibTagFinder=new CalibTagFinder();
    calibTagFinder->setShowFinalImage(true);
    calibTagFinder->setSaveFinalImage(true);
    calibTagFinder->setShowIntermediateImages(false);
    calibTagFinder->setSaveIntermediateImages(true);

    ////////////////////////////PARSING////////////////////////////////////////////
    // Read the "argv" function input arguments
    for(int i = 1; i < argc; i++ )
    {
        const char* s = argv[i];
        if( strcmp( s, "--help" ) == 0 )
        {
            cout <<"this will be helping someday...\n";
        }
        if( strcmp( s, "-w" ) == 0 )
        {
            unsigned int argValue;
            if( sscanf( argv[++i], "%u", &argValue) != 1 || argValue <= 0 )
            {
                error << "Invalid board width" << endl;
                error.close();
                return -1;
            }
            else
                calibTagFinder->setBoard_size_width(argValue);
        }
        else if( strcmp( s, "-h" ) == 0 )
        {
            unsigned int argValue;
            if( sscanf( argv[++i], "%u", &argValue) != 1 || argValue <= 0 )
            {
                error << "Invalid board height" << endl;
                error.close();
                return -1;
            }
            else
                calibTagFinder->setBoard_size_height(argValue);
        }
        else if( strcmp( s, "-m" ) == 0 )
        {
            unsigned int argValue;
            if( sscanf( argv[++i], "%u", &argValue ) != 1 )
            {
                error << "Invalid minimal number of corners" << endl;
                error.close();
                return -1;
            }
            else
                calibTagFinder->setMin_number_of_corners(argValue);
        }
        else if( s[0] != '-' )
            input_filename = s;
        else
        {
            error << "Unknown option" << endl;
            error.close();
            return -1;
        }
    }
    // Close error message file
    error.close();
    //////////////////////IMAGE READOUT//////////////////////////////////////////
    // Figure out what kind of image input needs to be prepared
    if( input_filename )
    {
        // Try to open a video sequence
        //capture = cvCreateFileCapture( input_filename ); //OBRAND commented out
        if( !capture )
        {
            // Try to open an input image
            f = fopen( input_filename, "rt" );
            if( !f )
                return fprintf( stderr, "The input file could not be opened\n" ), -1;
        }
    }
    else
        // Open a live video stream
        capture = cvCreateCameraCapture(0);

    // Nothing could be opened -> error
    if( !capture && !f )
        return fprintf( stderr, "Could not initialize video capture\n" ), -2;

    //image_points_seq = cvCreateSeq( 0, sizeof(CvSeq), elem_size, storage );
    //////////////////////PROCESS ALL IMAGES////////////////////////////////////////
    // For loop which goes through all images specified above
    for(int j = 1;; j++)
    {
        // Initializations
        IplImage *view = 0;//, *view_gray = 0;
        CvSize text_size; text_size.width=0;text_size.height=0;
        //int base_line = 0;
        // Load the correct image...
        if( f && fgets( imagename, sizeof(imagename)-2, f ))
        {
            int l = (int) strlen(imagename);
            if( l > 0 && imagename[l-1] == '\n' )
                imagename[--l] = '\0';
            if( l > 0 )
            {
                if( imagename[0] == '#' )
                    continue;
                cout<<"attempt to load "<< imagename <<"\n";

                // Load as BGR 3 channel image
                view = cvLoadImage( imagename, 1 );
                // Currently the following file formats are supported:
                // Windows bitmaps				BMP, DIB
                // JPEG files					JPEG, JPG, JPE
                // Portable Network Graphics	PNG
                // Portable image format		PBM, PGM, PPM
                // Sun rasters					SR, RAS
                // TIFF files					TIFF, TIF
                // NOTABLY: GIF IS NOT SUPPORTED!
            }
        }


        // ...Or capture the correct frame from the video
        else if( capture )
        {
            IplImage* view0 = cvQueryFrame( capture );
            if( view0 )
            {
                view = cvCreateImage( cvGetSize(view0), IPL_DEPTH_8U, view0->nChannels );
                if( view0->origin == IPL_ORIGIN_BL )
                    cvFlip( view0, view, 0 );
                else
                    cvCopy( view0, view );
            }
        }

        // If no more images are to be processed -> break
        if( !view)
        {
            break;
        }
        // If esc key was pressed -> break
        int key = cvWaitKey(10);
        if( key == 27)
        {
            break;
        }

        //TODO: bvdp to remove later....
        system("rm pictureVis/*.ppm");

        found = calibTagFinder->cvFindChessboardCorners3(view);
         IplImage* imageDebugColor=  calibTagFinder->getImageDebugColor();
         cvNamedWindow( "Final Result", 1 );
         cvShowImage( "Final Result", imageDebugColor);
         cvWaitKey(0);

        if( !view )
            break;
        cvReleaseImage( &view );
    }
    delete calibTagFinder;
    if( capture )
        cvReleaseCapture( &capture );
    cout<<"return value"<< found<<"\n";

    return found;
}
