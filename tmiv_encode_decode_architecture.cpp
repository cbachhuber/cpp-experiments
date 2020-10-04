
#include <cassert>
#include <cstdint>
#include <functional>
#include <iostream>
#include <optional>
#include <type_traits>

constexpr std::size_t bits_int = 27;
constexpr std::size_t bits_optional_unsigned = 5;

template <typename T>
auto putField(std::ostream& stream, const std::string& fieldName, T&& fieldValue) {
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
template <typename FunctionType>
void writeToStream(Bitstream& stream, FunctionType&& function, std::size_t number_of_bits = 0) {
    if constexpr (std::is_same_v<bool, decltype(function())>) {
        stream.putFlag(function());
        // The below two could actually be summarized in an else statement
    } else if constexpr (std::is_same_v<std::int32_t, decltype(function())>) {
        stream.writeBits<decltype(function())>(function(), number_of_bits);
    } else if constexpr (std::is_same_v<std::uint8_t, decltype(function())>) {
        stream.writeBits<decltype(function())>(function(), number_of_bits);
    }
}

// TODO(CB) extend to indexed (multi argument) setters
// TODO(CB) this function signature is too constraining. You want to generalize to returning functions!
template <typename ArgumentType>
void readFromStream(Bitstream& stream, std::function<void(ArgumentType)> setter, std::size_t number_of_bits = 0) {
    if constexpr (std::is_same_v<bool, ArgumentType>) {
        setter(stream.getFlag());
    } else {
        setter(stream.readBits<ArgumentType>(number_of_bits));
    }
}

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
        putField(stream, "my_int", msg.my_int());
        putField(stream, "my_flag", msg.my_flag());
        if (msg.my_flag()) {
            putField(stream, "my_optional", msg.my_optional());
        }
        return stream;
    }

    static Message decodeFrom(Bitstream& stream) {
        Message result{};
        // TODO(CB) can we get away without those lambdas? Is the current lambda requirement an architecture smell?
        const std::function<void(std::int32_t)> set_my_int = [&result](std::int32_t value) { result.my_int(value); };
        const std::function<void(std::uint8_t)> set_my_optional = [&result](std::uint8_t value) {
            result.my_optional(value);
        };
        const std::function<void(bool)> set_my_flag = [&result](bool value) { result.my_flag(value); };

        readFromStream(stream, set_my_int, bits_int);
        readFromStream(stream, set_my_flag);
        if (result.my_flag()) {
            readFromStream(stream, set_my_optional, bits_optional_unsigned);
        }
        return result;
    }

    void encodeTo(Bitstream& stream) const {
        const auto my_int_lambda = [this]() { return this->my_int(); };
        const auto my_flag_lambda = [this]() { return this->my_flag(); };
        const auto my_optional_lambda = [this]() { return this->my_optional(); };

        writeToStream(stream, my_int_lambda, bits_int);
        writeToStream(stream, my_flag_lambda);
        if (my_flag()) {
            writeToStream(stream, my_optional_lambda, bits_optional_unsigned);
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
