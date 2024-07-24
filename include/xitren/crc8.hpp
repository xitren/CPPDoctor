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
\brief CRC8

The CRC-8-CCITT algorithm is a polynomial-based cyclic redundancy check (CRC) algorithm that generates a 8-bit checksum
used to detect errors in data transmission. It is a common standard used in many industries, including
telecommunications and computing.

This class provides an implementation of the CRC-8-CCITT algorithm, including methods for calculating the checksum of a
data stream and verifying the integrity of data.

\section Usage Example Usage Example

Here is an example of how to use the CRC-8-CCITT algorithm in C++:

```c++
#include "crc8.hpp"

#include <iostream>

int main() {
    // create a CRC-8 object
    xitren::crc::crc8 crc;

    // calculate the checksum of a data stream
    std::uint8_t data[] = {0x12, 0x34, 0x56};
    std::uint8_t checksum = crc.calculate(data, data + 3);

    // output the checksum
    std::cout << "Checksum: " << static_cast<int>(checksum) << std::endl;

    return 0;
}
*/
class crc8 {
    static constexpr uint16_t polynome = 0x07;
    /*!
     * \brief Calculate table for speed up crc computation
     *
     * This function calculates the CRC table for the given polynomial. The table is used to speed up the computation of
     * the CRC.
     *
     * \param[in] poly Polynome initial value (usually equals polynome = 0x07)
     * \return crc table in array
     */
    static constexpr auto
    prepare_table(std::uint8_t const poly)
    {
        std::uint8_t const            table[2] = {0x0U, poly};
        std::array<std::uint8_t, 256> array{};
        std::uint16_t                 i = 0;
        for (std::uint8_t& item : array) {
            auto crc_mb = static_cast<std::uint8_t>(i++);
            for (char bit = 0; bit < 8; bit++) {
                std::uint8_t xort = (crc_mb & 0x80) >> 7;
                crc_mb <<= 1;
                crc_mb ^= table[xort];
            }
            item = crc_mb;
        }
        return array;
    }

public:
    using value_type = std::uint8_t;

    /*!
     * \brief Calculate crc on data by input iterator
     *
     * This function calculates the CRC value of a data stream using the CRC-8-CCITT algorithm. The data stream is
     * specified by an input iterator that points to the beginning of the data and an end iterator that points to the
     * end of the data.
     *
     * \tparam InputIterator Type of the input iterator. It must be a random access iterator that can access elements of
     * type std::uint8_t. \param[in] begin Begin iterator of byte data \param[in] end End iterator of byte data \return
     * crc value
     */
    template <crc_iterator InputIterator>
    static constexpr value_type
    calculate(InputIterator begin, InputIterator end)
    {
        static_assert(sizeof(*begin) == sizeof(std::uint8_t));
        // prepare the crc table
        constexpr auto crc8_tbl = crc8::prepare_table(polynome);
        // initialize the crc value
        value_type crc_mb{0x0U};
        // iterate over the data
        for (std::input_iterator auto byte = begin; byte != end; byte++) {
            // calculate the next crc value
            crc_mb = crc8_tbl[crc_mb ^ (*byte)];
        }
        // return the calculated crc value
        return crc_mb;
    }

    /*!
     * \brief Calculate crc on data array
     *
     * This function calculates the CRC value of a data stream using the CRC-8-CCITT algorithm. The data stream is
     * specified by an input iterator that points to the beginning of the data and an end iterator that points to the
     * end of the data.
     *
     * \tparam InputIterator Type of the input iterator. It must be a random access iterator that can access elements of
     * type std::uint8_t. \param[in] begin Begin iterator of byte data \param[in] end End iterator of byte data \return
     * crc value
     */
    template <std::size_t Size>
    static consteval value_type
    calculate(std::array<std::uint8_t, Size> const& data) noexcept
    {
        constexpr auto crc8_tbl = crc8::prepare_table(polynome);
        value_type     crc_mb{0x0U};
        for (auto& byte : data) {
            crc_mb = crc8_tbl[crc_mb ^ (byte)];
        }
        return crc_mb;
    }
};

}    // namespace xitren::crc
