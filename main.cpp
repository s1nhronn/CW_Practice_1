#include <chrono>

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

int main()
{}
