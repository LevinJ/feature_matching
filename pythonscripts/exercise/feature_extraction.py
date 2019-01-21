import numpy as np
import cv2
import matplotlib.pyplot as plt



class FeatureExtraction(object):
    def __init__(self):
        return
    def orb_match_1(self):
        img1 = cv2.imread('/home/aiways/workspace/slambook/ch7/1.png',0)          # queryImage
        img2 = cv2.imread('/home/aiways/workspace/slambook/ch7/2.png',0) # trainImage
        # Initiate ORB detector
        orb = cv2.ORB_create()
        # find the keypoints and descriptors with ORB
        kp1, des1 = orb.detectAndCompute(img1,None)
        kp2, des2 = orb.detectAndCompute(img2,None)
        
        # create BFMatcher object
        bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)
        # Match descriptors.
        matches = bf.match(des1,des2)
        # Sort them in the order of their distance.
        matches = sorted(matches, key = lambda x:x.distance)
        # Draw first 10 matches.
        img3 = cv2.drawMatches(img1,kp1,img2,kp2,matches[::],None,flags=2)
        plt.imshow(img3);
        plt.show()
        return
    def siftmatch(self):
        img1 = cv2.imread('/home/aiways/workspace/slambook/ch7/1.png',0)          # queryImage
        img2 = cv2.imread('/home/aiways/workspace/slambook/ch7/2.png',0) # trainImage
        # Initiate SIFT detector
        sift = cv2.xfeatures2d.SIFT_create()
        # find the keypoints and descriptors with SIFT
        kp1, des1 = sift.detectAndCompute(img1,None)
        kp2, des2 = sift.detectAndCompute(img2,None)
        # BFMatcher with default params
        bf = cv2.BFMatcher()
        matches = bf.knnMatch(des1,des2, k=2)
        # Apply ratio test
        good = []
        for m,n in matches:
            if m.distance < 0.75*n.distance:
                good.append([m])
        # cv2.drawMatchesKnn expects list of lists as matches.
        
        good = sorted(good, key = lambda x:x[0].distance)
        
        img3 = cv2.drawMatchesKnn(img1,kp1,img2,kp2,good[:10],None, flags=2)

        plt.imshow(img3);
        plt.show()
        return
    def sift_flann(self):
        cv2.ocl.setUseOpenCL(False)
        img1 = cv2.imread('/home/aiways/workspace/slambook/ch7/1.png',0)          # queryImage
        img2 = cv2.imread('/home/aiways/workspace/slambook/ch7/2.png',0) # trainImage
        
        
        # Initiate SIFT detector
        sift = cv2.xfeatures2d.SIFT_create()
        # find the keypoints and descriptors with SIFT
        kp1, des1 = sift.detectAndCompute(img1,None)
        kp2, des2 = sift.detectAndCompute(img2,None)
        # FLANN parameters
        FLANN_INDEX_KDTREE = 1
        index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees = 5)
        search_params = dict(checks=50)   # or pass empty dictionary
        flann = cv2.FlannBasedMatcher(index_params,search_params)
        matches = flann.knnMatch(des1,des2,k=2)
        # Need to draw only good matches, so create a mask
        matchesMask = [[0,0] for i in xrange(len(matches))]
        # ratio test as per Lowe's paper
        for i,(m,n) in enumerate(matches):
            if m.distance < 0.7*n.distance:
                matchesMask[i]=[1,0]
        draw_params = dict(matchColor = (0,255,0),
                           singlePointColor = (255,0,0),
                           matchesMask = matchesMask,
                           flags = 0)
        img3 = cv2.drawMatchesKnn(img1,kp1,img2,kp2,matches,None,**draw_params)
        plt.imshow(img3,)
        plt.show()
        return
    def orbmatch_threshold(self):
        img1 = cv2.imread('/home/aiways/workspace/slambook/ch7/1.png',0)          # queryImage
        img2 = cv2.imread('/home/aiways/workspace/slambook/ch7/2.png',0) # trainImage
        # Initiate ORB detector
        orb = cv2.ORB_create()
        # find the keypoints and descriptors with ORB
        kp1, des1 = orb.detectAndCompute(img1,None)
        kp2, des2 = orb.detectAndCompute(img2,None)
        
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
        # Draw first 10 matches.
        
        fig = plt.figure()
#         fig.suptitle('intial matches', fontsize=20)
        img3 = cv2.drawMatches(img1,kp1,img2,kp2,matches[::],None,flags=2)
        plt.imshow(img3);
        
        plt.figure()
        img3 = cv2.drawMatches(img1,kp1,img2,kp2,good_matches[::],None,flags=2)
        plt.imshow(img3);
        
        plt.show()
        return
    
    def run(self):
#         self.siftmatch()
#         self.sift_flann()
        self.orbmatch_threshold()
        return



if __name__ == "__main__":   
    obj= FeatureExtraction()
    obj.run()