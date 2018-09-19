// Copyright 2018 Glen Joseph Fernandes
// (glenjofe@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MULTI_ARRAY_ALLOCATORS_HPP
#define BOOST_MULTI_ARRAY_ALLOCATORS_HPP

#include <boost/config.hpp>

#if defined(BOOST_NO_CXX11_ALLOCATOR)
#include <new>
#else
#include <memory>
#endif

namespace boost { namespace detail { namespace multi_array {

    template <typename A, typename T>
    inline void destroy(A& allocator, T* ptr, T* end)
    {
        for (; ptr != end; ++ptr)
        {
#if defined(BOOST_NO_CXX11_ALLOCATOR)
            ptr->~T();
#else
            std::allocator_traits<A>::destroy(allocator, ptr);
#endif
        }
    }

    template <typename A, typename T>
    inline void construct(A& allocator, T* ptr)
    {
#if defined(BOOST_NO_CXX11_ALLOCATOR)
        ::new(static_cast<void*>(ptr)) T();
#else
        std::allocator_traits<A>::construct(allocator, ptr);
#endif
    }

#if defined(BOOST_NO_EXCEPTIONS)
    template <typename A, typename T>
    inline void construct(A& allocator, T* ptr, T* end)
    {
        for (; ptr != end; ++ptr)
        {
            boost::detail::multi_array::construct(allocator, ptr);
        }
    }
#else
    template <typename A, typename T>
    inline void construct(A& allocator, T* ptr, T* end)
    {
        T* start = ptr;

        try
        {
            for (; ptr != end; ++ptr)
            {
                boost::detail::multi_array::construct(allocator, ptr);
            }
        }
        catch (...)
        {
            boost::detail::multi_array::destroy(allocator, start, ptr);
            throw;
        }
    }
#endif // BOOST_NO_EXCEPTIONS
}}} // namespace boost::detail::multi_array

#endif // BOOST_MULTI_ARRAY_ALLOCATORS_HPP

