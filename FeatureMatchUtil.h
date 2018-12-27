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

class FeatureMatchUtil {
public:
	FeatureMatchUtil();
	void manual_matching(cv::Mat img_1, cv::Mat img_2);
	void basicmatching(cv::Mat img_1, cv::Mat img_2);
	virtual ~FeatureMatchUtil();
	std::vector<cv::KeyPoint> m_keypoints_1, m_keypoints_2;
	std::vector< cv::DMatch > m_matches1to2;
};

#endif /* FEATUREMATCHUTIL_H_ */
