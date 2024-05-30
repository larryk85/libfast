cmake_minimum_required(VERSION 3.14)

project(libfast VERSION 1.0.0.0)

message( STATUS "Building libfast v${libfast_VERSION}..." )

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads)
link_libraries(Threads::Threads)

include(FetchContent)

set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(
   cturtle
	GIT_REPOSITORY https://github.com/larryk85/cturtle
	GIT_TAG main
)
FetchContent_MakeAvailable(cturtle)

include(CMakeDependentOption)
option(LIBFAST_ENABLE_INSTALL "enable installation" ON)
option(LIBFAST_ENABLE_TESTS "enable building of unit tests" ON)

if (MSVC)
   if (CMAKE_SIZEOF_VOID_P EQUAL 8)
      add_compile_definitions(_AMD64_)
   else()
      add_compile_definitions(_X86_)
   endif()
endif()

add_subdirectory(external)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/fast/arch/loader/${CMAKE_SYSTEM_NAME}.hpp
               ${CMAKE_CURRENT_BINARY_DIR}/include/fast/arch/loader.hpp @ONLY)

add_library(libfast INTERFACE)
target_include_directories(libfast INTERFACE
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
   $<INSTALL_INTERFACE:include>)
target_link_libraries(libfast INTERFACE cturtle)
add_library(fast::fast ALIAS libfast)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(LIBFAST_ENABLE_TESTS)
   FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2
      GIT_TAG v3.6.0
   )
   
   FetchContent_GetProperties(Catch2)
   if (NOT Catch2_POPULATED)
      FetchContent_MakeAvailable(Catch2)
      list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/contrib") 
   endif()

   include(CTest)
   include(Catch)

   enable_testing()
   add_subdirectory(tests)
endif()

# ##################################################################################################
# Configure version info.
# ##################################################################################################
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/modules/libfast-config-version.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/libfast-config-version.cmake @ONLY)

# ##################################################################################################
# Installation.
# ##################################################################################################
if(LIBFAST_ENABLE_INSTALL)
   include(CMakePackageConfigHelpers)
   include(GNUInstallDirs)

   message(STATUS "Installing libfast ...")
   set(libfast_INCLUDE_DIRS ${CMAKE_INSTALL_INCLUDEDIR})
   configure_package_config_file(modules/libfast-config.cmake.in
      ${CMAKE_CURRENT_BINARY_DIR}/libfast-config.cmake
      INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libfast
      PATH_VARS libfast_INCLUDE_DIRS
   )
   write_basic_package_version_file(
      ${CMAKE_CURRENT_BINARY_DIR}/libfast-config-version.cmake
      VERSION ${libfast_VERSION}
      COMPATIBILITY SameMajorVersion
   )

   install(TARGETS libfast
           EXPORT libfast-targets
           LIBRARY
           PUBLIC_HEADER
              DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
              COMPONENT Headers
   )
   install(EXPORT libfast-targets
      NAMESPACE fast::
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libfast
   )
   install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/fast
           DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
   )
   install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libfast-config.cmake
                 ${CMAKE_CURRENT_BINARY_DIR}/libfast-config-version.cmake
                 DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libfast
   )
endif()