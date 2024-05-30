#pragma once

#include "constants.hpp"

//#ifdef LIB_FAST_WINDOWS_BUILD
//#include "./arch/win.hpp"
//#else
//#include "./arch/unix.hpp"
//#endif

#include <fast/arch/loader.hpp>

namespace fast {
   class loader : public loader_impl{
      public:
         loader() = default;
         ~loader() = default;
         inline auto size() const { return loader_impl::get_size(); }
         constexpr inline auto view() const { return loader_impl::get_view(); }
         constexpr inline auto mapping() const { return loader_impl::get_mapping(); }
         constexpr inline auto file() const { return loader_impl::get_file(); }
         inline std::wstring file_name() { return loader_impl::get_file_name(); }
         inline void load(std::string_view fn) { return loader_impl::load(fn); }
         inline void unload() { return loader_impl::unload(); }
         constexpr inline auto data() const { return (uint8_t*)get_view(); }
         constexpr inline auto data() { return (uint8_t*)get_view(); }
         constexpr inline auto begin() const { return data(); }
         constexpr inline auto begin() { return data(); }
         inline auto end() const { return data() + get_size(); }
         inline auto end() { return data() + get_size(); }
   };
}