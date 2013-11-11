/*
 * NetSimInputHandler.h
 *
 *  Created on: Nov 10, 2013
 *      Author: Johno
 */

#ifndef NETSIMINPUTHANDLER_H_
#define NETSIMINPUTHANDLER_H_

// JSON parser headers:
#include <autolink.h>
#include <config.h>
#include <features.h>
#include <forwards.h>
#include <json.h>
#include <reader.h>
#include <value.h>
#include <writer.h>

// Standard C++ Library components:
#include <unordered_map>
#include <list>
#include <stdio.h>

// File reading components
#include <iostream>
#include <fstream>

// NetSim components
#include <Link.h>
#include <Host.h>
#include <Router.h>
#include <Handler.h>

using namespace std;

class NetSimInputHandler {
	public:
		NetSimInputHandler(string file_path);
		Handler create_objects_from_input();

	private:
		string input_file_path;

		// Helper functions to create network components:
		Link create_link_from_table(unordered_map<string, Json::Value> &settings_table);
		Router create_router_from_table(unordered_map<string, Json::Value> &settings_table);
		Host create_host_from_table(unordered_map<string, Json::Value> &settings_table);



} NetSimInputHandler;





#endif /* NETSIMINPUTHANDLER_H_ */
