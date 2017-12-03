/*
 * global_listener.h
 *
 * A qtnp UAV ROS node for listening global CDT constrains
 * The header.
 *
 *  Created on: Nov 25, 2017
 *      Author: Fotis Balampanis
 *		License: GNU GPL v3
 */

#ifndef QTNP_UAV_CORE_H
#define QTNP_UAV_CORE_H

// ROS includes.
#include "ros/ros.h"
#include "ros/time.h"
//#include "std_msgs/String.h"

// Other includes
//#include <sstream>
//#include <iostream>
//#include <fstream>
//#include <iomanip>
//#include "boost/ref.hpp"

// Custom message includes. Auto-generated from msg/ directory.
#include "qtnp_uav/GlobalConstraints.h"
#include "qtnp_uav/CDTConstraints.h"
#include "qtnp_uav/Coordinates.h"

// Dynamic reconfigure includes.
#include <dynamic_reconfigure/server.h>
// Auto-generated from cfg/ directory.
#include <qtnp_uav/qtnp_uav_paramsConfig.h>

using std::string;

class QtnpUAV
{
public:
  //! Constructor.
  QtnpUAV();

  //! Destructor.
  ~QtnpUAV();

  //! Callback function for dynamic reconfigure server.
  void configCallback(qtnp_uav::qtnp_uav_paramsConfig &config, uint32_t level);

  //! Publish the message.
  //void publishMessage(ros::Publisher *pub_message);

  //! Callback function for subscriber.
  void globalplanCallback(const qtnp_uav::GlobalConstraints::ConstPtr &msg);

  //! The actual message.
  int uav_id;
  qtnp_uav::GlobalConstraints global_constraints;
  bool is_initial_plan{true};
  bool is_published{false};
  bool has_own_plan{false};
  bool is_master{false};

  // Getters, Setters  
  float getUAVchecksum();
  qtnp_uav::CDTConstraints& getCDTbyUAVid(int uav_id);

  void setCDTS(std::vector<qtnp_uav::CDTConstraints> cdts);
  void setConstraints(int uav_id, std::vector<qtnp_uav::Coordinates> constraints);

private:

  std::vector<qtnp_uav::CDTConstraints> cdts;

};

#endif // QTNP_UAV_CORE_H