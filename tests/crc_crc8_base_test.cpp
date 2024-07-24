/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 11.02.2024
*/

#include <xitren/crc8.hpp>

#include <gtest/gtest.h>

using namespace xitren::crc;

TEST(crc8_test, calc_match)
{
    std::array<std::uint8_t, 5> arr{1, 2, 3, 4, 5};
    EXPECT_EQ(crc8::calculate(arr.begin(), arr.end()), 0xbc);
}

TEST(crc8_test, stm32_id)
{
    std::array<std::uint8_t, 12> id{0x75, 0x37, 0x53, 0x46, 0x4a, 0x35, 0x47, 0x16, 0x56, 0x54, 0x38, 0x42};
    EXPECT_EQ(crc8::calculate(id.begin(), id.end()), 0x80);
}
