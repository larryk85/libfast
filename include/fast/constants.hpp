#pragma once

#include <cstdint>

namespace fast::cons {
   /**
    * Enumeration for different build types.
    */
   enum class build : uint8_t {
      windows, /**< Windows build */
      unix,    /**< Unix build */
      apple,   /**< Apple build */
      other    /**< Other build */
   };

   /**
    * Enumeration for different endianness types.
    */
   enum class endian : uint8_t {
      little, /**< Little endian */
      big     /**< Big endian */
   };

   /**
    * Enumeration for different architecture types.
    */
   enum class arch : uint8_t {
      x86,    /**< x86 architecture */
      x64,    /**< x64 architecture */
      arm,    /**< ARM architecture */
      arm64,  /**< ARM64 architecture */
      other   /**< Other architecture */
   };

   /**
    * Determines the build type based on the platform.
    */
#if defined(_WIN32) || defined(_WIN64)
   constexpr inline static build build_type = build::windows;
   #define LIB_FAST_WINDOWS_BUILD
#elif defined(__unix__)
   constexpr inline static build build_type = build::unix;
   #define LIB_FAST_UNIX_BUILD
#elif defined(__APPLE__)
   constexpr inline static build build_type = build::apple;
   #define LIB_FAST_APPLE_BUILD
#else
   constexpr inline static build build_type = build::other;
   #define LIB_FAST_OTHER_BUILD
#endif

   /**
    * Determines the architecture type based on the platform.
    */
#if defined(__x86_64__) || defined(_M_X64)
   constexpr inline static arch arch_type = arch::x64;
   #define LIB_FAST_X64_ARCH
#elif defined(__i386__) || defined(_M_IX86)
   constexpr inline static arch arch_type = arch::x86;
   #define LIB_FAST_X86_ARCH
#elif defined(__aarch64__)
   constexpr inline static arch arch_type = arch::arm64;
   #define LIB_FAST_ARM64_ARCH
#elif defined(__arm__)
   constexpr inline static arch arch_type = arch::arm;
   #define LIB_FAST_ARM_ARCH
#else
   constexpr inline static arch arch_type = arch::other;
   #define LIB_FAST_OTHER_ARCH
#endif

   /**
    * Determines the endianness for the platform.
    */
#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN || defined(__LITTLE_ENDIAN__) || __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
   constexpr inline static endian registry_endian = endian::little;
   #define LIB_FAST_LITTLE_ENDIAN
#elif REG_DWORD == REG_DWORD_BIG_ENDIAN || defined(__BIG_ENDIAN__) || __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
   constexpr inline static endian registry_endian = endian::big;
   #define LIB_FAST_BIG_ENDIAN
#else
   error "Unknown registry endian type"
#endif
   
   /**
    * Structure that holds information about the build, architecture, and endianness.
    */
   struct build_info {
      build build_type;         /**< The build type */
      arch arch_type;           /**< The architecture type */
      endian registry_endian;   /**< The endianness of the registry */
   };

   /**
    * The build information.
    */
   constexpr inline static build_info info = {build_type, arch_type, registry_endian};

   /**
    * Boolean flag indicating if the build type is Windows.
    */
   constexpr inline static bool is_windows = build_type == build::windows;

   /**
    * Boolean flag indicating if the build type is Unix.
    */
   constexpr inline static bool is_unix = build_type == build::unix;

   /**
    * Boolean flag indicating if the build type is Apple.
    */
   constexpr inline static bool is_apple = build_type == build::apple;

   /**
    * Boolean flag indicating if the build type is other.
    */
   constexpr inline static bool is_other = build_type == build::other;

   /**
    * Boolean flag indicating if the architecture type is x86.
    */
   constexpr inline static bool is_x86 = arch_type == arch::x86;

   /**
    * Boolean flag indicating if the architecture type is x64.
    */
   constexpr inline static bool is_x64 = arch_type == arch::x64;

   /**
    * Boolean flag indicating if the architecture type is ARM.
    */
   constexpr inline static bool is_arm = arch_type == arch::arm;

   /**
    * Boolean flag indicating if the architecture type is ARM64.
    */
   constexpr inline static bool is_arm64 = arch_type == arch::arm64;

   /**
    * Boolean flag indicating if the architecture type is other.
    */
   constexpr inline static bool is_other_arch = arch_type == arch::other;

   /**
    * Boolean flag indicating if the registry endianness is little endian.
    */
   constexpr inline static bool is_little_endian = registry_endian == endian::little;

   /**
    * Boolean flag indicating if the registry endianness is big endian.
    */
   constexpr inline static bool is_big_endian = registry_endian == endian::big;

} // namespace fast::cons