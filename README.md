# ostreams
A constexpr output stream implementation.  See [tests](https://github.com/beached/ostreams/tree/master/tests) for example of usage

To create a formatted buffer

```cpp
daw::static_string_t<char, 100> buffer{};   // Uses a static string, but any contiguous memory area is fine

buffer.resize(buffer.size( ));  // makes static string marked as fully used


auto buff_stream = daw::io::make_memory_buffer_stream( buffer.data( ), buffer.size( ) );    // create stream

buff_stream << "The meaning of life is " << 42 << '\n';

buffer.shrink_to_fit( );    // reclaim unused space in buffer
```
Console output
```cpp
daw::con_out << "The meaning of life is " << 42 << '\n';
```

File output
```cpp
auto fs = daw::make_file_stream( "file_name" );
if( !fs ) {
    daw::con_err << "Error opening file\n";
    exit( EXIT_FAILURE );
}
fs << "The meaining of life is " << 42 << '\n';
fs.close( );    // or let it go out of scope
```
## Initial Benchmarks
Using a format string(or equivilent) of `"The asnwer to the meaning of life is %d %f\n"`

The following are the results:

#### Mac 2017 Macbook Air - 1.8 GHz Intel Core i5 
#### char buffer
* std::string_stream took 96.72ms to process 100000 items at 967.20ns per item
* snprintf took 29.56ms to process 100000 items at 295.58ns per item
* daw::memory_stream took 3.86ms to process 100000 items at 38.64ns per item

### console
* srd::cerr took 809.98ms to process 100000 items at 8.10us per item
* printf took 161.36ms to process 100000 items at 1.61us per item
* daw::console_stream took 797.48ms to process 100000 items at 7.97us per item


#### Windows Intel i7-7500U	
### char buffer
* std::string_stream took 163.26ms to process 100000 items at 1.63us per item
* snprintf took 95.73ms to process 100000 items at 957.26ns per item
* daw::memory_stream took 9.63ms to process 100000 items at 96.29ns per item

### console
* srd::cerr took 574.13ms to process 100000 items at 5.74us per item
* printf took 2.92s to process 100000 items at 29.16us per item
* daw::console_stream took 430.69ms to process 100000 items at 4.31us per item
