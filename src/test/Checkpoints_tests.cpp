// Copyright (c) 2011-2013 The Bitcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Unit tests for block-chain checkpoints
//

#include "checkpoints.h"

#include "uint256.h"

#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(Checkpoints_tests)

BOOST_AUTO_TEST_CASE(sanity)
{
    uint256 p11111 = uint256("0xae23a3406fbce0049505c3cf2d3ca0e384e27e6ccc45988c556fce4e98b70c01");
    uint256 p12345= uint256("0x6a89732f27c4c4d3503fd3c641934c29c924bb2f3459f6e865312e3888fc7b3d");
    BOOST_CHECK(Checkpoints::CheckBlock(11111, p11111));
    BOOST_CHECK(Checkpoints::CheckBlock(12345, p12345));


    // Wrong hashes at checkpoints should fail:
    BOOST_CHECK(!Checkpoints::CheckBlock(11111, p12345));
    BOOST_CHECK(!Checkpoints::CheckBlock(12345, p11111));
// p134444
    // ... but any hash not at a checkpoint should succeed:
    BOOST_CHECK(Checkpoints::CheckBlock(11111+1, p12345));
    BOOST_CHECK(Checkpoints::CheckBlock(12345+1, p12345));

    BOOST_CHECK(Checkpoints::GetTotalBlocksEstimate() >= 12345);
}

BOOST_AUTO_TEST_SUITE_END()
