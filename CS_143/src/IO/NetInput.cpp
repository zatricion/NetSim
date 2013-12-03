#include <iostream>
#include "NetInput.h"

//int main(int argc, char *argv[]) {
Handler& read_input(Handler &h) {
  //std::cout << "Running test\n";

  parse(std::cin, h);

  return h;
}

