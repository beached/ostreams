# ostreams
A constexpr output stream.  See [tests](https://github.com/beached/ostreams/tree/master/tests) for example of usage

## Initial Benchmarks
Using a format string(or equivilent) of `"The asnwer to the meaning of life is %d %f\n"`

The following are the results:

###char buffer
* std::string_stream took 96.72ms to process 100000 items at 967.20ns per item
* snprintf took 29.56ms to process 100000 items at 295.58ns per item
* daw::memory_stream took 3.86ms to process 100000 items at 38.64ns per item

###console
* srd::cerr took 809.98ms to process 100000 items at 8.10us per item
* printf took 161.36ms to process 100000 items at 1.61us per item
* daw::console_stream took 797.48ms to process 100000 items at 7.97us per item
