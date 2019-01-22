import sys
import os
# from _pickle import dump
sys.path.insert(0, os.path.abspath('../../'))

import numpy as np
import cv2

from pythonscripts.exercise.display_image import DisplayImg


class CheckStereoRectify(object):
    def __init__(self):
#         self.img_1_str = '/home/aiways/workspace/dataset/camera_calibration/zk_snapshot/1229/backup/777036_left.jpg'
#         self.img_2_str = '/home/aiways/workspace/dataset/camera_calibration/zk_snapshot/1229/backup/777036_right.jpg' 
        
        self.img_1_str = '/home/aiways/workspace/dataset/camera_calibration/zk_snapshot/1229/backup/1046000_left.jpg'
        self.img_2_str = '/home/aiways/workspace/dataset/camera_calibration/zk_snapshot/1229/backup/1046000_right.jpg' 
        self.img_1_pnt = (0,200)
        self.img_2_pnt = (1280,200)
       
        return
   
           
    def drawmatch(self):
        img_1 = cv2.imread(self.img_1_str, cv2.IMREAD_COLOR)
        img_2 = cv2.imread(self.img_2_str, cv2.IMREAD_COLOR)
        img = np.concatenate((img_1, img_2), axis=1)
        
        img_width = img_1.shape[1]
        
        
        
        lineThickness = 2
        ys = np.linspace(10, 728, 15)
        for y in ys:
            y = int(round(y))
            x1 = 0
            x2 = img_width
            cv2.line(img, (x1, y), (x2 + img_width, y), (0,255,0), lineThickness)
        
        si = DisplayImg(win_name="Check StereoRectify Result")
#         textlines = self.compute3d()
        si.show_image(img, [], img_width)
        
        
            
        return
    def run(self):
       
        self.drawmatch()
        
        
        return



if __name__ == "__main__":   
    obj= CheckStereoRectify()
    obj.run()