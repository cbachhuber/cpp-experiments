
#include <cassert>
#include <cstdint>
#include <functional>
#include <iostream>
#include <optional>
#include <type_traits>

constexpr std::size_t bits_int = 27;
constexpr std::size_t bits_optional_unsigned = 5;

template <typename T>
auto putField(std::ostream& stream, const std::string& fieldName, const T& fieldValue) {
    stream << fieldName << "=";
    if constexpr (std::is_same_v<std::uint8_t, std::decay_t<T>>) {
        stream << static_cast<unsigned>(fieldValue) << "\n";
    } else if (std::is_same_v<bool, std::decay_t<T>>) {
        stream << std::boolalpha << fieldValue << "\n";
    } else {
        stream << fieldValue << "\n";
    }
}

class Bitstream {
  public:
    void putFlag(bool /*flag*/) { std::cout << "Called putFlag\n"; }
    template <typename T>
    void writeBits(T /*data*/, std::size_t number_of_bits) {
        std::cout << "Called writeBits with " << number_of_bits << "bits\n";
    }

    bool getFlag() {
        std::cout << "Called getFlag\n";
        return true;
    }
    template <typename T>
    T readBits(std::size_t number_of_bits) {
        std::cout << "Called readBits with " << number_of_bits << "bits\n";
        return {};
    }
};

// Option: this function could take the string fieldName and not use it!
template <typename T>
void writeToStream(Bitstream& stream, const T& value, std::size_t number_of_bits = 0) {
    if constexpr (std::is_same_v<bool, std::decay_t<T>>) {
        stream.putFlag(value);
        // The below two could actually be summarized in an else statement
    } else {
        stream.writeBits<std::decay_t<T>>(value, number_of_bits);
    }
}

// TODO(CB) extend to indexed (multi argument) setters, replace T by Args..., then get last
template <typename T>
void readFromStream(Bitstream& stream, T& member_to_set, std::size_t number_of_bits = 0) {
    if constexpr (std::is_same_v<bool, std::decay_t<T>>) {
        member_to_set = stream.getFlag();
    } else {
        member_to_set = stream.readBits<std::decay_t<T>>(number_of_bits);
    }
}

class Message {
  public:
    [[nodiscard]] constexpr auto my_int() const noexcept -> std::int32_t const& { return m_my_int; }
    [[nodiscard]] constexpr auto my_flag() const noexcept -> bool const& { return m_flag_for_optional; }
    [[nodiscard]] constexpr auto my_optional() const -> std::uint8_t const& {
        assert(my_flag() && m_optional_uint.has_value());
        return m_optional_uint.value();
    }

    auto my_int() -> std::int32_t& { return m_my_int; }
    auto my_flag() -> bool& { return m_flag_for_optional; }
    auto my_optional() -> std::uint8_t& {
        m_optional_uint.emplace(std::uint8_t{});
        return *m_optional_uint;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Message& msg) {
        putField(stream, "my_int", msg.my_int());
        putField(stream, "my_flag", msg.my_flag());
        if (msg.my_flag()) {
            putField(stream, "my_optional", msg.my_optional());
        }
        return stream;
    }

    static Message decodeFrom(Bitstream& stream) {
        Message result{};
        readFromStream(stream, result.my_int(), bits_int);
        readFromStream(stream, result.my_flag());
        if (result.my_flag()) {
            readFromStream(stream, result.my_optional(), bits_optional_unsigned);
        }
        return result;
    }

    void encodeTo(Bitstream& stream) const {
        writeToStream(stream, my_int(), bits_int);
        writeToStream(stream, my_flag());
        if (my_flag()) {
            writeToStream(stream, my_optional(), bits_optional_unsigned);
        }
    }

    // Not void for decodeFrom! But that can be deduced
    //    void callSyntaxOnFunction(stream (bitstream or ostream) /*of function*/) {
    //        // Type stored in return type of getter
    //        function(my_int, 27); // give function handle, not result!
    //        function(my_flag);
    //        if (my_flag()) {
    //            function(my_optional, 5);
    //        }
    //    }

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
