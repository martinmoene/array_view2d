// Copyright 2015 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// array_view2d is inspired on https://github.com/rhysd/array_view by Linda_pp.


#include "array_view2d.hpp"
#include "array_view2d_output.hpp"

#include <iostream>

void consume( nonstd::array_view2d<int> const & view )
{
    std::cout << view << "\n";
}

int main()
{
    int a[] = { 1, 2, 3 };

    consume( a );
    consume( { &a[0], 3 } );
    consume( { 1, 2, 3, 4 } );
    consume( std::array<int, 5>{ 1, 2, 3, 4, 5 } );
    consume( std::vector<int>{ 1, 2, 3, 4, 5, 6 } );
}

// cl -EHsc I../include 02-reference-c++11.cpp && 02-reference-c++11.exe
// g++ -std=c++11 I../include -o 02-reference-c++11.exe 02-reference-c++11.cpp && 02-reference-c++11.exe
