
#include <cassert>
#include <cstdint>
#include <iostream>
#include <optional>

class Bitstream {
  public:
    void putFlag(bool /*flag*/) {}
    template <typename T>
    void writeBits(std::size_t /*number_of_bits*/) {}

    bool getFlag() { return true; }
    template <typename T>
    T readBits(std::size_t /*number_of_bits*/) {
        return {};
    }
};

class Message {
  public:
    [[nodiscard]] constexpr auto my_int() const noexcept -> std::int32_t { return m_my_int; }
    [[nodiscard]] constexpr auto my_flag() const noexcept -> bool { return m_flag_for_optional; }
    [[nodiscard]] constexpr auto my_optional() const -> std::uint8_t {
        assert(my_flag() && m_optional_uint.has_value());
        return m_optional_uint.value();
    }

    void my_int(std::int32_t i) { m_my_int = i; }
    void my_flag(bool flag) { m_flag_for_optional = flag; }
    void my_optional(std::uint8_t val) { m_optional_uint = val; }

    friend std::ostream& operator<<(std::ostream& stream, const Message& msg) {
        stream << "my_int=" << msg.my_int() << "\n";
        stream << "my_flag=" << msg.my_flag() << "\n";
        if (msg.my_flag()) {
            stream << "my_optional=" << static_cast<unsigned>(msg.my_optional()) << "\n";
        }
        return stream;
    }

    static Message decodeFrom(Bitstream& stream) {
        Message result{};
        result.my_int(stream.readBits<std::int32_t>(27));
        result.my_flag(stream.getFlag());
        if (result.my_flag()) {
            result.my_optional(stream.readBits<std::uint8_t>(5));
        }
        return result;
    }

    void encodeTo(Bitstream& stream) const {
        stream.writeBits<std::int32_t>(27);
        stream.putFlag(my_flag());
        if(my_flag()){
            stream.writeBits<std::uint8_t>(5);
        }
    }

    // TODO(cb) callSyntaxOnFunction() or so?
  private:
    std::int32_t m_my_int{};
    bool m_flag_for_optional{};
    std::optional<std::uint8_t> m_optional_uint{};
};

int main() {
    Bitstream stream{};
    Message unit{Message::decodeFrom(stream)};
    unit.encodeTo(stream);
    std::cout << unit;
}
