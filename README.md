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

Otherwise, for composite classes you would overload operator<< like in C++ iostreams.  The requirement being that it is a template and uses SFINAE to only allow when ```cpp daw::io::is_output_stream_v<OutputStream> == true ```.
## Benchmarks
Using a format string(or equivilent) of `"The asnwer to the meaning of life is %d %f\n"` with a double of 42.0

The following are the results:
#### Mac 2017 Macbook Air - 1.8 GHz Intel Core i5 

| method             | count  | total time  | item time |
|--------------------|--------|-------------|-----------|
| daw::memory_stream | 100000 | 13.90ms     | 138.99ns  |
| snprintf           | 100000 | 31.98ms     | 319.83ns  |
| std::string_stream | 100000 | 77.30ms     | 772.96ns  |
| printf             | 100000 | 170.57ms    |   1.71us  |
| daw::con_err       | 100000 | 573.81ms    |   5.74us  |
| std::cerr          | 100000 | 800.21ms    |   8.00us  |

#### Windows Intel i7-7500U	

| method             | count  | total time  | item time |
|--------------------|--------|-------------|-----------|
| daw::memory_stream | 100000 |    21.70ms  | 217.00ns  |
| snprintf           | 100000 |    96.02ms  | 960.96ns  |
| std::string_stream | 100000 |   127.61ms  |   1.28us  |
| daw::con_err       | 100000 |   326.27ms  |   3.26us  |
| std::cerr          | 100000 |   842.78ms  |   8.43us  |
| printf             | 100000 |      3.99s  |  39.86us  |

# Round trip errors
Tested using strtod/strtof to return string to a double/float

| type| value| output| difference                                                                                                                                                                                                                                                                                                                                              | 
|-----|------|-------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------| 
| double| 0.1| 0.1| 0                                                                                                                                                                                                                                                                                                                                                          | 
| double| 0.12| 0.11999999999999998| -1.38778e-17                                                                                                                                                                                                                                                                                                                              | 
| double| 0.123| 0.12299999999999999| -1.38778e-17                                                                                                                                                                                                                                                                                                                             | 
| double| 0.1234| 0.12339999999999998| -1.38778e-17                                                                                                                                                                                                                                                                                                                            | 
| double| 1.2345| 1.23449999999999992| 0                                                                                                                                                                                                                                                                                                                                       | 
| double| 0.333333| 0.33333333333333332| 0                                                                                                                                                                                                                                                                                                                                     | 
| double| 0.666667| 0.66666666666666665| 0                                                                                                                                                                                                                                                                                                                                     | 
| double| 3.33333| 3.33333333333333349| 0                                                                                                                                                                                                                                                                                                                                      | 
| double| 6.66667| 6.66666666666666698| 0                                                                                                                                                                                                                                                                                                                                      | 
| double| 2.22507e-308| 0.0000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 0000000022250738585072009| -4.94066e-324  | 
| double| -2.22507e-308| -0.00000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 000000022250738585072009| 4.94066e-324 | 
| double| 1.79769e+308| 17976931348623156600000000000000000000000000000000 00000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 000000000| 0                               | 
| double| 4.94066e-324| 0| -4.94066e-324                                                                                                                                                                                                                                                                                                                                       | 
| double| 4| 3.99999999999999953| 0                                                                                                                                                                                                                                                                                                                                            | 
| double| 8.98847e+307| 89884656743115786500000000000000000000000000000000 00000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 00000000000000000000000000000000000000000000000000 00000000| 0                                | 
| float| 0.1| 0.1| 0                                                                                                                                                                                                                                                                                                                                                           | 
| float| 0.12| 0.119999996| 0                                                                                                                                                                                                                                                                                                                                                  | 
| float| 0.123| 0.123000001| 0                                                                                                                                                                                                                                                                                                                                                 | 
| float| 0.1234| 0.123400000| 0                                                                                                                                                                                                                                                                                                                                                | 
| float| 1.2345| 1.234500046| 0                                                                                                                                                                                                                                                                                                                                                | 
| float| 0.333333| 0.333333330| 0                                                                                                                                                                                                                                                                                                                                              | 
| float| 0.666667| 0.666666660| 0                                                                                                                                                                                                                                                                                                                                              | 
| float| 3.33333| 3.333333240| 0                                                                                                                                                                                                                                                                                                                                               | 
| float| 6.66667| 6.666666481| 0                                                                                                                                                                                                                                                                                                                                               | 
| float| 1.17549e-38| 0.0000000000000000000000000000000000000117549440| 0                                                                                                                                                                                                                                                                                                      | 
| float| 3.40282e+38| 340282346300000000000000000000000000000| 0                                                                                                                                                                                                                                                                                                               | 
| float| 1.4013e-45| 0| -1.4013e-45                                                                                                                                                                                                                                                                                                                                            | 
| float| 4| 3.999999788| 0                                                                                                                                                                                                                                                                                                                                                     | 
| float| 1.70141e+38| 170141175700000000000000000000000000000| 0                                                                                                                                                                                                                                                                                                               | 

