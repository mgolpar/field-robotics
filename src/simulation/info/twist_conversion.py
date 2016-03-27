#!/usr/bin/env python

import rospy 
from geometry_msgs.msg import Twist, TwistStamped
import time

def callback (cmdVel):
	# work 
	#TwistMsgs = cmdVel.twist 

	PublishTwistMsgs = rospy.Publisher('twist_command', Twist, queue_size=10)
	PublishTwistMsgs.publish(cmdVel) #TwistMsgs


def twist_conversion():
	rospy.init_node('twist_conversion', anonymous=True)
	rospy.Subscriber("cmd_vel",Twist, callback) #change to TwistStamped
	rospy.spin()


if __name__ == '__main__':
	try:
		twist_conversion()
	except rospy.ROSInterruptException:
		pass


#TODO, make the first 2 commands, you know, NOT be ignored would be great
