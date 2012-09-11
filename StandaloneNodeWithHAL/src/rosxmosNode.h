/*
 * rosXmosNode.h
 *
 *  Created on: 06.09.2012
 *      Author: choll
 */

#ifndef ROSXMOSNODE_H_
#define ROSXMOSNODE_H_



/**
 *	This struct stores the basic ROS node configuration
 */
typedef struct
{
	char 			*node_name; 			//!< The name of the node, must be <b>unique</b> in whole ROS!
	char 			*ros_master_host; 		//!< Hostname of the computer running roscore
	unsigned int 	ros_master_port; 		//!< RPC port of roscore, standard is: 11311
}ros_node_setup;




#endif /* ROSXMOSNODE_H_ */
