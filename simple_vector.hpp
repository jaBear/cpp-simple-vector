#pragma once

#include <array>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <initializer_list>
#include <utility>
#include "array_ptr.hpp"

class ReserveProxyObj {
public:
    ReserveProxyObj(size_t capacity_to_reserve) : capacity(capacity_to_reserve) {};
    size_t capacity;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;
    
    
    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) : SimpleVector(size, Type{})
    {
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type &value) : new_array(size), size_(size), capacity_(size)
    {
        std::fill(begin(), end(), value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) : SimpleVector(init.size(), Type{}){
        std::copy(init.begin(), init.end(), begin());
        // Напишите тело конструктора самостоятельно
    }
    
    SimpleVector(const SimpleVector& other) : new_array(other.size_) {
        size_ = other.size_;
        capacity_ = other.capacity_;
        std::copy(other.begin(), other.end(), begin());
    }
    
    SimpleVector(SimpleVector&& other){
        new_array = std::move(other.new_array);
        size_ = std::move(other.size_);
        capacity_ = std::move(other.capacity_);
        other.Clear();
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (this == &rhs) {
            return *this;
        }
            SimpleVector<Type> another_array(rhs);
            swap(another_array);
            return *this;
            // Напишите тело конструктора самостоятельно
    }
    
    SimpleVector& operator=(SimpleVector &&rhs) {
        if (*this != rhs || !rhs.IsEmpty()) {
            SimpleVector tmp = std::move(rhs);
            swap(tmp);
        }
            return *this;
    }
    
    explicit SimpleVector(ReserveProxyObj new_capacity) : new_array(new_capacity.capacity),
                                                          capacity_(new_capacity.capacity) {}

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        // Напишите тело самостоятельно
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        // Напишите тело самостоятельно
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        if(size_ == 0) {
            return true;
        } else {
            return false;
        }
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        return new_array[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return new_array[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (!(index >= size_)) {
            return new_array[index];
        } else {
            throw std::out_of_range("Exception: out_of_range");
        }
        // Напишите тело самостоятельно
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (!(index >= size_)) {
            return new_array[index];
        } else {
            throw std::out_of_range("Exception: out_of_range");
        }
        // Напишите тело самостоятельно
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
        // Напишите тело самостоятельно
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if (size_ < new_size) {
            capacity_ = new_size * 2;
            ArrayPtr<Type> buffer_array(capacity_);
            std::move(begin(), end(), Iterator(buffer_array.Get()));
            size_ = new_size;
            new_array.swap(buffer_array);
        } else if (size_ == 0) {
            size_ = new_size;
        } else {
            size_ = new_size;
        }
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return  Iterator(new_array.Get());
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return Iterator(new_array.Get() + size_);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return ConstIterator(new_array.Get());
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return ConstIterator(new_array.Get() + size_);
    }

// Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return ConstIterator(new_array.Get());
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return ConstIterator(new_array.Get() + size_);
    }
    
    void PushBack(const Type &item) {
            if (size_ == capacity_) {
                size_t new_capacity = capacity_ * 2;
                if (new_capacity == 0) {
                    ++new_capacity;
                }
                ArrayPtr<Type> new_items(new_capacity);
                std::copy(begin(), end(), new_items.Get());
                new_array.swap(new_items);
                capacity_ = new_capacity;
            }
            if (size_ == 0) {
                size_t new_capacity;
                capacity_ == 0 ? new_capacity = 1 : new_capacity = capacity_ * 2;
                ArrayPtr<Type> new_items(new_capacity);
            }
            new_array[size_] = item;
            ++size_;
        }
    
    void PushBack(Type &&item) {
            if (size_ == capacity_) {
                size_t new_capacity = capacity_ * 2;
                if (new_capacity == 0) {
                    ++new_capacity;
                }
                ArrayPtr<Type> new_items(new_capacity);
                std::move(begin(), end(), new_items.Get());
                new_array.swap(new_items);
                capacity_ = new_capacity;
            }
            if (size_ == 0) {
                size_t new_capacity;
                capacity_ == 0 ? new_capacity = 1 : new_capacity = capacity_ * 2;
                ArrayPtr<Type> new_items(new_capacity);
            }
            new_array[size_] = std::move(item);
            ++size_;
        }
    
    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        assert(begin() <= pos && pos <= end());
        auto gap = std::distance(begin(), Iterator(pos));
        if (pos == begin() + size_) {
            PushBack(value);
            return begin() + size_ - 1;
        }
        if (size_ == capacity_) {
            capacity_ *= 2;
            ArrayPtr<Type> new_items(capacity_);
            std::copy(begin(), Iterator(pos), new_items.Get());
            std::copy(Iterator(pos), end(), new_items.Get() + (pos - begin()) + 1);
            new_items[(pos - begin())] = value;
            new_array.swap(new_items);
            ++size_;
            return Iterator(begin() + gap);
        }
        std::copy_backward(pos, cend(), end() + 1);
        new_array[pos - begin()] = value;
        ++size_;
        return Iterator(begin() + gap);
        }
    
    Iterator Insert(ConstIterator pos, Type &&value) {
        auto gap = std::distance(begin(), Iterator(pos));
        if (pos == begin() + size_) {
            PushBack(std::move(value));
            return begin() + size_ - 1;
        }
        if (size_ == capacity_) {
            capacity_ *= 2;
            ArrayPtr<Type> new_items(capacity_);
            std::move(begin(), Iterator(pos), new_items.Get());
            std::move(Iterator(pos), end(), new_items.Get() + (pos - begin()) + 1);
            new_items[(pos - begin())] = std::move(value);
            new_array.swap(new_items);
            ++size_;
            return Iterator(begin() + gap);
        }
        std::move_backward(Iterator(pos), end(), end() + 1);
        new_array[pos - begin()] = std::move(value);
        ++size_;
        return Iterator(begin() + gap);
        }


    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        if (size_ != 0) {
            --size_;
        }
// Напишите тело самостоятельно
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        auto gap = std::distance(begin(), Iterator(pos));
        std::move(begin() + gap + 1, end(), begin() + gap);
        --size_;
        return begin() + gap;
    }
    
    void swap(SimpleVector& other) noexcept {
        new_array.swap(other.new_array);
        std::swap(this->capacity_, other.capacity_);
        std::swap(this->size_, other.size_);
            // Напишите тело самостоятельно
    }
    
    void Reserve(size_t new_capacity) {
            if (capacity_ < new_capacity) {
                ArrayPtr<Type> new_items(new_capacity);
                std::copy(begin(), end(), new_items.Get());
                new_array.swap(new_items);
                capacity_ = new_capacity;
            }
        }
    
private:
    ArrayPtr<Type> new_array;
    size_t size_ = 0;
    size_t capacity_ = 0;
};

template <typename Type>
inline bool operator==(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    if (lhs.GetSize() != rhs.GetSize()) {
        return false;
    }
    return std::equal(lhs.begin(), lhs.begin() + lhs.GetSize(), rhs.begin(), rhs.begin() + rhs.GetSize());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    return !(rhs < lhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    return !(lhs < rhs);
}

