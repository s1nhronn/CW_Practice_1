#include <cerrno>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <limits>

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
  long val = std::strtol(argv[1], &endptr, 10);

  using limit_t = std::numeric_limits< int >;
  if (endptr == argv[1] || *endptr != '\0' || errno == ERANGE || val < limit_t::min() || val > limit_t::max())
  {
    std::cerr << "Incorrect argument" << '\n';
    return 2;
  }

  // int n = static_cast< int >(val);
}
