// Copyright 2015 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// array_view2d is inspired on https://github.com/rhysd/array_view by Linda_pp.


#include "array_view2d.hpp"
#include "array_view2d_output.hpp"

#include <iostream>

struct packet
{
    size_t dim;
    std::vector<int> data;
};

packet produce()
{
    int a[] = { 0,0, 1,1, 2,2 };

    packet pack; pack.dim = 3;

    for ( int i = 0; i < 6; ++i )
        pack.data.push_back( a[i] );
    
    return pack;
}

int main()
{
    using namespace nonstd;
    
    packet pack = produce();
    array_view2d<int> view = make_view2d( pack.data, pack.dim );

    typedef array_view2d<int>::row_iterator row_iterator;

    for ( row_iterator pos = view.as_rows().begin(); pos != view.as_rows().end(); ++pos )
    {
        std::cout << *pos << "\n";
    }

    for ( unsigned i = 0u; i < view.rows(); ++i )
    {
        std::cout << view.row( i ) << "\n";
    }
}

// cl -EHsc I../include 01-iterate-c++98.cpp && 01-iterate-c++98.exe
// g++ -std=c++98 I../include -o 01-iterate-c++98.exe 01-iterate-c++98.cpp && 01-iterate-c++98.exe
