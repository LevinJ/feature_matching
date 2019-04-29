/*
 * StereoMatcher.cpp
 *
 *  Created on: Dec 26, 2018
 *      Author: aiways
 */

#include "FeatureMatchUtil.h"

#include <iostream>


using namespace std;
using namespace cv;


FeatureMatchUtil::FeatureMatchUtil() {
	// TODO Auto-generated constructor stub

}

FeatureMatchUtil::~FeatureMatchUtil() {
	// TODO Auto-generated destructor stub
}

void FeatureMatchUtil::basicmatching(Mat img_1, Mat img_2){
	//-- 初始化
		Mat descriptors_1, descriptors_2;
		Ptr<FeatureDetector> detector = ORB::create();
		Ptr<DescriptorExtractor> descriptor = ORB::create();
		// Ptr<FeatureDetector> detector = FeatureDetector::create(detector_name);
		// Ptr<DescriptorExtractor> descriptor = DescriptorExtractor::create(descriptor_name);
		Ptr<DescriptorMatcher> matcher  = DescriptorMatcher::create ( "BruteForce-Hamming" );
		//    Ptr<DescriptorMatcher> matcher  = DescriptorMatcher::create ( "FlannBased" );
		//    FlannBasedMatcher *matcher = new FlannBasedMatcher();
		//-- 第一步:检测 Oriented FAST 角点位置
		detector->detect ( img_1,m_keypoints_1 );
		detector->detect ( img_2,m_keypoints_2 );

		//-- 第二步:根据角点位置计算 BRIEF 描述子
		descriptor->compute ( img_1, m_keypoints_1, descriptors_1 );
		descriptor->compute ( img_2, m_keypoints_2, descriptors_2 );

		Mat outimg1;
		//	drawKeypoints( img_1, keypoints_1, outimg1, Scalar::all(-1), Scalar::all(-1),Scalar::all(-1),std::vector<char>(),DrawMatchesFlags::DEFAULT );
		//	imshow("ORB特征点",outimg1);

		//-- 第三步:对两幅图像中的BRIEF描述子进行匹配，使用 Hamming 距离
		vector<DMatch> matches;
		//BFMatcher matcher ( NORM_HAMMING );
		matcher->match ( descriptors_1, descriptors_2, matches );
		cout<< "match sized = "<<matches.size()<<endl;

		//-- 第四步:匹配点对筛选
		double min_dist=10000, max_dist=0;

		//找出所有匹配之间的最小距离和最大距离, 即是最相似的和最不相似的两组点之间的距离
		//	for ( int i = 0; i < descriptors_1.rows; i++ )
		//	{
		//		double dist = matches[i].distance;
		//		if ( dist < min_dist ) min_dist = dist;
		//		if ( dist > max_dist ) max_dist = dist;
		//	}

		// 仅供娱乐的写法
		min_dist = min_element( matches.begin(), matches.end(), [](const DMatch& m1, const DMatch& m2) {return m1.distance<m2.distance;} )->distance;
		max_dist = max_element( matches.begin(), matches.end(), [](const DMatch& m1, const DMatch& m2) {return m1.distance<m2.distance;} )->distance;

		printf ( "-- Max dist : %f \n", max_dist );
		printf ( "-- Min dist : %f \n", min_dist );

		//当描述子之间的距离大于两倍的最小距离时,即认为匹配有误.但有时候最小距离会非常小,设置一个经验值30作为下限.

		for ( int i = 0; i < descriptors_1.rows; i++ )
		{
			if ( matches[i].distance <= max ( 2*min_dist, 30.0 ) )
			{
				m_matches1to2.push_back ( matches[i] );

			}
		}
}

void FeatureMatchUtil::manual_matching(Mat img_1, Mat img_2){
	vector<vector<float>> matches;
//	matches.push_back({251,310,176,310});
//	matches.push_back({949,49,890,49});
//	matches.push_back({509,108,481,108});
//	matches.push_back({517,71,489,71});


	//for windows front
	matches.push_back({748,190,738,190});

	//test for windows front
//	matches.push_back({748,190,726,190});


	//test for window left side
//	matches.push_back({509,108,479,108});

	int id = 0;
	for(const auto &m: matches){
		float x1,y1,x2,y2;
		x1 = m[0];
		y1 = m[1];
		x2 = m[2];
		y2 = m[3];
		m_keypoints_1.push_back(KeyPoint(x1,y1,-1));
		m_keypoints_2.push_back(KeyPoint(x2,y2,-1));
		m_matches1to2.push_back(DMatch(id, id, 0,0));
		id++;
	}


}

MatchImageAndDisciption FeatureMatchUtil::generate_matching_result(int n){

	std::string title =  "                              " +
				split_str(m_img1, '/').back() + ",                                 " +
				split_str(m_img2, '/').back() ;
	cv::Mat img1_temp = cv::imread(m_img1, CV_LOAD_IMAGE_COLOR);
	cv::Mat img2_temp = cv::imread(m_img2, CV_LOAD_IMAGE_COLOR);
	vector<cv::DMatch> matches;
	for(auto const &item : m_matches1to2map){
		matches.push_back(item.second);
	}
	MatchImageAndDisciption res = generate_match_img(img1_temp, m_keypoints_1,img2_temp, m_keypoints_2,matches, n);
	cv::resize(res.m_img, res.m_img, cv::Size(), m_matimg_disp_ration, m_matimg_disp_ration);
	res.m_title = title;
	return res;
}

Mat FeatureMatchUtil::generate_img_texts(cv::Mat &img,  std::string title, std::string text){

	Mat res;
	res = addtext2img(img, title, false);
	res = addtext2img(res, text, true);
	return res;
}

void FeatureMatchUtil::clear_cache() {
	m_matches1to2map.clear();
}

