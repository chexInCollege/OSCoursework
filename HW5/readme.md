## Compiling and Running
Just run `make` then `./hw5 <numThreads> <stringLength>`.
## Note
I'm not sure if the threads were meant to cycle or grab the mutex the first change they got. By default, threads will run in cycles in the order they were created. This is a lot slower with many threads, so if it's problematic, the flag `WAIT_FOR_NEIGHBORS` at the top can be changed from `1` to `0` for first-come-first-served behavior.