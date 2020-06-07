#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <memory>

template <typename T>
struct LinkedListElement {
    explicit LinkedListElement(T _value) : value{std::move(_value)} {}
    T value;
    std::unique_ptr<LinkedListElement<T>> next;
};

template <typename T>
class LinkedList {
  public:
    LinkedList() : head{nullptr}, number_of_elements{0U} {}

    void append(const T& value) {
        if (head == nullptr) {
            head = std::make_unique<LinkedListElement>(value);
        } else {
            auto& element = head->next;
            while(element->next != nullptr){
                element = element->next;
            }
            element->next = std::make_unique<LinkedListElement>(value);
        }
    }

    std::size_t size() const {
        return number_of_elements;
    }

    bool empty() const {
        return 0U == number_of_elements;
    }

//    void insertAfter(const T& value, std::size_t position)

  private:
    std::unique_ptr<LinkedListElement<T>> head;
    std::size_t number_of_elements;
};

// TODO implement iterator

#endif  // LINKED_LIST_H
