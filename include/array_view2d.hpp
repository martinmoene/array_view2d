// Copyright 2015 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// array_view2d is inspired on https://github.com/rhysd/array_view by Linda_pp.

#ifndef NONSTD_ARRAY_VIEW2D_HPP_INCLUDED
#define NONSTD_ARRAY_VIEW2D_HPP_INCLUDED

#if __cplusplus >= 201103L
# define av_CPP11_OR_GREATER 1
#endif

#if defined( _MSC_VER ) && ( 1200 <= _MSC_VER && _MSC_VER < 1300 )
# define av_COMPILER_IS_MSVC6 1
#endif

#if defined( _MSC_VER ) && ( 1600 <= _MSC_VER && _MSC_VER < 1700 )
# define av_COMPILER_IS_MSVC10 1
#endif

#if defined( _MSC_VER ) && ( 1700 <= _MSC_VER && _MSC_VER < 1800 )
# define av_COMPILER_IS_MSVC11 1
#endif

#if defined( _MSC_VER ) && ( 1800 <= _MSC_VER && _MSC_VER < 1900 )
# define av_COMPILER_IS_MSVC12 1
#endif

#include <cassert>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <vector>

#if av_CPP11_OR_GREATER || av_COMPILER_IS_MSVC12 || av_COMPILER_IS_MSVC11 || av_COMPILER_IS_MSVC10
# include <array>
# include <memory>
#endif

#if av_CPP11_OR_GREATER || av_COMPILER_IS_MSVC12
# include <initializer_list>
#endif

#if defined( av_FEATURE_EXPECT_NOP     ) \
  + defined( av_FEATURE_EXPECT_ASSERTS ) \
  + defined( av_FEATURE_EXPECT_THROWS  ) > 1
#  error Only define one of av_FEATURE_EXPECT_NOP, av_FEATURE_EXPECT_ASSERT,av_FEATURE_EXPECT_THROWS.
#endif

#if !defined( av_FEATURE_EXPECT_NOP     ) \
 && !defined( av_FEATURE_EXPECT_ASSERTS ) \
 && !defined( av_FEATURE_EXPECT_THROWS  )
# define av_FEATURE_EXPECT_ASSERTS 1
#endif

#ifndef av_EXPECT
# if av_FEATURE_EXPECT_ASSERTS
#  define av_EXPECT( expr, excpt, note )  assert( ( expr ) && note )
# elif av_FEATURE_EXPECT_THROWS
#  define av_EXPECT( expr, excpt, note )  if ( !(expr) ) throw excpt( note )
# elif av_FEATURE_EXPECT_NOP
#  define av_EXPECT( expr, excpt, note )
# else
#  error Define one of av_FEATURE_EXPECT_NOP, av_FEATURE_EXPECT_ASSERT,av_FEATURE_EXPECT_THROWS.
# endif
#endif // av_EXPECT

#if av_CPP11_OR_GREATER || av_COMPILER_IS_MSVC12 || av_COMPILER_IS_MSVC11 || av_COMPILER_IS_MSVC10
# define av_addressof( x ) std::addressof( x )
#else
# define av_addressof( x ) ( & ( x ) )
#endif

#define av_dimensionof( a )  ( sizeof(a) / sizeof( 0[a] ) )

namespace nonstd
{

struct check_bound_t {};
static const check_bound_t check_bound;

/**
 * 2d view on an array or vector.
 */
template< typename T >
class array_view2d
{
public:
    typedef T value_type;
    typedef value_type const * pointer;
    typedef value_type const * const_pointer;
    typedef value_type const & reference;
    typedef value_type const & const_reference;
    typedef value_type const * iterator;
    typedef value_type const * const_iterator;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

#if av_COMPILER_IS_MSVC6
    typedef std::reverse_iterator< iterator, T > reverse_iterator;
    typedef std::reverse_iterator< const_iterator, T > const_reverse_iterator;
#else
    typedef std::reverse_iterator< iterator > reverse_iterator;
    typedef std::reverse_iterator< const_iterator > const_reverse_iterator;
#endif

    //
    // row iteration:
    //
    
    struct row_proxy
    {    
        row_proxy( array_view2d const & view )
        : view_( view ) {}

        struct iterator_ : std::iterator< std::forward_iterator_tag, array_view2d >
        {
            iterator_( array_view2d const & view, size_type const pos )
            : view_( view ), pos_( pos ) {}
            
            array_view2d operator*() const
            {
                return view_.row( pos_ );
            }

            iterator_ & operator++()
            {
                ++pos_;
                return *this;
            }

            iterator_ operator++( int )
            {
                iterator_ tmp( *this );
                ++( *this );
                return tmp;
            }
            
            bool operator==( iterator_ const & other ) const
            {
                return pos_ == other.pos_;
            }

            bool operator!=( iterator_ const & other ) const
            {
                return !( *this  == other );
            }

            array_view2d const & view_;
            size_type pos_;
        };
        
        iterator_ begin() const
        {
            return iterator_( view_, 0 );
        }
        
        iterator_ end() const
        {
            return iterator_( view_, view_.rows() );
        }

        array_view2d const & view_;
    };

    typedef typename row_proxy::iterator_ row_iterator;

    //
    // lifetime:
    //
    
    array_view2d()
    : data_( NULL )
    , size_( 0 )
    , rows_( 0 )
    {}

    /*implicit*/ array_view2d( const_pointer data, size_type size, size_type const rows = 1 )
    : data_( data )
    , size_( size )
    , rows_( rows )
    {
        av_EXPECT( ( size_ % rows_ ) == 0, std::runtime_error, "Must contain whole number of rows" );
    }

    template< class InputIterator >
    explicit array_view2d( InputIterator from, InputIterator to, size_type const rows = 1 )
    : data_( from )
    , size_( std::distance( from, to ) )
    , rows_( rows )
    {
        av_EXPECT( ( size_ % rows_ ) == 0, std::runtime_error, "Must contain whole number of rows" );
    }

#if av_CPP11_OR_GREATER || av_COMPILER_IS_MSVC12

    array_view2d( std::initializer_list<T> const & init, size_type const rows = 1 )
    : data_( std::begin( init ) )
    , size_( init.size() )
    , rows_( rows )
    {
        av_EXPECT( ( size_ % rows_ ) == 0, std::runtime_error, "Must contain whole number of rows" );
    }
#endif

#if av_CPP11_OR_GREATER || av_COMPILER_IS_MSVC12 || av_COMPILER_IS_MSVC11 || av_COMPILER_IS_MSVC10

    template<size_type N>
    /*implicit*/ array_view2d( std::array<T, N> const & a, size_type const rows = 1 )
    : data_( N > 0 ? a.data() : NULL )
    , size_( N )
    , rows_( rows )
    {
        av_EXPECT( ( size_ % rows_ ) == 0, std::runtime_error, "Must contain whole number of rows" );
    }

    template< size_type N >
    /*implicit*/ array_view2d( T const (& a)[N], size_type const rows = 1 )
    : data_( N > 0 ? av_addressof( a[0] ) : NULL )
    , size_( N )
    , rows_( rows )
    {
        av_EXPECT( ( size_ % rows_ ) == 0, std::runtime_error, "Must contain whole number of rows" );
        av_EXPECT( N > 0 , std::runtime_error, "Zero-length array is not permitted in ISO C++." );
    }
#endif

    /*implicit*/ array_view2d( std::vector<T> const & data, size_type const rows = 1 )
#if av_CPP11_OR_GREATER
    : data_( data.data() )
#else
    : data_( av_addressof( data[0] ) )
#endif
    , size_( data.size() )
    , rows_( rows )
    {
        av_EXPECT( ( size_ % rows_ ) == 0, std::runtime_error, "Must contain whole number of rows" );
    }

    //
    // copy-assignment construction: default
    //
    
#if av_CPP11_OR_GREATER
    array_view2d( array_view2d const & other ) = default;
#else
    array_view2d( array_view2d const & other )
    : data_( other.data_ )
    , size_( other.size_ )
    , rows_( other.rows_ )
    {}
#endif

    //
    // assignment: none
    //
    
#if av_CPP11_OR_GREATER
    array_view2d& operator=( array_view2d const & ) noexcept = delete;
    array_view2d& operator=( array_view2d && ) noexcept = delete;
#else
private:
    array_view2d& operator=( array_view2d const & );
public:
#endif

    //
    // element iterator interface:
    //
    
    const_iterator begin() const
    {
        return data_;
    }

    const_iterator end() const
    {
        return data_ + size_;
    }

    const_iterator cbegin() const
    {
        return begin();
    }

    const_iterator cend() const
    {
        return end();
    }

    const_reverse_iterator rbegin() const
    {
        return end();
    }

    const_reverse_iterator rend() const
    {
        return begin();
    }

    const_reverse_iterator crbegin() const
    {
        return rbegin();
    }

    const_reverse_iterator crend() const
    {
        return rend();
    }

    //
    // row iterator interface:
    //
    
    row_proxy as_rows() const
    {
        return row_proxy( *this );
    }
    
    //
    // access:
    //    

    size_type size() const
    {
        return size_;
    }

    size_type length() const
    {
        return size();
    }

    size_type max_size() const
    {
        return size();
    }

    bool empty() const
    {
        return size() == 0;
    }

    size_type rows() const
    {
        return rows_;
    }

    size_type row_size() const
    {
        return size_ / rows_;
    }

    const_reference operator[]( size_type const n ) const
    {
        return *( data_ + n );
    }

    const_reference at( size_type const n ) const
    {
        if ( n >= size_ )
        {
            throw std::out_of_range( "array_view2d::at()" );
        }
        return *( data_ + n );
    }

    const_pointer data() const
    {
        return data_;
    }

    const_reference front() const
    {
        return *data_;
    }

    const_reference back() const
    {
        return *( data_ + size_ - 1 );
    }

    //
    // slices, position optionally checked via av_EXPECT:
    //
    
    array_view2d slice( size_type const pos, size_type const count ) const
    {
        av_EXPECT( pos < size_ && pos + count <= size_ , std::out_of_range, "array_view2d::slice()" );

        return array_view2d( begin() + pos, begin() + pos + count );
    }

    array_view2d slice_before( size_type const pos ) const
    {
        av_EXPECT( pos < size_ , std::out_of_range, "array_view2d::slice()" );
        
        return array_view2d( begin(), begin() + pos );
    }

    array_view2d slice_from( size_type const pos ) const
    {
        av_EXPECT( pos < size_ , std::out_of_range, "array_view2d::slice()" );
        
        return array_view2d( begin() + pos, end() );
    }

    //
    // slices, position bound checked:
    //
    
    array_view2d slice( check_bound_t, size_type const pos, size_type const count ) const
    {
        if ( pos >= size_ || pos + count > size_ )
        {
            throw std::out_of_range( "array_view2d::slice()" ); 
        }
        return array_view2d( begin() + pos, begin() + pos + count );
    }

    array_view2d slice_before( check_bound_t, size_type const pos ) const
    {
        if ( pos >= size_ )
        {
            throw std::out_of_range( "array_view2d::slice()" ); 
        }
        return array_view2d( begin(), begin() + pos );
    }

    array_view2d slice_from( check_bound_t, size_type const pos ) const
    {
        if ( pos >= size_ )
        {
            throw std::out_of_range( "array_view2d::slice()" ); 
        }
        return array_view2d( begin() + pos, end() );
    }

    //
    // slices, iterator optionally checked via av_EXPECT:
    //
    
    array_view2d slice( iterator const first, iterator const last ) const
    {
        av_EXPECT( valid( first, last ), std::out_of_range, "array_view2d::slice()" );

        return array_view2d( first, last );
    }

    array_view2d slice_before( iterator const pos ) const
    {
        av_EXPECT( pos >= begin() && pos <= end(), std::out_of_range, "array_view2d::slice()" );

        return array_view2d( begin(), pos );
    }

    array_view2d slice_from( iterator const pos ) const
    {
        av_EXPECT( pos >= begin() && pos <= end(), std::out_of_range, "array_view2d::slice()" );

        return array_view2d( pos, end() );
    }

    //
    // slices, iterator bound checked:
    //
    
    array_view2d slice( check_bound_t, iterator const first, iterator const last ) const
    {
        if ( ! valid( first, last ) )
        {
            throw std::out_of_range( "array_view2d::slice()" ); 
        }
        return array_view2d( first, last );
    }

    array_view2d slice_before( check_bound_t, iterator const pos ) const
    {
        if ( pos < begin() || pos > end() )
        {
            throw std::out_of_range( "array_view2d::slice()" ); 
        }
        return array_view2d( begin(), pos );
    }

    array_view2d slice_from( check_bound_t, iterator const pos ) const
    {
        if ( pos < begin() || pos > end() )
        {
            throw std::out_of_range( "array_view2d::slice()" ); 
        }
        return array_view2d( pos, end() );
    }

    //
    // rows, index optionally checked via av_EXPECT:
    //
    
    array_view2d row( size_type const n ) const
    {
        av_EXPECT( n < rows_ , std::out_of_range, "array_view2d::row()" );

        return array_view2d( data_ + n * row_size(), row_size() );
    }

    //
    // rows, index bound checked:
    //
    
    array_view2d row( check_bound_t, size_type const n ) const
    {
        if ( n >= rows_ )
        {
            throw std::out_of_range( "array_view2d::row()" ); 
        }
        return array_view2d( data_ + n * row_size(), row_size() );
    }

private:
    bool valid( iterator const first, iterator const last ) const
    {
        return first <  end() 
            &&  last <= end() 
            && first <= last 
            && static_cast< size_t >(std::distance( first, last > end() ? end() : last ) ) <= size_ - std::distance( begin(), first );
    }
        
private:
    const_pointer data_;
    const size_type size_;
    const size_type rows_;
};

//
// comparison:
//

namespace av {

#if av_CPP11_OR_GREATER || av_COMPILER_IS_MSVC12 || av_COMPILER_IS_MSVC11 || av_COMPILER_IS_MSVC10

using std::begin;

#else

template< typename T > 
inline T *  
begin( T * a ) 
{ 
    return a; 
}

template< typename T, typename A > 
inline typename std::vector<T,A>::const_iterator 
begin( std::vector<T,A> const & vec ) 
{ 
    return vec.begin(); 
}

#endif // av_CPP11_OR_GREATER

} // namespace av

#if av_COMPILER_IS_MSVC6

namespace av {

struct lesser_match {};
struct better_match : lesser_match {};
 
template< typename T >
inline bool equal( array_view2d<T> const & lhs, array_view2d<T> const & rhs, av::better_match const & )
{
    return std::equal( lhs.begin(), lhs.end(), rhs.begin() );
}

template< typename T, typename C >
inline bool equal( array_view2d<T> const & lhs, C const & rhs, av::lesser_match const & )
{
    return std::equal( lhs.begin(), lhs.end(), av::begin( rhs ) );
}

template< typename T, typename C >
inline bool equal( C const & lhs, array_view2d<T> const & rhs, av::lesser_match const & )
{
    return std::equal( rhs.begin(), rhs.end(), av::begin( lhs ) );
}

} // namespace av

template< typename C1, typename C2 >
inline bool operator==( C1 const & lhs, C2 const & rhs )
{
    return av::equal( lhs, rhs, av::better_match() );
}

template< typename C1, typename C2 >
inline bool operator!=( C1 const & lhs, C2 const & rhs )
{
    return !( lhs == rhs );
}

#else //av_COMPILER_IS_MSVC6

template< typename T >
inline bool operator==( array_view2d<T> const & lhs, array_view2d<T> const & rhs )
{
    return std::equal( lhs.begin(), lhs.end(), rhs.begin() );
}

template< typename T, typename C >
inline bool operator==( array_view2d<T> const & lhs, C const & rhs )
{
    return std::equal( lhs.begin(), lhs.end(), av::begin( rhs ) );
}

template< typename T, typename C >
inline bool operator==( C const & lhs, array_view2d<T> const & rhs )
{
    return rhs == lhs;
}

template< typename T >
inline bool operator!=( array_view2d<T> const & lhs, array_view2d<T> const & rhs )
{
    return !( lhs == rhs );
}

template< typename T, typename C >
inline bool operator!=( array_view2d<T> const & lhs, C const & rhs )
{
    return !( lhs == rhs );
}

template< typename T, typename C >
inline bool operator!=( C const & lhs, array_view2d<T> const & rhs )
{
    return !( lhs == rhs );
}

#endif // av_COMPILER_IS_MSVC6

//
// make view:
//

template< typename T >
inline array_view2d<T> 
make_view2d( T const * p, typename array_view2d<T>::size_type const n, typename array_view2d<T>::size_type const rows )
{
    return array_view2d<T>( p, n, rows );
}

#if ! av_COMPILER_IS_MSVC6

template< typename T, size_t N >
inline array_view2d<T> 
make_view2d( T const (&a)[N], typename array_view2d<T>::size_type const rows )
{
    return array_view2d<T>( a, rows );
}
#endif

#if av_COMPILER_IS_MSVC6

template< typename T >
inline array_view2d<T> 
make_view2d( T const * first, T const * last, typename array_view2d<T>::size_type const rows )
{
    return array_view2d<T>( first, last );
}

#else

template< typename II >
inline array_view2d< typename std::iterator_traits< II >::value_type > 
make_view2d( II first, II last, typename array_view2d< typename std::iterator_traits< II >::value_type >::size_type const rows )
{
    return array_view2d< typename std::iterator_traits< II >::value_type >( first, last );
}
#endif // av_COMPILER_IS_MSVC6

#if av_CPP11_OR_GREATER || av_COMPILER_IS_MSVC12

template< typename T >
inline array_view2d<T> 
make_view2d( std::initializer_list<T> const & init, typename array_view2d<T>::size_type const rows )
{
    return array_view2d<T>( init, rows );
}
#endif

#if av_CPP11_OR_GREATER || av_COMPILER_IS_MSVC12 || av_COMPILER_IS_MSVC11 || av_COMPILER_IS_MSVC10

template< typename T, std::size_t N >
inline array_view2d<T> 
make_view2d( std::array<T,N> const & arr, typename array_view2d<T>::size_type const rows )
{
    return array_view2d<T>( arr, rows );
}
#endif

template< typename T >
inline array_view2d<T> 
make_view2d( std::vector<T> const & vec, typename array_view2d<T>::size_type const rows )
{
    return array_view2d<T>( vec, rows );
}

//
// conversion:
//

/**
 * to vector with specified allocator.
 */
template< typename T, class Allocator >
inline std::vector< T, Allocator > 
to_vector( array_view2d<T> const & av, Allocator const & alloc )
{
    return std::vector< T, Allocator >( av.begin(), av.end(), alloc );
}

/**
 * to vector with standard allocator.
 */
template< typename T >
inline std::vector<T, std::allocator<T> > 
to_vector( array_view2d<T> const & av )
{
    return std::vector< T, std::allocator<T> >( av.begin(), av.end(), std::allocator<T>() );
}

} // namespace nonstd

#endif // NONSTD_ARRAY_VIEW2D_HPP_INCLUDED

// End of file
