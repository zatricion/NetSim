#include <iostream>
#include "NetInput.h"

//int main(int argc, char *argv[]) {
//void read_input(Handler &h) {
Handler& read_input() {
  //std::cout << "Running test\n";

  return parse(std::cin);
}

