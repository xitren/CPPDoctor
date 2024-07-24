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
\brief CRC XOR
*/
class crc_xor {
public:
    using value_type = std::uint8_t;

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
        static_assert(sizeof(*begin) == sizeof(std::uint8_t));
        value_type crc_mb{0xFF};
        for (std::input_iterator auto byte = begin; byte != end; byte++)
            crc_mb = crc_mb ^ (*byte);
        return crc_mb;
    }

    /*!
    \brief Calculates the CRC value of an array of bytes using the XOR method.
    \param[in] data The array of bytes to calculate the CRC value for.
    \return crc The CRC value of the array of bytes.
    */
    template <std::size_t Size>
    static consteval value_type
    calculate(std::array<std::uint8_t, Size> const& data) noexcept
    {
        value_type crc_mb{0xFF};
        for (auto& byte : data) {
            crc_mb = crc_mb ^ (byte);
        }
        return crc_mb;
    }
};

}    // namespace xitren::crc
