
#include <coroutine>
#include <iostream>
#include <thread>
#include <utility>

struct FiboGenerator { // the coroutine return type
  struct PromiseType {
    FiboGenerator get_return_object() {
      return FiboGenerator{
          // binding ReturnType, and coroutine_handle objects together
          std::coroutine_handle<PromiseType>::from_promise(*this)};
    }

    std::suspend_always initial_suspend() const noexcept { return {}; }
    std::suspend_always final_suspend() const noexcept { return {}; }
    void return_void() const noexcept {}
    void unhandled_exception() const noexcept {}

    std::suspend_always yield_value(int v) { // for co_yield
      next_value_ = v;
      return {};
    }

    int next() const noexcept { return next_value_; }

  private:
    int next_value_{0}; // data to be passed out from coroutine
  };
  using promise_type = PromiseType; // for std::coroutine_traits

  FiboGenerator(std::coroutine_handle<promise_type> h) : handle_(h) {}
  ~FiboGenerator() {
    handle_.destroy(); // avoid memory leak of coroutine state object
  }

  int next() const {
    handle_.resume(); // execute once then get the value
    return handle_.promise().next();
  }

private:
  std::coroutine_handle<promise_type> handle_;
};

FiboGenerator makeFiboGenerator() { // the coroutine function
  int i1 = 1, i2 = 1;

  while (true) {
    co_yield i1;
    i1 = std::exchange(i2, i1 + i2);
  }
};

int main() {
  auto fibo = makeFiboGenerator();
  for (auto i = 0; i < 10; i++) {
    std::printf("fibo[%d]=%d\n", i, fibo.next());
  }
  return 0;
}
