#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

/** @function main */
 int main( int argc, char** argv )
 {
   Point2f srcTri[3];
   Point2f dstTri[3];

   Mat rot_mat( 2, 3, CV_32FC1 );
   Mat warp_mat( 2, 3, CV_32FC1 );
  
   /// Set your 3 points to calculate the  Affine Transform
   srcTri[0] = Point2f( 0,0 );
   srcTri[1] = Point2f( 1, 0 );
   srcTri[2] = Point2f( 0, 1 );

   dstTri[0] = Point2f( 0.0, 0.33 );
   dstTri[1] = Point2f( 0.85, 0.25 );
   dstTri[2] = Point2f( 0.15, 0.7 );

   /// Get the Affine Transform
   warp_mat = getAffineTransform( srcTri, dstTri );

     /// Compute a rotation matrix with respect to the center of the image
   Point center = Point( 20, 15 );
   double angle = -50.0;
   double scale = 0.6;

   /// Get the rotation matrix with the specifications above
   rot_mat = getRotationMatrix2D( center, angle, scale );

  // Declare what you need
  FileStorage file("some_name.yml", FileStorage::WRITE);
  
  // Write to file!
  file << rot_mat;
  
  file.release();
  
     /// Wait until user exits the program
   waitKey(0);

   return 0;
  }