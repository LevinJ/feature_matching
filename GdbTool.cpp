/*
 * GdbTool.cpp
 *
 *  Created on: Dec 12, 2018
 *      Author: aiways
 */

#include "GdbTool.h"
#include <algorithm>

using namespace std;
using namespace cv;

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

string mattype(cv::Mat m){
	return type2str(m.type());
}

std::string matstr(cv::Mat m){
	stringstream ss;
	ss<<"dims="<<m.dims<<",rows="<<m.rows<<",cols="<<m.cols;
	ss<<","<<mattype(m)<<" : ";
	ss<<m;
	string s = ss.str();
	s.erase(std::remove(s.begin(), s.end(), '\n'),
			s.end());
	return s;
}

std::string matrow(cv::Mat m, int n){
	return matstr(m.row(n));
}

std::string matcol(cv::Mat m, int n){
	return matstr(m.col(n));
}

void matimg(cv::Mat m){
	namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
	imshow( "Display window", m );                   // Show our image inside it.

	waitKey(0);
	cout<<"done with image showing"<<endl;
	destroyWindow("Display window");
	return;
}

std::string matele(cv::Mat mat, int row, int col){
	stringstream ss;
	const int type = mat.type();

	if(CV_32F == type){
		ss<<mat.at<float>(row, col);
	}else if(CV_64F == type){
		ss<<mat.at<double>(row, col);
	}else if(CV_8U == type){
		ss<<mat.at<char>(row, col);
	}
	return ss.str();
}
