# io_uring-echo-server     

## Build & Run 

```bash
# build and install liburing first
git clone https://github.com/axboe/liburing.git
cd liburing
./configure && make -j && make install     

# build and run
mkdir -p build && cd build
cmake .. -GNinja
ninja
./io_uring-echo-server 8888
```

## Benchmark    
Refer to [io_uring echo server benchmarks](https://github.com/frevib/io_uring-echo-server/blob/io-uring-feat-fast-poll/benchmarks/benchmarks.md) which leverages [haraldh/rust_echo_bench](https://github.com/haraldh/rust_echo_bench).      

## References
- [Why you should use io_uring for network I/O](https://developers.redhat.com/articles/2023/04/12/why-you-should-use-iouring-network-io)
- [Getting Hands on with io_uring using Go](https://developers.mattermost.com/blog/hands-on-iouring-go/)
- [axboe/liburing](https://github.com/axboe/liburing)
- [frevib/io_uring-echo-server](https://github.com/frevib/io_uring-echo-server)
- [haraldh/rust_echo_bench](https://github.com/haraldh/rust_echo_bench)
- [Lord of the io_uring - Completion](https://unixism.net/loti/ref-liburing/completion.html)

