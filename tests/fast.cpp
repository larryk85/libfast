#include <string>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <fast/fast.hpp>

using namespace fast;

TEST_CASE("Test", "[test_tests]") {
   std::cout << "hello\n";
   REQUIRE( true == true );

   auto li = loader_impl();

   li.load("./test.file");
}