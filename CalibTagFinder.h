
// Defines
#define MAX_CONTOUR_APPROX  7

// Definition Contour Struct
typedef struct CvContourEx
{
    CV_CONTOUR_FIELDS()
    int counter;
}
CvContourEx;


// Definition Corner Struct
typedef struct CvCBCorner
{
    CvPoint2D32f pt;					// X and y coordinates
    int row;							// Row and column of the corner
    int column;							// in the found pattern
    bool needsNeighbor;					// Does the corner require a neighbor?
    int count;							// number of corner neighbors
    struct CvCBCorner* neighbors[4];	// pointer to all corner neighbors
}
CvCBCorner;


// Definition Quadrangle Struct
// This structure stores information about the chessboard quadrange
typedef struct CvCBQuad
{
    int count;							// Number of quad neihbors
    int group_idx;						// Quad group ID
    float edge_len;						// Smallest side length^2
    CvCBCorner *corners[4];				// Coordinates of quad corners
    struct CvCBQuad *neighbors[4];		// Pointers of quad neighbors
    bool labeled;						// Has this corner been labeled?
}
CvCBQuad;

class CalibTagFinder
{

public:
    CalibTagFinder();

    int cvFindChessboardCorners3( const void* arr);
    //===========================================================================
    // FUNCTION PROTOTYPES
    //===========================================================================
private:
    int icvGenerateQuads( CvCBQuad **quads, CvCBCorner **corners,
                          CvMemStorage *storage, CvMat *image, int flags,
                          bool firstRun );

    void mrFindQuadNeighbors2( CvCBQuad *quads, int quad_count, int dilation);

    int mrAugmentBestRun( CvCBQuad *new_quads, int new_quad_count, int new_dilation,
                          CvCBQuad **old_quads, int old_quad_count, int old_dilation );

    int icvFindConnectedQuads( CvCBQuad *quads, int quad_count, CvCBQuad **quad_group,
                               int group_idx,
                               CvMemStorage* storage);

    void mrLabelQuadGroup( CvCBQuad **quad_group, int count, CvSize pattern_size,
                           bool firstRun );

    void mrCopyQuadGroup( CvCBQuad **temp_quad_group, CvCBQuad **out_quad_group,
                          int count );

    int icvCleanFoundConnectedQuads( int quad_count, CvCBQuad **quads,
                                     CvSize pattern_size );

    int mrWriteCorners( CvCBQuad **output_quads, int count, CvSize pattern_size,
                        int min_number_of_corners, CvMat *image=NULL );



    int determineQuadCode( CvCBQuad *quads, int res, IplImage *image,IplImage* imageRect,bool VisualizeResultsB, IplImage* imageDebugColor);
    //determineQuadCode( CvCBQuad *quads, int res, CvMat *image);


    //attributes
    bool ShowFinalImage;
    bool SaveFinalImage;
    bool ShowIntermediateImages;
    bool SaveIntermediateImagesForDebug;
    bool VisualizeResults;
    bool SaveTimerInfo;

    CvSize board_size;
    CvSize img_size;
    int min_number_of_corners;

    int elem_size;
    CvPoint2D32f* image_points_buf;

    int detectedCornersCount;

public:
    void setBoard_size_width(int argValue){
        board_size.width=argValue;
    }
    void setBoard_size_height(int argValue){
        board_size.height=argValue;
    }
    void setMin_number_of_corners(int argValue){
        min_number_of_corners=argValue;
    }
    void setImgSize(CvSize s){
        img_size =s;
    }
};
