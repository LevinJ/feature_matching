#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <random>

#include "FeatureMatchUtil.h"
#include "OrbDebugUtil.h"

using namespace std;
using namespace cv;

int main ( int argc, char** argv )
{

	//-- 读取图像
	std::string img_1_str = "/home/aiways/eclipse-workspace/featurematcher/imgs/000046_left.png";
	std::string img_2_str =  "/home/aiways/eclipse-workspace/featurematcher/imgs/000046_right.png";
	Mat img_1 = imread ( img_1_str, CV_LOAD_IMAGE_COLOR );
	Mat img_2 = imread (img_2_str, CV_LOAD_IMAGE_COLOR );

	FeatureMatchUtil matcher = FeatureMatchUtil();
//	matcher.basicmatching(img_1, img_2);
	matcher.manual_matching(img_1, img_2);
	std::vector<cv::KeyPoint> keypoints_1 = matcher.m_keypoints_1;
	std::vector<cv::KeyPoint> keypoints_2 = matcher.m_keypoints_2;
	std::vector< cv::DMatch > matches1to2 = matcher.m_matches1to2;

	cout<<"debug here"<<endl;
	g_orbdbg.m_cx = 632.9208;
	g_orbdbg.m_cy = 379.8993;
	g_orbdbg.m_fx = 1456.718;
	g_orbdbg.m_fy = 1455.906;
	g_orbdbg.m_b = 0.12;
	std::vector<float> matched_point_depth(keypoints_1.size(), -1);

	for(const auto &m: matches1to2){
		float ul = keypoints_1[m.queryIdx].pt.x;
		float ur = keypoints_2[m.trainIdx].pt.x;
		float d = ul - ur;
		float z = g_orbdbg.m_b * g_orbdbg.m_fx/d;
		matched_point_depth[m.queryIdx] = z;
	}
	g_orbdbg.dbgstereomatches(img_1_str, keypoints_1, img_2_str, keypoints_2, matches1to2, -1, &matched_point_depth);


	return 0;

}
