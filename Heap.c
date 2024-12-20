//  heap management

#include<stdio.h>
#include<stdlib.h>
#include <time.h>
// Total size of the heap
#define HEAP_SIZE 4096
// Fibonacci list for size of free-Lists
#define FibonacciList {1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181}
// Number of elements in FibonacciList
#define nFiboElements 18
#define fibonacciArray int fibonacci[nFiboElements] = FibonacciList
#define newLine printf("\n")
// struct for linked list
struct Node
{
	int data;
	int refCount;
	int marked;
	struct Node * next;
};

struct NodeAddress {
    struct Node* nodePtr;
    struct NodeAddress* next;
} *globalNodeList = NULL;

// Function used for Linked list
void insertAtStart(struct Node **ptr, int data);
void deleteList(struct Node *ptr);
void printLL(struct Node *ptr);
void printglobal(struct NodeAddress *ptr);
void reversal(struct Node **hptr);
void randomlyDisconnectNode(struct Node** head);
void refCountGarbageCollect(struct Node** head);
void mark(struct Node* node);
void sweep(struct Node** head);
void disconnectLastThreeNodes(struct Node** head);


// structure is used for heap management
struct Heap
{
    void *heap; // store the address to the heap memory
    // Linked List for managing free-lists
    struct freeListNode
    {
        // stores the address of the beginning of free space
        void *pointer;
        // Store the next freeListNode's address
        struct freeListNode *next;
    };
    // freeList Implemented with linked list
    struct freeList
    {
        // stores the array of array of free list of block size 1, 2, 3. 5, 8 ......
        struct freeListNode *list_pointers[nFiboElements];
        // stores the number of elements in free-list corresponds index of list_pointers
        int number_of_elements[nFiboElements];

    }freeList; // creating an instance

    // the size of metadata required in bytes
    int metadata;
    
}Heap; // creating an instance

int total();
void freeMemory(void *pointer);
void create_heap();
void *alloc(int size);
void add2freeList(struct freeListNode *lptr[], int size, void *pointer);
void print();
int getHighFiboIndex(int n);
int getLowFiboIndex(int n);

int main (void)
{
    srand(time(NULL));
    
    create_heap();
    printf("printing free list after allocation...\n");
    print();
    newLine;
    struct Node *head = NULL;
    for(int i = 0; i < 50; i++)
    {
        insertAtStart(&head,i+1);
    }
    printf("printing linked list...\n");
	printLL(head);
    newLine;
	reversal(&head);
    printf("printing reversed linked list...\n");
	printLL(head);
    newLine;
    printf("printing free list after creating linked list...\n");
    print();
    newLine;
    printf("---------------------------------------------------------");
    newLine;
    printf("Reference Counting Garbage Collection!");
    newLine;
    printf("---------------------------------------------------------");
    newLine;
    randomlyDisconnectNode(&head);
    newLine;
    printLL(head);
    printf("\nCollecting Garbage\n");
    newLine;
    refCountGarbageCollect(&head);
    newLine;
    printf("Done Collecting Garbage\n");
    newLine;
    
    printf("printing linked list after reference counting garbage collection...\n");
    printLL(head);
    newLine;
    printf("printing free list after reference counting garbage collection...\n");
    print();
    newLine;
    printf("---------------------------------------------------------");
    newLine;
    printf("Mark and Sweep Garbage Collection!");
    newLine;
    printf("---------------------------------------------------------");
    newLine;
    disconnectLastThreeNodes(&head);
    newLine;
    printLL(head);
    newLine;
    mark(head);
    printf("Marking Done...\n\n");
    sweep(&head);
    printf("After Sweeping...\n");
    newLine;
    
    printf("printing linked list after mark-and-sweep garbage collection...\n");
    printLL(head);
    newLine;
    printf("printing free list after mark-and-sweep garbage collection...\n");
    print();
    newLine;
    
    printf("---------------------------------------------------------");
    newLine;
    printf("Deleting Linked List!");
    newLine;
    printf("---------------------------------------------------------");
    newLine;
	deleteList(head);
    printf("printing free list after deleting linked list...\n");
    print();
    newLine;
    free(Heap.heap); // freeing heap memory
    
}

int getHighFiboIndex(int n)
{
    // list of fibonacci needed to make computations easy
    // if not used then we have to use fibonacci generating function but would be inefficient as fibonacci series is multiple times in every function
    fibonacciArray;
    int counter = 0; // for calculating index
    while (n > fibonacci[counter] && counter < nFiboElements)
    {
        counter++;
    }
    // returns maximum possible usable index
    (counter == nFiboElements) ? (counter = nFiboElements-1) : (counter = counter);
    return counter;
}

int getLowFiboIndex(int n)
{
    fibonacciArray;
    int counter = 0, prev = 0;
    while (n >= fibonacci[counter] && counter < nFiboElements) // if there is no = sign then say 3 will split into 1 + 2 even if 3 is also a fibonacci
    {
        prev = counter;
        counter++;
    }
    return prev;
}

// Prints the whole freelist
void print()
{
    fibonacciArray;
    for(int i = 0; i < nFiboElements; i++) // iterating over all the free list of different sizes
    {
        struct freeListNode *ptr = Heap.freeList.list_pointers[i];
        printf("%d size free list has %d elements, ",fibonacci[i], Heap.freeList.number_of_elements[i]);
        while(ptr != NULL)
        {
            printf("%p ",ptr->pointer);
            ptr = ptr->next;
        }
        printf("\n");
    }
}

// Function to add free space used for internal implementation of alloc and freeMemory function
void add2freeList(struct freeListNode *lptr[], int size, void *pointer)
{
    fibonacciArray;
    int found = 0; // if a buddy is found variable is changed to 1 and loop is exited
    struct freeListNode *traverse = NULL, *prev = NULL;
    if(size > 0) // making sure if memory space to be freed is greater than 0
    {
        // going through all the lists
        for(int i = 0; i < nFiboElements && !found; i++)
        {
            // Finding buddies
            // checking if list is not empty
            if(Heap.freeList.number_of_elements[i] != 0)
            {
                // prev is required to reform free list
                prev = NULL;
                traverse =  lptr[i];
                // going through the end of the list or until buddy is found
                while(traverse != NULL && !found)
                {
                    // this function is called recursively to find buddy of the combined free space found
                    // if buddy is left to the space to be freed
                    if(traverse->pointer + fibonacci[i] == pointer)
                    {
                        // changing free list data
                        (prev != NULL) ? (prev->next = traverse->next) : (lptr[i] = traverse->next);
                        Heap.freeList.number_of_elements[i]--;
                        // adding combined space to free list
                        add2freeList(lptr,(size + fibonacci[i]), traverse->pointer);
                        free(traverse);
                        found = 1;
                    }
                    else if(pointer + size == traverse->pointer) // if buddy is right to the space to be freed
                    {
                        // changing free list data
                        (prev != NULL) ? (prev->next = traverse->next) : (lptr[i] = traverse->next);
                        Heap.freeList.number_of_elements[i]--;
                        // adding combined space to free list
                        add2freeList(lptr,(size + fibonacci[i]), pointer);
                        free(traverse);
                        found = 1;
                    }
                    // moving forward
                    prev = traverse;
                    traverse = traverse->next;
                }

            }
        }
        // if there are no more buddies then space is divided into fibonacci sized chunks and storing in free list
        int total_space, index;
        struct freeListNode *temp;
        if(!found)
        {
            total_space = size;
            while(total_space > 0)
            {
                index = getLowFiboIndex(total_space); // index gives index in freelist that stores largest chunk of unallocated free space
                size = fibonacci[index]; // getting size
                temp = (struct freeListNode *)malloc(sizeof(struct freeListNode));
                temp->next = lptr[index];
                temp->pointer = pointer;
                lptr[index] = temp;
                Heap.freeList.number_of_elements[index]++; // increasing number of the elements
                pointer = pointer + fibonacci[index]; // moving to next space
                total_space -= size; // calculating remaining unallocated space
            }
        }
    }
    
}

// alloction function used same as malloc from stdlib library
void *alloc(int size)
{
    fibonacciArray;
    // need to allocate extra space for metadata
    int index = getHighFiboIndex(size + Heap.metadata); // searching for index of free list whose space is greater than required spce
    int success = 0;
    void *alloc_address = NULL; // address allocated
    // if successfully allocated then exit or if no space is available
    while(!success && index < nFiboElements)
    {
        // if list is empty move to next list
        if(Heap.freeList.number_of_elements[index] == 0)
        {
            index++;
        }
        else
        {
            // taking first element from the list
            struct freeListNode *address = Heap.freeList.list_pointers[index];
            // removing the current element from the free list
            Heap.freeList.number_of_elements[index]--;
            Heap.freeList.list_pointers[index] = address->next;
            void *pointer = address->pointer; // getting the address of free space
            int *payload = (int *)(pointer); // storing the metadata
            *payload = size;
            alloc_address = pointer + Heap.metadata; // allocated address will start after the memory allocated for metadata
            // finding remaining memory and adding to free list
            int remainder_space = fibonacci[index] - (size + Heap.metadata);
            void *nextPointer = pointer + size + Heap.metadata;
            free(address);
            if(remainder_space > 0)
            {
                add2freeList(Heap.freeList.list_pointers,remainder_space, nextPointer);
            }
            success = 1;
        }
    }
    return alloc_address;
}

// freeing memory used same as free from stdlib library
void freeMemory(void *pointer)
{
    // finds the size from the metadata
    int *size = pointer - Heap.metadata;
    // add the space combined with metadata to free list
    add2freeList(Heap.freeList.list_pointers, (*size + Heap.metadata), size);
}

void create_heap()
{
    // allocating memory of 1024 bytes for this demonstration
    Heap.heap = (void *) malloc(HEAP_SIZE);
    // setting to zero elements in each block initially
    for(int i = 0; i < nFiboElements; i++)
    {
        Heap.freeList.number_of_elements[i] = 0;
        Heap.freeList.list_pointers[i] = NULL;
    }
    // in initial free list the whole memory is divided into 2 free blocks of size 512
    add2freeList(Heap.freeList.list_pointers,HEAP_SIZE, Heap.heap);
    Heap.metadata = sizeof(int);

}

int total()
{
    fibonacciArray;
    int sum = 0;
    for(int i = 0; i < nFiboElements; i++)
    {
        sum += fibonacci[i] * Heap.freeList.number_of_elements[i];
    }
    return sum;
}

void addToGlobalList(struct Node* node) {
    struct NodeAddress* newAddress = (struct NodeAddress*)malloc(sizeof(struct NodeAddress));
    if (newAddress != NULL) {
        newAddress->nodePtr = node;
        newAddress->next = globalNodeList;
        globalNodeList = newAddress;
    }
}


// Function definition for linked list
void insertAtStart(struct Node **ptr, int data)
{
	struct Node *temp = NULL;
	temp = (struct Node *) alloc(sizeof(struct Node));
	if(temp != NULL)
	{
		temp->data = data;
		temp->refCount = 1;
		temp->marked = 0;
		temp->next = *ptr;
		if (*ptr != NULL) {
            (*ptr)->refCount++; // Increase refCount for the next node
        }
		*ptr = temp;
		addToGlobalList(temp);
	}
    else
    {
        printf("cannot allocate memory\n");
    }
}

void printglobal(struct NodeAddress *ptr)
{
    printf("Peinting Gloabal\n");
    while (ptr != NULL)
    {
        printf("%d ",ptr->nodePtr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

void printLL(struct Node *ptr)
{
    while (ptr != NULL)
    {
        printf("%d ",ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

void reversal(struct Node **hptr)
{

	struct Node *ptr = *hptr, *cur = (*hptr)->next, *first = *hptr;
	struct Node *temp = NULL;
	if(ptr != NULL)
	{
		while (cur != NULL)
		{
			temp = cur->next;
			cur->next = ptr;
			first->next = temp;
			ptr = cur;
			cur = temp;
		}
		*hptr = ptr;
	}
}

void deleteList(struct Node *ptr)
{
	struct Node *temp;
	while(ptr != NULL)
	{
		temp = ptr->next;
		freeMemory(ptr);
		ptr = temp;
	}
}

void randomlyDisconnectNode(struct Node** head) {
    int count = 0;
    struct Node* temp = *head;
    struct Node* prev = NULL;

    // Count the number of nodes
    while (temp != NULL) {
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
    printf("Randomly dereferencing node with data %d\n", temp->data);
    temp->refCount = 0;

    if (prev != NULL) {
        prev->next = temp->next;
    } else {
        *head = temp->next;
    }
}


// Garbage collection function to delete nodes with refCount zero
void refCountGarbageCollect(struct Node** head) {
    struct NodeAddress* currentAddress = globalNodeList;
    struct NodeAddress* prevAddress = NULL;

    while (currentAddress != NULL) {
        struct Node* node = currentAddress->nodePtr;

        // Check if the node's refCount is zero
        if (node->refCount == 0) {
            printf("Garbage collecting node with data %d\n", node->data);
           if (prevAddress != NULL) {
                prevAddress->next = currentAddress->next;
            } else {
                globalNodeList = currentAddress->next;
            }
            struct NodeAddress* tempAddress = currentAddress;
            
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

void mark(struct Node* node) {
    while (node != NULL && node->marked == 0) {
        node->marked = 1; // Mark the node as reachable
        node = node->next;
    }
}

void sweep(struct Node** head) {
    struct NodeAddress* currentAddress = globalNodeList;
    struct NodeAddress* prevAddress = NULL;
    // first 
    while (currentAddress != NULL) {
        struct Node* node = currentAddress->nodePtr;
        
        // Check if the node's marked flag is false
        if (node->marked == 0) {
            printf("Sweeping node with data %d\n", node->data);
            
            // Disconnect the node from globalNodeList
            if (prevAddress != NULL) {
                prevAddress->next = currentAddress->next;
            } else {
                globalNodeList = currentAddress->next;
            }
            
            struct NodeAddress* tempAddress = currentAddress;
            
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

void disconnectLastThreeNodes(struct Node** head) {
    struct Node* temp = *head;
    int count = 0;
    
    // Count the total number of nodes
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    
    if (count < 3) return; // Not enough nodes to disconnect
    
    temp = *head;
    for (int i = 0; i < count - 3; i++) {
        temp = temp->next;
    }
    
    printf("Disconnecting last nodes...\n");
    temp->next = NULL;
}
