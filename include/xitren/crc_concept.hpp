/*!
_ _
__ _(_) |_ _ _ ___ _ _
\ \ / |  _| '_/ -_) ' \
/_\_\_|\__|_| \___|_||_|
* @date 11.02.2024
*/
#pragma once

#include <array>
#include <concepts>
#include <cstdint>
#include <cstring>

namespace xitren::crc {
/**
 * @brief A concept that checks if a type is a crc type
 *
 * @tparam T the type to check
 * @return true if the type is a crc type
 * @return false if the type is not a crc type
 */
template <typename T>
concept crc_concept = requires {
    typename T::value_type;
    /**
     * @brief Calculates the crc of a given data buffer
     *
     * @param data a pointer to the data buffer
     * @param size the size of the data buffer in bytes
     * @return the calculated crc
     */
    T::calculate(std::array<std::uint8_t, 8>::iterator{nullptr},
                 std::array<std::uint8_t, 8>::iterator{reinterpret_cast<unsigned char*>(10)});
};

template <typename T>
concept crc_iterator = std::input_iterator<T>;

}    // namespace xitren::crc
