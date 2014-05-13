/**
 * @file /include/App/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef App_QNODE_HPP_
#define App_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <string>
#include <QThread>
#include <std_msgs/String.h>
#include <QStringListModel>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace App {

/*****************************************************************************
** Class
*****************************************************************************/

class QNode : public QThread {
    Q_OBJECT
public:
	QNode(int argc, char** argv );
	virtual ~QNode();
	bool init();
    bool init(const std::string &master_url, const std::string &host_url);
    void run();
    void chatterCallback(const std_msgs::String::ConstPtr& msg);
    void imageCallback(const sensor_msgs::ImageConstPtr& msg);
    std::string _msg;
    QImage _image;

Q_SIGNALS:
	void loggingUpdated();
    void rosShutdown();
    void updatePic();

private:
	int init_argc;
	char** init_argv;
    ros::Subscriber chatter_subscriber;
    image_transport::Subscriber sub;
};

}  // namespace App

#endif /* App_QNODE_HPP_ */
