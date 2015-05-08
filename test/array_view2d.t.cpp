// Copyright 2015 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// array_view2d is inspired on https://github.com/rhysd/array_view by Linda_pp.

#include "array_view2d.hpp"
#include "array_view2d_output.hpp"

#include "lest_cpp03.hpp"

#include <algorithm>
#include <iostream>

#define CASE( name ) lest_CASE( specification(), name )

#define dimension_of(a)  ( sizeof(a) / sizeof( 0[a] ) )

extern lest::tests & specification();

#ifdef lest_MAIN

lest::tests & specification() 
{ 
    static lest::tests tests; 
    return tests; 
}
#endif // lest_MAIN

namespace {

using namespace nonstd;

template< typename T >
std::ostream & operator<<( std::ostream & os, std::vector<T> const & v )
{
    os << "{ ";

    if ( ! v.empty() )
    {
        typename std::vector<T>::const_iterator pos = v.begin();
        typename std::vector<T>::const_iterator const end = v.end();
        
        while ( true )
        {
            os << *pos;
            if ( ++pos != end ) os << ", ";
            else                break;
        }
    }
    return os << " }";
}

#if av_CPP11_OR_GREATER || av_COMPILER_IS_MSVC12 

template< typename T >
std::ostream & operator<<( std::ostream & os, std::initializer_list<T> const & v )
{
    os << "{ ";

    if ( ! v.empty() )
    {
        typename std::initializer_list<T>::const_iterator pos = v.begin();
        typename std::initializer_list<T>::const_iterator const end = v.end();
        
        while ( true )
        {
            os << *pos;
            if ( ++pos != end ) os << ", ";
            else                break;
        }
    }
    return os << " }";
}
#endif

//
// Test cases:
//

CASE( "Print several views, slices and rows" " [.]" )
{
#if av_CPP11_OR_GREATER || av_COMPILER_IS_MSVC12 
    int a[] = { 0,1,2,3,4,5,6,7,8,9 };
    array_view2d<int> va( a, 1 );
    std::cout << 
        "\n1-row view on array, complete view:\n" << 
        va << "\n" ;
#endif

    std::vector<int> v;
    
    for ( int i = 0; i < 12; ++i )
    {
        v.push_back(i);
    }
    
    array_view2d<int> vv1( v );
    std::cout << 
        "\n1-row view on std::vector<int>:\n" <<
        vv1 << " - complete view\n" << 
        vv1.row(0) << " - row 0\n" <<
        vv1.slice( 1, vv1.size() -1 ) << " - slice pos 1 - end\n" <<
        vv1.slice( 1, 3 ) << " - slice pos 1, 3 elements\n" <<
        vv1.slice( check_bound, 1, 3 ) << " - slice pos 1, 3 elements (checked)\n" <<
        vv1.slice_before( 5 ) << " - slice before pos 5\n" <<
        vv1.slice_from( 5 ) << " - slice after pos 5\n";
        
    array_view2d<int> vv3( v, 3 );
    std::cout << 
        "\n3-row view on std::vector<int>:\n" <<
        vv1 << " - complete view\n" << 
        vv3.row(0) << " - row 0\n" << 
        vv3.row(1) << " - row 1\n" << 
        vv3.row(2) << " - row 2\n" <<
        vv3.row(2).slice_from(1) << " - row 2, slice after pos 1\n" <<
        to_vector( vv3.row(1) ) << " - row 1 converted to vector\n";

#if av_CPP11_OR_GREATER || av_COMPILER_IS_MSVC12 

    std::array<int, 6> arr( {0,1,2,3,4,5} );
    array_view2d<int> vva( arr, 3 );
    std::cout << 
        "\n3-row view on 6-element std::array:\n" <<
        vva << " - complete view\n" <<
        vva.row(0) << " - row 0\n" << 
        vva.row(1) << " - row 1\n" << 
        vva.row(2) << " - row 2\n" ;
#endif
}

CASE( "A default-constructed view is empty" " [ctor]" )
{
    array_view2d<int> av;
    
    EXPECT( av.empty() );
}

#if av_CPP11_OR_GREATER || av_COMPILER_IS_MSVC12 

CASE( "A view constructed from an array..." " [ctor]" )
{
    SETUP( "" ) {
        int a[] = { 0, 1, 2, 3 };
        array_view2d<int> av( a );

    SECTION( "has expected number of elements" ) {
        EXPECT( av.size()   == av_dimensionof( a ) );
        EXPECT( av.length() == av_dimensionof( a ) );
    }
    SECTION( "has expected elements" ) {
        EXPECT( std::equal(av.begin(), av.end(), a ) );
    }
    SECTION( "throws when it does not exactly contain the specified number of rows" ) {
        EXPECT_THROWS(    array_view2d<int>( a, 3 ) );
        EXPECT_THROWS_AS( array_view2d<int>( a, 3 ), std::runtime_error );
    }
    }
}

CASE( "A view constructed from an initializer list..." " [ctor]" )
{
    SETUP( "" ) {
        int a[] = { 0, 1, 2, 3 };
        array_view2d<int> av( { 0, 1, 2, 3 }  );

    SECTION( "has expected number of elements" ) {
        EXPECT( av.size()   == 4 );
        EXPECT( av.length() == 4 );
    }
    SECTION( "has expected elements" ) {
        EXPECT( std::equal(av.begin(), av.end(), a ) );
    }
    SECTION( "throws when it does not exactly contain the specified number of rows" ) {
        EXPECT_THROWS(    array_view2d<int>( a, 3 ) );
        EXPECT_THROWS_AS( array_view2d<int>( a, 3 ), std::runtime_error );
    }
    }
}

CASE( "A view constructed from a std::array..." " [ctor]" )
{
    SETUP( "" ) {
        std::array<int, 4> a( { 0, 1, 2, 3 } );
        array_view2d<int> av( a );

    SECTION( "has expected number of elements" ) {
        EXPECT( av.size()   == a.size() );
        EXPECT( av.length() == a.size() );
    }
    SECTION( "has expected elements" ) {
        EXPECT( std::equal(av.begin(), av.end(), a.begin() ) );
    }
    SECTION( "throws when it does not exactly contain the specified number of rows" ) {
        EXPECT_THROWS(    array_view2d<int>( a, 3 ) );
        EXPECT_THROWS_AS( array_view2d<int>( a, 3 ), std::runtime_error );
    }
    }
}
#endif

CASE( "A view constructed from a std::vector..." " [ctor]" )
{
    SETUP( "" ) {
        int a[] = { 0, 1, 2, 3 };
        std::vector<int> v;
        for ( unsigned int i = 0; i < av_dimensionof(a); ++i )
            v.push_back( a[i] );
        array_view2d<int> av( v );

    SECTION( "has expected number of elements" ) {
        EXPECT( av.size()   == v.size() );
        EXPECT( av.length() == v.size() );
    }
    SECTION( "has expected elements" ) {
        EXPECT( std::equal(av.begin(), av.end(), v.begin() ) );
    }
    SECTION( "throws when it does not exactly contain the specified number of rows" ) {
        EXPECT_THROWS(    array_view2d<int>( v, 3 ) );
        EXPECT_THROWS_AS( array_view2d<int>( v, 3 ), std::runtime_error );
    }
    }
}

CASE( "A view constructed from two iterators..." " [ctor]" )
{
    SETUP( "" ) {
        int a[] = { 0, 1, 2, 3 };
        array_view2d<int> av( a,  a + av_dimensionof( a ) );

    SECTION( "has expected number of elements" ) {
        EXPECT( av.size()   == av_dimensionof( a ) );
        EXPECT( av.length() == av_dimensionof( a ) );
    }
    SECTION( "has expected elements" ) {
        EXPECT( std::equal(av.begin(), av.end(), a ) );
    }
    SECTION( "throws when it does not exactly contain the specified number of rows" ) {
        EXPECT_THROWS(    array_view2d<int>( a, a + av_dimensionof( a ), 3 ) );
        EXPECT_THROWS_AS( array_view2d<int>( a, a + av_dimensionof( a ), 3 ), std::runtime_error );
    }
    }
}

CASE( "A view constructed via make_view2d from..." " [array_view2d][make_view2d]" )
{
    SETUP( "" ) {
        int a[] = { 0, 1, 2, 3 };

    SECTION( "a C-array plus size" ) {
        array_view2d<int> av( make_view2d( a, av_dimensionof( a ), 1 ) );

        EXPECT( std::equal( av.begin(), av.end(), a ) );
    }
#if ! av_COMPILER_IS_MSVC6
    SECTION( "a C-array a[N]" ) {
        array_view2d<int> av( make_view2d( a, 1 ) );

        EXPECT( std::equal( av.begin(), av.end(), a ) );
    }
#endif
    SECTION( "two iterators" ) {
        array_view2d<int> av( make_view2d( a, a + av_dimensionof( a ), 1 ) );

        EXPECT( std::equal( av.begin(), av.end(), a ) );
    }
#if av_CPP11_OR_GREATER || av_COMPILER_IS_MSVC12 
    SECTION( "a std::initializer_list" ) {
        auto il = { 0, 1, 2, 3 };
        array_view2d<int> av( make_view2d( il, 1 ) );

        EXPECT( std::equal( av.begin(), av.end(), il.begin() ) );
    }
    SECTION( "a std::array" ) {
        auto arr = std::array<int, 4>{ 0, 1, 2, 3 };
        array_view2d<int> av( make_view2d( arr, 1 ) );

        EXPECT( std::equal( av.begin(), av.end(), arr.begin() ) );
    }
#endif
    SECTION( "a std::vector" ) {
        std::vector<int> vec( a, a + av_dimensionof( a ) );
        array_view2d<int> av( make_view2d( vec, 1 ) );

        EXPECT( std::equal( av.begin(), av.end(), vec.begin() ) );
    }
    }
}

CASE( "A view compared..." " [array_view2d][compare]" )
{
    SETUP( "" ) {
        int a[] = { 0, 1, 2, 3 };
        int b[] = { 3, 2, 1, 0 };

    SECTION( "to a C-array" ) {
        array_view2d<int> av( a,  a + av_dimensionof( a ) );
        array_view2d<int> bv( b,  b + av_dimensionof( b ) );

        EXPECT( av == av );
        EXPECT( av == a  );
        EXPECT( a  == av );
        EXPECT( bv != av );
        EXPECT( bv != a  );
        EXPECT( a  != bv );
    }
#if av_CPP11_OR_GREATER || av_COMPILER_IS_MSVC12 
    SECTION( "to a std::initializer_list" ) {
        auto il = { 0, 1, 2, 3 };
        auto kl = { 3, 2, 1, 0 };
        array_view2d<int> av( make_view2d( il, 1 ) );

        EXPECT( (av == il) );
        EXPECT( (il == av) );
        EXPECT( (av != kl) );
        EXPECT( (kl != av) );
    }
    SECTION( "to a std::array" ) {
        auto arr = std::array<int, 4>{ 0, 1, 2, 4 };
        auto brr = std::array<int, 4>{ 4, 2, 1, 0 };
        array_view2d<int> av( make_view2d( arr, 1 ) );

        EXPECT( (av  == arr) );
        EXPECT( (arr == av ) );
        EXPECT( (av  != brr) );
        EXPECT( (brr != av ) );
    }
#endif
    SECTION( "to a std::vector" ) {
        std::vector<int>  v1( a, a + av_dimensionof( a ) );
        std::vector<int>  v2( b, b + av_dimensionof( b ) );
        array_view2d<int> av( make_view2d( v1, 1 ) );

        EXPECT( (av == v1 ) );
        EXPECT( (v1 == av ) );
        EXPECT( (av != v2 ) );
        EXPECT( (v2 != av ) );
    }
    }
}

CASE( "Accessing elements..." " [element]" )
{
    SETUP( "" ) {
        int a[] = { 0, 1, 2, 3 };
        array_view2d<int> av( a,  a + av_dimensionof( a ) );

    SECTION( "from front and back yields correct values" ) {
        EXPECT( av.front() == 0 );
        EXPECT( av.back()  == 3 );
    }
    SECTION( "indexed yields correct values" ) {
        EXPECT( av[0] == 0 );
        EXPECT( av[1] == 1 );
        EXPECT( av[2] == 2 );
        EXPECT( av[3] == 3 );
    }
    SECTION( "via at() yields correct values" ) {
        EXPECT( av.at(0) == 0 );
        EXPECT( av.at(1) == 1 );
        EXPECT( av.at(2) == 2 );
        EXPECT( av.at(3) == 3 );
    }
    SECTION( "via at() throws when out of bound" ) {
        EXPECT_THROWS(    av.at( 4 ) );
        EXPECT_THROWS_AS( av.at( 4 ), std::out_of_range );
    }
    }
}

CASE( "Slicing..." " [slice][pos]" )
{
    SETUP( "" ) {
        int a[] = { 0, 1, 2, 3 };
        array_view2d<int> av( a,  a + av_dimensionof( a ) );

    SECTION( "with valid pos and length yields correct values" ) {
        int b[] = { 1,2 };
        array_view2d<int> as( av.slice( 1, 2 ) );
        
        EXPECT( std::equal( as.begin(), as.end(), b ) );
    }
    SECTION( "before valid pos yields correct values" ) {
        int b[] = { 0, 1 };
        array_view2d<int> as( av.slice_before( 2 ) );
        
        EXPECT( std::equal( as.begin(), as.end(), b ) );
    }
    SECTION( "after valid pos yields correct values" ) {
        int b[] = { 1, 2, 3 };
        array_view2d<int> as( av.slice_from( 1 ) );
        
        EXPECT( std::equal( as.begin(), as.end(), b ) );
    }
    SECTION( "with invalid pos or length throws" ) {
        EXPECT_THROWS(    av.slice( 4, 1 ) );
        EXPECT_THROWS(    av.slice( 0, 5 ) );
        EXPECT_THROWS_AS( av.slice( 4, 1 ), std::out_of_range );
        EXPECT_THROWS_AS( av.slice( 0, 5 ), std::out_of_range );
    }
    SECTION( "before invalid pos throws" ) {
        EXPECT_THROWS(    av.slice_before( 5 ) );
        EXPECT_THROWS_AS( av.slice_before( 5 ), std::out_of_range );
    }
    SECTION( "after invalid pos throws" ) {
        EXPECT_THROWS(    av.slice_from( 5 ) );
        EXPECT_THROWS_AS( av.slice_from( 5 ), std::out_of_range );
    }
    }
}

CASE( "Slicing..." " [slice][iterator]" )
{
    SETUP( "" ) {
        int a[] = { 0, 1, 2, 3 };
        array_view2d<int> av( a,  a + av_dimensionof( a ) );

    SECTION( "with valid start and end yields correct values" ) {
        array_view2d<int> as( av.slice( av.begin(), av.end() ) );
        
        EXPECT( std::equal( as.begin(), as.end(), a ) );
    }
    SECTION( "before valid pos yields correct values" ) {
        int b[] = { 0, 1 };
        array_view2d<int> as( av.slice_before( av.begin() + 2 ) );
        
        EXPECT( std::equal( as.begin(), as.end(), b ) );
    }
    SECTION( "after valid pos yields correct values" ) {
        int b[] = { 1, 2, 3 };
        array_view2d<int> as( av.slice_from( av.begin() + 1 ) );
        
        EXPECT( std::equal( as.begin(), as.end(), b ) );
    }
    SECTION( "with invalid start or end throws" ) {
        EXPECT_THROWS(    av.slice( av.end()  , av.end() + 0 ) );
        EXPECT_THROWS(    av.slice( av.begin(), av.end() + 1 ) );
        EXPECT_THROWS_AS( av.slice( av.end()  , av.end() + 0 ), std::out_of_range );
        EXPECT_THROWS_AS( av.slice( av.begin(), av.end() + 1 ), std::out_of_range );
    }
    SECTION( "before invalid pos throws" ) {
        EXPECT_THROWS(    av.slice_before( av.end() + 1 ) );
        EXPECT_THROWS_AS( av.slice_before( av.end() + 1 ), std::out_of_range );
    }
    SECTION( "after invalid pos throws" ) {
        EXPECT_THROWS(    av.slice_from( av.end() + 1 ) );
        EXPECT_THROWS_AS( av.slice_from( av.end() + 1 ), std::out_of_range );
    }
    }
}

CASE( "Slicing..." " [slice][pos][check_bound]" )
{
    SETUP( "" ) {
        int a[] = { 0, 1, 2, 3 };
        array_view2d<int> av( a,  a + av_dimensionof( a ) );

    SECTION( "with valid pos and length yields correct values" ) {
        int b[] = { 1,2 };
        array_view2d<int> as( av.slice( check_bound, 1, 2 ) );
        
        EXPECT( std::equal( as.begin(), as.end(), b ) );
    }
    SECTION( "before valid pos yields correct values" ) {
        int b[] = { 0, 1 };
        array_view2d<int> as( av.slice_before( check_bound, 2 ) );
        
        EXPECT( std::equal( as.begin(), as.end(), b ) );
    }
    SECTION( "after valid pos yields correct values" ) {
        int b[] = { 1, 2, 3 };
        array_view2d<int> as( av.slice_from( check_bound, 1 ) );
        
        EXPECT( std::equal( as.begin(), as.end(), b ) );
    }
    SECTION( "with invalid pos or length throws" ) {
        EXPECT_THROWS(    av.slice( check_bound, 4, 1 ) );
        EXPECT_THROWS(    av.slice( check_bound, 0, 5 ) );
        EXPECT_THROWS_AS( av.slice( check_bound, 4, 1 ), std::out_of_range );
        EXPECT_THROWS_AS( av.slice( check_bound, 0, 5 ), std::out_of_range );
    }
    SECTION( "before invalid pos throws" ) {
        EXPECT_THROWS(    av.slice_before( check_bound, 5 ) );
        EXPECT_THROWS_AS( av.slice_before( check_bound, 5 ), std::out_of_range );
    }
    SECTION( "after invalid pos throws" ) {
        EXPECT_THROWS(    av.slice_from( check_bound, 5 ) );
        EXPECT_THROWS_AS( av.slice_from( check_bound, 5 ), std::out_of_range );
    }
    }
}

CASE( "Slicing..." " [slice][iterator][check_bound]" )
{
    SETUP( "" ) {
        int a[] = { 0, 1, 2, 3 };
        array_view2d<int> av( a,  a + av_dimensionof( a ) );

    SECTION( "with valid start and end yields correct values" ) {
        array_view2d<int> as( av.slice( check_bound, av.begin(), av.end() ) );
        
        EXPECT( std::equal( as.begin(), as.end(), a ) );
    }
    SECTION( "before valid pos yields correct values" ) {
        int b[] = { 0, 1 };
        array_view2d<int> as( av.slice_before( check_bound, av.begin() + 2 ) );
        
        EXPECT( std::equal( as.begin(), as.end(), b ) );
    }
    SECTION( "after valid pos yields correct values" ) {
        int b[] = { 1, 2, 3 };
        array_view2d<int> as( av.slice_from( check_bound, av.begin() + 1 ) );
        
        EXPECT( std::equal( as.begin(), as.end(), b ) );
    }
    SECTION( "with invalid start or end throws" ) {
        EXPECT_THROWS(    av.slice( check_bound, av.end()  , av.end() + 0 ) );
        EXPECT_THROWS(    av.slice( check_bound, av.begin(), av.end() + 1 ) );
        EXPECT_THROWS_AS( av.slice( check_bound, av.end()  , av.end() + 0 ), std::out_of_range );
        EXPECT_THROWS_AS( av.slice( check_bound, av.begin(), av.end() + 1 ), std::out_of_range );
    }
    SECTION( "before invalid pos throws" ) {
        EXPECT_THROWS(    av.slice_before( check_bound, av.end() + 1 ) );
        EXPECT_THROWS_AS( av.slice_before( check_bound, av.end() + 1 ), std::out_of_range );
    }
    SECTION( "after invalid pos throws" ) {
        EXPECT_THROWS(    av.slice_from( check_bound, av.end() + 1 ) );
        EXPECT_THROWS_AS( av.slice_from( check_bound, av.end() + 1 ), std::out_of_range );
    }
    }
}

CASE( "Selecting a row..." " [row]" )
{
    SETUP( "" ) {
        int a[] = { 0, 1, 2, 3 };
        array_view2d<int> av( a,  a + av_dimensionof( a ), 2 );

    SECTION( "with valid index yields correct values" ) {
        int r0[] = { 0, 1 };
        int r1[] = { 2, 3 };
        array_view2d<int> ar0( av.row( 0 ) );
        array_view2d<int> ar1( av.row( 1 ) );
        
        EXPECT( std::equal( ar0.begin(), ar0.end(), r0 ) );
        EXPECT( std::equal( ar1.begin(), ar1.end(), r1 ) );
    }
    SECTION( "with invalid index throws" ) {
        EXPECT_THROWS(    av.row( 2 ) );
        EXPECT_THROWS_AS( av.row( 2 ), std::out_of_range );
    }
    }
}

CASE( "Selecting a row..." " [row][check_bound]" )
{
    SETUP( "" ) {
        int a[] = { 0, 1, 2, 3 };
        array_view2d<int> av( a,  a + av_dimensionof( a ), 2 );

    SECTION( "with valid index yields correct values" ) {
        int r0[] = { 0, 1 };
        int r1[] = { 2, 3 };
        array_view2d<int> ar0( av.row( check_bound, 0 ) );
        array_view2d<int> ar1( av.row( check_bound, 1 ) );
        
        EXPECT( std::equal( ar0.begin(), ar0.end(), r0 ) );
        EXPECT( std::equal( ar1.begin(), ar1.end(), r1 ) );
    }
    SECTION( "with invalid index throws" ) {
        EXPECT_THROWS(    av.row( check_bound, 2 ) );
        EXPECT_THROWS_AS( av.row( check_bound, 2 ), std::out_of_range );
    }
    }
}

CASE( "Conversion to vector yields vector with correct values" " [conversion]" )
{
    int a[] = { 0, 1, 2, 3 };
    array_view2d<int> av( a,  a + av_dimensionof( a ), 2 );
    std::vector<int> v1( to_vector( av ) );
    std::vector<int> v2( to_vector( av, std::allocator<int>() ) );

    EXPECT( std::equal( av.begin(), av.end(), v1.begin() ) );
    EXPECT( std::equal( av.begin(), av.end(), v2.begin() ) );
}

} // anonymous namespace

#ifdef lest_MAIN

int main( int argc, char * argv[] )
{
    return lest::run( specification(), argc, argv /*, std::cout */ );
}
#endif // lest_MAIN

#if 0

cl -nologo -EHsc -W3 -Dlest_MAIN -Dav_FEATURE_EXPECT_THROWS=1 -D_CRT_SECURE_NO_WARNINGS -I../include array_view2d.t.cpp && array_view2d.t.exe

g++ -std=c++98 -Dlest_MAIN -Dav_FEATURE_EXPECT_THROWS=1 -I../include -o array_view2d.t.exe array_view2d.t.cpp && array_view2d.t.exe

g++ -std=c++11 -Dlest_MAIN -Dav_FEATURE_EXPECT_THROWS=1 -I../include -o array_view2d.t.exe array_view2d.t.cpp && array_view2d.t.exe

#endif // 0
