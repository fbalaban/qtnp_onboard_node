/*
 * qtnp_uav_planner.cpp
 *
 * A qtnp UAV ROS node for listening global CDT constrains
 *
 *  Created on: Nov 25, 2017
 *      Author: Fotis Balampanis
 *		License: GNU GPL v3
 *
 * Derived from: http://wiki.ros.org/ROSNodeTutorialC%2B%2B
 */

#include "qtnp_uav_core.h" 

int main(int argc, char **argv)
{

    ros::init(argc, argv, "qtnp_uav_planner");
    ros::NodeHandle n;

    int uav_id;

	int rate;
	string topic;

	// Initialize node parameters from launch file or command line.
	// Use a private node handle so that multiple instances of the node can be run simultaneously
	// while using different parameters.
	ros::NodeHandle private_node_handle_("~");
	private_node_handle_.param("uav_id", uav_id, int(1));
	private_node_handle_.param("rate", rate, int(40));
	private_node_handle_.param("topic", topic, string("global_constraints"));

    QtnpUAV *qtnp_uav = new QtnpUAV();

    // Set up a dynamic reconfigure server.
    dynamic_reconfigure::Server<qtnp_uav::qtnp_uav_paramsConfig> dr_srv;
    dynamic_reconfigure::Server<qtnp_uav::qtnp_uav_paramsConfig>::CallbackType cb;
    cb = boost::bind(&QtnpUAV::configCallback, qtnp_uav, _1, _2);
    dr_srv.setCallback(cb);

    // Always listening for a global plan
    ros::Subscriber globalCDT_sub = n.subscribe(topic.c_str(), 1000, &QtnpUAV::globalplanCallback, qtnp_uav);

    // collection of poses to produce a path for path planning
    // ros::Publisher path_pub = n.advertise<nav_msgs::Path>("path_planning", qtnp_uav->);

  	ros::Rate r(rate);

  	// Main loop.
  	while (n.ok())
  	{
    	ROS_INFO("[Global] UAV ID: %i", qtnp_uav->uav_id);
    	ros::spinOnce();
    	r.sleep();
  	}

	return 0;
}