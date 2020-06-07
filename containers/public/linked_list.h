#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <experimental/optional>
#include <memory>

using std::experimental::optional;

template <typename T>
struct LinkedListElement {
    explicit LinkedListElement(T _value) : value{std::move(_value)} {}
//    explicit LinkedListElement(T&& _value) : value{std::move(_value)} {}
    T value;
    std::unique_ptr<LinkedListElement<T>> next;
};

template <typename T>
class LinkedList {
  public:
    LinkedList() : head{nullptr}, number_of_elements{0U} {}

    void emplace_front(T&& value) {
        if (head == nullptr) {
            head = std::make_unique<LinkedListElement<T>>(std::move(value));
        } else {
            auto new_element = std::make_unique<LinkedListElement<T>>(std::move(value));
            new_element->next = std::move(this->head);
            this->head = std::move(new_element);
        }
        ++number_of_elements;
    }

    optional<T> front() const { return this->empty() ? optional<T>{} : optional<T>{head->value}; }

    std::size_t size() const { return number_of_elements; }

    bool empty() const { return 0U == number_of_elements; }

  private:
    std::unique_ptr<LinkedListElement<T>> head;
    std::size_t number_of_elements;
};

// TODO implement iterator

#endif  // LINKED_LIST_H
