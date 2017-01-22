#!/usr/bin/env python
# -*- coding: latin-1 -*-

from math import *

class Point(object):
    """Point class with public x and y attributes 
        Un point n'as pas de cap a priori. Il sera utilise pour definir des positions de reference sur la table
        Ce sont en general des positions que l'ont abordera depuis plusieurs directions differentes
    """
    def __init__(self, x=0., y=0.):
        if True == isinstance(x,str) and x =="FREE":
            self.x = 'FREE'
        else:
            self.x = float(x)
            assert abs(self.x) < 3., "x is out of table %s" % x
        
        if True == isinstance(y,str) and y =="FREE":
            self.y = 'FREE'
        else:
            self.y = float(y)
            assert abs(self.y) < 3., "y is out of table %s" % y
            
 
    def dist(self, p):
        """return the Euclidian distance between self and p"""
        dx = self.x - p.x
        dy = self.y - p.y
        return sqrt(dx*dx + dy*dy)
 
    def norm(self):
        """return the Euclidian norm"""
        return sqrt(self.x*self.x + self.y*self.y)
 
    def angle(self, p):
        """return the Euclidian distance between self and p"""
        dx = self.x - p.x
        dy = self.y - p.y
        return atan2(dy,dx)
 
    def reset(self):
        self.x = 0.
        self.y = 0.

    def __add__(self, p):
        """return a new point found by adding self and p. This method is
        called by e.g. p+q for points p and q"""
        return Point(self.x+p.x, self.y+p.y)
    
    def __mul__(self, k):
        return Point(self.x*k, self.y*k)
    
    def __sub__(self, p):
        return Point(self.x-p.x, self.y-p.y)
 
    def __repr__(self):
        """return a string representation of this point. This method is
        called by the repr() function, and
        also the str() function. It should produce a string that, when
        evaluated, returns a point with the 
        same data."""
        return '[%0.3f , %0.3f]' % (self.x, self.y)     
    
