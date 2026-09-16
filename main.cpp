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
    Clicker();
    double millisec() const;

  private:
    std::chrono::high_resolution_clock::time_point start_;
  };

  using data_t = std::vector< unsigned long long >;
  using value_t = data_t::value_type;

  value_t sum(data_t::const_iterator start, data_t::const_iterator end);
}

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Argument is not specified" << '\n';
    return 1;
  }

  char* endPtr = nullptr;
  errno = 0;

  long val = std::strtol(argv[1], &endPtr, 10);
  if (endPtr == argv[1] || *endPtr != '\0' || errno == ERANGE || val < 1)
  {
    std::cerr << "Incorrect argument" << '\n';
    return 2;
  }

  size_t threads = static_cast< size_t >(val);

  constexpr size_t size{1'000'000'000};
  mtt::data_t values(size, 1);
  mtt::value_t sum{0};

  double init{0}, total{0};
  mtt::Clicker cl;

  std::vector< std::future< mtt::value_t > > results;
  results.reserve(threads);
  size_t base = size / threads;
  size_t remainder = size % threads;
  auto chunkBegin = values.cbegin();
  using diff_t = mtt::data_t::difference_type;

  init = cl.millisec();

  for (size_t i = 0; i < threads; ++i)
  {
    size_t chunkSize = base + (i < remainder ? 1 : 0);
    auto chunkEnd = chunkBegin + static_cast< diff_t >(chunkSize);

    results.emplace_back(std::async(std::launch::async, mtt::sum, chunkBegin, chunkEnd));
    chunkBegin = chunkEnd;
  }

  for (auto& ft : results)
  {
    sum += ft.get();
  }

  total = cl.millisec();

  std::cout << "Result: " << sum << '\n';
  std::cout << "Execution time: " << total - init << '\n';
}

mtt::Clicker::Clicker():
  start_(std::chrono::high_resolution_clock::now())
{}

double mtt::Clicker::millisec() const
{
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;
  auto t = high_resolution_clock::now();
  return static_cast< double >(duration_cast< milliseconds >(t - start_).count());
}

mtt::value_t mtt::sum(data_t::const_iterator start, data_t::const_iterator end)
{
  value_t res = 0;
  for (; start != end; ++start)
  {
    res += *start;
  }
  return res;
}
