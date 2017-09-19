#include <ros/ros.h>

#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include "geometry_msgs/TwistStamped.h"

#include <image_transport/image_transport.h>

#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>

#include <cv_bridge/cv_bridge.h>
#include <sstream>


class DetectionImagePublisher
{
  public:
    DetectionImagePublisher()   
    : nh("~"){ 
        InitializeSubscribers();
        InitializePublishers();
      }

  private:  

  void InitializePublishers() {
    image_transport::ImageTransport img_trans(nh);
    image_pub = img_trans.advertise("speed_labeled_rgb", 10);
  }

  void InitializeSubscribers() {
    image_sub = nh.subscribe("/flight/r200/color/image_raw", 1, &DetectionImagePublisher::OnImage, this);
    twist_sub = nh.subscribe("/twist", 1, &DetectionImagePublisher::OnTwist, this);
    //time_sync_ptr = std::make_shared<message_filters::TimeSynchronizer<sensor_msgs::Image, geometry_msgs::TwistStamped>>(*image_sub_ptr, *twist_sub_ptr, 10);
    //time_sync_ptr->registerCallback(boost::bind(&DetectionImagePublisher::SynchronizedCallback, this, _1, _2));

  }

  void OnImage(const sensor_msgs::Image::ConstPtr& image_msg) {
      cv_bridge::CvImagePtr input_image_ptr = cv_bridge::toCvCopy(image_msg, sensor_msgs::image_encodings::RGB8);
      cv::Point p1(100, 100);
      cv::Point p2(200, 200);
      cv::Scalar color(0, 255, 0);
      cv::rectangle(input_image_ptr->image, p1, p2, color, 2);
      std::ostringstream strs;
      strs << speed;
      std::string speed_str = strs.str();
      cv::putText(input_image_ptr->image, "speed", p1 + cv::Point(5, 20), 0, 0.75, color, 2);
      cv::putText(input_image_ptr->image, speed_str, p1 + cv::Point(5, 70), 0, 0.75, color, 2);
      image_pub.publish(input_image_ptr->toImageMsg());
  }

  void OnTwist(const geometry_msgs::TwistStamped& twist) {
    speed = sqrt(twist.twist.linear.x*twist.twist.linear.x + twist.twist.linear.y*twist.twist.linear.y + twist.twist.linear.z*twist.twist.linear.z);
    std::cout << speed << std::endl;
  }

 // void SynchronizedCallback(const sensor_msgs::Image::ConstPtr& image_msg, const geometry_msgs::TwistStamped& twist_msg) {
 //    cv_bridge::CvImagePtr input_image_ptr = cv_bridge::toCvCopy(image_msg, sensor_msgs::image_encodings::RGB8);

 //    // for(auto const& detection : detections_msg->detections) {

 //    //   cv::Point p1(detection.x_min, detection.y_min);
 //    //   cv::Point p2(detection.x_max, detection.y_max);

 //    //   cv::Scalar color(0, 255, 0);
 //    //   cv::rectangle(input_image_ptr->image, p1, p2, color, 2);
 //    //   cv::putText(input_image_ptr->image, detection.class_name, p1 + cv::Point(5, 20), 0, 0.75, color, 2);
 //    // }

 //    image_pub.publish(input_image_ptr->toImageMsg());
 // }


  ros::NodeHandle nh;

  image_transport::Publisher image_pub; 
  
  ros::Subscriber image_sub;
  ros::Subscriber twist_sub;
  double speed = 0.0;
};


int main(int argc, char *argv[])
{

  ros::init(argc, argv, "detection_image");
  
  ros::NodeHandle nh("~");
  
  DetectionImagePublisher detection_pub;
  ros::spin();
  return 0;
}