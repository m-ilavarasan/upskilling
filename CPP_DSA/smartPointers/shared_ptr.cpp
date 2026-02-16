#include <utility>
#include <cstddef>

// Control block to manage reference counting
template <typename T>
class ControlBlock {
public:
    T* ptr;
    int refCount;

    ControlBlock(T* p) : ptr(p), refCount(1) {}

    ~ControlBlock() {
        delete ptr;
    }

    void incrementRef() {
        ++refCount;
    }

    void decrementRef() {
        --refCount;
    }

    int getRefCount() const {
        return refCount;
    }
};

// Custom SharedPtr implementation
template <typename T>
class SharedPtr {
private:
    T* ptr;
    ControlBlock<T>* controlBlock;

    // Helper function to cleanup
    void cleanup() {
        if (controlBlock) {
            controlBlock->decrementRef();
            if (controlBlock->getRefCount() == 0) {
                delete controlBlock;
            }
            controlBlock = nullptr;
            ptr = nullptr;
        }
    }

public:
    // Constructor from raw pointer
    explicit SharedPtr(T* p = nullptr) : ptr(p), controlBlock(nullptr) {
        if (p != nullptr) {
            controlBlock = new ControlBlock<T>(p);
        }
    }

    // Destructor
    ~SharedPtr() {
        cleanup();
    }

    // Copy constructor - increment reference count
    SharedPtr(const SharedPtr& other) : ptr(other.ptr), controlBlock(other.controlBlock) {
        if (controlBlock) {
            controlBlock->incrementRef();
        }
    }

    // Copy assignment operator
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            cleanup();
            ptr = other.ptr;
            controlBlock = other.controlBlock;
            if (controlBlock) {
                controlBlock->incrementRef();
            }
        }
        return *this;
    }

    // Move constructor
    SharedPtr(SharedPtr&& other) noexcept : ptr(other.ptr), controlBlock(other.controlBlock) {
        other.ptr = nullptr;
        other.controlBlock = nullptr;
    }

    // Move assignment operator
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            cleanup();
            ptr = other.ptr;
            controlBlock = other.controlBlock;
            other.ptr = nullptr;
            other.controlBlock = nullptr;
        }
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

    // Conversion to bool
    explicit operator bool() const {
        return ptr != nullptr;
    }

    // Get the raw pointer
    T* get() const {
        return ptr;
    }

    // Get reference count
    int use_count() const {
        return controlBlock ? controlBlock->getRefCount() : 0;
    }

    // Check if this is the only owner
    bool unique() const {
        return use_count() == 1;
    }

    // Reset the pointer
    void reset(T* p = nullptr) {
        cleanup();
        ptr = p;
        if (p != nullptr) {
            controlBlock = new ControlBlock<T>(p);
        } else {
            controlBlock = nullptr;
        }
    }

    // Swap with another SharedPtr
    void swap(SharedPtr& other) noexcept {
        std::swap(ptr, other.ptr);
        std::swap(controlBlock, other.controlBlock);
    }
};

// Helper function to create SharedPtr
template <typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}
