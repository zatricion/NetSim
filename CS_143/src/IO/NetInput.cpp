#include "NetInput.h"

/**
 * Run the parser on a network description and return 
 * a reference to the resulting handler.
 *
 * @param file an ifstream to read input from.
 *
 * @return reference to a handler containing the necessary network objects.
 */
Handler& read_input(std::ifstream file) {
  return parse(file);
}

