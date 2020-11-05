#include <array>
#include <iostream>
#include <vector>

#include "include/randomGenerator.hpp"

int main() {

  tool::RandomGenerator* rg;
  rg = &rg->getInstance();

  // *********** distributions ***********

  std::cout << " some random numbers:\n";
  std::cout << rg->normalDistribution(0., 1.) << std::endl;
  std::cout << rg->uniformDistribution(0, 1) << std::endl;
  std::cout << rg->zeroOne() << std::endl;
  std::cout << rg->trueFalse() << std::endl;

  constexpr int NUM_VALS = 10;
  constexpr int NUM_EXPERIMENTS = 10000;

  std::cout << "\na Uniform [0,9] distribution:\n";

  std::array<int, NUM_VALS> num_count;
  for (std::size_t i = 0; i < NUM_VALS; i++) {
    num_count[i] = 0;
  }

  tool::UniformDistribution<int> ud(0, NUM_VALS - 1);
  for (int i = 0; i < NUM_EXPERIMENTS; i++) {
    size_t random_number = static_cast<size_t>(ud.get());
    num_count[random_number]++;
  }

  for (int& v : num_count) {
    const int num_ticks = v / static_cast<int>(0.01 * NUM_EXPERIMENTS);
    std::cout << "|";
    for (int j = 0; j < num_ticks; j++) {
      std::cout << "#";
    }
    std::cout << v << std::endl;
    v = 0;
  }

  std::cout << "\na N(4,1.2) distribution:\n";
  tool::NormalDistribution nd(4., 1.2);
  for (int i = 0; i < NUM_EXPERIMENTS; i++) {
    int random_number = std::round(nd.get());
    if (random_number > -1 && random_number < 10) {
      num_count[random_number]++;
    }
  }

  for (int& v : num_count) {
    const int num_ticks = v / static_cast<int>(0.01 * NUM_EXPERIMENTS);
    std::cout << "|";
    for (int j = 0; j < num_ticks; j++) {
      std::cout << "#";
    }
    std::cout << v << std::endl;
  }


  // *********** rand iterators ***********
  std::array<int, 3> a = {{1, 2, 3}};
  std::vector<int> b;
  b.reserve(4);
  for (int i = 0; i < 4; i++) {
    b.emplace_back(ud.get());
  }

  std::cout << "and now a random element of an array:\n[";

  for (const auto& i : a) {
    std::cout << i << ", ";
  }
  std::cout << "]\nchoose [" << *(rg->get<std::array<int, 3>>(a)) << "]" << std::endl;

  std::cout << "and now a random element of an vector:\n[";

  for (const auto& i : b) {
    std::cout << i << ", ";
  }
  std::cout << "]\nchoose [" << *(rg->get<std::vector<int>>(b)) << "]" << std::endl;



  return 0;
}
