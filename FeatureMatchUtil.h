/*
 * StereoMatcher.h
 *
 *  Created on: Dec 26, 2018
 *      Author: aiways
 */

#ifndef FEATUREMATCHUTIL_H_
#define FEATUREMATCHUTIL_H_

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include "DebugUtil.h"

class FeatureMatchUtil: public DebugUtil {
public:
	FeatureMatchUtil();
	void manual_matching(cv::Mat img_1, cv::Mat img_2);
	void basicmatching(cv::Mat img_1, cv::Mat img_2);
	virtual ~FeatureMatchUtil();
	std::vector<cv::KeyPoint> m_keypoints_1, m_keypoints_2;
	std::vector< cv::DMatch > m_matches1to2;

	//return the matching image, title,and description
	MatchImageAndDisciption generate_matching_result(std::string img1, const std::vector<cv::KeyPoint>& keypoints1,
			std::string img2, const std::vector<cv::KeyPoint>& keypoints2,
			const std::map<int, cv::DMatch> &matches1to2, int n=3);
	cv::Mat generate_matching_result2(std::string img1, const std::vector<cv::KeyPoint>& keypoints1,
				std::string img2, const std::vector<cv::KeyPoint>& keypoints2,
				const std::map<int, cv::DMatch> &matches1to2, int n=3);


};

#endif /* FEATUREMATCHUTIL_H_ */
