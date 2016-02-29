#include "ros/ros.h"
#include "boost/asio.hpp"
#include "jaws2_msgs/PwmStamped.h"

boost::asio::io_service i_o;
boost::asio::serial_port s_p(i_o);

void callback(const jaws2_msgs::PwmStamped::ConstPtr& force)
{
  const int SIZE = 21;
  unsigned char packet[SIZE];

  packet[0]  = '#';

  packet[1]  = force->pwm.f1 >> 8;
  packet[2]  = force->pwm.f1;

  packet[3]  = force->pwm.f2 >> 8;
  packet[4]  = force->pwm.f2;

  packet[5]  = force->pwm.f3 >> 8;
  packet[6]  = force->pwm.f3;

  packet[7]  = force->pwm.f4 >> 8;
  packet[8]  = force->pwm.f4;

  packet[9]  = force->pwm.u1 >> 8;
  packet[10]  = force->pwm.u1;

  packet[11]  = force->pwm.u2 >> 8;
  packet[12]  = force->pwm.u2;

  packet[13]  = force->pwm.u3 >> 8;
  packet[14]  = force->pwm.u3;

  packet[15]  = force->pwm.u4 >> 8;
  packet[16]  = force->pwm.u4;

  packet[17]  = force->pwm.s1 >> 8;
  packet[18]  = force->pwm.s1;

  packet[19]  = force->pwm.s2 >> 8;
  packet[20]  = force->pwm.s2;

  s_p.write_some(boost::asio::buffer(packet, SIZE));
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "serial_port");

  std::string port_name;
  int baud_rate;

  ros::NodeHandle nh;
  ros::Subscriber sub;

  nh.param<std::string>("serial_port/name", port_name, "/dev/ttyUSB0");
  nh.param<int>("serial_port/rate", baud_rate, 9600);

  s_p.open(port_name);
  ROS_INFO("Serial port name: %s", port_name.c_str());
  s_p.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
  ROS_INFO("Serial port rate: %i", baud_rate);

  sub = nh.subscribe<jaws2_msgs::PwmStamped>("thrust_cal/pwm", 1, &callback);

  ros::spin();
}
