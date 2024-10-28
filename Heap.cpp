#include <iostream>
#include <cstdlib>

#define HEAP_SIZE 1000
#define HEAPSIZE 11

char first_heap[HEAP_SIZE];
char best_heap[HEAP_SIZE];

struct mem_map {
    int start;
    bool free;
    size_t size;
    mem_map *next;
};

mem_map *first_free_list = reinterpret_cast<mem_map*>(first_heap);
mem_map *best_free_list = reinterpret_cast<mem_map*>(best_heap);

void initialize_first() {
    first_free_list->size = HEAP_SIZE - sizeof(mem_map);
    first_free_list->start = 0;
    first_free_list->free = true;
    first_free_list->next = nullptr;
}

void initialize_best() {
    best_free_list->size = HEAP_SIZE - sizeof(mem_map);
    best_free_list->start = 0;
    best_free_list->free = true;
    best_free_list->next = nullptr;
}

void split_block(mem_map *fitting_slot, size_t size) {
    mem_map *new_block = reinterpret_cast<mem_map*>(reinterpret_cast<char*>(fitting_slot) + size + sizeof(mem_map));
    new_block->size = fitting_slot->size - size - sizeof(mem_map);
    new_block->start = fitting_slot->start + size + sizeof(mem_map);
    new_block->free = true;
    new_block->next = fitting_slot->next;

    fitting_slot->size = size;
    fitting_slot->free = false;
    fitting_slot->next = new_block;
}

void print_heap(int best) {
    mem_map *mi_ptr = (best == 0) ? first_free_list : best_free_list;
    std::cout << "\nTotal Heap information: ";
    int start = 0;
    while (mi_ptr != nullptr) {
        if (mi_ptr->free) {
            std::cout << "\nFree: " << mi_ptr->free << ", Size: from start " << mi_ptr->start << " to "
                      << (mi_ptr->start + mi_ptr->size + sizeof(mem_map) - 1);
        } else {
            std::cout << "\nFree: " << mi_ptr->free << ", Size: from start " << mi_ptr->start << " to "
                      << (mi_ptr->start + mi_ptr->size + sizeof(mem_map) - 1);
        }
        start += mi_ptr->size + sizeof(mem_map);
        mi_ptr = mi_ptr->next;
    }
    std::cout << "\n\n";
}

void *firstMalloc(size_t noOfBytes) {
    mem_map *curr = first_free_list;
    while ((curr->size < noOfBytes + sizeof(mem_map) || !curr->free) && curr->next != nullptr) {
        curr = curr->next;
    }

    if (curr->size == noOfBytes && curr->free) {
        curr->free = false;
        return reinterpret_cast<void*>(++curr);
    } else if (curr->size > noOfBytes + sizeof(mem_map) && curr->free) {
        split_block(curr, noOfBytes);
        return reinterpret_cast<void*>(++curr);
    } else {
        std::cout << "Sorry. No sufficient memory to allocate\n";
        return nullptr;
    }
}

void *bestMalloc(size_t noOfBytes) {
    mem_map *curr = best_free_list;
    mem_map *best_fit = nullptr;

    while (curr != nullptr) {
        if (curr->free && curr->size >= noOfBytes && 
            (!best_fit || curr->size < best_fit->size)) {
            best_fit = curr;
        }
        curr = curr->next;
    }

    if (best_fit && best_fit->size == noOfBytes) {
        best_fit->free = false;
        return reinterpret_cast<void*>(++best_fit);
    } else if (best_fit && best_fit->size > noOfBytes + sizeof(mem_map)) {
        split_block(best_fit, noOfBytes);
        return reinterpret_cast<void*>(++best_fit);
    } else {
        std::cout << "Sorry. No sufficient memory to allocate\n";
        return nullptr;
    }
}

void merge(int best) {
    mem_map *curr = (best == 0) ? first_free_list : best_free_list;
    while (curr != nullptr && curr->next != nullptr) {
        if (curr->free && curr->next->free) {
            curr->size += curr->next->size + sizeof(mem_map);
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}

void Free(int start, int best) {
    mem_map *ptr = (best == 1) ? best_free_list : first_free_list;
    while (ptr && ptr->start != start && ptr->next) {
        ptr = ptr->next;
    }
    if (ptr && ptr->start == start) {
        ptr->free = true;
        merge(best);
    } else {
        std::cout << "Please provide a valid start index allocated by Malloc\n";
    }
}

void first_Free_ptr(void *ptr) {
    if (ptr >= first_heap && ptr <= first_heap + HEAP_SIZE) {
        mem_map *curr = reinterpret_cast<mem_map*>(ptr);
        --curr;
        curr->free = true;
        merge(0);
    } else {
        std::cout << "Please provide a valid pointer allocated by firstMalloc\n";
    }
}

void best_Free_ptr(void *ptr) {
    if (ptr >= best_heap && ptr <= best_heap + HEAP_SIZE) {
        mem_map *curr = reinterpret_cast<mem_map*>(ptr);
        --curr;
        curr->free = true;
        merge(1);
    } else {
        std::cout << "Please provide a valid pointer allocated by bestMalloc\n";
    }
}

#include <iostream>
#include <cstdlib>

#define HEAPSIZE 11

struct ListNode {
    int index;
    int start;
    int end;
    int size;
    void *start_address;
    ListNode *next;
};

ListNode *buddyFreeList[HEAPSIZE];
ListNode *allocList = nullptr;
void *heap;

ListNode *initializeNode() {
    heap = std::malloc(1024);
    buddyFreeList[HEAPSIZE - 1] = new ListNode;
    buddyFreeList[HEAPSIZE - 1]->start = 0;
    buddyFreeList[HEAPSIZE - 1]->end = 1023;
    buddyFreeList[HEAPSIZE - 1]->size = 1024;
    buddyFreeList[HEAPSIZE - 1]->index = 10;
    buddyFreeList[HEAPSIZE - 1]->next = nullptr;
    buddyFreeList[HEAPSIZE - 1]->start_address = static_cast<char*>(heap) + buddyFreeList[HEAPSIZE - 1]->start;
    return buddyFreeList[HEAPSIZE - 1];
}

int getIndex(int size) {
    int i = 0;
    int num = 1;
    while (num < size) {
        num *= 2;
        i++;
    }
    return i;
}

ListNode *getAllocBlock(int index) {
    if (index == HEAPSIZE - 1 && buddyFreeList[index] == nullptr) {
        return nullptr;
    } else if (buddyFreeList[index] == nullptr) {
        return getAllocBlock(index + 1);
    } else {
        ListNode *retval = buddyFreeList[index];
        while (retval->next != nullptr) {
            retval = retval->next;
        }
        return retval;
    }
}

void appendToBuddyFreeList(int index, ListNode *newNode) {
    if (buddyFreeList[index] == nullptr) {
        buddyFreeList[index] = newNode;
    } else {
        ListNode *temp = buddyFreeList[index];
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void *updateBuddyFreeList(ListNode *block, int index) {
    if (block->index != index) {
        ListNode *new1 = new ListNode;
        new1->start = (block->end + block->start) / 2 + 1;
        new1->end = block->end;
        new1->size = block->size / 2;
        new1->next = nullptr;
        new1->index = block->index - 1;
        new1->start_address = static_cast<char*>(heap) + new1->start;
        appendToBuddyFreeList(block->index - 1, new1);

        ListNode *new2 = new ListNode;
        new2->start = block->start;
        new2->end = (block->end + block->start) / 2;
        new2->size = block->size / 2;
        new2->next = nullptr;
        new2->index = block->index - 1;
        new2->start_address = static_cast<char*>(heap) + new2->start;
        return updateBuddyFreeList(new2, index);
    } else {
        ListNode *new3 = new ListNode;
        new3->start = block->start;
        new3->end = block->end;
        new3->size = block->size;
        new3->next = nullptr;
        new3->index = block->index;
        new3->start_address = static_cast<char*>(heap) + new3->start;

        if (allocList == nullptr) {
            allocList = new3;
        } else {
            ListNode *temp = allocList;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = new3;
        }
        return new3->start_address;
    }
}

void *buddyMalloc(int allocSize) {
    int index = getIndex(allocSize);
    ListNode *blockToAlloc = getAllocBlock(index);
    void *address = updateBuddyFreeList(blockToAlloc, index);

    ListNode *temp = buddyFreeList[blockToAlloc->index];
    if (temp->next == nullptr) {
        buddyFreeList[blockToAlloc->index] = nullptr;
        delete temp;
    } else {
        while (temp->next != blockToAlloc) {
            temp = temp->next;
        }
        temp->next = nullptr;
        delete blockToAlloc;
    }
    return address;
}

ListNode *removeFromAllocList(void *address) {
    ListNode *curr = allocList;
    ListNode *prev = nullptr;
    while (curr->start_address != address) {
        prev = curr;
        curr = curr->next;
    }
    if (!prev) {
        allocList = allocList->next;
    } else {
        prev->next = curr->next;
    }
    return curr;
}

void insertIntoBuddyFreeList(ListNode *node, int index) {
    node->next = buddyFreeList[index];
    buddyFreeList[index] = node;
}

ListNode *findBuddy(int start_index, int index) {
    ListNode *curr = buddyFreeList[index];
    ListNode *prev = nullptr;
    while (curr != nullptr && curr->start != start_index) {
        prev = curr;
        curr = curr->next;
    }
    if (curr != nullptr && prev) {
        prev->next = curr->next;
    }
    return curr;
}

void coalesceBuddies(ListNode *node, int index) {
    int buddyNum = node->start / node->size;
    int buddyStartIndex = (buddyNum % 2 == 0) ? node->start + node->size : node->start - node->size;

    ListNode *buddy = findBuddy(buddyStartIndex, index);
    if (buddy != nullptr) {
        ListNode *newNode = new ListNode;
        newNode->start = std::min(node->start, buddy->start);
        newNode->end = std::max(node->end, buddy->end);
        newNode->size = node->size * 2;
        newNode->index = getIndex(newNode->size);
        newNode->next = nullptr;
        newNode->start_address = static_cast<char*>(heap) + newNode->start;
        delete buddy;
        buddyFreeList[index] = buddyFreeList[index]->next;
        delete node;
        insertIntoBuddyFreeList(newNode, newNode->index);
        coalesceBuddies(newNode, index + 1);
    }
}

void buddyFree(int start) {
    ListNode *f = allocList;
    while (f->start != start && f->next != nullptr) {
        f = f->next;
    }
    if (f->start != start) {
        std::cout << "No start index " << start << " found\n";
        return;
    }
    ListNode *freed = removeFromAllocList(f->start_address);
    f->start_address = nullptr;
    int index = getIndex(freed->size);
    insertIntoBuddyFreeList(freed, index);
    coalesceBuddies(freed, index);
}

void buddyFreePtr(void **address) {
    ListNode *freed = removeFromAllocList(*address);
    *address = nullptr;
    int index = getIndex(freed->size);
    insertIntoBuddyFreeList(freed, index);
    coalesceBuddies(freed, index);
}

void printLists() {
    std::cout << "\n----------List printing----------\n";
    std::cout << "Free list:\n";
    for (int i = 0; i < HEAPSIZE; i++) {
        ListNode *temp = buddyFreeList[i];
        std::cout << "buddyFreeList[" << i << "] --> ";
        while (temp != nullptr) {
            std::cout << "(" << temp->start << "," << temp->end << ") --> ";
            temp = temp->next;
        }
        std::cout << "NULL\n";
    }
    std::cout << "\nAlloc list:\n";
    ListNode *tmp = allocList;
    std::cout << "allocList --> ";
    while (tmp != nullptr) {
        std::cout << "(" << tmp->start << "," << tmp->end << ") --> ";
        tmp = tmp->next;
    }
    std::cout << "NULL\n";
    std::cout << "\n------------------------------\n";
}



#include <iostream>

int main()
{
    initializeNode(); // Initialization of buddy system node
    initialize_best();
    initialize_first();
    int choice;
    bool done = false;
    
    while (!done)
    {
        std::cout << "\n---------Heap Management can allocate memory in 3 allocation systems ------------\n\n";
        std::cout << "****Enter 1 for first fit____Enter 2 for best fit____Enter 3 for Buddy System****\n\n";
        std::cout << "************Enter 4 for printing all lists_____Enter 5 for exiting***************\n\n";
        std::cin >> choice;

        if (choice == 1)
        {
            print_heap(0);
            std::cout << "Enter 4 for allocation\nEnter 5 for deallocation\n";
            int ch1;
            std::cin >> ch1;

            if (ch1 == 4)
            {
                int size_data;
                std::cout << "Enter size of data: ";
                std::cin >> size_data;
                firstMalloc(size_data);
                print_heap(0);
            }
            else
            {
                std::cout << "Enter start index of allocation you want to delete: ";
                int start;
                std::cin >> start;
                Free(start, 0);
                print_heap(0);
            }
        }
        else if (choice == 2)
        {
            print_heap(1);
            std::cout << "Enter 4 for allocation\nEnter 5 for deallocation\n";
            int ch1;
            std::cin >> ch1;

            if (ch1 == 4)
            {
                int size_data;
                std::cout << "Enter size of data: ";
                std::cin >> size_data;
                bestMalloc(size_data);
                print_heap(1);
            }
            else
            {
                std::cout << "Enter start index of allocation you want to delete: ";
                int start;
                std::cin >> start;
                Free(start, 1);
                print_heap(1);
            }
        }
        else if (choice == 3)
        {
            printLists();
            std::cout << "Enter 4 for allocation\nEnter 5 for deallocation\n";
            int ch;
            std::cin >> ch;

            if (ch == 4)
            {
                int size_data;
                std::cout << "Enter size of data: ";
                std::cin >> size_data;
                buddyMalloc(size_data);
                printLists();
            }
            else
            {
                std::cout << "Enter start index of allocation you want to delete: ";
                int start;
                std::cin >> start;
                buddyFree(start);
                printLists();
            }
        }
        else if (choice == 4)
        {
            print_heap(0);
            print_heap(1);
            printLists();
        }
        else
        {
            done = true;
        }
    }

    return 0;
}
