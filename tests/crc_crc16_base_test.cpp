/*!
     _ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 11.02.2024
*/

#include <xitren/crc16.hpp>

#include <gtest/gtest.h>

using namespace xitren::crc;

TEST(crc16_test, calc_match)
{
    std::array<std::uint8_t, 5> data = {1, 2, 3, 4, 5};
    crc16::value_type           crc(crc16::calculate(data.begin(), data.end()));
    EXPECT_EQ(crc, 47914);
}