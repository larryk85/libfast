#pragma once

#include <cstdint>

#include <array>
#include <format>
#include <memory>
#include <vector>
#include <type_traits>
#include <sstream>

#include "utils.hpp"

namespace fast {
   namespace detail {
      constexpr inline static uint32_t sha224_h[8] = {
         0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
         0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
      };

      constexpr inline static uint32_t sha256_h[8] = {
         0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
         0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
      };

      constexpr inline static uint32_t sha256_k[64] = {
         0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
         0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
         0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
         0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
         0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
         0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
         0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
         0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
         0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
         0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
         0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
         0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
         0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
         0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
         0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
         0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
      };

      template <typename T, std::size_t N>
      struct aligned_block {
         constexpr inline static std::size_t length = N / sizeof(T);
         std::aligned_storage_t<sizeof(T), alignof(T)> data[length];
      };

      template <typename T>
      using block_16_t = aligned_block<T, 16>;
      
      template <typename T>
      using block_32_t = aligned_block<T, 32>;

      template <typename T>
      using block_64_t = aligned_block<T, 64>;
   } // namespace detail

   template <size_t N, typename W=uint64_t>
   struct digest {
      consteval std::size_t size() const noexcept { return N / sizeof(W); }
      constexpr std::string_view to_string_view() const noexcept { return std::string_view(reinterpret_cast<const char*>(data.data()), size()); }
      constexpr std::string to_hex() const noexcept {
         std::string result;
         for (std::size_t i = 0; i < size(); i++) {
            result += std::format("{:02x}", data[i]);
         }
         return result;
      }

      detail::aligned_block<W, N/sizeof(W)> data;
   };

   template <std::size_t N, auto H, auto K>
   class sha2 {
      public:
         static constexpr inline std::size_t block_size = 64;
         static constexpr inline std::size_t digest_size = N / 8;
         static constexpr inline std::size_t num_rounds = 64;

         sha2() = default;
         ~sha2() = default;

         void update(const uint8_t* data, std::size_t size) {
            total_len += size;
            std::array<uint8_t, block_size> block = {};

            for (std::size_t i = 0; i < size; i++) {
               block[bit_len / 8 % block_size] = data[i];
               bit_len += 8;
               if (bit_len % (block_size * 8) == 0) {
                  transform(block);
                  block.fill(0);
               }
            }
         }

         constexpr 

         void transform(const std::array<uint8_t, block_size>& block) {
            for (std::size_t i=0; i < 16; i++) {
               w[i] = to_big_endian<uint32_t>(*(uint32_t*)(&block[i*4]));
            }

            for (std::size_t i=16; i < 64; i++) {
               auto s0 = std::rotr(w[i-15], 7) ^ std::rotr(w[i-15], 18) ^ (w[i-15] >> 3);
               auto s1 = std::rotr(w[i-2], 17) ^ std::rotr(w[i-2], 19) ^ (w[i-2] >> 10);
               w[i] = w[i-16] + s0 + w[i-7] + s1;
            }

            auto [a, b, c, d, e, f, g, h] = hash;

            for (std::size_t i=0; i < 64; i++) {
               auto s1 = std::rotr(e, 6) ^ std::rotr(e, 11) ^ std::rotr(e, 25);
               auto ch = (e & f) ^ (~e & g);
               auto temp1 = h + s1 + ch + K[i] + w[i];
               auto s0 = std::rotr(a, 2) ^ std::rotr(a, 13) ^ std::rotr(a, 22);
               auto maj = (a & b) ^ (a & c) ^ (b & c);
               auto temp2 = s0 + maj;

               h = g;
               g = f;
               f = e;
               e = d + temp1;
               d = c;
               c = b;
               b = a;
               a = temp1 + temp2;
            }

            hash[0] += a;
            hash[1] += b;
            hash[2] += c;
            hash[3] += d;
            hash[4] += e;
            hash[5] += f;
            hash[6] += g;
            hash[7] += h;
         }

         std::string finalize() {
            std::array<uint8_t, block_size> block = {};
            std::size_t index = bit_len / 8 % block_size;
            block[index++] = 0x80;
            if (index > 56) {
               std::fill(block.begin() + index, block.end(), 0);
               transform(block);
               block.fill(0);
            }

            bit_len += total_len * 8;
            for (std::size_t i = 0; i < 8; i++) {
               block[63 - i] = bit_len >> (i * 8);
            }

            transform(block);

            std::ostringstream res;
            for (const auto &b : hash) {
               res << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(b);
            }
            return res.str();
         }

      private:
         constexpr static inline uint32_t ch(uint32_t x, uint32_t y, uint32_t z) noexcept {
            return (x & y) ^ (~x & z);
         }
         constexpr static inline uint32_t ma(uint32_t x, uint32_t y, uint32_t z) noexcept {
            return (x & y) ^ (x & z) ^ (y & z);
         }
         constexpr static inline uint32_t sig0(uint32_t x) noexcept {
            return std::rotr(x, 2) ^ std::rotr(x, 13) ^ std::rotr(x, 22);
         }
         constexpr static inline uint32_t sig1(uint32_t x) noexcept {
            return std::rotr(x, 6) ^ std::rotr(x, 11) ^ std::rotr(x, 25);
         }
         std::array<uint32_t, 8> hash = {H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]};
         std::array<uint32_t, 64> w = {};
         uint64_t bit_len = 0;
         uint64_t total_len = 0;
   };

   using sha224_t = sha2<224, detail::sha224_h, detail::sha256_k>;
   using sha256_t = sha2<256, detail::sha256_h, detail::sha256_k>;

} // namespace fast