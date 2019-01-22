import sys
import os
# from _pickle import dump
sys.path.insert(0, os.path.abspath('../../'))

import numpy as np
import cv2
from pythonscripts.exercise.display_image import DisplayImg


class Stereo3DEstimation(object):
    def __init__(self):
#         self.img_1_str = '/home/aiways/workspace/featurematcher/imgs/000046_left.png'
#         self.img_2_str = '/home/aiways/workspace/featurematcher/imgs/000046_right.png' 
#         self.img_1_pnt = (748,190)
#         self.img_2_pnt = (738,190)
        
        
        
        self.img_1_str = '/home/aiways/workspace/featurematcher/imgs/000046_left.png'
        self.img_2_str = '/home/aiways/workspace/featurematcher/imgs/000046_right.png' 
        self.img_1_pnt = (512,108)
        self.img_2_pnt = (486,108)
#         
        
        self.img_1_str = '/home/aiways/workspace/featurematcher/imgs/000448_left.png'
        self.img_2_str = '/home/aiways/workspace/featurematcher/imgs/000448_right.png' 
        self.img_1_pnt = (462,88)
        self.img_2_pnt = (434,88)
        
        
        self.img_1_str = '/home/aiways/workspace/featurematcher/imgs/000678_left.png'
        self.img_2_str = '/home/aiways/workspace/featurematcher/imgs/000678_right.png' 
        self.img_1_pnt = (428,76)
        self.img_2_pnt = (398,76)
         
        
        
        print(self.img_1_str)
        print(self.img_2_str)
        self.mtx = np.array([[1.28879712e+03, 0.00000000e+00, 6.84754189e+02],
         [0.00000000e+00, 1.28879712e+03, 3.78154942e+02],
         [0.00000000e+00, 0.00000000e+00, 1.00000000e+00]])

        self.f = 6.0e-03
        self.b = 12e-02
        
        return
   
   
    def compute3d(self):
        fx = self.mtx[0][0]
        cx = self.mtx[0][2]
        fy = self.mtx[1][1]
        cy = self.mtx[1][2]
        ul = self.img_1_pnt[0]
        ur = self.img_2_pnt[0]
        vl = self.img_1_pnt[1]
        
       
        
    
        z = self.b * fx / (ul - ur)
        x = (ul -cx) * z / fx
        y = (vl - cy) * z/ fy
        
        res = []
        res.append("l pnts ={}".format(self.img_1_pnt))
        res.append("r pnts ={}".format(self.img_2_pnt))
        res.append("z={}, x = {}, y={}".format(z, x, y))
        
        return res
           
    def drawmatch(self):
        img_1 = cv2.imread(self.img_1_str, cv2.IMREAD_COLOR)
        img_2 = cv2.imread(self.img_2_str, cv2.IMREAD_COLOR)
        img = np.concatenate((img_1, img_2), axis=1)
        
        img_width = img_1.shape[1]
        
        
        
        lineThickness = 2
        cv2.line(img, self.img_1_pnt, (self.img_2_pnt[0] + img_1.shape[1], self.img_2_pnt[1]), (0,255,0), lineThickness)
        
        si = DisplayImg("Estimate 3D position")
        textlines = self.compute3d()
        print(textlines)
        si.show_image(img, textlines, img_width)
        
        
            
        return
    def run(self):
       
        self.drawmatch()
        
        
        return



if __name__ == "__main__":   
    obj= Stereo3DEstimation()
    obj.run()