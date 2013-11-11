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

	unordered_map<string, EventGenerator *> component_table =
			new unordered_map<string, EventGenerator *>;

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

	/* TODO: For now, I am collecting all the links I make into a list called links. */
	list<Link> links = new list<Link>;

	/* Set up the link default table. */
	string REQUIRED_LINK_PARAMETERS [ ] = Link.REQUIRED_PARAMETERS;
	std::unordered_map<string, Json::Value> link_defaults = new std::unordered_map;
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
		unordered_map current_link_settings = new unordered_map( &link_defaults );
		Json::Value current_link_input = *link_itr;

		/* Iterate through the input for the current link and update current
		 * link settings.
		 */
		for ( Json::Value::iterator setting_itr = current_link_input.begin();
				setting_itr != current_link_input.end(); setting_itr++) {
			string param_key = setting_itr.key().asString();
			Json::Value param_value = *setting_itr;

			if (param_key.compare("end_points") == 0) {
				/* Make sure that the user input two endpoints. If not, output
				 * errors and return NULL.
				 */
				if (param_value.size() != 2) {
					// TODO: write awesome error messages.
					return NULL;
				}
				Device *node1 = device_table [ *(param_value.begin()) ];
				Device *node2 = device_table [ *(param_value.end()) ];
			}

			/* If the parameter value has a numeric type, convert the value to
			 * double. TODO: actually iterate over the numeric settings array
			 * instead of assuming.
			 */
			current_link_settings[ param_key ] = param_value.asDouble();
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

		/* Finally, instantiate the link from the hash table using helper function. */
		links.push_front( &( create_link_from_table( &current_link_settings ) ) );
	}

	/* TODO: Do the same thing for routers. */

	/* TODO: Do the same thing for hosts. */
}


Link create_link_from_table(unordered_map &settings) {
	/* TODO: Implement */
	return NULL;
}
