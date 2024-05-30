#pragma once

#include <cstdint>

#include "constants.hpp"

namespace fast {
   template <typename T>
   constexpr inline static T byteswap(T value) {
      if constexpr (sizeof(T) == 1) {
         return value;
      } else if constexpr (sizeof(T) == 2) {
         if constexpr (cons::is_windows)
            return _byteswap_ushort(value);
         else
            return __builtin_bswap16(value);
      } else if constexpr (sizeof(T) == 4) {
         if constexpr (cons::is_windows)
            return _byteswap_ulong(value);
         else
            return __builtin_bswap32(value);
      } else {
         if constexpr (cons::is_windows)
            return _byteswap_uint64(value);
         else
            return __builtin_bswap64(value);
      }
   }

   template <typename T>
   constexpr inline static T to_little_endian(T value) {
      if constexpr (cons::is_little_endian)
         return value;
      else
         return byteswap(value);
   }

   template <typename T>
   constexpr inline static T to_big_endian(T value) {
      if constexpr (cons::is_big_endian)
         return value;
      else
         return byteswap(value);
   }
} // namespace fast