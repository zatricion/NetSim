#include "NetInput.h"

Handler& read_input(std::ifstream file) {
  return parse(file);
}

