// Online C++ compiler to run C++ program online
#include <iostream>
#include <cstdlib>
#include <ctime>
#define HEAP_SIZE 4096
#define FibonacciList {1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181}
#define nFiboElements 18

class Node {
public:
    int data;
    int refCount;
    int marked;
    Node* next;

    // Constructor
    Node(int value) : data(value), refCount(1), marked(0), next(nullptr) {}
    // Destructor
    ~Node();
};

class NodeAddress {
public:
    Node* nodePtr;
    NodeAddress* next;
    static NodeAddress* globalNodeList;

    // Constructor
    NodeAddress() : nodePtr(nullptr), next(nullptr) {}
};

class Heap {
public:
    void* heap; // Pointer to the start of the heap memory

    struct FreeListNode {
        void* pointer; // Address of the beginning of free space
        FreeListNode* next;
    };

    struct FreeList {
        FreeListNode* list_pointers[nFiboElements];
        int number_of_elements[nFiboElements];

        // Constructor
        FreeList();
    } freeList;
    
    
    
    int fibonacci[nFiboElements] = FibonacciList;

    int metadata; // Size of metadata required in bytes

    // Constructor
    Heap() {
        heap = nullptr;
        metadata = sizeof(int);
    }
    // Destructor
    ~Heap() {
        // Free any dynamically allocated memory and perform necessary cleanup
        // For example, free the heap memory
        free(heap);
    
        // You can also free any other dynamically allocated structures in the class if needed
    }

    
    NodeAddress *globalNodeList = NULL;

    void createHeap();
    void* alloc(int size);
    void print();
    void addToGlobalList(Node* node);
    int total() const;
    void freeMemory(void* pointer);
    void add2freeList(FreeListNode* lptr[], int size, void* pointer);
    void printGlobal(NodeAddress* ptr);
    
    
    
    int getHighFiboIndex(int n) {
        int fibonacci[nFiboElements] = FibonacciList;
        int counter = 0;

        // Find the index where the Fibonacci number exceeds `n`
        while (n > fibonacci[counter] && counter < nFiboElements) {
            counter++;
        }

        // Ensure the index is within bounds
        return (counter == nFiboElements) ? (nFiboElements - 1) : counter;
    }

    // Method to get the lowest Fibonacci index for `n`
    int getLowFiboIndex(int n) {
        int fibonacci[nFiboElements] = FibonacciList;
        int counter = 0, prev = 0;

        // Find the highest Fibonacci number less than or equal to `n`
        while (n >= fibonacci[counter] && counter < nFiboElements) {
            prev = counter;
            counter++;
        }

        return prev;
    }
    void refCountGarbageCollect(Node** head);
    void mark(Node* node);
    void sweep(Node** head);
};

// Linked list methods for Node
void insertAtStart(Heap heap, Node** ptr, int data);
void deleteList(Heap heap, Node* ptr);
void printLL(Node* ptr);

void reversal(Node** hptr);
void randomlyDisconnectNode(Node** head);


void disconnectLastThreeNodes(Node** head);


int main() {
    srand(time(NULL));

    // Create an instance of the Heap
    Heap heapManager;
    heapManager.createHeap();
    std::cout << "Printing free list after allocation..." << std::endl;
    heapManager.print();
    std::cout << std::endl;

    // Linked list head pointer
    Node* head = nullptr;

    // Insert nodes into the linked list
    for (int i = 0; i < 50; i++) {
        insertAtStart(heapManager, &head, i + 1);
    }

    std::cout << "Printing linked list..." << std::endl;
    printLL(head);
    std::cout << std::endl;

    reversal(&head);
    std::cout << "Printing reversed linked list..." << std::endl;
    printLL(head);
    std::cout << std::endl;

    std::cout << "Printing free list after creating linked list..." << std::endl;
    heapManager.print();
    std::cout << std::endl;

    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << "Reference Counting Garbage Collection!" << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    randomlyDisconnectNode(&head);
    std::cout << std::endl;
    printLL(head);

    std::cout << "\nCollecting Garbage" << std::endl;
    std::cout << std::endl;
    heapManager.refCountGarbageCollect(&head);
    std::cout << std::endl;
    std::cout << "Done Collecting Garbage" << std::endl;
    std::cout << std::endl;

    std::cout << "Printing linked list after reference counting garbage collection..." << std::endl;
    printLL(head);
    std::cout << std::endl;

    std::cout << "Printing free list after reference counting garbage collection..." << std::endl;
    heapManager.print();
    std::cout << std::endl;

    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << "Mark and Sweep Garbage Collection!" << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    disconnectLastThreeNodes(&head);
    std::cout << std::endl;
    printLL(head);
    std::cout << std::endl;

    heapManager.mark(head);
    std::cout << "Marking Done..." << std::endl << std::endl;
    heapManager.sweep(&head);
    std::cout << "After Sweeping..." << std::endl;
    std::cout << std::endl;

    std::cout << "Printing linked list after mark-and-sweep garbage collection..." << std::endl;
    printLL(head);
    std::cout << std::endl;

    std::cout << "Printing free list after mark-and-sweep garbage collection..." << std::endl;
    heapManager.print();
    std::cout << std::endl;

    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << "Deleting Linked List!" << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    deleteList(heapManager, head);
    std::cout << "Printing free list after deleting linked list..." << std::endl;
    heapManager.print();
    std::cout << std::endl;

    // Free the allocated heap memory
    free(heapManager.heap);

    return 0;
}

Heap::FreeList::FreeList() {
    // Initialize the list_pointers to nullptr
    for (int i = 0; i < nFiboElements; i++) {
        list_pointers[i] = nullptr;
        number_of_elements[i] = 0;
    }
}

void Heap::print() {
    int fibonacci[nFiboElements] = FibonacciList;

    for (int i = 0; i < nFiboElements; i++) {
        FreeListNode* ptr = freeList.list_pointers[i]; // Pointer to iterate over the free list

        std::cout << fibonacci[i] << " size free list has " << freeList.number_of_elements[i] << " elements, ";

        // Print all addresses in the current free list
        while (ptr != nullptr) {
            std::cout << ptr->pointer << " ";
            ptr = ptr->next;
        }

        std::cout << std::endl; // Print a new line after each list
    }
}
void Heap::add2freeList(FreeListNode* lptr[], int size, void* pointer) {
    int fibonacci[nFiboElements] = FibonacciList;
    bool found = false;
    FreeListNode* traverse = nullptr;
    FreeListNode* prev = nullptr;

    if (size > 0) {
        // Iterate through all the free lists
        for (int i = 0; i < nFiboElements && !found; i++) {
            if (freeList.number_of_elements[i] != 0) {
                prev = nullptr;
                traverse = lptr[i];

                // Traverse the list to find buddies
                while (traverse != nullptr && !found) {
                    // Check for a buddy on the left
                    if (static_cast<char*>(traverse->pointer) + fibonacci[i] == pointer) {
                        prev ? (prev->next = traverse->next) : (lptr[i] = traverse->next);
                        freeList.number_of_elements[i]--;
                        add2freeList(lptr, (size + fibonacci[i]), traverse->pointer);
                        delete traverse;
                        found = true;
                    }
                    // Check for a buddy on the right
                    else if (static_cast<char*>(pointer) + size == traverse->pointer) {
                        prev ? (prev->next = traverse->next) : (lptr[i] = traverse->next);
                        freeList.number_of_elements[i]--;
                        add2freeList(lptr, (size + fibonacci[i]), pointer);
                        delete traverse;
                        found = true;
                    }
                    prev = traverse;
                    traverse = traverse->next;
                }
            }
        }

        if (!found) {
            int total_space = size;
            FreeListNode* temp;

            while (total_space > 0) {
                int index = getLowFiboIndex(total_space);
                size = fibonacci[index];

                temp = new FreeListNode;
                temp->next = lptr[index];
                temp->pointer = pointer;
                lptr[index] = temp;
                freeList.number_of_elements[index]++;

                pointer = static_cast<char*>(pointer) + fibonacci[index];
                total_space -= size;
            }
        }
    }
}

void* Heap::alloc(int size) {
    int fibonacci[nFiboElements] = FibonacciList;
    int index = getHighFiboIndex(size + metadata);
    bool success = false;
    void* alloc_address = nullptr;

    while (!success && index < nFiboElements) {
        if (freeList.number_of_elements[index] == 0) {
            index++;
        } else {
            FreeListNode* address = freeList.list_pointers[index];
            freeList.number_of_elements[index]--;
            freeList.list_pointers[index] = address->next;

            void* pointer = address->pointer;
            int* payload = static_cast<int*>(pointer);
            *payload = size;
            alloc_address = static_cast<int*>(pointer) + metadata;

            int remainder_space = fibonacci[index] - (size + metadata);
            void* nextPointer = static_cast<int*>(pointer) + size + metadata;

            delete address;

            if (remainder_space > 0) {
                add2freeList(freeList.list_pointers, remainder_space, nextPointer);
            }
            success = true;
        }
    }
    return alloc_address;
}


void Heap::freeMemory(void* pointer) {
    // Find the size from the metadata stored before the actual data pointer
    int* size = (int*)(pointer) - metadata;  // Accessing the metadata relative to the pointer
    // Add the space combined with metadata to the free list
    add2freeList(freeList.list_pointers, (*size + metadata), size); 
}


void Heap::createHeap() {
    // Allocate memory for the heap
    heap = malloc(HEAP_SIZE);

    // Initialize the free list with zero elements
    for (int i = 0; i < nFiboElements; i++) {
        freeList.number_of_elements[i] = 0;
        freeList.list_pointers[i] = nullptr;
    }

    // Divide the initial heap memory into a single free block of HEAP_SIZE
    add2freeList(freeList.list_pointers, HEAP_SIZE, heap);

    // Set metadata size for each allocated block
    metadata = sizeof(int);
}

int Heap::total() const {
    int fibonacci[nFiboElements] = FibonacciList;
    int sum = 0;

    // Calculate the total free memory in all free lists
    for (int i = 0; i < nFiboElements; i++) {
        sum += fibonacci[i] * freeList.number_of_elements[i];
    }

    return sum;
}


// Function to add a Node to the global list of allocated nodes
void Heap::addToGlobalList(Node* node) {
    NodeAddress* newAddress = new NodeAddress; // Using new for dynamic allocation
    if (newAddress != nullptr) {
        newAddress->nodePtr = node;
        newAddress->next = globalNodeList;
        globalNodeList = newAddress;
    }
}

// Function to insert a new node at the start of the linked list
void insertAtStart(Heap heap, Node** ptr, int data) {
    Node* temp = nullptr;
    
    // Allocate memory for a new node using custom allocator
    std::cout << "After 1..." << std::endl;
    temp = static_cast<Node*>(heap.alloc(sizeof(Node)));  // Use heap instance to call alloc
    std::cout << "After 2..." << std::endl;
    if (temp != nullptr) {
        temp->data = data;
        temp->refCount = 1;  // New node gets refCount 1
        temp->marked = 0;    // Initialize marked to 0
        temp->next = *ptr;   // Point new node to the old head
        
        if (*ptr != nullptr) {
            (*ptr)->refCount++;  // Increase refCount for the existing head node
        }
        
        *ptr = temp;           // Update head to point to the new node
        heap.addToGlobalList(temp); // Add the newly allocated node to the global list
    } else {
        std::cout << "Cannot allocate memory for new node." << std::endl;
    }
}

// Function to print all nodes in the global list (for debugging or garbage collection tracking)
void Heap::printGlobal(NodeAddress* ptr) {
    std::cout << "Printing Global List" << std::endl;
    while (ptr != nullptr) {
        std::cout << ptr->nodePtr->data << " ";
        ptr = ptr->next;
    }
    std::cout << std::endl;
}

// Function to print the linked list of nodes
void printLL(Node* ptr) {
    while (ptr != nullptr) {
        std::cout << ptr->data << " ";
        ptr = ptr->next;
    }
    std::cout << std::endl;
}

// Function to reverse the linked list
void reversal(Node** hptr) {
    Node* ptr = *hptr;
    Node* cur = (*hptr)->next;
    Node* first = *hptr;
    Node* temp = nullptr;

    if (ptr != nullptr) {
        while (cur != nullptr) {
            temp = cur->next;
            cur->next = ptr;
            first->next = temp;
            ptr = cur;
            cur = temp;
        }
        *hptr = ptr;  // Update the head pointer to the new first node
    }
}

// Function to delete the entire linked list and free its memory
void deleteList(Heap heap, Node* ptr) {
    Node* temp;
    while (ptr != nullptr) {
        temp = ptr->next;
        heap.freeMemory(ptr);  // Free the memory using the custom memory manager
        ptr = temp;
    }
}



// Function to randomly disconnect a node in the linked list
void randomlyDisconnectNode(Node** head) {
    int count = 0;
    Node* temp = *head;
    Node* prev = nullptr;

    // Count the number of nodes
    while (temp != nullptr) {
        count++;
        temp = temp->next;
    }

    if (count == 0) return; // No nodes to dereference

    // Choose a random node index
    int targetIndex = rand() % count;
    temp = *head;

    for (int i = 0; i < targetIndex; i++) {
        prev = temp;
        temp = temp->next;
    }

    // Dereference and disconnect the randomly chosen node
    std::cout << "Randomly dereferencing node with data " << temp->data << std::endl;
    temp->refCount = 0;

    if (prev != nullptr) {
        prev->next = temp->next;
    } else {
        *head = temp->next;
    }
}

// Garbage collection function to delete nodes with refCount zero
void Heap::refCountGarbageCollect(Node** head) {
    NodeAddress* currentAddress = globalNodeList;
    NodeAddress* prevAddress = nullptr;

    while (currentAddress != nullptr) {
        Node* node = currentAddress->nodePtr;

        // Check if the node's refCount is zero
        if (node->refCount == 0) {
            std::cout << "Garbage collecting node with data " << node->data << std::endl;
            if (prevAddress != nullptr) {
                prevAddress->next = currentAddress->next;
            } else {
                globalNodeList = currentAddress->next;
            }
            NodeAddress* tempAddress = currentAddress;

            currentAddress = currentAddress->next;

            // Free the memory and the global node list entry
            freeMemory(node);
            free(tempAddress);
        } else {
            prevAddress = currentAddress;
            currentAddress = currentAddress->next;
        }
    }
}

// Mark function for garbage collection
void Heap::mark(Node* node) {
    while (node != nullptr && node->marked == 0) {
        node->marked = 1; // Mark the node as reachable
        node = node->next;
    }
}

// Sweep function for garbage collection
void Heap::sweep(Node** head) {
    NodeAddress* currentAddress = globalNodeList;
    NodeAddress* prevAddress = nullptr;

    // Sweeping process
    while (currentAddress != nullptr) {
        Node* node = currentAddress->nodePtr;

        // Check if the node's marked flag is false
        if (node->marked == 0) {
            std::cout << "Sweeping node with data " << node->data << std::endl;

            // Disconnect the node from globalNodeList
            if (prevAddress != nullptr) {
                prevAddress->next = currentAddress->next;
            } else {
                globalNodeList = currentAddress->next;
            }

            NodeAddress* tempAddress = currentAddress;

            currentAddress = currentAddress->next;

            // Free the memory and the global node list entry
            freeMemory(node);
            free(tempAddress);

        } else {
            node->marked = 0; // Reset the mark for future GC cycles
            prevAddress = currentAddress;
            currentAddress = currentAddress->next;
        }
    }
}

// Function to disconnect the last three nodes in the list
void disconnectLastThreeNodes(Node** head) {
    Node* temp = *head;
    int count = 0;

    // Count the total number of nodes
    while (temp != nullptr) {
        count++;
        temp = temp->next;
    }

    if (count < 3) return; // Not enough nodes to disconnect

    temp = *head;
    for (int i = 0; i < count - 3; i++) {
        temp = temp->next;
    }

    std::cout << "Disconnecting last nodes..." << std::endl;
    temp->next = nullptr;
}
