#include <utility>
#include <cstddef>
#include <iostream>
#include <string>

// Forward declarations
template <typename T>
class SharedPtr;

template <typename T>
class WeakPtr;

// Control block to manage reference counting
template <typename T>
class ControlBlock {
public:
    T* ptr;
    int refCount;
    int weakCount;

    ControlBlock(T* p) : ptr(p), refCount(1), weakCount(0) {}

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

    void incrementWeak() {
        ++weakCount;
    }

    void decrementWeak() {
        --weakCount;
    }

    int getWeakCount() const {
        return weakCount;
    }
};

// Custom SharedPtr implementation
template <typename T>
class SharedPtr {
private:
    T* ptr;
    ControlBlock<T>* controlBlock;

    void cleanup() {
        if (controlBlock) {
            controlBlock->decrementRef();
            if (controlBlock->getRefCount() == 0 && controlBlock->getWeakCount() == 0) {
                delete controlBlock;
            }
            controlBlock = nullptr;
            ptr = nullptr;
        }
    }

public:
    explicit SharedPtr(T* p = nullptr) : ptr(p), controlBlock(nullptr) {
        if (p != nullptr) {
            controlBlock = new ControlBlock<T>(p);
        }
    }

    ~SharedPtr() {
        cleanup();
    }

    SharedPtr(const SharedPtr& other) : ptr(other.ptr), controlBlock(other.controlBlock) {
        if (controlBlock) {
            controlBlock->incrementRef();
        }
    }

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

    SharedPtr(SharedPtr&& other) noexcept : ptr(other.ptr), controlBlock(other.controlBlock) {
        other.ptr = nullptr;
        other.controlBlock = nullptr;
    }

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

    T& operator*() const {
        return *ptr;
    }

    T* operator->() const {
        return ptr;
    }

    explicit operator bool() const {
        return ptr != nullptr;
    }

    T* get() const {
        return ptr;
    }

    int use_count() const {
        return controlBlock ? controlBlock->getRefCount() : 0;
    }

    bool unique() const {
        return use_count() == 1;
    }

    void reset(T* p = nullptr) {
        cleanup();
        ptr = p;
        if (p != nullptr) {
            controlBlock = new ControlBlock<T>(p);
        } else {
            controlBlock = nullptr;
        }
    }

    void swap(SharedPtr& other) noexcept {
        std::swap(ptr, other.ptr);
        std::swap(controlBlock, other.controlBlock);
    }

    friend class WeakPtr<T>;
};

// Custom WeakPtr implementation - non-owning reference to SharedPtr
template <typename T>
class WeakPtr {
private:
    T* ptr;
    ControlBlock<T>* controlBlock;

    void cleanup() {
        if (controlBlock) {
            controlBlock->decrementWeak();
            if (controlBlock->getRefCount() == 0 && controlBlock->getWeakCount() == 0) {
                delete controlBlock;
            }
            controlBlock = nullptr;
            ptr = nullptr;
        }
    }

public:
    // Default constructor
    WeakPtr() : ptr(nullptr), controlBlock(nullptr) {}

    // Constructor from SharedPtr - does not increment refCount
    WeakPtr(const SharedPtr<T>& sp) : ptr(sp.ptr), controlBlock(sp.controlBlock) {
        if (controlBlock) {
            controlBlock->incrementWeak();
        }
    }

    // Copy constructor
    WeakPtr(const WeakPtr& other) : ptr(other.ptr), controlBlock(other.controlBlock) {
        if (controlBlock) {
            controlBlock->incrementWeak();
        }
    }

    // Copy assignment operator
    WeakPtr& operator=(const WeakPtr& other) {
        if (this != &other) {
            cleanup();
            ptr = other.ptr;
            controlBlock = other.controlBlock;
            if (controlBlock) {
                controlBlock->incrementWeak();
            }
        }
        return *this;
    }

    // Assignment from SharedPtr
    WeakPtr& operator=(const SharedPtr<T>& sp) {
        cleanup();
        ptr = sp.ptr;
        controlBlock = sp.controlBlock;
        if (controlBlock) {
            controlBlock->incrementWeak();
        }
        return *this;
    }

    // Move constructor
    WeakPtr(WeakPtr&& other) noexcept : ptr(other.ptr), controlBlock(other.controlBlock) {
        other.ptr = nullptr;
        other.controlBlock = nullptr;
    }

    // Move assignment operator
    WeakPtr& operator=(WeakPtr&& other) noexcept {
        if (this != &other) {
            cleanup();
            ptr = other.ptr;
            controlBlock = other.controlBlock;
            other.ptr = nullptr;
            other.controlBlock = nullptr;
        }
        return *this;
    }

    // Destructor
    ~WeakPtr() {
        cleanup();
    }

    // Convert to SharedPtr - returns empty SharedPtr if object has been deleted
    SharedPtr<T> lock() const {
        if (controlBlock && controlBlock->getRefCount() > 0) {
            SharedPtr<T> sp;
            sp.ptr = ptr;
            sp.controlBlock = controlBlock;
            controlBlock->incrementRef();
            return sp;
        }
        return SharedPtr<T>();
    }

    // Get reference count of the managed object
    int use_count() const {
        return controlBlock ? controlBlock->getRefCount() : 0;
    }

    // Check if the managed object has been deleted
    bool expired() const {
        return controlBlock ? controlBlock->getRefCount() == 0 : true;
    }

    // Swap with another WeakPtr
    void swap(WeakPtr& other) noexcept {
        std::swap(ptr, other.ptr);
        std::swap(controlBlock, other.controlBlock);
    }

    // Reset
    void reset() {
        cleanup();
    }
};

// Helper function to create SharedPtr
template <typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

// Example class
class Person {
public:
    std::string name;

    Person(const std::string& n) : name(n) {
        std::cout << "Person created: " << name << std::endl;
    }

    ~Person() {
        std::cout << "Person destroyed: " << name << std::endl;
    }
};

int main() {
    std::cout << "===== WeakPtr Example =====" << std::endl << std::endl;

    // Example 1: Basic WeakPtr
    {
        std::cout << "--- Creating SharedPtr ---" << std::endl;
        SharedPtr<Person> sp = make_shared<Person>("Alice");
        std::cout << "use_count: " << sp.use_count() << std::endl;

        std::cout << "\n--- Creating WeakPtr from SharedPtr ---" << std::endl;
        WeakPtr<Person> wp = sp;
        std::cout << "use_count (unchanged): " << sp.use_count() << std::endl;

        std::cout << "\n--- Locking WeakPtr to use object ---" << std::endl;
        SharedPtr<Person> locked = wp.lock();
        if (locked) {
            std::cout << "Name: " << locked->name << std::endl;
            std::cout << "use_count after lock: " << sp.use_count() << std::endl;
        }
    }
    std::cout << std::endl;

    // Example 2: Object deleted but WeakPtr remains
    {
        std::cout << "--- WeakPtr after object is deleted ---" << std::endl;
        WeakPtr<Person> wp;
        
        {
            SharedPtr<Person> sp = make_shared<Person>("Bob");
            wp = sp;
            std::cout << "WeakPtr expired: " << wp.expired() << std::endl;
        }

        std::cout << "After SharedPtr deleted, WeakPtr expired: " << wp.expired() << std::endl;
        
        SharedPtr<Person> locked = wp.lock();
        if (!locked) {
            std::cout << "lock() returned null - object has been deleted" << std::endl;
        }
    }
    std::cout << std::endl;

    std::cout << "===== End =====" << std::endl;
    return 0;
}
