import sys
import os
# from _pickle import dump
sys.path.insert(0, os.path.abspath('..'))

import numpy as np
import cv2
import matplotlib.pyplot as plt


class DisplayImg(object):
    def __init__(self, win_name=""):
        
        self.win_name = win_name

        self.resize_ratio = 0.5
        self.mouse_pos_text= ""
        return
    def appendtext2img(self, m, text):
        
        return self.addtext2img(m, text, True)
    
    def show_image(self, img,textlines=[], img_width=0):
#         img = cv2.resize(img, (0,0), fx=self.resize_ratio, fy=self.resize_ratio) 
        

        win_name = self.win_name
        cv2.namedWindow( win_name, cv2.WINDOW_AUTOSIZE )
        cv2.setMouseCallback(win_name,self.on_mouse)
        self.img_width = img_width
        while True:
            res_img = img.copy()
            description_text = textlines.copy()
            cur_img_width = res_img.shape[1]
            if(cur_img_width > 2000):
                res_img = cv2.resize(res_img, (0,0), fx=self.resize_ratio, fy=self.resize_ratio)
            
            if (self.mouse_pos_text != ""):
                    description_text.append(self.mouse_pos_text)
            if(len(description_text) != 0):
                res_img = self.appendtext2img(res_img, description_text)
            
            cv2.imshow(win_name,res_img)
            key = cv2.waitKey(10)
            if  key != -1:
                if key == 1048677:
                    break
                print('key={}'.format(key))
#                 break;
        cv2.destroyWindow(win_name);
        for _ in range(6):
            cv2.waitKey(1)
        
        return

    def addtext2img(self, m, text, append):
            
        #draw the blank area with text lines
        height = m.shape[0]
        width = m.shape[1]
        text_lines = text
        blank_height = (len(text_lines) + 1) * 30
        
        blank_area = np.zeros((blank_height, m.shape[1], m.shape[2]), m.dtype)
    
        #draw the line
        y0 = 25
        dy = 30
        i = 0

        for text_line in text_lines:
            y = y0 + i*dy
            cv2.putText(blank_area, text_line, (5,y), cv2.FONT_HERSHEY_COMPLEX_SMALL, 1, (0,255,0), 1)
            i = i + 1
            
        
        #do the concatenation

        if(append):
            res_img = np.concatenate((m, blank_area), axis = 0)
        else:
            res_img = np.concatenate((blank_area,m), axis = 0)
            
        
        return res_img
    def on_mouse(self,event, x, y, flag, param):
        if(event == cv2.EVENT_MOUSEMOVE):
            x = x/self.resize_ratio
            y = y/self.resize_ratio
            if x> self.img_width:
                x = x - self.img_width
#             print("x={},y={}".format(x,y))
            self.mouse_pos_text= "x={},y={}".format(x,y)
        
    def run(self):
        return
