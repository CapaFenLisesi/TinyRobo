#!/usr/bin/python

# Script for recognizing gestures from a recording of user input
# Events are either events of the form:
# event = {"time": event.time_update,
#          "uid": event.uid,
#          "start_time" : event.time_start,
#          "end_time" : event.time_end,
#          "event_x" : event.x,
#          "event_y" : event.y,
#          "update_time" : event.time_update,
#          "shape" : event.shape}
# or metaevents of the form:
# event = {"time": time.time(),
#          "desc": desc}


import Image, ImageDraw, ImageFont
import uuid
import numpy as np
import pickle
import math

infile = "26-08-17_17-10-15_s555_c10.pickle"

def isMeta(event):
	if "desc" in event.keys():
		return True
	return False

def distanceEvents(eventA, eventB):
	x1 = eventA["event_x"]
	y1 = eventA["event_y"]
	x2 = eventB["event_x"]
	y2 = eventB["event_y"]

	return math.sqrt(pow(x1 - x2, 2) + pow(y1 -y2, 2))

def distance(x1, y1, x2, y2):
	return math.sqrt(pow(x1 - x2, 2) + pow(y1 -y2, 2))

def clamp(n, minn, maxn):
    return max(min(maxn, n), minn)

#Debug function, dumps strokes to image files for viewing
def dumpStroke(stroke):
	pxlPad = 16
	#Load parameters for the image, pad the width and height
	path = "stroke_{0}.png".format(stroke.id)
	width = int((stroke.maxX - stroke.minX)) + pxlPad
	height = int((stroke.maxY - stroke.minY)) + pxlPad

	#Create a new image with the required shape
	img = Image.new("RGB", (width, height))
	draw = ImageDraw.Draw(img)

	#Set up the colors of the logged events by time
	color_length = (len(stroke.events) + 3)/3
	reds = np.linspace(0,255, color_length, dtype=int)
	reds = np.append(reds[:-1], np.fliplr([reds])[0])
	reds = np.append(reds, np.zeros(2*color_length, dtype = int))
	greens = np.roll(reds, color_length)
	blues = np.roll(greens, color_length)
	#Combine them all
	colors = zip(reds, greens, blues)

	#The event didn't have enoug points to bother with colors, it's probably a tap or doubletap
	#So do it all in black
	#if len(colors) == 0:
	#	colors = [(0,0,0)] * 3

	for event in stroke.events:
		#Convert to image coordinates
		x = int(event['event_x'])
		#This flips the image, kivy coordinates are upside-down relative to PIL coordinates
		y = stroke.maxY - int(event['event_y'])
		#Then move them from full screen space to the smaller image, including padding
		x = x - stroke.minX + pxlPad/2
		y = y - stroke.minY + pxlPad/2

		#Draw the event
		draw.ellipse([(x-3,y-3),(x+3,y+3)], fill=colors.pop(0))

	#Draw the stroke centroid
	#Convert to image coordinates
	x = int(stroke.centroid[0])
	#This flips the image, kivy coordinates are upside-down relative to PIL coordinates
	y = stroke.maxY - int(stroke.centroid[1])
	#Then move them from full screen space to the smaller image, including padding
	x = x - stroke.minX + pxlPad/2
	y = y - stroke.minY + pxlPad/2

	#Draw an X
	draw.line([(x-3,y-3),(x+3,y+3)], fill=(255, 0, 0))
	draw.line([(x-3,y+3),(x+3,y-3)], fill=(255, 0, 0))

	font = ImageFont.truetype("DejaVuSans.ttf", 16)
		
	#Can't have an angle between less than two events
	if len(stroke.events) > 2:
		#Get the angle between the end points around the centroid
		#Start by getting the distances between the start and end events and the centroid
		x1 = stroke.events[0]["event_x"]
		y1 = stroke.events[0]["event_y"]
		x2, y2 = stroke.centroid
		d1 = distance(x1, y1, x2, y2)
		x1 = stroke.events[-1]["event_x"]
		y1 = stroke.events[-1]["event_y"]
		d2 = distance(x1, y1, x2, y2)
		#Distance between the start and end events
		d3 = distanceEvents(stroke.events[0], stroke.events[-1])
		#From the law of cosines and https://stackoverflow.com/questions/1211212/how-to-calculate-an-angle-from-three-points
		#This should be in radians, math.acos is in radians, according to the docs
		#value is capped at 1 because numerical imprecision was causing math domain errors by feeding
		#acos values like -1.00000000002, which is, technically, out of ranges
		if (d1 * d2) == 0:
			draw.text((0, 0),"d1 or d2 was zero",(255,255,255),font=font)
			return
		capped = clamp((pow(d1, 2) + pow(d2, 2) - pow(d3, 2))/(2 * d1 * d2), -1, 1)
		angle = math.acos(capped)
		if angle < 1:
			draw.text((0, 0), "Circle {0} points".format(len(stroke.events)) ,(255,255,255),font=font)
		elif angle < 2.5:
			draw.text((0, 0), "Arc {0} points".format(len(stroke.events)) ,(255,255,255),font=font)
		else:
			draw.text((0, 0), "Line {0} points".format(len(stroke.events)) ,(255,255,255),font=font)		
	else:
		draw.text((0, 0), "Point" ,(255,255,255),font=font)
	#Write the file
	img.save(path)

class GestureStroke():
	def __init__(self, event):
		self.id = event['uid']
		self.startTime = event['start_time']
		self.endTime = None
		self.centroid = [event['event_x'], event['event_y']]
		self.isEnded = False
		self.events = [event]
		#Lucky me, Kivy doesn't use negative coordinates or put the origin in the middle of the screen
		self.maxX = self.minX = self.maxY = self.minY = 0


	def addEvent(self, event):
		#Events have ends, and stuff can't occur after the end of the event
		#It does happen, there appear to be some duplicate events, but not often
		if self.isEnded:
			print "Attempt to add event to ended stroke {0}".format(self.id)
			return None

		self.events.append(event)

		#Update the running centroid total
		#The centroid isn't correct until the event is ended
		self.centroid[0] += event['event_x']
		self.centroid[1] += event['event_y']

		#Update the bounding box
		if event['event_x'] > self.maxX:
			self.maxX = event['event_x']
		if event['event_y'] > self.maxY:
			self.maxY = event['event_y']
		if event['event_x'] < self.minX:
			self.minX = event['event_x']
		if event['event_y'] < self.minY:
			self.minY = event['event_y']

		#This was the last event of this stroke, so end it and calculate the centroid
		if event["end_time"] != -1:
			self.endTime = event['end_time']
			self.isEnded = True
			self.centroid[0] = self.centroid[0]/len(self.events)
			self.centroid[1] = self.centroid[1]/len(self.events)

	def overlaps(otherEvent):
		if otherEvent.startTime < self.startTime < otherEvent.endTime:
			#This event started while the other event was going on
			return True
		if self.startTime < otherEvent.startTime < self.endTime:
			#The other event started while this event was going on
			return True
		return False

strokes = {}

#Load the recorded strokes from the user interface
with open(infile, 'r') as inputData:

	#Cut the extension off the input file name
	outfile_suffix = infile[:-7]
	
	try:
		event = pickle.load(inputData)
		while event is not None:
			if isMeta(event):
				#Don't do anything, meta is stuff like file loading, and this deals with contacts
				pass
			else:
				if event['uid'] not in strokes.keys():
					#If this event is just starting, create a new entry for it in strokes
					strokes[event['uid']] = GestureStroke(event)
				else:
					#Event already started, extend its entry
					strokes[event['uid']].addEvent(event)

			event = pickle.load(inputData)
	except EOFError:
		print "Done loading events"
	

#Now strokes contains every touch event series
for stroke in strokes.values():

	dumpStroke(stroke)


