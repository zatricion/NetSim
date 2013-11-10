/*
 * CreateObjectsFromInput.c++
 *
 *  Created on: Nov 6, 2013
 *      Author: Johno
 */

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

using namespace std;

struct network_components {
	list<Link> *links;
	list<Host> *hosts;
	list<Router> *routers;
};

/* TODO: figure out required router and host parameters and fix required link
 * parameters.
 */
const string REQUIRED_HOST_PARAMETERS [ ] = { /* TODO */ };
const string REQUIRED_ROUTER_PARAMETERS [ ] = { /* TODO */ };
const string REQUIRED_LINK_PARAMETERS [ ] = {"buffer_size", "prop_delay",
		"queue_size", "queue_delay", "capacity", "queue_time", "end_points"};


network_components create_network_from_input(string inputFilePath) {
	// Create file stream from input file to pass to JsonReader
	ifstream is (inputFilePath, ifstream::in);

	/* If unable to open specified input file path, output error messages
	 * and return NULL.
	 */
	if (! is.is_open()) {
		// TODO : handle error messages.
		return NULL;
	}

	// Let's parse the input.
	Json::Value root; // Contains the entire input dictionary, searchable by key.
	Json::Reader reader;

	/* Parse the input file stream, loading the contents into the
	 * "root" Json::Value. The false parameter tells the reader to ignore any
	 * comments. reader.parse() outputs false if it failed to read the stream.
	 * If this happens, give error and return NULL.
	 */
	if (! reader.parse(is, root, false)) {
		// TODO: write error messages and return null.
		return NULL;
	}

	/* Initialize links. Start by collecting defaults. */
	list<Link> links;
	const Json::Value input_links = root["links"];
	const Json::Value link_defaults = root["link_defaults"];

	// Parse the input links.
	for (unsigned int i = 0; i < input_links.size(); ++i) {
		float buffer_size, prop_delay, queue_size, queue_delay, capacity;
		Device *node1, *node2;

		Link current_link = new Link;
		Json::Value current_settings = input_links[i];

		/* Iterate through each of the settings enumerated in REQUIRED_LINK_SETTINGS.
		 * For each required setting, check whether this setting has been defined
		 * on the per-link level. If not, check whether the setting has been
		 * given a default (where applicable). If not, return an error.
		 */
		for (unsigned int s = 0; s < sizeof(REQUIRED_LINK_SETTINGS); ++s) {
			// Name of the current setting
			string setting_name = REQUIRED_LINK_SETTINGS[s];
			// Offset of the setting within the link class.
			int offset = -1;

			/* If the user has failed to specify a required setting for both the
			 * current link and as a default, output an insufficient input error
			 * message and return NULL.
			 */
			if ( !current_settings[setting_name] && !link_defaults[setting_name]) {
				// TODO: output error messages.
				return NULL;
			}

			/* Update our blank link by searching through the JSON input to find
			 * the value for each setting.
			 */
			hash<string> setting_hash, test_hash;
			switch ( setting_hash (setting_name) )
			{
				/* Float/Double settings: */
				case test_hash ("buffer_size"):
						offset = offsetof(Link, buffer_size);
						goto numeric_setting;
				case test_hash("prop_delay"):
						offset = offsetof(Link, prop_delay);
						goto numeric_setting;
				case test_hash("queue_size"):
						offset = offsetof(Link, queue_size);
						goto numeric_setting;
				case test_hash("queue_delay"):
						offset = offsetof(Link, queue_delay);
						goto numeric_setting;
				case test_hash("capacity"):
						offset = offsetof(Link, capacity);
						goto numeric_setting;
				case test_hash("queue_time"):
						offset = offsetof(Link, queue_time);
						goto numeric_setting;

				/* All numeric settings have to be cast from Json::Value to a
				 * numeric type. */
				case numeric_setting:
					float value = current_settings[setting_name].asDouble();
					break;

				/* The "endpoints" setting is handled differently. */
				case test_hash ("endpoint")

				case test_hash ("buffer_size"):
					if ( current_settings[setting_name] )
						current_link.buffer_size =
								current_settings[setting_name].asDouble();
					else
						current_link.buffer_size =
								link_defaults[setting_name].asDouble();
					break;


				case test_hash (prop_delay):
				case test_hash (queue_size):
				case test_hash ()
			}
		}
	}


}
