/*
 * DebugUtil.cpp
 *
 *  Created on: Dec 14, 2018
 *      Author: aiways
 */

#include "DebugUtil.h"
#include <algorithm>
#include <random>

using namespace std;
using namespace cv;

class FeatureMatchUpdater: public KeyHandler{
public:
	FeatureMatchUpdater(Mat img1, const std::vector<KeyPoint>& keypoints1,
			Mat img2, const std::vector<KeyPoint>& keypoints2,
			const std::vector<DMatch>& matches1to2, int n,DebugUtil * dbg_tuil):m_img1(img1),
			m_keypoints1(keypoints1),m_img2(img2),m_keypoints2(keypoints2),
			m_matches1to2(matches1to2),m_n(n),m_dbg_tuil(dbg_tuil){

	}
	DebugUtil * m_dbg_tuil;
	Mat m_img1;
	const std::vector<KeyPoint>& m_keypoints1;
	Mat m_img2;
	const std::vector<KeyPoint>& m_keypoints2;
	const std::vector<DMatch>& m_matches1to2;
	int m_n;
	virtual void update_img(int key){
		if(key == m_dbg_tuil->m_update_key){
			cout<<"update key pressed"<<endl;
			MatchImageAndDisciption res = m_dbg_tuil->generate_match_img(m_img1, m_keypoints1,m_img2, m_keypoints2,m_matches1to2, m_n);
			m_img = res.m_img;
			m_text = res.m_text;
		}
	};
};
DebugUtil::DebugUtil() {
	// TODO Auto-generated constructor stub
	m_matimg_disp_ration = 0.6;
	m_update_key = 1048693;
}
static void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	KeyHandler  *p_key_handler = (KeyHandler *)userdata;
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
		x = x / p_key_handler->m_resize_ratio;
		y = y / p_key_handler->m_resize_ratio;
		if(x>p_key_handler->m_original_single_img_width){
			x = x - p_key_handler->m_original_single_img_width;
		}

		stringstream ss;
		ss<<"mouse pos x="<<x<<",y="<<y<<endl;
		p_key_handler->m_mouse_pos = ss.str();

	}
}
void DebugUtil::matimg_internal(cv::Mat m, std::string text, KeyHandler *p_key_handler){
	KeyHandler &key_handler = *p_key_handler;
	string win_name =  "AIWAYS SLAM Image Viewer";
	namedWindow( win_name, WINDOW_AUTOSIZE );// Create a window for display.
	key_handler.m_img = m;
	key_handler.m_text = text;
	//set the callback function for any mouse event
	setMouseCallback(win_name, CallBackFunc, (void *)&key_handler);
	while(true){
		Mat res_img;
		key_handler.m_img.copyTo(res_img);

		if(m.cols > 2000){
			cv::resize(key_handler.m_img, res_img, cv::Size(), m_matimg_disp_ration, m_matimg_disp_ration);
			key_handler.m_resize_ratio = m_matimg_disp_ration;
		}else{
			key_handler.m_resize_ratio = 1.0;
		}
		string desciption_text = key_handler.m_text  + key_handler.m_mouse_pos;
		if(desciption_text != ""){
			res_img = appendtext2img(res_img, desciption_text);
		}
		imshow(win_name, res_img );
		int key = cv::waitKey(1);
		if(key == -1) {
			//proceed with next loop as no key is pressed.
			continue;
		}
		if(key == 1048677){
			//key `e` is pressed, exit the loop
			break;
		}
		key_handler.update_img(key);
	}

	cout<<"done with image showing"<<endl;
	destroyWindow(win_name);
	return;
}
void DebugUtil::matimg(cv::Mat m, std::string text, KeyHandler  key_handler){
	key_handler.m_original_single_img_width = m.cols;
	matimg_internal(m, text, &key_handler);
}
std::vector< DMatch > DebugUtil::gen_rnd_matches(const std::vector< DMatch > &matches, int n){
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
std::vector<std::string> DebugUtil::split_str(const std::string &s, char delim) {
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
		// elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
	}
	return elems;
}
cv::Mat DebugUtil::appendtext2img(cv::Mat m, std::string text){
	Mat outImg;
	m.copyTo(outImg);

	//draw the blank area with text lines
	int height = outImg.rows;
	int width = outImg.cols;
	Mat blank_area = Mat::zeros(height/2, width, outImg.type());
	if(text!=""){
		int y0 = 30;
		int dy = 30;
		int i = 0;

		for(auto &text_line: split_str(text, '\n')){
			int y = y0 + i*dy;
			cv::putText(blank_area, text_line, cv::Point(5,y), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0,255,0), 1);
			i++;
		}
	}
	//do the concatenation
	Mat res_img;
	vconcat(outImg,blank_area,res_img);
	return res_img;
}
MatchImageAndDisciption DebugUtil::generate_match_img(cv::Mat img1, const std::vector<cv::KeyPoint>& keypoints1,
		cv::Mat img2, const std::vector<cv::KeyPoint>& keypoints2,
		const std::vector<cv::DMatch>& matches1to2, int n){
	Mat img_match;
	std::vector< DMatch > rnd_matches;
	rnd_matches = gen_rnd_matches(matches1to2, n);
	drawMatches ( img1, keypoints1, img2, keypoints2, rnd_matches, img_match,
			Scalar::all(-1),Scalar::all(-1),std::vector<char>(),DrawMatchesFlags::DEFAULT| DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//string to describe the rnd matches
	stringstream ss;
	for(const auto match : rnd_matches){
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
	MatchImageAndDisciption res;
	res.m_img = img_match;
	res.m_text = ss.str();
	return res;

}
void DebugUtil::draw_matches(Mat img1, const std::vector<KeyPoint>& keypoints1,
		Mat img2, const std::vector<KeyPoint>& keypoints2,
		const std::vector<DMatch>& matches1to2, int n){
	FeatureMatchUpdater fmu(img1, keypoints1,img2, keypoints2,matches1to2, n,this);
	fmu.m_original_single_img_width = img1.cols;
	fmu.update_img(m_update_key);
	matimg_internal(fmu.m_img, fmu.m_text, &fmu);

}
DebugUtil::~DebugUtil() {
	// TODO Auto-generated destructor stub
}

