#include <iostream>
#include <memory>
#include <utility>

// Custom UniquePtr implementation (single object version)
template <typename T>
class UniquePtr {
private:
    T* ptr;

public:
    // Constructor
    explicit UniquePtr(T* p = nullptr) : ptr(p) {}

    // Destructor - automatically deletes the owned resource
    ~UniquePtr() {
        delete ptr;
    }

    // Delete copy constructor and copy assignment (unique ownership)
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Move constructor - transfer ownership
    UniquePtr(UniquePtr&& other) noexcept : ptr(other.release()) {}

    // Move assignment operator - transfer ownership
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        reset(other.release());
        return *this;
    }

    // Dereference operator
    T& operator*() const {
        return *ptr;
    }

    // Arrow operator for member access
    T* operator->() const {
        return ptr;
    }

    // Conversion to bool - check if pointer is not null
    explicit operator bool() const {
        return ptr != nullptr;
    }

    // Get the raw pointer
    T* get() const {
        return ptr;
    }

    // Release ownership and return raw pointer
    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    // Replace the managed object
    void reset(T* p = nullptr) {
        T* old = ptr;
        ptr = p;
        delete old;
    }

    // Swap ownership with another UniquePtr
    void swap(UniquePtr& other) noexcept {
        std::swap(ptr, other.ptr);
    }
};

// Helper function to create UniquePtr
template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}
