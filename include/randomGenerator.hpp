#include <cassert>
#include <random>
#include <type_traits>

namespace tool {

class RandomGenerator {

  template <typename...>
  using void_t = void;

  template <class T, class = void>
  struct is_iterator : std::false_type {};

  template <class T>
  struct is_iterator<T, void_t<typename std::iterator_traits<T>::iterator_category>>
      : std::true_type {};

 private:
  RandomGenerator() { generator = std::default_random_engine(rd()); }
  // Stop the compiler generating methods of copy the object
  RandomGenerator(RandomGenerator const& copy);             // Not Implemented
  RandomGenerator& operator=(RandomGenerator const& copy);  // Not Implemented

 public:
  static RandomGenerator& getInstance() {
    static RandomGenerator instance;
    return instance;
  }

  template <class T>
  T uniformDistribution(T min, T max) {
    static_assert(std::is_arithmetic<T>::value, "The type T must be numeric");
    assert("getRandomGenerator::uniform_distribution: given min > max" && min < max);
    if constexpr (std::is_floating_point<T>::value) {
      std::uniform_real_distribution<> distribution(min, max);
      return distribution(generator);
    } else if constexpr (std::is_integral<T>::value) {
      std::uniform_int_distribution<T> distribution(min, max);
      return distribution(generator);
    }
    assert(
        "RandomGenerator::uniform_distribution: How did you find a type which "
        "IS numeric but neither float nor integral???" &&
        false);
  }

  template <class T = double>
  T normalDistribution(T mean, T variance) {
    static_assert(std::is_floating_point<T>::value,
                  "The type T must be floating point");
    std::normal_distribution<T> distribution(mean, variance);
    return distribution(generator);
  }

  bool trueFalse() {
    if (zeroOne()) {
      return true;
    }
    return false;
  }

  int zeroOne() {
    std::uniform_int_distribution<int> distribution(0, 1);
    return distribution(generator);
  }

  template <typename InputIt>
  static typename std::enable_if<is_iterator<InputIt>::value, InputIt>::type get(InputIt first,
                                                                                 InputIt last) {
    const auto size = std::distance(first, last);
    if (0 == size)
      return last;
    using diff_t = typename std::iterator_traits<InputIt>::difference_type;
    return std::next(first, get<diff_t>(0, size - 1));
  }

  template <typename Container>
  static auto get(Container& container) ->
      typename std::enable_if<is_iterator<decltype(std::begin(container))>::value,
                              decltype(std::begin(container))>::type {
    return get(std::begin(container), std::end(container));
  }

 private:
  std::random_device rd;
  std::default_random_engine generator;
};

template <class T = double>
class UniformDistribution {
  using T_ = T;

 public:
  UniformDistribution() { rg = rg->getInstance(); }
  UniformDistribution(T min, T max) : min(min), max(max) {
    rg = &rg->getInstance();
  }

  T get() const { return rg->uniformDistribution<T>(min, max); }

  RandomGenerator* rg;
  T min = 0.;
  T max = 1.;
};

template <class T = double>
class NormalDistribution {
 public:
  NormalDistribution() { rg = rg->getInstance(); };
  NormalDistribution(T mean, T var) : mean(mean), var(var) {
    rg = &rg->getInstance();
  };

  T get() const { return rg->normalDistribution(mean, var); }
  RandomGenerator* rg;
  T mean = 0;
  T var = 1;
};
}  // namespace tool
