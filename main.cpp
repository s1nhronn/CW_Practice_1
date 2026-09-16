#include <cerrno>
#include <chrono>
#include <cstdlib>
#include <future>
#include <iostream>
#include <vector>

namespace mtt
{
  class Clicker
  {
  public:
    Clicker():
      start_(std::chrono::high_resolution_clock::now())
    {}
    double millisec() const
    {
      using std::chrono::duration_cast;
      using std::chrono::high_resolution_clock;
      using std::chrono::milliseconds;
      auto t = high_resolution_clock::now();
      return static_cast< double >(duration_cast< milliseconds >(t - start_).count());
    }

  private:
    std::chrono::high_resolution_clock::time_point start_;
  };

  using data_t = std::vector< unsigned long long >;
  using value_t = data_t::value_type;

  size_t sum(const data_t& values, size_t start, size_t finish)
  {
    size_t sum = 0;
    for (size_t i = start; i < finish; ++i)
    {
      sum += values[i];
    }
    return sum;
  }
}

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Argument is not specified" << '\n';
    return 1;
  }

  char* endptr = nullptr;
  errno = 0;

  size_t threads = std::strtoul(argv[1], &endptr, 10);
  if (endptr == argv[1] || *endptr != '\0' || errno == ERANGE || !threads)
  {
    std::cerr << "Incorrect argument" << '\n';
    return 2;
  }

  constexpr size_t size{1'000'000'000};
  mtt::data_t values(size, 1);
  mtt::value_t sum{0};

  double init{0}, total{0};
  mtt::Clicker cl;

  init = cl.millisec();

  std::vector< std::future< size_t > > results;
  results.reserve(threads);
  size_t base = size / threads;
  size_t remainder = size % threads;
  for (size_t i = 0; i < threads; ++i)
  {
    size_t start = i * base + std::min(i, remainder);
    size_t end = start + base + (i < remainder ? 1 : 0);

    results.emplace_back(std::async(std::launch::async, mtt::sum, std::cref(values), start, end));
  }

  for (size_t i = 0; i < threads; ++i)
  {
    sum += results[i].get();
  }

  total = cl.millisec();

  std::cout << "Result: " << sum << '\n';
  std::cout << "Execution time: " << total - init << '\n';
}
