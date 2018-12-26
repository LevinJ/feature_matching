/*
 * OrbDebugUtil.cpp
 *
 *  Created on: Dec 21, 2018
 *      Author: aiways
 */

#include "OrbDebugUtil.h"
#include <vector>
using namespace cv;
using namespace std;

OrbDebugUtil::OrbDebugUtil() {
	// TODO Auto-generated constructor stub
	m_no_window_display = false;

}

OrbDebugUtil::~OrbDebugUtil() {
	// TODO Auto-generated destructor stub
}

//void OrbDebugUtil::dbgtlm(std::string img_path, const std::vector<cv::KeyPoint> & keypoints,
//		const std::vector<ORB_SLAM2::MapPoint*> &old,
//		const std::vector<ORB_SLAM2::MapPoint*> &updated,
//		const std::vector<ORB_SLAM2::MapPoint*> &valid){
//	if(m_no_window_display) return;
//	const int n = keypoints.size();
//	Mat img = cv::imread(img_path, CV_LOAD_IMAGE_COLOR);
//
//	vector<int> cnts(4, 0);
//	cnts[0] = n;
//	for(int i=0; i< n;i++){
//		cv::Point pnt = keypoints[i].pt;
//		//draw old points
//		if(old[i] != NULL){
//			cv::circle(img, pnt, 2, Scalar(255,0,0), 1);
//			cnts[1] ++;
//		}
//		//draw added points
//		if(updated[i] != NULL && old[i] == NULL){
//			cv::circle(img, pnt, 2, Scalar(0,255,0), 1);
//			cnts[2]++;
//		}
//		//draw invalid points
//		if(updated[i] != NULL && valid[i] == NULL){
//			cv::circle(img, pnt, 4, Scalar(0,0,255), 1);
//			cnts[3]++;
//		}
//	}
//	stringstream ss;
//	ss<<"All="<<cnts[0]<<", Old="<<cnts[1]<<", Added="<<cnts[2]<<", Invalid="<<cnts[3]<<endl;
//	ss<<"Valid points="<<cnts[1] + cnts[2] - cnts[3]<<endl;
//	KeyHandler dummy_handler = KeyHandler();
//	string text = ss.str();
//	ss.str("");
//
//	ss<<"                                             Track Local Map"<<endl;
//	ss<<"                                             "<<split_str(img_path, '/').back();
//	matimg_internal(img, text, &dummy_handler, ss.str());
//
//}
void OrbDebugUtil::dbgstereomatches(std::string img1, const std::vector<cv::KeyPoint>& keypoints1,
		std::string img2, const std::vector<cv::KeyPoint>& keypoints2,
		const std::vector<cv::DMatch> &matches1to2, int n, std::vector<float> *matched_point_depth){
	if(m_no_window_display) return;
	m_matched_point_depth = matched_point_depth;
	stringstream ss;
	ss<<"                                            Compute Stereo matches"<<endl;

	ss<<"                              left: " <<
			split_str(img1, '/').back() << ",                                 right: " <<
			split_str(img2, '/').back() ;
	draw_matches_internal(img1, keypoints1,img2,keypoints2,matches1to2,n, ss.str() );
	//after we are done with matches display, clear m_matched_point_depth so that next match display can be done properly
	m_matched_point_depth = NULL;

}
void OrbDebugUtil::dbgtrf(std::string img1, const std::vector<cv::KeyPoint>& keypoints1,
		std::string img2, const std::vector<cv::KeyPoint>& keypoints2,
		const std::map<int, cv::DMatch> &matches1to2, int n, std::vector<float> *matched_point_depth){
	if(m_no_window_display) return;
	m_matched_point_depth = matched_point_depth;
	stringstream ss;
	ss<<"                                            Track reference key frame"<<endl;

	ss<<"                              " <<
			split_str(img1, '/').back() << ",                                 " <<
			split_str(img2, '/').back() ;
	draw_matches_internal(img1, keypoints1,img2,keypoints2,matches1to2,n, ss.str() );
	m_matched_point_depth = NULL;

}
void OrbDebugUtil::dbgtmm(std::string img1, const std::vector<cv::KeyPoint>& keypoints1,
		std::string img2, const std::vector<cv::KeyPoint>& keypoints2,
		const std::map<int, cv::DMatch> &matches1to2, int n, std::vector<float> *matched_point_depth){
	if(m_no_window_display) return;
	m_matched_point_depth = matched_point_depth;
	stringstream ss;
	ss<<"                                            Track With Motion Model"<<endl;

	ss<<"                              " <<
			split_str(img1, '/').back() << ",                                 " <<
			split_str(img2, '/').back() ;
	draw_matches_internal(img1, keypoints1,img2,keypoints2,matches1to2,n, ss.str() );
	m_matched_point_depth = NULL;

}
OrbDebugUtil g_orbdbg;
