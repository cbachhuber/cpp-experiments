
#include <cassert>
#include <cstdint>
#include <functional>
#include <iostream>
#include <optional>
#include <type_traits>
#include <vector>

constexpr std::size_t bits_int = 27;
constexpr std::size_t bits_optional_unsigned = 5;
constexpr std::size_t bits_vector_int = 22;

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

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

const auto putField = [](std::ostream& stream, const std::string& fieldName, const auto& fieldValue,
                         std::size_t /*number_of_bits*/ = 0) {
    stream << fieldName << "=";
    if constexpr (std::is_same_v<std::uint8_t, std::decay_t<decltype(fieldValue)>>) {
        stream << static_cast<unsigned>(fieldValue) << "\n";
    } else if (std::is_same_v<bool, std::decay_t<decltype(fieldValue)>>) {
        stream << std::boolalpha << fieldValue << "\n";
    } else {
        stream << fieldValue << "\n";
    }
};

const auto putFieldIndexed = [](std::ostream& stream, std::size_t i, const std::string& fieldName,
                                const auto& fieldValue, std::size_t /*number_of_bits*/ = 0) {
    stream << fieldName << "(" << i << ")=";
    if constexpr (std::is_same_v<std::uint8_t, std::decay_t<decltype(fieldValue)>>) {
        stream << static_cast<unsigned>(fieldValue) << "\n";
    } else if (std::is_same_v<bool, std::decay_t<decltype(fieldValue)>>) {
        stream << std::boolalpha << fieldValue << "\n";
    } else {
        stream << fieldValue << "\n";
    }
};

const auto writeToStream = [](Bitstream& stream, const std::string& /*field_name*/, const auto& value,
                              std::size_t number_of_bits = 0) {
    if constexpr (std::is_same_v<bool, std::decay_t<decltype(value)>>) {
        stream.putFlag(value);
    } else {
        stream.writeBits<std::decay_t<decltype(value)>>(value, number_of_bits);
    }
};

const auto writeToStreamIndexed = [](Bitstream& stream, std::size_t /*i*/, const std::string& /*field_name*/,
                                     const auto& value, std::size_t number_of_bits = 0) {
    writeToStream(stream, "", value, number_of_bits);
};

const auto readFromStream = [](Bitstream& stream, const std::string& /*field_name*/, auto& member_to_set,
                               std::size_t number_of_bits = 0) {
    if constexpr (std::is_same_v<bool, std::decay_t<decltype(member_to_set)>>) {
        member_to_set = stream.getFlag();
    } else {
        member_to_set = stream.readBits<std::decay_t<decltype(member_to_set)>>(number_of_bits);
    }
};

const auto readFromStreamIndexed = [](Bitstream& stream, std::size_t /*i*/, const std::string& /*field_name*/,
                                      auto& member_to_set, std::size_t number_of_bits = 0) {
    readFromStream(stream, "", member_to_set, number_of_bits);
};

class Message {
  public:
    [[nodiscard]] constexpr auto my_int() const noexcept -> std::int32_t const& { return m_my_int; }
    [[nodiscard]] constexpr auto my_flag() const noexcept -> bool const& { return m_flag_for_optional; }
    [[nodiscard]] constexpr auto my_optional() const -> std::uint8_t const& {
        assert(my_flag() && m_optional_uint.has_value());
        return m_optional_uint.value();
    }
    [[nodiscard]] constexpr auto my_vec(std::size_t i) const -> std::int32_t const& {
        assert(i < m_vector.size());
        return m_vector[i];
    }

    auto my_int() -> std::int32_t& { return m_my_int; }
    auto my_flag() -> bool& { return m_flag_for_optional; }
    auto my_optional() -> std::uint8_t& {
        m_optional_uint.emplace(std::uint8_t{});
        return *m_optional_uint;
    }
    auto my_vec(std::size_t i) -> std::int32_t& {
        assert(i < m_vector.size());
        return m_vector[i];
    }

    friend std::ostream& operator<<(std::ostream& stream, const Message& msg) {
        callSyntaxOn(msg, stream, overloaded{putField, putFieldIndexed});
        return stream;
    }

    static Message decodeFrom(Bitstream& stream) {
        Message result{};
        callSyntaxOn(result, stream, overloaded{readFromStream, readFromStreamIndexed});
        return result;
    }

    void encodeTo(Bitstream& stream) const {
        callSyntaxOn(*this, stream, overloaded{writeToStream, writeToStreamIndexed});
    }

    template <typename MessageType, typename StreamType, typename FunctionType>
    static void callSyntaxOn(MessageType&& msg, StreamType&& stream, FunctionType&& func) {
        // TODO(CB) verify that decayed Message type is Message (and reference?)

        func(stream, "my_int", msg.my_int(), bits_int);
        func(stream, "my_flag", msg.my_flag());
        if (msg.my_flag()) {
            func(stream, "my_optional", msg.my_optional(), bits_optional_unsigned);
        }
        for (std::size_t i = 0; i < 2; ++i) {  // Hardcoding, as this is only a demo
            func(stream, i, "my_vec", msg.my_vec(i), bits_vector_int);
        }
    }

  private:
    std::int32_t m_my_int{};
    bool m_flag_for_optional{};
    std::optional<std::uint8_t> m_optional_uint{};
    std::vector<std::int32_t> m_vector{std::vector<std::int32_t>{1, 2}};
};

int main() {
    Bitstream stream{};
    Message unit{Message::decodeFrom(stream)};
    std::cout << "\n";
    unit.encodeTo(stream);
    std::cout << "\n";
    std::cout << unit;
}
