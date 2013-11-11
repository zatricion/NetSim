/*
 * CreateObjectsFromInputv2.c++
 *
 *  Created on: Nov 8, 2013
 *      Author: Johno
 */

#include <NetSimInputHandler.h>
using namespace std;

/* TODO: figure out required router and host parameters and fix required link
 * parameters.
 */
const string REQUIRED_HOST_PARAMETERS [ ] = { /* TODO */ };
const string REQUIRED_ROUTER_PARAMETERS [ ] = { /* TODO */ };
const string REQUIRED_LINK_PARAMETERS [ ] = {"buffer_size", "prop_delay",
		"queue_size", "queue_delay", "capacity", "queue_time", "end_points"};

// Constructor takes input file path as argument and uses that to set attribute
NetSimInputHandler::NetSimInputHandler(string file_path) {
	input_file_path = file_path;
}

Handler NetSimInputHandler::create_objects_from_input() {

	// Create the handler to return.
	Handler new_handler = new Handler;

	/* Convert the input file into a stream so we can feed it to the parser.
	 * If unable to open the specified input path, output error messages and
	 * return NULL.
	 */
	ifstream is (input_file_path, ifstream::in);
	if (! is.is_open()) {
		// TODO : handle error messages.
		return nullptr;
	}

	/* Load the contents of the stream into the
	 * "root" Json::Value. The false parameter tells the reader to ignore any
	 * comments. reader.parse() outputs false if it failed to read the stream.
	 * If this happens, give error and return NULL.
	 */
	Json::Value root; // Contains the entire input dictionary, searchable by key.
	Json::Reader reader;
	if (! reader.parse(is, root, false)) {
		// TODO: write error messages and return null.
		return nullptr;
	}

	/* Set up the link default table. */
	string REQUIRED_LINK_PARAMETERS [ ] = Link.REQUIRED_PARAMETERS;
	std::unordered_map<string, Json::Value> link_defaults =
			new std::unordered_map<string, Json::Value>;
	Json::Value link_default_input = root["link_defaults"];
	for (unsigned int i = 0; i < sizeof(REQUIRED_LINK_PARAMETERS); ++i) {
		string param_key = REQUIRED_LINK_PARAMETERS[i];
		Json::Value param_value = link_default_input[ param_key ];
		link_defaults[ param_key ] = param_value;
	}

	// Now grab the input for specific links.
	Json::Value link_input = root["links"];
	for ( Json::Value::iterator link_itr = link_input.begin();
			link_itr != link_input.end();  link_itr++) {

		/* Initialize a hash map containing the current link's parameters. At first,
		 * this is initialized to the default setting using the copy constructor.
		 */
		unordered_map<string, Json::Value> current_link_settings =
				new unordered_map<string, Json::Value>( &link_defaults );
		Json::Value current_link_input = *link_itr;

		/* Iterate through the input for the current link and update current
		 * link settings.
		 */
		for ( Json::Value::iterator setting_itr = current_link_input.begin();
				setting_itr != current_link_input.end(); setting_itr++) {
			string param_key = setting_itr.key().asString();
			Json::Value param_value = *setting_itr;

			// Update the current settings using param_key and param_value
			current_link_settings[ param_key ] = param_value;

		}

		/* Check that the user has defined all of the input parameters. */
		for ( auto setting_itr = current_link_settings.begin();
				setting_itr != current_link_settings.end(); setting_itr++) {
			/* If the current link has any settings that are still set to NULL,
			 * output an error message and return NULL.
			 */
			if (! setting_itr -> second) {
				/* TODO: Write awesome error messages. */
				return NULL;
			}
		}

		// Instantiate the link and add it to the Handler's generator map.
		string link_name = current_link_settings["id"];
		unique_ptr<Link> new_link = unique_ptr<Link>(create_link_from_table( &current_link_settings));
		new_handler.add_generator(link_name, new_link);
	}

	/* TODO: Do the same thing for routers. */

	/* TODO: Do the same thing for hosts. */
}


Link create_link_from_table(unordered_map<string, Json::Value> *settings) {
	float buffer_size = (*settings)["buffer_size"].asDouble();
	float prop_delay = (*settings)["prop_delay"].asDouble();
	float queue_size = (*settings)["queue_size"].asDouble();
	float queue_delay = (*settings)["queue_delay"].asDouble();
	float capacity = (*settings)["capacity"].asDouble();
	string node1 = (*settings)["node1"].asString();
	string node2 = (*settings)["node2"].asString();
	string link_id = (*settings)["id"].asString();

	Link new_link =
			Link(buffer_size, prop_delay, capacity, node1, node2, link_id);

	return new_link;
}

/* TODO: Implement */
Router create_router_from_table(unordered_map<string, Json::Value> *settings) {

}


Host create_host_from_table(unordered_map<string, Json::Value> *settings) {
	CongestionAlg congestion_algorithm = nullptr; // TODO: Fix this.


}
