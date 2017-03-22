#!/usr/bin/env python
# -*- coding: latin-1 -*-

from generated import *
from core import Music

class Tone:
    def __init__(self, freq = 3000, duration = 1000):
        self.frequency = freq #Hz
        self.duration = duration #ms
        
    def toMsg(self, msg):
        msg.frequency = self.frequency
        msg.duration = self.duration
        
class Melody:
    def __init__(self):
        self.tones = list()
        
    def toMsg(self, msg):
        for tone in self.tones:
            toneMsg = msg.tones.add()
            tone.toMsg(toneMsg)
    
    #append a new tone to the melody
    def add(self, tone):
        assert isinstance(tone, Tone)
        self.tones.append(tone)