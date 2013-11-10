/*
 * CreateObjectsFromInputv2.c++
 *
 *  Created on: Nov 8, 2013
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

/* TODO: figure out required router and host parameters and fix required link
 * parameters.
 */
const string REQUIRED_HOST_PARAMETERS [ ] = { /* TODO */ };
const string REQUIRED_ROUTER_PARAMETERS [ ] = { /* TODO */ };
const string REQUIRED_LINK_PARAMETERS [ ] = {"buffer_size", "prop_delay",
		"queue_size", "queue_delay", "capacity", "queue_time", "end_points"};



Handler create_objects_from_input(string input_file_path) {

	/* Parse the input file stream, loading the contents into the
	 * "root" Json::Value. The false parameter tells the reader to ignore any
	 * comments. reader.parse() outputs false if it failed to read the stream.
	 * If this happens, give error and return NULL.
	 */
	ifstream is (inputFilePath, ifstream::in);

	/* If unable to open specified input file path, output error messages
	 * and return NULL.
	 */
	if (! is.is_open()) {
		// TODO : handle error messages.
		return NULL;
	}
	Json::Value root; // Contains the entire input dictionary, searchable by key.
	Json::Reader reader;
	if (! reader.parse(is, root, false)) {
		// TODO: write error messages and return null.
		return NULL;
	}

	/* TODO: For now, I am collecting all the links I make into a list called. */
	list<Link> links = new List<Link>;

	/* Set up the link default table. */
	unordered_map<string, Json::Value> link_defaults = new unordered_map;
	Json::Value link_default_input = root["link_defaults"];
	for (unsigned int i = 0; i < sizeof(REQUIRED_LINK_PARAMETERS); ++i) {
		string param_key = REQUIRED_LINK_PARAMETERS[i];
		Json::Value param_value = link_default_input[ REQUIRED_LINK_PARAMETERS[i] ];
		link_defaults[ param_key ] = param_value;
	}

	// Now grab the input for specific links.
	Json::Value link_input = root["links"];
	for ( Json::ValueIterator li_itr = link_input.begin();
			li_itr != link_input.end();  li_itr++) {
		/* Initialize a hash map containing the current link's parameters. At first,
		 * this is initialized to the default setting using the copy constructor.
		 */
		unordered_map current_link_settings = new unordered_map( &link_defaults );
		Json::Value current_link_input = li_itr;

		/* Iterate through the input for the current link and update current
		 * link settings.
		 */
		for ( Json::ValueIterator itr = current_link_input.begin();
				itr != current_link_input.end(); itr++) {
			string param_key = itr.key().asString();
			Json::Value param_value = itr;
			current_link_settings[ param_key ] = param_value;
		}

		/* Check that the user has defined all of the input parameters. */
		for ( auto itr = current_link_settings.begin();
				itr != current_link_settings.end(); itr++) {
			/* If the current link has any settings that are still set to NULL,
			 * output an error message and return NULL.
			 */
			if (! itr -> second) {
				/* TODO: Write awesome error messages. */
				return NULL;
			}
		}

		/* Finally, instantiate the link from the hash table using helper function. */
		links.push_front( &(create_link_from_table(&current_link_settings)));
	}

	/* TODO: Do the same thing for routers. */

	/* TODO: Do the same thing for hosts. */
}


Link create_link_from_table(unordered_map &settings) {
	/* TODO: Implement */
	return NULL;
}
