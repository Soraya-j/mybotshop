#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Software License Agreement (BSD)
#
# @author    Salman Omar Sohail <support@mybotshop.de>
# @copyright (c) 2024, MYBOTSHOP GmbH, Inc., All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# * Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
# * Neither the name of MYBOTSHOP GmbH nor the names of its contributors
#   may be used to endorse or promote products derived from this software
#   without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

# Redistribution and use in source and binary forms, with or without
# modification, is not permitted without the express permission
# of MYBOTSHOP GmbH.

import math
import rclpy
import tf2_ros
import numpy as np
from rclpy.node import Node
from tf2_ros import Buffer, TransformListener
from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import TransformStamped
from tf2_ros.static_transform_broadcaster import StaticTransformBroadcaster

class AprilTagPublisher(Node):
    
    def __init__(self):
        
        super().__init__('april_tag_publisher')
        self.get_logger().info(self.colorize("Initializing april tag publisher!", 'orange'))
        
        # Parameters Declaration
        self.declare_parameters(namespace='', parameters=[('source_frame', 'odom'),
                                                          ('target_frame', 'home_dock'),
                                                          ('pub_topic', 'docker'),]) 
        
        self.param_source_frame = self.get_parameter('source_frame').value
        self.get_logger().info(f'{self.colorize(f"source_frame: {self.param_source_frame}","blue")}')
        
        self.param_target_frame = self.get_parameter('target_frame').value
        self.get_logger().info(f'{self.colorize(f"target_frame: {self.param_target_frame}","blue")}')
        
        self.param_pub_topic = self.get_parameter('pub_topic').value
        self.get_logger().info(f'{self.colorize(f"pub_topic: {self.param_pub_topic}","blue")}')

        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)
        self.pose_publisher = self.create_publisher(PoseStamped, self.param_pub_topic, 10)
        self.timer = self.create_timer(0.1, self.timer_callback)  # Publish at 10 Hz
        self.tf_static_broadcaster = StaticTransformBroadcaster(self)
        
        self.get_logger().info(self.colorize("Initialized april tag publisher!", 'green'))

    def timer_callback(self):
        try:
            # Get the latest transform
            transform = self.tf_buffer.lookup_transform(self.param_target_frame, self.param_source_frame, rclpy.time.Time())
            
            # Convert TransformStamped to PoseStamped
            pose_stamped = PoseStamped()
            pose_stamped.header.stamp = self.get_clock().now().to_msg()
            pose_stamped.header.frame_id = self.param_target_frame
            
            pose_stamped.pose.position.x = transform.transform.translation.x
            pose_stamped.pose.position.y = transform.transform.translation.y
            pose_stamped.pose.position.z = transform.transform.translation.z            
               
            pose_stamped.pose.orientation = transform.transform.rotation
            
            t = TransformStamped()

            t.header.stamp = self.get_clock().now().to_msg()
            t.header.frame_id = self.param_target_frame
            t.child_frame_id = 'new_frame'

            t.transform.translation.x =  pose_stamped.pose.position.x
            t.transform.translation.y =  pose_stamped.pose.position.y
            t.transform.translation.z =  pose_stamped.pose.position.z
            t.transform.rotation = pose_stamped.pose.orientation

            # Publish the PoseStamped message
            self.pose_publisher.publish(pose_stamped)
            self.tf_static_broadcaster.sendTransform(t)
            
            self.get_logger().info(self.colorize('Transform Detected', 'green'), once=True)
            
        except tf2_ros.LookupException:
            self.get_logger().warn(self.colorize('Transform lookup error', 'yellow'),throttle_duration_sec=5.0)
        except tf2_ros.ConnectivityException:
            self.get_logger().warn('Transform connectivity error')
        except tf2_ros.ExtrapolationException:
            self.get_logger().warn('Transform extrapolation error')

    def colorize(self, text, color):
        color_codes = {
            'green': '\033[92m',
            'yellow': '\033[93m',
            'orange': '\033[38;5;208m', 
            'blue': '\033[94m',
            'red': '\033[91m'
        }
        return color_codes[color] + text + '\033[0m'
    
    def quaternion_from_euler(self, ai, aj, ak):
        ai /= 2.0
        aj /= 2.0
        ak /= 2.0
        ci = math.cos(ai)
        si = math.sin(ai)
        cj = math.cos(aj)
        sj = math.sin(aj)
        ck = math.cos(ak)
        sk = math.sin(ak)
        cc = ci*ck
        cs = ci*sk
        sc = si*ck
        ss = si*sk

        q = np.empty((4, ))
        q[0] = cj*sc - sj*cs
        q[1] = cj*ss + sj*cc
        q[2] = cj*cs - sj*sc
        q[3] = cj*cc + sj*ss

        return q
        
    def destroy_node(self):
        self.get_logger().info(f'{self.colorize("Shutting down april tag publisher!","red")}')            
        super().destroy_node()    
        