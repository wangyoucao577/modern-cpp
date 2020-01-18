
// C++ Most vexing parse sample
// referred from https://en.wikipedia.org/wiki/Most_vexing_parse

class Timer {
public:
  Timer();
};

class TimeKeeper {
public:
  TimeKeeper(const Timer &t);

  int get_time();
};

int main() {

  // In contrast to interpret `time_keeper` as an object,
  // C++ standard requires `time_keeper` interpreted as a function.
  // https://en.wikipedia.org/wiki/Most_vexing_parse
  TimeKeeper time_keeper(Timer());
  return time_keeper.get_time();
}
