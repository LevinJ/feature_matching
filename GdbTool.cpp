/*
 * GdbTool.cpp
 *
 *  Created on: Dec 12, 2018
 *      Author: aiways
 */

#include "GdbTool.h"
#include <algorithm>
#include <random>

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
static std::vector<std::string> split_str(const std::string &s, char delim) {
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
		// elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
	}
	return elems;
}
static void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if  ( event == EVENT_LBUTTONDOWN )
	{
		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if  ( event == EVENT_RBUTTONDOWN )
	{
		cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if  ( event == EVENT_MBUTTONDOWN )
	{
		cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if ( event == EVENT_MOUSEMOVE )
	{
		cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;

	}
}

void matimg(cv::Mat m, string text=""){
	string win_name =  "AIWAYS SLAM Image Viewer";
	namedWindow( win_name, WINDOW_AUTOSIZE );// Create a window for display.

	Mat outImg;
	m.copyTo(outImg);
	if(m.cols > 2000){
		cv::resize(m, outImg, cv::Size(), 0.6, 0.6);
	}
	int height = outImg.rows;
	int width = outImg.cols;
	Mat blank_area = Mat::zeros(height/2, width, outImg.type());
	if(text!=""){
		int y0 = 20;
		int dy = 30;
		int i = 0;

		for(auto &text_line: split_str(text, '\n')){
			int y = y0 + i*dy;
			cv::putText(blank_area, text_line, cv::Point(5,y), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0,255,0), 1);
			i++;
		}
	}
	Mat res_img;
	vconcat(outImg,blank_area,res_img);

	//set the callback function for any mouse event
	//	 setMouseCallback(win_name, CallBackFunc, NULL);

	while(true){
		imshow(win_name, res_img );
		int key = cv::waitKey(1);
		if(key == -1) continue;
		cout<<"key="<<key<<endl;
		if(key == 1048693){
			continue;
		}else if(key == 1048677){
			break;
		}
	}

	//	     radius = input('Input radius'
	// Show our image inside it.

	//	waitKey(0);
	cout<<"done with image showing"<<endl;
	destroyWindow(win_name);
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

std::vector< DMatch > gen_rnd_matches(const std::vector< DMatch > &matches, int n){
	if(n==-1) return matches;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<int> distribution(0,matches.size());

	std::vector< DMatch > res;
	for(int i=0; i<n; i++){
		int ind = distribution(gen);
		res.push_back(matches[ind]);
	}
	return res;
}

std::vector< DMatch > g_matches;

void draw_matches(Mat img1, const std::vector<KeyPoint>& keypoints1,
		Mat img2, const std::vector<KeyPoint>& keypoints2,
		const std::vector<DMatch>& matches1to2, int n){
	Mat img_match;
	g_matches = gen_rnd_matches(matches1to2, n);
	stringstream ss;
	for(const auto match : g_matches){
		//query iamge
		ss<<"["<<keypoints1[match.queryIdx].pt.x<<","<<keypoints1[match.queryIdx].pt.y<<"]";
		ss<<",size="<<keypoints1[match.queryIdx].size<<",angle="<<keypoints1[match.queryIdx].angle<<",response="<<keypoints1[match.queryIdx].response;

		//train image
		ss<<"\n["<<keypoints2[match.trainIdx].pt.x<<","<<keypoints2[match.trainIdx].pt.y<<"]";
		ss<<",size="<<keypoints2[match.trainIdx].size<<",angle="<<keypoints2[match.trainIdx].angle<<",response="<<keypoints2[match.trainIdx].response;

		//distance
		ss<<"\nd="<<match.distance<<endl;

	}
	cout<<ss.str()<<endl;
	drawMatches ( img1, keypoints1, img2, keypoints2, g_matches, img_match,
			Scalar::all(-1),Scalar::all(-1),std::vector<char>(),DrawMatchesFlags::DEFAULT| DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	matimg(img_match, ss.str());

}
