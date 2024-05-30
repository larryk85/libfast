#include <string>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <fstream>
#include <fast/fast.hpp>
#include <fast/sha2.hpp>
#include <fast/sha256.hpp>

using namespace fast;

std::string to_hex(const std::vector<uint8_t>& data) {
   std::string hex_str;
   for (const auto& byte : data) {
      char hex_byte[3];
      sprintf(hex_byte, "%02X", byte);
      hex_str += hex_byte;
   }
   return hex_str;
}

TEST_CASE("Test", "[test_tests]") {
   //REQUIRE( true == true );

   auto li = loader();
   li.load("./al.safetensors");


   //std::cout << "size: " << li.get_size() << std::endl;
   //std::vector<uint8_t> data(32);
   //picosha2::hash256(li.begin(), li.end(), data.begin(), data.end());
   //std::cout << "SHA-256: " << to_hex(data) << std::endl;
   sha256_t sha;
   std::string str = "Hello, World!";
   sha.update((const uint8_t*)str.c_str(), str.size());
   auto digest = sha.finalize();
   std::cout << "SHA-256: " << digest << std::endl;

   std::vector<uint8_t> data(32);
   picosha2::hash256(str.begin(), str.end(), data.begin(), data.end());
   std::cout << "Sha-256: " << to_hex(data) << std::endl;
}