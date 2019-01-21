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
        matches = sorted(matches, key = lambda x:x.distance)
        
        print("max/min dist = {}/{}".format(matches[-1].distance, matches[0].distance))
        
        good_matches = []
        for match in matches:
            if match.distance < max(30, 2*matches[0].distance):
                good_matches.append(match)
        
            
        print("matches size = {}/{}".format(len(matches), len(good_matches)))
        
        return kp1, kp2, matches
    
    def run(self):
        img_1 = cv2.imread('/home/aiways/workspace/slambook/ch7/1.png',cv2.IMREAD_COLOR )          # queryImage
        img_2 = cv2.imread('/home/aiways/workspace/slambook/ch7/2.png', cv2.IMREAD_COLOR) # trainImage
        
        keypoints_1, keypoints_2, matches = self.find_feature_matches(img_1, img_2)

        return



if __name__ == "__main__":   
    obj= PoseEstimation()
    obj.run()