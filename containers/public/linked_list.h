#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iterator>
#include <memory>
#include <vector>

template <typename T>
struct LinkedListElement {
    explicit LinkedListElement(T&& _value) : value{std::move(_value)} {}
    explicit LinkedListElement(const T& _value) : value{_value} {}

    T value;
    std::unique_ptr<LinkedListElement<T>> next;
};

template <typename T>
class LinkedList {
  public:
    LinkedList() : head{nullptr}, number_of_elements{0U} {}
    LinkedList(std::vector<T>&& elements) : head{nullptr}, number_of_elements{0U} {
        for (auto element = std::make_move_iterator(elements.rbegin());
             element != std::make_move_iterator(elements.rend()); ++element) {
            // std::initializer_list elements are always const, can't be moved from. That's why we use vector
            emplace_front(*element);
        }
    }

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

    T pop_front() {
        const auto result = head->value;
        head = std::move(head->next);
        --number_of_elements;
        return result;
    }

    const T& front() const { return head->value; }

    T& front() { return head->value; }

    std::size_t size() const { return number_of_elements; }

    bool empty() const { return 0U == number_of_elements; }

  private:
    std::unique_ptr<LinkedListElement<T>> head;
    std::size_t number_of_elements;
};

// TODO implement iterator

#endif  // LINKED_LIST_H
