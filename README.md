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
## Extending to your classes
Add a function in your classes namespace that returns a type that is string like(has a data( ) and size( ) method).  If you want constexpr formatting this function must be constexpr.  The provided static_string_t can help or a string_view may work too.

Custom streams must provide operator() for characters and string like data.  They must also specialize daw::io::supports_output_stream_interface<T> to inherit from std::true_type


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

# Round trip errors
| type   |  value          |  output                                                                                                                                                                                                                                                                                                                   |  difference    | 
|--------|-----------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------------| 
| double |  0.1            |  0.1                                                                                                                                                                                                                                                                                                                      |  0             | 
| double |  0.12           |  0.11999999999999998                                                                                                                                                                                                                                                                                                      |  -1.38778e-17  | 
| double |  0.123          |  0.12299999999999999                                                                                                                                                                                                                                                                                                      |  -1.38778e-17  | 
| double |  0.1234         |  0.12339999999999998                                                                                                                                                                                                                                                                                                      |  -1.38778e-17  | 
| double |  1.2345         |  1.23449999999999992                                                                                                                                                                                                                                                                                                      |  0             | 
| double |  0.333333       |  0.33333333333333332                                                                                                                                                                                                                                                                                                      |  0             | 
| double |  0.666667       |  0.66666666666666665                                                                                                                                                                                                                                                                                                      |  0             | 
| double |  3.33333        |  3.33333333333333349                                                                                                                                                                                                                                                                                                      |  0             | 
| double |  6.66667        |  6.66666666666666698                                                                                                                                                                                                                                                                                                      |  0             | 
| double |  2.22507e-308   |  0                                                                                                                                                                                                                                                                                                                        |  -2.22507e-308 | 
| double |  -2.22507e-308  |  -0                                                                                                                                                                                                                                                                                                                       |  2.22507e-308  | 
| double |  1.79769e+308   |  17976931348623156600000000000000000000000000000000000000000000000000000000 0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000 0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000 |  0             | 
| double |  4.94066e-324   |  0                                                                                                                                                                                                                                                                                                                        |  -4.94066e-324 | 
| double |  4              |  3.99999999999999953                                                                                                                                                                                                                                                                                                      |  0             | 
| double |  8.98847e+307   |  89884656743115786500000000000000000000000000000000000000000000000000000000 0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000 0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000 0000000000000000000000000000000000000000  |  0             | 
| float  |  0.1            |  0.1                                                                                                                                                                                                                                                                                                                      |  0             | 
| float  |  0.12           |  0.119999996                                                                                                                                                                                                                                                                                                              |  0             | 
| float  |  0.123          |  0.123000001                                                                                                                                                                                                                                                                                                              |  0             | 
| float  |  0.1234         |  0.123400000                                                                                                                                                                                                                                                                                                              |  0             | 
| float  |  1.2345         |  1.234500046                                                                                                                                                                                                                                                                                                              |  0             | 
| float  |  0.333333       |  0.333333330                                                                                                                                                                                                                                                                                                              |  0             | 
| float  |  0.666667       |  0.666666660                                                                                                                                                                                                                                                                                                              |  0             | 
| float  |  3.33333        |  3.333333240                                                                                                                                                                                                                                                                                                              |  0             | 
| float  |  6.66667        |  6.666666481                                                                                                                                                                                                                                                                                                              |  0             | 
| float  |  1.17549e-38    |  0                                                                                                                                                                                                                                                                                                                        |  -1.17549e-38  | 
| float  |  3.40282e+38    |  340282346300000000000000000000000000000                                                                                                                                                                                                                                                                                  |  0             | 
| float  |  1.4013e-45     |  0                                                                                                                                                                                                                                                                                                                        |  -1.4013e-45   | 
| float  |  4              |  3.999999788                                                                                                                                                                                                                                                                                                              |  0             | 
| float  |  1.70141e+38    |  170141175700000000000000000000000000000                                                                                                                                                                                                                                                                                  |  0             | 

