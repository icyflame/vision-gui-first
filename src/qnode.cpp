/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>
#include "../include/App/qnode.hpp"

#include <QtGui>

#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

using namespace std;
//using namespace cv;

/*****************************************************************************
** Namespaces
*****************************************************************************/
namespace App {

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{

    //    cv::Mat I;

    //    cv_bridge::CvImagePtr cv_ptr;
    //    try
    //    {
    //        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    //    }
    //    catch (cv_bridge::Exception& e)
    //    {
    //        ROS_ERROR("cv_bridge exception: %s", e.what());
    //        return;
    //    }

    //    I = cv_ptr->image;
    //    imshow("Input Image", cv_ptr->image);

    std::cout << "Came into the callback;" << "\n";

    //    QImage temp(&(msg->data[0]), msg->width, msg->height, QImage::Format_RGB888);

    //    QImage image;
    //    image=temp;
    //    // QT Layout with button and image

    //    static QWidget *window = new QWidget;
    //    static QLabel *imageLabel= new QLabel;
    //    static QPushButton* quitButton= new QPushButton("Quit");
    //    static QPushButton* exitButton= new QPushButton("Exit Image");
    //    QVBoxLayout* layout= new QVBoxLayout;


    //    imageLabel->setPixmap(QPixmap::fromImage(image));
    //    layout->addWidget(imageLabel);
    //    layout->addWidget(exitButton);
    //    layout->addWidget(quitButton);

    //    window->setLayout(layout);
    //    QObject::connect(quitButton, SIGNAL(clicked()),window, SLOT(close()));  // Adding Buttons
    //    QObject::connect(exitButton, SIGNAL(clicked()),imageLabel, SLOT(close()));

    //    window->show();
    //    cvWaitKey(1);

}

/*****************************************************************************
** Implementation
*****************************************************************************/

QNode::QNode(int argc, char** argv ) :
    init_argc(argc),
    init_argv(argv)
{}

QNode::~QNode() {
    if(ros::isStarted()) {
        ros::shutdown(); // explicitly needed since we use ros::start();
        ros::waitForShutdown();
    }
    wait();
}

bool QNode::init() {
    ros::init(init_argc,init_argv,"App");
    if ( ! ros::master::check() ) {
        return false;
    }
    ros::start(); // explicitly needed since our nodehandle is going out of scope.
    ros::NodeHandle n;
    // Add your ros communications here.
    /*chatter_publisher = n.advertise<std_msgs::String>("chatter", 1000);
    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
    ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);*/
    image_transport::ImageTransport it(n);

    sub = it.subscribe("imageStream", 1000, &QNode::imageCallback, this);

    //        chatter_subscriber = n.subscribe("depth", 1, &QNode::chatterCallback,this);

    start();

    return true;
}

bool QNode::init(const std::string &master_url, const std::string &host_url) {
    std::map<std::string,std::string> remappings;
    remappings["__master"] = master_url;
    remappings["__hostname"] = host_url;
    ros::init(remappings,"App");
    if ( ! ros::master::check() ) {
        return false;
    }
    ros::start(); // explicitly needed since our nodehandle is going out of scope.
    ros::NodeHandle n;

    image_transport::ImageTransport it(n);
    image_transport::Subscriber sub;

    sub = it.subscribe("imageStream", 1000, &QNode::imageCallback, this);
    // Add your ros communications here.
    //    chatter_subscriber = n.subscribe("depth", 1000, &QNode::chatterCallback,this);
    cvNamedWindow("view");
    start();
    return true;
}

void QNode::run() {
    ros::Rate loop_rate(10);
    int count = 0;
    while ( ros::ok() ) {

        //        cout << "Inside the run() function \n";

        std_msgs::String msg;
        std::stringstream ss;
        ss << "hello world " << count;
        msg.data = ss.str();
        //chatter_subscriber.publish(msg);
        //log(Info,std::string("I sent: ")+msg.data);
        ros::spinOnce();
        loop_rate.sleep();
        ++count;


    }
    std::cout << "Ros shutdown, proceeding to close the gui." << std::endl;
    Q_EMIT rosShutdown(); // used to signal the gui for a shutdown (useful to roslaunch)
}


void QNode::chatterCallback(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO("I heard: [%s]", msg->data.c_str());
    _msg = std::string(msg->data.c_str());
    Q_EMIT loggingUpdated();

}

void QNode::imageCallback(const sensor_msgs::ImageConstPtr& msg)
{

    //    ROS_INFO("inside the callback function.\n");

    //    cv::Mat I;

    //    cv_bridge::CvImagePtr cv_ptr;
    //    try
    //    {
    //        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    //    }
    //    catch (cv_bridge::Exception& e)
    //    {
    //        ROS_ERROR("cv_bridge exception: %s", e.what());
    //        return;
    //    }

    //    I = cv_ptr->image;
    //    cv::imshow("name", cv_ptr->image);
    //    cvWaitKey(40);

    QImage temp(&(msg->data[0]), msg->width, msg->height, QImage::Format_RGB888);

    _image = temp;
    // QT Layout with button and image

//    static QWidget *window = new QWidget;
//    static QLabel *imageLabel= new QLabel;
//    static QPushButton* quitButton= new QPushButton("Quit");
//    static QPushButton* exitButton= new QPushButton("Exit Image");
//    QVBoxLayout* layout= new QVBoxLayout;


//    imageLabel->setPixmap(QPixmap::fromImage(image));
//    layout->addWidget(imageLabel);
//    layout->addWidget(exitButton);
//    layout->addWidget(quitButton);

//    window->setLayout(layout);
//    QObject::connect(quitButton, SIGNAL(clicked()),window, SLOT(close()));  // Adding Buttons
//    QObject::connect(exitButton, SIGNAL(clicked()),imageLabel, SLOT(close()));

//    window->show();
//    cvWaitKey(1);
    Q_EMIT updatePic();
}

}  // namespace App
