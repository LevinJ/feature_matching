import numpy as np
import cv2
import matplotlib.pyplot as plt



class PoseEstimation(object):
    def __init__(self):
        return
    
    def find_feature_matches (self,img_1, img_2):
        orb = cv2.ORB_create()
        # find the keypoints and descriptors with ORB
        kp1, des1 = orb.detectAndCompute(img_1,None)
        kp2, des2 = orb.detectAndCompute(img_2,None)
        
        # create BFMatcher object
        bf = cv2.BFMatcher(cv2.NORM_HAMMING)
        # Match descriptors.
        matches = bf.match(des1,des2)
        # Sort them in the order of their distance.
        sorted_matches = sorted(matches, key = lambda x:x.distance)
        
        min_dist = sorted_matches[0].distance
        max_dist = sorted_matches[-1].distance
        
        print("max/min dist = {}/{}".format( max_dist, min_dist))
        
#         matches = sorted_matches
        good_matches = []
        for match in matches:
            if match.distance <= max(30, 2* min_dist):
                good_matches.append(match)
        
            
        print("matches size = {}/{}".format(len(matches), len(good_matches)))
        
        return kp1, kp2, good_matches
    
    def pixel2cam ( self, p, K ):
        
        x = (p[0] - K[ 0, 2 ])  / K[ 0,0 ]
        y = (p[1] - K[ 1,2 ] )/ K[ 1,1 ]
        
        return (x, y)
           
        
    
    def pose_estimation_2d2d(self,keypoints_1, keypoints_2, matches):
        K = np.array([[520.9, 0, 325.1], [0, 521.0, 249.7], [0, 0, 1]])
        
        points1 = []
        points2 = []
        
        for match in matches:
            points1.append(keypoints_1[match.queryIdx].pt)
            points2.append( keypoints_2[match.trainIdx].pt )
        points1 = np.array(points1)
        points2 = np.array(points2)
        #-- 计算基础矩阵
        fundamental_matrix, mask = cv2.findFundamentalMat ( points1, points2, cv2.FM_8POINT );
        print("fundamental_matrix is \n{}".format(fundamental_matrix))
        
        principal_point = ( 325.1, 249.7 );    #相机光心, TUM dataset标定值
        focal_length = 521.0;            #相机焦距, TUM dataset标定值

        essential_matrix,mask = cv2.findEssentialMat ( points1, points2, focal_length, principal_point );
        print("essential_matrix is \n{}".format(essential_matrix))
        
        homography_matrix,mask  = cv2.findHomography ( points1, points2, cv2.RANSAC, 3 );
        print("homography_matrix is \n{}".format(homography_matrix))
        
        #-- 从本质矩阵中恢复旋转和平移信息.
        retval, R, t, mask= cv2.recoverPose ( essential_matrix, points1, points2,K );
        print("R is \n{}".format(R))
        print("t is \n{}".format(t))
        return R, t, essential_matrix
   
    
    def run(self):
        img1 = cv2.imread('/home/aiways/workspace/slambook/ch7/1.png',cv2.IMREAD_COLOR )          # queryImage
        img2 = cv2.imread('/home/aiways/workspace/slambook/ch7/2.png', cv2.IMREAD_COLOR) # trainImage
        
        kp1, kp2, matches = self.find_feature_matches(img1, img2)
        R, t, essential_matrix = self.pose_estimation_2d2d(kp1, kp2, matches)
        
        K = np.array([[520.9, 0, 325.1], [0, 521.0, 249.7], [0, 0, 1 ]])
        
        t_x = [
                [0,                      -t[ 2,0 ],     t[ 1,0 ]],
                [t[2,0 ],      0,                      -t[ 0,0 ]],
                [-t[ 1,0 ],     t[0,0 ],      0 ]]
        print("t^R=\n{}".format(t_x @R))
        
        #verify epipolar geometry
        for m in  matches:
        
            pt1 = self.pixel2cam ( kp1[ m.queryIdx ].pt, K );
            y1 = np.array([ pt1[0], pt1[1], 1]).reshape(3,1)
            pt2 = self.pixel2cam ( kp2[ m.trainIdx ].pt, K );
            y2 = np.array([pt2[0], pt2[1], 1]).reshape(3,1)
#             d = y2.T @ t_x @ R @ y1
#             d = y2.T.dot(essential_matrix)
#             d = d.dot(y1)
            d = y2.T @ essential_matrix @ y1;
            print("epipolar constraint = {}".format(d[0][0]))
        
    
        img3 = cv2.drawMatches(img1,kp1,img2,kp2,matches[:5], None, flags=2)
        plt.imshow(img3[...,::-1])
        
        plt.show()

        return



if __name__ == "__main__":   
    obj= PoseEstimation()
    obj.run()