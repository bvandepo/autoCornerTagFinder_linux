#include <opencv2/opencv.hpp>

template<typename T> CvSeq*
icvApproxPolyDP( CvSeq* src_contour, int header_size,
                 CvMemStorage* storage, double eps );

CV_IMPL CvSeq*
cvApproxPoly( const void*  array, int  header_size,
              CvMemStorage*  storage, int  method,
              double  parameter, int parameter2 );
