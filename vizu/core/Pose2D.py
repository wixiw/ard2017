#!/usr/bin/env python
# -*- coding: latin-1 -*-

from math import *
from Point import *
from ArdMath import *
from generated import *

class Pose2D(Point):
    def __init__(self, x=0., y=0., h=0.):
        Point.__init__(self,x,y)
        
        if True == isinstance(h,str) and h =='FREE':
            self.h='FREE'
        else:
            self.h = float(h)
            assert abs(self.h) < 10., "h is out of range %s" % h
            normalizeAngle(self.h)
    
    #/**
    #* Les Pose2D évoluent dans le groupe spécial euclidien nommé SE(2)\n
    #* L'opérateur * représente la loi de composition interne de ce groupe.\n
    #* Elle est très pratique pour réaliser des changements de repère.\n
    #* Ex :
    #* \li soit H_1_0 une Pose2D correspondant à la position d'un repère 1 par rapport à un repère 0
    #* \li soit H_2_1 une Pose2D correspondant à la position d'un repère 2 par rapport au repère 1
    #* \li soit H_2_0 une Pose2D correspondant à la position du repère 2 par rapport au repère 0
    #* \li soit H_0_2 une Pose2D correspondant à la position du repère 0 par rapport au repère 2
    #* Alors :
    #* \li H_2_0 = H_1_0 * H_2_1;
    #* \li H_0_2 = H_2_0.inverse();
    #* \li H_0_2 = H_2_1.inverse() * H_1_0.inverse();
    #*/
    # @param Pose2D pose : the other pose to compose 
    def compose(self, pose):
        x = cos(self.h)*pose.x - sin(self.h)*pose.y + self.x
        y = sin(self.h)*pose.x + cos(self.h)*pose.y + self.y
        angle = self.h + pose.h
        return Pose2D( x, y, angle)
    
    #/** Permet de calculer l'inverse de la Pose2D.
    #* La Pose2D inverse a un vecteur translation et un angle opposés à ceux initiaux.
    #* \remarks l'objet n'est pas modifié.
    #* \returns la Pose2D inversée
    # * \warning l'inverse n'est pas l'opposé ! Il s'agit de l'inverse tel que la Pose2D composée avec son inverse donne la Pose2D nulle. */
    def inverse(self):
        if not isinstance(self.x,str):
            x = -cos(-self.h)*self.x + sin(-self.h)*self.y
        if not isinstance(self.y,str):
            y = -sin(-self.h)*self.x - cos(-self.h)*self.y
        if not isinstance(self.h,str):
           angle = -self.h
        return Pose2D( x, y, angle) 
    
    
    #realise une translation du point en polaire
    def polarTranslate(self,ro,h):
         if not isinstance(self.x,str):
             x = self.x + ro*cos(h)
         if not isinstance(self.y,str):
             y = self.y + ro*sin(h)
         return Pose2D( x, y, self.h) 
        
    #Convert a PoseMsg proto message type into a Pose2D
    #@param CommonMsg_pb2.PointCap PoseMsg : pose to convert
    #@return Pose2D
    @staticmethod
    def fromPoseMsg(poseMsg):
        p = Pose2D()
        p.x = poseMsg.x
        p.y = poseMsg.y
        p.h = poseMsg.h
        return p
    
    #Convert a Pose2D to a PoseMsg protobuf message
    #@param CommonMsg_pb2.PointCap msg : the msg which receive the data
    def toPoseMsg(self, msg):
        msg.x = self.x
        msg.y = self.y
        msg.h = self.h
    
    
    def __add__(self, p):
        if not isinstance(self.x,str) and not isinstance(p.x,str):
            x = self.x+p.x
        if not isinstance(self.y,str) and not isinstance(p.y,str):
            y = self.y+p.y
        if not isinstance(self.h,str) and not isinstance(p.h,str):
            h = normalizeAngle(self.h+p.h)    
        return Pose2D(x, y, h)
    
    def __mul__(self, k):
        if not isinstance(self.x,str) and not isinstance(p.x,str):
            x = self.x*p.x
        if not isinstance(self.y,str) and not isinstance(p.y,str):
            y = self.y*p.y
        if not isinstance(self.h,str) and not isinstance(p.h,str):
            h = normalizeAngle(self.h*p.h)    
        return Pose2D(x, y, h)
    
    def __sub__(self, p):
        if not isinstance(self.x,str) and not isinstance(p.x,str):
            x = self.x-p.x
        if not isinstance(self.y,str) and not isinstance(p.y,str):
            y = self.y-p.y
        if not isinstance(self.h,str) and not isinstance(p.h,str):
            h = normalizeAngle(self.h-p.h)    
        return Pose2D(x, y, h)
 
    def __repr__(self):
        """return a string representation of this point. This method is
        called by the repr() function, and
        also the str() function. It should produce a string that, when
        evaluated, returns a point with the 
        same data."""
        if True == isinstance(self.x,str):
            l_x = float('nan')
        else:
            l_x = self.x
            
        if True == isinstance(self.y,str):
            l_y = float('nan')
        else:
            l_y = self.y
            
        if True == isinstance(self.h,str):
            l_h = float('nan')
        else:
            l_h = self.h
        
        return '[%0.3f , %0.3f , %0.2f]' % (l_x, l_y, l_h) 
 