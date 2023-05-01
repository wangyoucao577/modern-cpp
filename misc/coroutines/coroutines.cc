

#include <coroutine>
#include <iostream>
#include <thread>

#define PRINT_FUNC_LINE                                                        \
  (std::cout << "tid:" << std::this_thread::get_id() << "|" << __func__ << ":" \
             << __LINE__ << std::endl);

struct PromiseType;

struct ReturnObjectType { // coroutine return type
  using promise_type = PromiseType;

  ReturnObjectType(std::coroutine_handle<promise_type> h) : handle_(h) {
    PRINT_FUNC_LINE
  }
  ~ReturnObjectType() {
    PRINT_FUNC_LINE
    handle_.destroy();
  }

  std::coroutine_handle<promise_type> handle() { return handle_; }

private:
  std::coroutine_handle<promise_type> handle_;
};

struct PromiseType {
  ReturnObjectType get_return_object() {
    PRINT_FUNC_LINE
    return {std::coroutine_handle<ReturnObjectType::promise_type>::from_promise(
        *this)};
  }

  std::suspend_always initial_suspend() noexcept { PRINT_FUNC_LINE return {}; }
  std::suspend_always final_suspend() noexcept { PRINT_FUNC_LINE return {}; }
  void return_void() { PRINT_FUNC_LINE }
  void unhandled_exception() { PRINT_FUNC_LINE }
};

struct Awaitable {
  bool await_ready() const noexcept { PRINT_FUNC_LINE return false; }
  void await_suspend(std::coroutine_handle<PromiseType> h) const noexcept {
    PRINT_FUNC_LINE
  }
  void await_resume() const noexcept { PRINT_FUNC_LINE }
};

ReturnObjectType coroutine_func() {
  PRINT_FUNC_LINE

  co_await Awaitable{};

  PRINT_FUNC_LINE

  co_return;
}

int main() {

  PRINT_FUNC_LINE
  auto r = coroutine_func();
  PRINT_FUNC_LINE
  r.handle().resume();
  PRINT_FUNC_LINE
  r.handle().resume();
  PRINT_FUNC_LINE

  return 0;
}