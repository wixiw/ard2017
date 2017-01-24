#!/usr/bin/env python
# -*- coding: latin-1 -*-

from math import *
from core.Point import *
    
def intersectCircle(x0,y0,r0,x1,y1,r1):
    #see http://paulbourke.net/geometry/2circle/ for justif
    
    #compute the intersection of 2 circles
    #x0,y0,r0 is the first circle
    #x1,y1,r1 the second circle
    #RETURN:
    # x3_1, y3_1 the first point of intersection
    # x3_2, y3_2 the second intersection
    
    d=sqrt((x0-x1)**2+(y0-y1)**2)
    if d>r0+r1:
        return
    
    a = (r0**2 - r1**2 + d**2 ) / (2* d)
    if (r0**2 - a**2 <=0.0):
        return
    h = sqrt(r0**2 - a**2) 
    
    x2 = x0 + a * ( x1 - x0 ) / d
    y2 = y0 + a * ( y1 - y0 ) / d
    
    x3_1 = x2 + h* ( y1 - y0 ) / d 
    y3_1 = y2 - h* ( x1 - x0 ) / d 
    
    x3_2 = x2 - h* ( y1 - y0 ) / d 
    y3_2 = y2 + h* ( x1 - x0 ) / d 
    
    return (x3_1,y3_1,x3_2,y3_2)

def averageAngle(h1,h2):
    #compute the mean angle, between - PI and +PI, of 2 angles. takes the smallest angle between h1 and h2 to define the direction
    return normalizeAngle(h1+normalizeAngle(h2-h1)/2)

def isInRectangle(pointObj,pointRect1,pointRect2,widthRect,deltaLenght):
    #answer if the considered point is in a rectangle
    #
    #    ------------------------------
    #    |                            |
    #    |    x                   x   |
    #    |____________________________|
    #
    #   the 2 "x" :    pointRect1 and  pointRect2
    #
    #    the lenght:  norm(pointRect2-pointRect1) + 2*deltaLenght
    #    the height:   widthRect
    
    if pointRect1.dist(pointRect2)==0.0:
        return False
    
    #vecteur unitaire oriente de P1 vers P2
    unitVect=(pointRect2-pointRect1)*(1/pointRect1.dist(pointRect2))


    # et son ami otrhogonal
    orthoVect=Point(-unitVect.y,unitVect.x)

    A=pointRect1-unitVect*deltaLenght+orthoVect*widthRect*(0.5)
    B=pointRect1-unitVect*deltaLenght-orthoVect*widthRect*(0.5)
    C=pointRect2+unitVect*deltaLenght-orthoVect*widthRect*(0.5)
    D=pointRect2+unitVect*deltaLenght+orthoVect*widthRect*(0.5)

    #je cree les 4 points de mon rectangle
    rect=[]
    rect.append([A.x,A.y])
    rect.append([B.x,B.y])
    rect.append([C.x,C.y])
    rect.append([D.x,D.y])
    return point_inside_polygon(pointObj.x,pointObj.y,rect)
    


# determine if a point is inside a given polygon or not
# Polygon is a list of (x,y) pairs.
#taken from the net

#y'a quentin qui a explique le principe: tu traces une demidroite qui part du point et tu regardes combien de fois (pair ou impair) ca intersecte un segment

def point_inside_polygon(x,y,poly):

    n = len(poly)
    inside =False

    p1x,p1y = poly[0]
    for i in range(n+1):
        p2x,p2y = poly[i % n]
        if y > min(p1y,p2y):
            if y <= max(p1y,p2y):
                if x <= max(p1x,p2x):
                    if p1y != p2y:
                        xinters = (y-p1y)*(p2x-p1x)/(p2y-p1y)+p1x
                    if p1x == p2x or x <= xinters:
                        inside = not inside
        p1x,p1y = p2x,p2y

    return inside


    