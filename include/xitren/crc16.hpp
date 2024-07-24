/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 11.02.2024
*/
#pragma once

#include <xitren/crc_concept.hpp>

namespace xitren::crc {

/*!
\brief CRC16

The CRC-16 is a cyclic redundancy check (CRC) algorithm that is commonly used in computer hardware and software for
error-detection purposes. It is a 16-bit algorithm, which means that it generates a 16-bit output from a given input
data stream. The algorithm is designed such that it can detect up to 2^16-1 errors in the input data.

The CRC-16 algorithm is specified in various standards and specifications, including the ISO/IEC 13239:1992 and ANSI
X3.28-1988. The algorithm uses a polynomial of x^16 + x^15 + x^2 + 1, which is reflected over the bit-reversed input
data. The resulting output is then masked with 0x1021 to obtain the final 16-bit output value.

The CRC-16 algorithm is widely used in various applications, such as:

- Data transmission over serial links, such as RS-232, RS-422, and RS-485
- Data storage and retrieval, such as on hard drives and optical discs
- Wireless communication protocols, such as Bluetooth and Wi-Fi
- Mobile phone communication protocols, such as GSM and CDMA
- Networking protocols, such as IP and Ethernet

In this implementation, the `crc16` class provides two static member functions for calculating the CRC-16 value. The
`calculate` function takes an input iterator range as input and calculates the CRC-16 value for the given data. The
`calculate` function can be used to calculate the CRC-16 value for a single byte, an array of bytes, or a string.

The `crc16::prepare_table` function is a helper function that calculates the CRC-16 lookup table used for faster
computation. The lookup table is calculated based on the given polynomial value.

Overall, the `crc16` class provides an efficient and flexible implementation of the CRC-16 algorithm in C++. It can be
used in various applications that require error detection and correction in data streams.
*/
class crc16 {
    static constexpr uint16_t polynome = 0xA001;
    /*!
     * \brief prepare_table
     *
     * Calculate table for speed up crc computation
     *
     * \param poly Polynome initial value (usually equals polynome = 0xA001)
     * \return crc table in array
     */
    static constexpr std::array<std::uint16_t, 256>
    prepare_table(uint16_t const poly)
    {
        std::uint16_t const            table[2] = {0x0000, poly};
        std::array<std::uint16_t, 256> array{};
        int                            i = 0;
        for (std::uint16_t& item : array) {
            std::uint16_t crc_mb = i++;
            for (char bit = 0; bit < 8; bit++) {
                std::uint8_t xort = crc_mb & 0x01;
                crc_mb >>= 1;
                crc_mb ^= table[xort];
            }
            item = crc_mb;
        }
        return array;
    }

public:
    using value_type = std::uint16_t;

    /*!
     * \brief Calculate crc on data by input iterator
     *
     * \param[in] begin Begin iterator of byte data
     * \param[in] end End iterator of byte data
     * \return crc value
     */
    template <crc_iterator InputIterator>
    static constexpr value_type
    calculate(InputIterator begin, InputIterator end)
    {
        static_assert(sizeof(*begin) == 1);
        constexpr auto crc16_tbl = crc16::prepare_table(polynome);
        value_type     crc_mb{0xFFFF};
        for (std::input_iterator auto byte = begin; byte < end; byte++)
            crc_mb = (crc_mb >> 8) ^ crc16_tbl[(crc_mb & 0xFF) ^ (*byte)];
        return crc_mb;
    }

    /*!
     * \brief Calculate crc on data array
     *
     * \param[in] data Array
     * \return crc value
     */
    template <std::size_t Size>
    static consteval value_type
    calculate(std::array<std::uint8_t, Size> const& data) noexcept
    {
        auto       crc16ansi_tbl = crc16::prepare_table(polynome);
        value_type crc_mb{0xFFFF};
        for (auto& byte : data)
            crc_mb = (crc_mb >> 8) ^ crc16ansi_tbl[(crc_mb & 0xFF) ^ (byte)];
        return crc_mb;
    }
};
}    // namespace xitren::crc
