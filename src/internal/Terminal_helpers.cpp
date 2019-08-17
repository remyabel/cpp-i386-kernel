#include <internal/Terminal_helpers.hpp>

namespace vga {
namespace internal {
color make_color(color fg, color bg) {
    return static_cast<color>(static_cast<uint8_t>(fg) |
                              (static_cast<uint8_t>(bg) << 4));
}

colored_char make_colored_char(char code_byte, color attribute) {
    return {code_byte, attribute};
}
} // namespace internal
} // namespace vga
