#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <iostream>
#include <limits>
#include <numeric>
#include <stdio.h>

using namespace cv;
using namespace std;
 
// We need 4 corresponding 2D points(x,y) to calculate homography.
vector<Point2f> topview_image;      // Stores 4 points(x,y) of the logo image. Here the four points are 4 corners of image.
vector<Point2f> perspective__image;    // stores 4 points that the user clicks(mouse left click) in the main image.
 
// Image containers for main and logo image
Mat imageMain;
Mat imageLogo;
 
// Function to add main image and transformed logo image and show final output.
// Icon image replaces the pixels of main image in this implementation.
void showFinal(Mat src1,Mat src2)
{
 
    Mat gray,gray_inv,src1final,src2final;
    cvtColor(src2,gray,CV_BGR2GRAY);
    threshold(gray,gray,0,255,CV_THRESH_BINARY);
    //adaptiveThreshold(gray,gray,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,5,4);
    bitwise_not ( gray, gray_inv );
    src1.copyTo(src1final,gray_inv);
    src2.copyTo(src2final,gray);
    Mat finalImage = src1final+src2final;
    namedWindow( "output", WINDOW_AUTOSIZE );
    imshow("output",finalImage);
    cvWaitKey(0);
 
}
 
// Here we get four points from the user with left mouse clicks.
// On 5th click we output the overlayed image.
void on_mouse( int e, int x, int y, int d, void *ptr )
{
   
    if (e == EVENT_LBUTTONDOWN )
    {
    
        if(perspective__image.size() < 4)
        {
	    perspective__image.push_back(Point2f(float(x),float(y)));
            cout << x << " "<< y <<endl;
	    
        }
                
        else
        {
	 // imshow( "Perspective view", imageLogo );
	  //setMouseCallback("Perspective view",on_mouse2, NULL );
           // cout << " Select 4 corresponding Points in Topview  " <<endl;
         //  cvDestroyWindow("Display Window");
	  //cvReleaseImage("Display Window");
        }
 
    }
}
 
 void on_mouse2( int e, int x, int y, int d, void *ptr)
{
   
    if (e == EVENT_LBUTTONDOWN )
    {
    
        if(topview_image.size() < 4)
        {
	    topview_image.push_back(Point2f(float(x),float(y)));
            cout << x << " "<< y <<endl;
	    
        }
                
        else
        {
            cout << " Calculating Homography " <<endl;
            // Deactivate callback
            cv::setMouseCallback("Display window", NULL, NULL);
            // once we get 4 corresponding points in both images calculate homography matrix
          //  Mat H = findHomography(topview_image,perspective__image,0 );
	   //   Mat H = findHomography(perspective__image, topview_image,0);
	      Mat H = findHomography(perspective__image, topview_image,CV_RANSAC);
	    FileStorage file("CameraMatrix.txt", FileStorage::WRITE);
  
	    // Write to file!
	    file <<"Matrix"<< H;
	    //Calculate inverted Matrix
	    Mat inverse=H.inv();
	   // cout << " Inverted Camera Matrix "<< inverse <<endl;
	     cout << " Calculate Inverted Camera Matrix " <<endl;
	    file <<"Inverted Matrix"<< inverse;
	    file.release();
	    // store input POI in vector
	     vector<Point2f> src;
	    src.push_back(Point(132,345));
	     // calculate new points and give output
	    vector<Point2f> dst2;
	    perspectiveTransform(src, dst2, H);
	    cout << "Input-POI: p" << 1 << "(" << src << ")\t POI in realworld: p" << 1 << "(" << dst2 << ")" << endl;  
	    Mat logoWarped;
	   // Mat ResizedTopview;
            // Warp the logo image to change its perspective
            //warpPerspective(imageLogo,logoWarped,H,imageMain.size() );
	    warpPerspective(imageMain,logoWarped,H,imageLogo.size() );
	    //resize(imageLogo, ResizedTopview , imageMain.size(),0,0, INTER_LINEAR )
	    imwrite( "/home/ip-linux/Documents/Sport-Tracker/CameraMatrix_extraction/Homography and Project/Warp_Result2.jpg", logoWarped );
	    Mat merged;
	    addWeighted(logoWarped, 0.8,imageLogo , 0.2, 0.0, merged);
	   imwrite( "/home/ip-linux/Documents/Sport-Tracker/CameraMatrix_extraction/Homography and Project/Warp_Overlay.jpg", merged );
	   showFinal(imageLogo,logoWarped);
	    
           }
 
    }
}
 
int main( int argc, char** argv )
{
//  We need tow argumemts. "Main image" and "logo image"
    if( argc != 3 )
    {
        cout <<" Usage: Executable (PerspectiveImage) (Topview-image)" << endl;
        return -1;
    }
 
 
// Load images from arguments passed.
    imageMain = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    imageLogo = imread(argv[2], CV_LOAD_IMAGE_COLOR);
// Push the 4 corners of the logo image as the 4 points for correspondence to calculate homography.
//    topview_image.push_back(Point2f(float(0),float(0)));
//    topview_image.push_back(Point2f(float(0),float(imageLogo.rows)));
//    topview_image.push_back(Point2f(float(imageLogo.cols),float(imageLogo.rows)));
//    topview_image.push_back(Point2f(float(imageLogo.cols),float(0)));
  namedWindow( "Top view", WINDOW_AUTOSIZE );// Create a window for display.
   imshow( "Top view", imageLogo );
 
 
    namedWindow( "Perspective view", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Perspective view", imageMain );
    setMouseCallback("Perspective view",on_mouse, NULL );
    cout <<"Click 4 times in perspective image, then close window, then select 4 corresponding points in top view image and confirm with 1 mouse click into the image"<<endl;
    
  
   setMouseCallback("Top view",on_mouse2, NULL );
   
//  Press "Escape button" to exit
  //  while(1)
   // {
    //    int key=cvWaitKey(10);
    //    if(key==27) break;
   // }
    
 waitKey(0);


    return 0;
}