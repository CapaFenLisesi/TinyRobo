#!/usr/bin/python

#A Kivy app with a ROS camera feed as the background
import kivy
kivy.require('1.9.1')

#Don't resize the window
#This has to be before any other Kivy imports, or it fails quietly
from kivy.config import Config
Config.set('graphics', 'resizable', False)
#Config.set('graphics', 'fullscreen', 'fake')
Config.set('graphics', 'borderless', False)
from kivy.app import App

from kivy.core.window import Window
from kivy.uix.widget import Widget
from kivy.uix.image import Image as kvImage

import rospy
from std_msgs.msg import Header
from sensor_msgs.msg import Image as rosImage
from cv_bridge import CvBridge, CvBridgeError

#TODO this should probably be customizable
cam_topic = "/usb_cam/image_raw"

#At the moment, just listens for the quit command
class KeyboardListener(Widget):
    def __init__(self, **kwargs):
        super(KeyboardListener, self).__init__(**kwargs)
        self._kbrd = Window.request_keyboard(self._keyboard_closed, self, 'text')
        if self._kbrd.widget:
            # If it exists, this widget is a VKeyboard object which you can use
            # to change the keyboard layout.
            pass
        self._kbrd.bind(on_key_down=self._on_keyboard_down)
    
    def _keyboard_closed(self):
        self._kbrd.unbind(on_key_down=self._on_keyboard_down)
        self._kbrd = None

    def _on_keyboard_down(self, keyboard, keycode, text, modifiers):
        if keycode[1] == 'q':
            keyboard.release()
            App.get_running_app().stop()

        # Return True to accept the key. Otherwise, it will be used by
        # the system.
        return True

class ROSImage(kvImage):
    def __init__(self, **kwargs):
        super(ROSImage, self).__init__(**kwargs)
        
        #Subscribe to images from the overhead camera
        self.cfg = Config.get_configparser('app')
        print self.cfg.get("CamTopic", "path")
        rospy.Subscriber(self.cfg.get("CamTopic", "path"), rosImage, self.update_image)
        
    def on_touch_down(self, touch):
        if self.collide_point(*touch.pos):
            pass

    def on_touch_up(self, touch):
        if self.collide_point(*touch.pos):
            pass

    def on_touch_move(self, touch):
        if self.collide_point(*touch.pos):
            pass

    def update_image(self, msg):
        print "Got an image"
        img = self.bridge.imgmsg_to_cv2(msg, desired_encoding="bgr8")
        self.source = img
        self.canvas.ask_update()

class RobotUIMainApp(App):
    def __init__(self, **kwargs):
        super(RobotUIMainApp, self).__init__(**kwargs)
        
    def build_config(self, config):
        #If you don't set any defaults, Kivy won't load your config at all 
        config.setdefaults('CamTopic', {'path': cam_topic})
        
    def build(self):
        pass

    def on_pause(self):
        #Not sure this should do anything
        return True

    def on_stop(self):
        #Unsubscribe from ROS messages
        return True


if __name__ == '__main__':
    rospy.init_node('RobotUI')
    RobotUIMainApp().run()