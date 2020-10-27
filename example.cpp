#include <array>
#include <iostream>
#include <vector>

#include "include/randomGenerator.hpp"

int main() {

  tool::RandomGenerator* rg;
  rg = &rg->getInstance();

  std::cout << rg->normalDistribution(0., 1.) << std::endl;
  std::cout << rg->uniformDistribution(0, 1) << std::endl;
  std::cout << rg->zeroOne() << std::endl;
  std::cout << rg->trueFalse() << std::endl;

  std::array<int, 3> a;
  std::vector<int> b;
  b.reserve(4);



  constexpr int NUM_VALS = 10;
  constexpr int NUM_EXPERIMENTS = 10000;

  std::array<int, NUM_VALS> num_count;
  for (std::size_t i = 0; i < NUM_VALS; i++) {
    num_count[i] = 0;
  }

  tool::UniformDistribution<int> ud(0, NUM_VALS - 1);
  for (int i = 0; i < NUM_EXPERIMENTS; i++) {
    size_t random_number = static_cast<size_t>(ud.get());
    num_count[random_number]++;
  }

  for (const int& v : num_count) {
    const int num_ticks = v / static_cast<int>(0.01 * NUM_EXPERIMENTS);
    std::cout << "|";
    for (int j = 0; j < num_ticks; j++) {
      std::cout << "#";
    }
    std::cout << v << std::endl;
  }


  return 0;
}
