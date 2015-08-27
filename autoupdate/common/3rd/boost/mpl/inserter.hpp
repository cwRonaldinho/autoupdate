
#ifndef BOOST_MPL_INSERTER_HPP_INCLUDED
#define BOOST_MPL_INSERTER_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /repo/3rd/boost/mpl/inserter.hpp,v $
// $Date: 2010/04/29 03:06:03 $
// $Revision: 1.1.1.1 $

namespace boost { namespace mpl {

template<
      typename Sequence
    , typename Operation
    >
struct inserter
{
    typedef Sequence    state;
    typedef Operation   operation;
};

}}

#endif // BOOST_MPL_INSERTER_HPP_INCLUDED
