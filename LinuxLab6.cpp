#include <iostream>

#define MEMORY_SIZE 20000

char memory[MEMORY_SIZE];

struct block {
    size_t size;
    bool free;
    struct block* next;
} *freeList = static_cast<block*>(static_cast<void*>(memory));

void init()
{
    freeList->size = MEMORY_SIZE - sizeof(struct block);
    freeList->free = true;
    freeList->next = nullptr;
}


void split(struct block* block, size_t size)
{
    struct block* newBlock = (struct block*)((char*)block + sizeof(struct block) + size);
    newBlock->size = block->size - size - sizeof(struct block);
    newBlock->free = true;
    newBlock->next = block->next;
    block->size = size;
    block->free = false;
    block->next = newBlock;
}
void* _malloc(size_t size)
{
    if(freeList->size == 0)
    {
        init();
        std::cout << "Memory initialized!\n";
    }
    struct block* current = freeList;
    while ((current->size < size || !current->free) && current->next != nullptr)
    {
        current = current->next;
        std::cout << "Memory block checked\n";
    }

    if(current->size == size)
    {
        current->free = false;
        std::cout << "Exact size allocated\n";
        return static_cast<void*>(++current);
    }
    if(current->size > size + sizeof(struct block))
    {
        split(current, size);
        std::cout << "Allocated with split\n";
        return static_cast<void*>(++current);
    }
    std::cout << "Not enough memory\n";
    return nullptr;
}

//���������� ��������� ����� ������ � 1
void merge()
{
    struct block* current = freeList;
    while (current->next != nullptr)
    {
        if (current->free && current->next->free)
        {
            current->size += current->next->size + sizeof(struct block);
            current->next = current->next->next;
            continue;
        }
        current = current->next;
    }
}
void _free(void* pointer)
{
    if ((void*)memory <= pointer && pointer <= (void*)(memory + MEMORY_SIZE))
    {
        struct block* current = (struct block*)pointer;
        --current; 
        current->free = true;
        merge();
        pointer = nullptr;
    }
    else
        std::cout << "Invalid pointer provided\n";
}
void* _realloc(void* pointer, size_t size)
{
    _free(pointer);
    if (!pointer)
        return _malloc(size);
    void* result = _malloc(size);
    if (!result)
        return nullptr;
    memcpy(result, pointer, ((struct block*)pointer)->size);
    return result;
}

void main()
{
    int* arr = static_cast<int*>(_malloc(MEMORY_SIZE - 1000 - sizeof(struct block) * 2));

    for (size_t i = 0; i < 100; i++)
        arr[i] = i;
    for (size_t i = 0; i < 100; i++)
        std::cout << arr[i] << '\t';
    std::cout << std::endl;
    _free(arr);
    int* arr1 = static_cast<int*>(_malloc(1000));
    for (size_t i = 0; i < 100; i++)
        arr1[i] = i;
    for (size_t i = 0; i < 100; i++)
        std::cout << arr1[i] << '\t';
    std::cout << std::endl;

    int* arr2 = static_cast<int*>(_malloc(4));
    arr2 = static_cast<int*>(_realloc(arr2, 10004));
    arr2[0] = 1;
    _free(arr1);
    _free(arr2);

    int* arr3 = (int*)_realloc(nullptr, MEMORY_SIZE - sizeof(struct block));
    arr3[0] = 2;
    _free(arr3);
    system("pause");
}
