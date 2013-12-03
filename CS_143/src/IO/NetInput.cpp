#include <iostream>
#include "NetInput.h"

Handler& read_input() {
  //std::cout << "Running test\n";

  return parse(std::cin);
}

