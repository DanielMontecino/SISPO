#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;
 


 // lo saque de http://opencv-code.blogspot.com/2016/12/how-to-draw-circle-opencv-c.html
int main( )
{    
  // Create black empty images
  /*It basically defines the size of the window where circle would be drawn*/
  Mat image = Mat::zeros( 400, 400, CV_8UC3 );
  int i,x,y; 
  // Draw a circle 
  while(true)
  {
  cout<<"Enter the co-ordinates of the centre"<<endl;
  //Get value of centre from the user it should be 0<x<400 and 0<y<400
  //Bcoz size of window is (400,400)
  cin>>x>>y;
  //Display the co-ordinate of the centre in the form (x,y)
  cout<<"Co-ordinates of the centre is ("<<x<<","<<y<<")"<<endl;
  cout <<"Enter the value of radius"<<endl;
   //Take the value of centre from the user
  cin >>i;
  //Function for drawing the circle
  circle( image, Point( x, y ), i, Scalar( 0, 0, 255 ), 1, 8 );
  imshow("Image",image);
  waitKey( 1000 );
  }
  return(0);
}