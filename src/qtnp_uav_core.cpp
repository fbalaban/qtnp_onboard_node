/*
 * qtnp_uav_core.cpp
 *
 * A qtnp UAV ROS node for listening global CDT constrains
 *
 *  Created on: Nov 25, 2017
 *      Author: Fotis Balampanis
 *		License: GNU GPL v3
 */

#include "qtnp_uav_core.h"

// TODO include further data processing in util file/class
// here only seperate the uav specific coordinates (probably)

//#include "polygon_triangulation.h"

QtnpUAV::QtnpUAV()
{    
}

QtnpUAV::~QtnpUAV()
{    
}

void QtnpUAV::setCDTS(std::vector<qtnp_uav::CDTConstraints> cdts){
  this->cdts = cdts; 
}

float QtnpUAV::getUAVchecksum(){
  return getCDTbyUAVid(this->uav_id).checksum;
}

qtnp_uav::CDTConstraints& QtnpUAV::getCDTbyUAVid(int uav_id){
  for (auto &cdt: this->cdts){
    if (cdt.uav_id == uav_id)
    {
      return cdt;
    }
  }
}

void QtnpUAV::setConstraints(int uav_id, std::vector<qtnp_uav::Coordinates> constraints){
  qtnp_uav::CDTConstraints &that_uav_constraints = getCDTbyUAVid(uav_id);
  that_uav_constraints.constraints = constraints;
}

void QtnpUAV::globalplanCallback(const qtnp_uav::GlobalConstraints::ConstPtr& msg)
{
    
  // std::cout << std::setprecision(7);
  // int uav_id = (int) msg->uav_id;
  // ROS_INFO("I heard the latitude: [%3.7f]", latitude);

  ROS_INFO("[Global] I heard the voice:");
  
  if (is_initial_plan)
  {
      // Creating the initial global plan without further questions
      this->is_initial_plan = false;
      // Now it has a plan
      this->is_published=true;

      setCDTS(msg->CDTS);

      for (auto const& that_cdt: this->cdts)
      {
        if ( that_cdt.uav_id == this->uav_id )
        {
          this->has_own_plan = true;
        }
      }

      if (has_own_plan)
      {
        ROS_INFO("[Global: UAV %i] I have a plan, initiating planning", this->uav_id);
        /* perform triangulation, path generation, upload new plan on autopilot  */
      }
  } else 
  {
    for (auto const& that_cdt: msg->CDTS)
    {
      if ( that_cdt.uav_id == this->uav_id )
      {
        if (that_cdt.checksum != getUAVchecksum())
        {
          setConstraints(this->uav_id, that_cdt.constraints);
          // TODO setChecksum(this->uav_id, that_cdt.constraints);
        }
      }
    }   
  }
	// int a = 0;
    // a = perform_tnp(longitude, latitude, uav_id);
}

void QtnpUAV::configCallback(qtnp_uav::qtnp_uav_paramsConfig &config, uint32_t level)
{
  // Set class variables to new values. They should match what is input at the dynamic reconfigure GUI.
  this->uav_id = config.uav_id;
} // end configCallback()