#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

// Memory Pool with fixed-size blocks
class MemoryPool
{
private:
    struct Block
    {
        void* data;
        bool isAllocated;
        size_t size;
    };
    
    vector<Block> blocks;
    size_t blockSize;
    size_t totalBlocks;
    size_t allocatedBlocks;
    
public:
    // Constructor - Initialize memory pool with fixed block size and number of blocks
    MemoryPool(size_t blockSize, size_t numBlocks) 
        : blockSize(blockSize), totalBlocks(numBlocks), allocatedBlocks(0)
    {
        cout << "Initializing Memory Pool:" << endl;
        cout << "  Block Size: " << blockSize << " bytes" << endl;
        cout << "  Total Blocks: " << numBlocks << endl;
        cout << "  Total Memory: " << (blockSize * numBlocks) << " bytes" << endl << endl;
        
        // Allocate memory for each block
        for (size_t i = 0; i < numBlocks; i++)
        {
            Block block;
            block.data = malloc(blockSize);
            block.isAllocated = false;
            block.size = blockSize;
            blocks.push_back(block);
        }
        
        cout << "Memory Pool created successfully!" << endl << endl;
    }
    
    // Destructor - Free all allocated memory
    ~MemoryPool()
    {
        cout << "Destroying Memory Pool..." << endl;
        for (size_t i = 0; i < blocks.size(); i++)
        {
            if (blocks[i].data != nullptr)
            {
                free(blocks[i].data);
            }
        }
        cout << "Memory Pool destroyed!" << endl;
    }
    
    // Allocate a block from the pool
    void* allocate(size_t requestedSize)
    {
        if (requestedSize > blockSize)
        {
            cout << "ERROR: Requested size (" << requestedSize 
                 << " bytes) exceeds block size (" << blockSize << " bytes)" << endl;
            return nullptr;
        }
        
        // Find the first free block
        for (size_t i = 0; i < blocks.size(); i++)
        {
            if (!blocks[i].isAllocated)
            {
                blocks[i].isAllocated = true;
                allocatedBlocks++;
                cout << "Allocated block #" << i << " (" << requestedSize 
                     << " bytes requested, " << blockSize << " bytes block size)" << endl;
                return blocks[i].data;
            }
        }
        
        cout << "ERROR: No free blocks available in memory pool!" << endl;
        return nullptr;
    }
    
    // Allocate and construct object using placement new
    template<typename T>
    T* allocateObject()
    {
        void* ptr = allocate(sizeof(T));
        if (ptr == nullptr) return nullptr;
        
        // Placement new - calls constructor on pre-allocated memory
        T* obj = new (ptr) T();
        cout << "  Constructor called for object at block" << endl;
        return obj;
    }
    
    // Destruct and deallocate object
    template<typename T>
    void deallocateObject(T* ptr)
    {
        if (ptr == nullptr) return;
        
        // Explicitly call destructor
        ptr->~T();
        cout << "  Destructor called for object" << endl;
        
        deallocate((void*)ptr);
    }
    
    // Deallocate a block and return it to the pool
    void deallocate(void* ptr)
    {
        for (size_t i = 0; i < blocks.size(); i++)
        {
            if (blocks[i].data == ptr)
            {
                if (blocks[i].isAllocated)
                {
                    blocks[i].isAllocated = false;
                    allocatedBlocks--;
                    cout << "Deallocated block #" << i << endl;
                    memset(blocks[i].data, 0, blockSize);  // Clear the block
                    return;
                }
                else
                {
                    cout << "ERROR: Block #" << i << " is already free!" << endl;
                    return;
                }
            }
        }
        
        cout << "ERROR: Pointer does not belong to this memory pool!" << endl;
    }
    
    // Get pool statistics
    void printStatistics()
    {
        cout << "\n========== Memory Pool Statistics ==========" << endl;
        cout << "Total Blocks: " << totalBlocks << endl;
        cout << "Allocated Blocks: " << allocatedBlocks << endl;
        cout << "Free Blocks: " << (totalBlocks - allocatedBlocks) << endl;
        cout << "Utilization: " << ((float)allocatedBlocks / totalBlocks * 100) << "%" << endl;
        cout << "Total Memory Used: " << (allocatedBlocks * blockSize) << " bytes" << endl;
        cout << "Total Memory Available: " << ((totalBlocks - allocatedBlocks) * blockSize) << " bytes" << endl;
        cout << "============================================\n" << endl;
    }
    
    // Display block status
    void displayBlockStatus()
    {
        cout << "\n========== Block Status ==========" << endl;
        for (size_t i = 0; i < blocks.size(); i++)
        {
            cout << "Block #" << i << ": " 
                 << (blocks[i].isAllocated ? "ALLOCATED" : "FREE") 
                 << " (Size: " << blocks[i].size << " bytes)" << endl;
        }
        cout << "=================================\n" << endl;
    }
};

// Example: Custom data structure to store in memory pool
struct Person
{
    int id;
    char name[50];
    int age;
    
    // Constructor to show when it's called
    Person()
    {
        cout << "    [Person Constructor Called]" << endl;
        id = 0;
        strcpy(name, "");
        age = 0;
    }
    
    // Destructor to show when it's called
    ~Person()
    {
        cout << "    [Person Destructor Called]" << endl;
    }
    
    void display()
    {
        cout << "Person: ID=" << id << ", Name=" << name << ", Age=" << age << endl;
    }
};

int main()
{
    // Create a memory pool with 5 blocks, each 256 bytes
    MemoryPool pool(256, 5);
    
    cout << "\n========== Test 1: Basic Allocation with malloc ==========" << endl;
    void* ptr1 = pool.allocate(100);
    void* ptr2 = pool.allocate(150);
    void* ptr3 = pool.allocate(200);
    
    pool.printStatistics();
    pool.displayBlockStatus();
    
    cout << "========== Test 2: Using malloc - No Constructor Called ==========" << endl;
    // Cast and use the allocated memory (no constructor)
    Person* person = (Person*)ptr1;
    person->id = 1;
    strcpy(person->name, "John Doe");
    person->age = 30;
    
    cout << "Stored Person: ";
    person->display();
    
    pool.displayBlockStatus();
    
    cout << "========== Test 3: Deallocate Memory ==========" << endl;
    pool.deallocate(ptr1);
    pool.deallocate(ptr2);
    
    pool.printStatistics();
    pool.displayBlockStatus();
    
    cout << "========== Test 4: Reuse Deallocated Memory ==========" << endl;
    void* ptr4 = pool.allocate(128);  // Should reuse ptr1's block
    
    pool.printStatistics();
    pool.displayBlockStatus();
    
    cout << "========== Test 5: Using Placement new (with Constructor) ==========" << endl;
    cout << "Allocating Person using placement new:" << endl;
    Person* person2 = pool.allocateObject<Person>();  // Constructor CALLED
    person2->id = 2;
    strcpy(person2->name, "Jane Smith");
    person2->age = 28;
    
    cout << "Stored Person: ";
    person2->display();
    
    cout << "\nDeallocating Person using placement delete:" << endl;
    pool.deallocateObject<Person>(person2);  // Destructor CALLED
    
    pool.printStatistics();
    
    cout << "\n========== Comparison: malloc vs new ==========" << endl;
    cout << "malloc/free:" << endl;
    cout << "  - No constructor/destructor overhead" << endl;
    cout << "  - Fast allocation/deallocation" << endl;
    cout << "  - Good for raw memory pools" << endl;
    cout << "  - Risk: forgot to initialize" << endl;
    cout << "\nPlacement new/delete:" << endl;
    cout << "  - Calls constructor/destructor" << endl;
    cout << "  - Type-safe initialization" << endl;
    cout << "  - Good for objects with complex initialization" << endl;
    cout << "  - More overhead but safer" << endl;
    
    return 0;
}
