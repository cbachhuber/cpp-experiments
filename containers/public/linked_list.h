#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <memory>

template <typename T>
struct LinkedListElement {
    explicit LinkedListElement(T _value) : value{std::move(_value)} {}
    T value;
    std::unique_ptr<LinkedListElement<T>> next;
//    std::unique_ptr<LinkedListElement<T>> previous;
};

template <typename T>
class LinkedList {
  public:
    LinkedList() : head{nullptr} {}

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

//    void insertAfter(const T& value, std::size_t position)

  private:
    std::unique_ptr<LinkedListElement<T>> head;
};

// TODO implement iterator

#endif  // LINKED_LIST_H
