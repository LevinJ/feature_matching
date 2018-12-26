/*
 * OrbDebugUtil.h
 *
 *  Created on: Dec 21, 2018
 *      Author: aiways
 */

#ifndef SRC_UTILITY_ORBDEBUGUTIL_H_
#define SRC_UTILITY_ORBDEBUGUTIL_H_
#include "DebugUtil.h"
//#include "MapPoint.h"



class OrbDebugUtil: public DebugUtil{
public:
	OrbDebugUtil();
	virtual ~OrbDebugUtil();
//	void dbgtlm(std::string img_path, const std::vector<cv::KeyPoint> & keypoints,
//			const std::vector<ORB_SLAM2::MapPoint*> &old,
//			const std::vector<ORB_SLAM2::MapPoint*> &updated,
//			const std::vector<ORB_SLAM2::MapPoint*> &valid);
	void dbgstereomatches(std::string img1, const std::vector<cv::KeyPoint>& keypoints1,
			std::string img2, const std::vector<cv::KeyPoint>& keypoints2,
			const std::vector<cv::DMatch> &matches1to2, int n, std::vector<float> *matched_point_depth);
	void dbgtrf(std::string img1, const std::vector<cv::KeyPoint>& keypoints1,
				std::string img2, const std::vector<cv::KeyPoint>& keypoints2,
				const std::map<int, cv::DMatch> &matches1to2, int n, std::vector<float> *matched_point_depth);
	void dbgtmm(std::string img1, const std::vector<cv::KeyPoint>& keypoints1,
					std::string img2, const std::vector<cv::KeyPoint>& keypoints2,
					const std::map<int, cv::DMatch> &matches1to2, int n, std::vector<float> *matched_point_depth);
	bool m_no_window_display;
};

extern OrbDebugUtil g_orbdbg;


#endif /* SRC_UTILITY_ORBDEBUGUTIL_H_ */
