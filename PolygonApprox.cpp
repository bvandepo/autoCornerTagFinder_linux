
#include "PolygonApprox.h"

/*
BVDP:  This are copy of the two openCV functions in modules/imgproc/src/approx.cpp
template<typename T> static CvSeq* icvApproxPolyDP( CvSeq* src_contour, int header_size,  CvMemStorage* storage, double eps )
CV_IMPL CvSeq* cvApproxPoly

in order to try to improve the polygonization for  quadrangles
*/
/****************************************************************************************\
*                               Polygonal Approximation                                  *
\****************************************************************************************/

/* Ramer-Douglas-Peucker algorithm for polygon simplification */

/* the version for integer point coordinates */
template<typename T> CvSeq*
icvApproxPolyDP( CvSeq* src_contour, int header_size,
                 CvMemStorage* storage, double eps )
{
    typedef cv::Point_<T> PT;
    int             init_iters = 3;
    CvSlice         slice;
    slice.start_index=0;
    slice.end_index=0;
    //= {0, 0},
    CvSlice         right_slice;
    right_slice.start_index=0;
    right_slice.end_index=0;
    //= {0, 0};
    CvSeqReader     reader, reader2;
    CvSeqWriter     writer;
    PT              start_pt(-1000000, -1000000), end_pt(0, 0), pt(0,0);
    int             i = 0, j, count = src_contour->total, new_count;
    int             is_closed = CV_IS_SEQ_CLOSED( src_contour );
    bool            le_eps = false;
    CvMemStorage*   temp_storage = 0;
    CvSeq*          stack = 0;
    CvSeq*          dst_contour;

    assert( CV_SEQ_ELTYPE(src_contour) == cv::DataType<PT>::type );
    cvStartWriteSeq( src_contour->flags, header_size, sizeof(pt), storage, &writer );

    if( src_contour->total == 0  )
        return cvEndWriteSeq( &writer );

    temp_storage = cvCreateChildMemStorage( storage );

    assert( src_contour->first != 0 );
    stack = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvSlice), temp_storage );
    eps *= eps;
    cvStartReadSeq( src_contour, &reader, 0 );

    if( !is_closed )
    {
        right_slice.start_index = count;
        end_pt = *(PT*)(reader.ptr);
        start_pt = *(PT*)cvGetSeqElem( src_contour, -1 );

        if( start_pt.x != end_pt.x || start_pt.y != end_pt.y )
        {
            slice.start_index = 0;
            slice.end_index = count - 1;
            cvSeqPush( stack, &slice );
        }
        else
        {
            is_closed = 1;
            init_iters = 1;
        }
    }

    if( is_closed )
    {
        /* 1. Find approximately two farthest points of the contour */
        right_slice.start_index = 0;

        for( i = 0; i < init_iters; i++ )
        {
            double dist, max_dist = 0;
            cvSetSeqReaderPos( &reader, right_slice.start_index, 1 );
            CV_READ_SEQ_ELEM( start_pt, reader );   /* read the first point */

            for( j = 1; j < count; j++ )
            {
                double dx, dy;

                CV_READ_SEQ_ELEM( pt, reader );
                dx = pt.x - start_pt.x;
                dy = pt.y - start_pt.y;

                dist = dx * dx + dy * dy;

                if( dist > max_dist )
                {
                    max_dist = dist;
                    right_slice.start_index = j;
                }
            }

            le_eps = max_dist <= eps;
        }

        /* 2. initialize the stack */
        if( !le_eps )
        {
            slice.start_index = cvGetSeqReaderPos( &reader );
            slice.end_index = right_slice.start_index += slice.start_index;

            right_slice.start_index -= right_slice.start_index >= count ? count : 0;
            right_slice.end_index = slice.start_index;
            if( right_slice.end_index < right_slice.start_index )
                right_slice.end_index += count;

            cvSeqPush( stack, &right_slice );
            cvSeqPush( stack, &slice );
        }
        else
            CV_WRITE_SEQ_ELEM( start_pt, writer );
    }

    /* 3. run recursive process */
    while( stack->total != 0 )
        //bvdp: try to do it only once
    {
        cvSeqPop( stack, &slice );

        cvSetSeqReaderPos( &reader, slice.end_index );
        CV_READ_SEQ_ELEM( end_pt, reader );

        cvSetSeqReaderPos( &reader, slice.start_index );
        CV_READ_SEQ_ELEM( start_pt, reader );

        if( slice.end_index > slice.start_index + 1 )
        {
            double dx, dy, dist, max_dist = 0;

            dx = end_pt.x - start_pt.x;
            dy = end_pt.y - start_pt.y;

            assert( dx != 0 || dy != 0 );

            for( i = slice.start_index + 1; i < slice.end_index; i++ )
            {
                CV_READ_SEQ_ELEM( pt, reader );
                dist = fabs((pt.y - start_pt.y) * dx - (pt.x - start_pt.x) * dy);

                if( dist > max_dist )
                {
                    max_dist = dist;
                    right_slice.start_index = i;
                }
            }

            le_eps = max_dist * max_dist <= eps * (dx * dx + dy * dy);
        }
        else
        {
            assert( slice.end_index > slice.start_index );
            le_eps = true;
            /* read starting point */
            cvSetSeqReaderPos( &reader, slice.start_index );
            CV_READ_SEQ_ELEM( start_pt, reader );
        }

        if( le_eps )
        {
            CV_WRITE_SEQ_ELEM( start_pt, writer );
        }
        else
        {
            right_slice.end_index = slice.end_index;
            slice.end_index = right_slice.start_index;
            cvSeqPush( stack, &right_slice );
            cvSeqPush( stack, &slice );
        }
    }

    is_closed = CV_IS_SEQ_CLOSED( src_contour );
    if( !is_closed )
        CV_WRITE_SEQ_ELEM( end_pt, writer );

    dst_contour = cvEndWriteSeq( &writer );

    // last stage: do final clean-up of the approximated contour -
    // remove extra points on the [almost] stright lines.

    cvStartReadSeq( dst_contour, &reader, is_closed );
    CV_READ_SEQ_ELEM( start_pt, reader );

    reader2 = reader;
    CV_READ_SEQ_ELEM( pt, reader );

    new_count = count = dst_contour->total;
    for( i = !is_closed; i < count - !is_closed && new_count > 2; i++ )
    {
        double dx, dy, dist, successive_inner_product;
        CV_READ_SEQ_ELEM( end_pt, reader );

        dx = end_pt.x - start_pt.x;
        dy = end_pt.y - start_pt.y;
        dist = fabs((pt.x - start_pt.x)*dy - (pt.y - start_pt.y)*dx);
        successive_inner_product = (pt.x - start_pt.x) * (end_pt.x - pt.x) +
                (pt.y - start_pt.y) * (end_pt.y - pt.y);

        if( dist * dist <= 0.5*eps*(dx*dx + dy*dy) && dx != 0 && dy != 0 &&
                successive_inner_product >= 0 )
        {
            new_count--;
            *((PT*)reader2.ptr) = start_pt = end_pt;
            CV_NEXT_SEQ_ELEM( sizeof(pt), reader2 );
            CV_READ_SEQ_ELEM( pt, reader );
            i++;
            continue;
        }
        *((PT*)reader2.ptr) = start_pt = pt;
        CV_NEXT_SEQ_ELEM( sizeof(pt), reader2 );
        pt = end_pt;
    }

    if( !is_closed )
        *((PT*)reader2.ptr) = pt;

    if( new_count < count )
        cvSeqPopMulti( dst_contour, 0, count - new_count );

    cvReleaseMemStorage( &temp_storage );
    return dst_contour;
}




CV_IMPL CvSeq*
cvApproxPoly( const void*  array, int  header_size,
              CvMemStorage*  storage, int  method,
              double  parameter, int parameter2 )
{
    CvSeq* dst_seq = 0;
    CvSeq *prev_contour = 0, *parent = 0;
    CvContour contour_header;
    CvSeq* src_seq = 0;
    CvSeqBlock block;
    int recursive = 0;

    if( CV_IS_SEQ( array ))
    {
        src_seq = (CvSeq*)array;
        if( !CV_IS_SEQ_POLYLINE( src_seq ))
            CV_Error( CV_StsBadArg, "Unsupported sequence type" );

        recursive = parameter2;

        if( !storage )
            storage = src_seq->storage;
    }
    else
    {
        src_seq = cvPointSeqFromMat(
                    CV_SEQ_KIND_CURVE | (parameter2 ? CV_SEQ_FLAG_CLOSED : 0),
                    array, &contour_header, &block );
    }

    if( !storage )
        CV_Error( CV_StsNullPtr, "NULL storage pointer " );

    if( header_size < 0 )
        CV_Error( CV_StsOutOfRange, "header_size is negative. "
                                    "Pass 0 to make the destination header_size == input header_size" );

    if( header_size == 0 )
        header_size = src_seq->header_size;

    if( !CV_IS_SEQ_POLYLINE( src_seq ))
    {
        if( CV_IS_SEQ_CHAIN( src_seq ))
        {
            CV_Error( CV_StsBadArg, "Input curves are not polygonal. "
                                    "Use cvApproxChains first" );
        }
        else
        {
            CV_Error( CV_StsBadArg, "Input curves have uknown type" );
        }
    }

    if( header_size == 0 )
        header_size = src_seq->header_size;

    if( header_size < (int)sizeof(CvContour) )
        CV_Error( CV_StsBadSize, "New header size must be non-less than sizeof(CvContour)" );

    if( method != CV_POLY_APPROX_DP )
        CV_Error( CV_StsOutOfRange, "Unknown approximation method" );

    while( src_seq != 0 )
    {
        CvSeq *contour = 0;

        switch (method)
        {
        case CV_POLY_APPROX_DP:
            if( parameter < 0 )
                CV_Error( CV_StsOutOfRange, "Accuracy must be non-negative" );

            if( CV_SEQ_ELTYPE(src_seq) == CV_32SC2 )
                contour = icvApproxPolyDP<int>( src_seq, header_size, storage, parameter );
            else
                contour = icvApproxPolyDP<float>( src_seq, header_size, storage, parameter );
            break;
        default:
            assert(0);
            CV_Error( CV_StsBadArg, "Invalid approximation method" );
        }

        assert( contour );

        if( header_size >= (int)sizeof(CvContour))
            cvBoundingRect( contour, 1 );

        contour->v_prev = parent;
        contour->h_prev = prev_contour;

        if( prev_contour )
            prev_contour->h_next = contour;
        else if( parent )
            parent->v_next = contour;
        prev_contour = contour;
        if( !dst_seq )
            dst_seq = prev_contour;

        if( !recursive )
            break;

        if( src_seq->v_next )
        {
            assert( prev_contour != 0 );
            parent = prev_contour;
            prev_contour = 0;
            src_seq = src_seq->v_next;
        }
        else
        {
            while( src_seq->h_next == 0 )
            {
                src_seq = src_seq->v_prev;
                if( src_seq == 0 )
                    break;
                prev_contour = parent;
                if( parent )
                    parent = parent->v_prev;
            }
            if( src_seq )
                src_seq = src_seq->h_next;
        }
    }

    return dst_seq;
}



