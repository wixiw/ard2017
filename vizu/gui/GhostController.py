#!/usr/bin/python3
# -*- coding: utf-8 -*-

from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from core import *
import math
import copy

#Manage the ghost displacement with mouse
class GhostController():
    
    def __init__(self):
        #position of the mouse when the translation or rotation was started
        self.rotate_entry_clic = None
        #position of the robot at the time a translation or rotation is in progress
        self.move_ghost_start_pos = None
        
        self.state = "Idle"
        
        #TODO

        
    #@param pressed : boolean
    #@param hitPoint : Point
    def mouseLeftClicCb(self, pressed, hitPoint, ghostPose, isInCarriage):
        print("HIT : ", str(hitPoint))
        
        if self.state == "Idle" and pressed:
            if isInCarriage:
                print("Ghost clicked")
                self.rotate_entry_clic = hitPoint
                self.move_ghost_start_pos = ghostPose
                self.state = "MoveGhost"
            else:
                self.rotate_entry_clic = Point(hitPoint.x,hitPoint.y)
                self.move_ghost_start_pos = ghostPose
                self.state = "RotateGhost"
                       
        if self.state == "MoveGhost" and not pressed:
            print("Ghost released")
            self.rotate_entry_clic = None
            self.move_ghost_start_pos = None
            self.state = "Idle"
            
        if self.state == "RotateGhost" and not pressed:
            print("Ghost rotate released")
            self.rotate_entry_clic = None
            self.move_ghost_start_pos = None
            self.state = "Idle"  

    #callback for ArdMouseController
    def mouseSlideCb(self, hitPoint, ghostPose):
        newGhostPose = copy.copy(ghostPose)
            
        if self.state == "MoveGhost":
            dx = hitPoint.x - self.rotate_entry_clic.x
            dy = hitPoint.y - self.rotate_entry_clic.y
            newGhostPose = Pose2D(self.move_ghost_start_pos.x + dx, self.move_ghost_start_pos.y + dy, math.degrees(self.move_ghost_start_pos.h))
            
        elif self.state == "RotateGhost":
            #get clic angles relatively to the robot center or mass
            entryAngle = atan2(self.rotate_entry_clic.y - ghostPose.y, self.rotate_entry_clic.x - ghostPose.x)
            clicAngle = math.atan2(hitPoint.y - ghostPose.y, hitPoint.x - ghostPose.x)
            #print("hit " + str(hitPoint) + " ghostPose=" + str(ghostPose) + " entry=" + str(self.rotate_entry_clic))
            #print("entry=" + str(entryAngle) + " clic=" + str(clicAngle) + "delta rot=" + str(clicAngle - entryAngle))
            newGhostPose.h = normalizeAngle(self.move_ghost_start_pos.h + clicAngle - entryAngle)
            
        return newGhostPose
