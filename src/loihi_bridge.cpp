// Copyright 2015 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include <memory>
#include <utility>

// include ROS 1
#ifdef __clang__
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunused-parameter"
#endif
#include "ros/ros.h"
#include "std_msgs/Float64MultiArray.h"
#ifdef __clang__
# pragma clang diagnostic pop
#endif

// include ROS 2
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"

rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr float_px4_command_pub;

void px4_commandsCallback(const std_msgs::Float64MultiArray::ConstPtr & ros1_msg)
{
  std::cout << "Received from ROS1: " << ros1_msg->data[0] << ", " << ros1_msg->data[1] << ", " << ros1_msg->data[2] << ", " << ros1_msg->data[3] << std::endl;
  auto ros2_msg = std::make_unique<std_msgs::msg::Float64MultiArray>();
  ros2_msg->data = ros1_msg->data;
  float_px4_command_pub->publish(std::move(ros2_msg));
}

int main(int argc, char * argv[])
{
  // ROS 2 node and publisher
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("talker");
  float_px4_command_pub = node->create_publisher<std_msgs::msg::Float64MultiArray>("/ros2_px4_commands", 10);

  // ROS 1 node and subscriber
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber float_px4_command_sub = n.subscribe("/px4_commands", 10, px4_commandsCallback);

  ros::spin();

  return 0;
}
