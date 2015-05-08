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

    for ( auto const & pos : view.as_rows() )
    {
        std::cout << pos << "\n";
    }

    for ( auto i = 0u; i < view.rows(); ++i )
    {
        std::cout << view.row( i ) << "\n";
    }
}

// cl -EHsc I../include 01-iterate-c++11.cpp && 01-iterate-c++11.exe
// g++ -std=c++11 -I../include -o 01-iterate-c++11.exe 01-iterate-c++11.cpp && 01-iterate-c++11.exe
