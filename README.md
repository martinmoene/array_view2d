array_view2d &ndash; 2D view on array or vector for C++98 and C++11&ensp;[![Build Status](https://travis-ci.org/martinmoene/array_view2d.png?branch=master)](https://travis-ci.org/martinmoene/array_view2d)
=========================================================================

*array_view2d* is a two-file header-only library to provide a two-dimensional view on a one-dimensional array or vector. The library is inspired on array_view [1] and can be used with C++98 and later and with Visual C++ 6 (VC6). With C++11 *array_view2d* provides a unified way to reference an array or vector (see [example](example/02-reference-c++11.cpp)).  

**Contents**  
- [Example usage](#example-usage)
- [In a nutshell](#in-a-nutshell)
- [License](#license)
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Synopsis](#synopsis)
- [Reported to work with](#reported-to-work-with)
- [C++ standard proposals on array views](#c-standard-proposals-on-array-views)
- [Notes and references](#notes-and-references)

<!--
- [Comparison of std::optional, optional lite and Boost.Optional](#comparison-of-stdoptional-array_view2d-and-boostoptional)
- [Implementation notes](#implementation-notes)
-->


Example usage
-------------

```C++
#include "array_view2d.hpp"
#include "array_view2d_output.hpp"

#include <iostream>

struct packet
{
    std::size_t dim;
    std::vector<int> data;
};

packet produce()
{
    return { 3, { 0,0, 1,1, 2,2 } };
}

int main()
{
    auto pack = produce();
    auto view = nonstd::make_view2d( pack.data, pack.dim );

    for ( auto pos : view.as_rows() )
    {
        std::cout << pos << "\n";
    }
}
```

### Compile and run

```
prompt>g++ -Wall -std=c++11 -o example.exe example.cpp && example
{ 0, 0 }
{ 1, 1 }
{ 2, 2 }
```

Here is [the same example expressed in C++98](example/01-iterate-c++98.cpp).


In a nutshell
-------------
**array_view2d** ...

**Features and properties of array_view2d** are ease of installation (single header), ...

**Not provided** are ...


License
-------
*array_view2d* uses the [Boost Software License](LICENSE_1_0.txt).
 

Dependencies
------------
*array_view2d* has no other dependencies than the [C++ standard library](http://en.cppreference.com/w/cpp/header).


Installation
------------

*array_view2d* is a two-file header-only library. Put `array_view2d.hpp` and `array_view2d_output.hpp` directly into the project source tree or somewhere reachable from your project.


Synopsis
--------

**Contents**  
[Types in namespace nonstd](#types-in-namespace-nonstd)  
[Interface of *array_view2d*](#interface-of-array_view2d)  
[Algorithms for *array_view2d*](#algorithms-for-array_view2d)  
[Macros to control error reporting](#macros-to-control-error-reporting)  

### Types in namespace nonstd

| Purpose          | Type | Object |
|------------------|------|--------|
| View             | template< typename T ><br>class array_view2d; |&nbsp;|
| Bound checking   | struct check_bound_t; | check_bound_t check_bound; |
| Error reporting  | see [this section](#macros-to-control-error-reporting) |&nbsp;  |

### Interface of *array_view2d*

| Kind              | Method                                       | Result |
|-------------------|----------------------------------------------|--------|
| Construction      | **array_view2d**() | A blind eye |
| &nbsp;            | **array_view2d**(...)<br>C-array, std::array, std::vector, std::initializer_list | A view |
| Iteration         | **begin**(), **end**() | An iterator |
| &nbsp;            | **cbegin**(), **cend()** | &nbsp; |
| Iteration,reverse | **rbegin**(), **rend**() | &nbsp; |
| &nbsp;            | **crbegin**(), **crend**() | &nbsp; |
| Iteration, row    | **as_rows**() | [A forward-iteratable](https://www.sgi.com/tech/stl/ForwardIterator.html) |
| Access            | **empty**() | True if no elements |
| &nbsp;            | **size**(), **length**() | Number of elements |
| &nbsp;            | **max_size**() | Number of elements |
| &nbsp;            | **row_size**() | Number of elements in a row |
| &nbsp;            | **rows**() | Number of rows |
| Access, content   | **operator[]**( n ) | Element|
| &nbsp;            | **at**( n ) | Element, bound checked |
| &nbsp;            | **data**() | Pointer to first element |
| &nbsp;            | **front**() | First element |
| &nbsp;            | **back**() | Last element |
| Slicing           | **slice**( index, count ) | [A subview](http://en.wikipedia.org/wiki/Array_slicing) |
| &nbsp;            | **slice_before**( index ) | &nbsp; |
| &nbsp;            | **slice_from**( index ) | &nbsp; |
| Slicing, iterator | **slice**( first, last ) | &nbsp; |
| &nbsp;            | **slice_before**( pos ) | &nbsp; |
| &nbsp;            | **slice_from**( pos ) | &nbsp; |
| Slicing, checked  | **slice**( check_bound, index, count ) | &nbsp; |
| &nbsp;            | **slice_before**( check_bound, index ) | &nbsp; |
| &nbsp;            | **slice_from**( check_bound, index ) | &nbsp; |
| Slicing, iterator | **slice**( check_bound, first, last ) | &nbsp; |
| &nbsp;            | **slice_before**( check_bound, pos ) | &nbsp; |
| &nbsp;            | **slice_from**( check_bound, pos ) | &nbsp; |
| Row access        | **row**( n ) | A view on row elements |
| Row, checked      | **row**( check_bound, n ) | &nbsp; |


### Algorithms for array_view2d

| Kind              | Function                          | Result |
|-------------------|-----------------------------------|--------|
| Equality          | &nbsp;                            | &nbsp; | 
| ==                | **operator==**( view1, view2 )    | True if equal | 
| &nbsp;            | **operator==**( view, container ) | &nbsp; | 
| &nbsp;            | **operator==**( container, view ) | &nbsp; | 
| !=                | **operator!=**( view1, view2 )    | True if not equal | 
| &nbsp;            | **operator!=**( view, container ) | &nbsp; | 
| &nbsp;            | **operator!=**( container, view ) | &nbsp; | 
| Other algorithms  | &nbsp; | &nbsp; | 
| Create            | **make_view2d**(...)<br>C-array, pointer and count, iterator pair,&emsp;<br>std::array, std::vector, std::initializer_list | A view | 
| Convert           | **to_vector**(...)<br>view and optional allocator | A std::vector  | 


### Macros to control error reporting

*array_view2d* can perform sanity checks on construction and perform bound checking on methods such as `row()`. Sanity checks in constructors use the same mechanism as implicit bound checks explained below.

*array_view2d* provides two approaches to bound checking: 
- explicitly when calling `row(check_bound, n)` and 
- implicitly when using `row(n)`.

The explicit variants with the `check_bound_t` parameter throw a `std::out_of_range` exception when a parameter is out of bound.

The implicit variants without the `check_bound_t` parameter, behave in the following way:
- Assert that a parameter is within bounds; this is the default for debug builds.
- Throw an exception when a parameter violates its bounds, see `av_FEATURE_EXPECT_THROWS`.
- Do not check for bound violation; this is the default for release build, see `av_FEATURE_EXPECT_NOP`.
- Perform a user-specified action, see `av_EXPECT`.

The following macros control boundary checking behaviour for methods that don't use **check_bound_t**.

-D<b>av_FEATURE_EXPECT_NOP</b>=0  
Define this to 1 to turn off boundary checking assertions. Default is 0.

-D<b>av_FEATURE_EXPECT_ASSERTS</b>=1  
At default, boundary checks assert. Default is 1.  
Turn it off by defining av_FEATURE_EXPECT_NOP.  
Use exceptions instead by defining av_FEATURE_EXPECT_THROWS.  
Note that the assertion mechanism is active in debug builds, not in release builds.

-D<b>av_FEATURE_EXPECT_THROWS</b>=0  
Define this to 1 to activate boundary checking exceptions. Default is 0.  
When an out of bounds error occurs, a `std::out_of_range` exception is thrown.  
When a sanity check fails, a `std::runtime_error` exception is thrown.

-D<b>av_EXPECT( expr, excpt, note )</b>  
Define this macro to specify boundary checking behaviour, e.g.:  
```
#define av_EXPECT( expr, excpt, note )  assert( ( expr ) && note )
```
Defining av_EXPECT deactivates macros av_FEATURE_EXPECT_NOP, av_FEATURE_EXPECT_ASSERTS and av_FEATURE_EXPECT_THROWS.


Reported to work with
---------------------

*array_view2d* is reported to work with the following compilers: 
- Visual C++ 6 SP6 (VS6), VC10, (VS2010), VC11 (VS2012), VC12 (VS2013)
- GNUC 4.8.1 with -std=c++98, -std=c++03, -std=c++11, -std=c++1y 
- clang 3.4 with -std=c++03, -std=c++11 (on Travis)


C++ standard proposals on array views
-------------------------------------

At the time of writing there are two proposals for the C++ standard on providing multidimensional views on arrays [2-4]. These proposals take a general approach to dimensionality, whereas *array_view2d* restricts itself to unstrided rows and slices.


Notes and references
--------------------

[1] [array_view](https://github.com/rhysd/array_view).  
[2] [N3851: Multidimensional bounds, index and array_view](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3851.pdf) (PDF).  
[3] [N4346: Multidimensional bounds, index and array_view, revision 6](https://isocpp.org/files/papers/N4494.html).  
[4] [N4355: Shared Multidimensional Array with Polymorphic Layout](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4355.pdf) (PDF).  


<!--
Comparison of std::optional, optional lite and Boost.Optional
-------------------------------------------------------------
Implementation notes
--------------------
-->

