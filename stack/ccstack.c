#include "ccstack.h"
#include "common.h"

#define INITIAL_SIZE 100

static int StArrayRealloc(CC_STACK* Stack, int Size);

int StCreate(CC_STACK **Stack)
{
    CC_STACK *newStack;

    if (Stack == NULL)
    {
        return -1;
    }

    newStack = (CC_STACK*)malloc(sizeof(CC_STACK));
    if (newStack == NULL)
    {
        return -1;
    }

    memset(newStack, 0, sizeof(*newStack));

    int *newArray = (int*)malloc(sizeof(int) * INITIAL_SIZE);
    if (newArray == NULL)
    {
        free(newStack);
        return -1;
    }

    newStack->Array = newArray;
    newStack->Count = 0;
    newStack->Size = INITIAL_SIZE;
    *Stack = newStack;

    return 0;
    
}

int StDestroy(CC_STACK **Stack)
{
    CC_STACK *stack = NULL;

    if (Stack == NULL)
    {
        return -1;
    }

    stack = *Stack;
    if (stack == NULL)
    {
        return -1;
    }

    free(stack->Array);
    free(stack);
    *Stack = NULL;

    return 0;
}

int StPush(CC_STACK *Stack, int Value)
{
    if (Stack == NULL)
    {
        return -1;
    }

    if (Stack->Count >= Stack->Size)
    {
        if (StArrayRealloc(Stack, Stack->Size * 2) < 0)
        {
            return -1;
        }
    }

    Stack->Array[Stack->Count] = Value;
    Stack->Count++;

    return 0;
}

int StPop(CC_STACK *Stack, int *Value)
{
    if (Stack == NULL || Value == NULL)
    {
        return -1;
    }

    if (Stack->Count <= 0)
    {
        return -1;
    }

    *Value = Stack->Array[Stack->Count - 1];
    Stack->Count--;

    return 0;
}

int StPeek(CC_STACK *Stack, int *Value)
{
    if (Stack == NULL || Value == NULL)
    {
        return -1;
    }

    if (Stack->Count <= 0)
    {
        return -1;
    }

    *Value = Stack->Array[Stack->Count - 1];

    return 0;
}

int StIsEmpty(CC_STACK *Stack)
{
    if (Stack == NULL)
    {
        return -1;
    }

    if (Stack->Count == 0)
    {
        return 1;
    }
    return 0;
}

int StGetCount(CC_STACK *Stack)
{
    if (Stack == NULL)
    {
        return -1;
    }
    return Stack->Count;
}

int StClear(CC_STACK *Stack)
{
    if (Stack == NULL)
    {
        return -1;
    }

    if (StArrayRealloc(Stack, INITIAL_SIZE) < 0)
    {
        return -1;
    }

    Stack->Count = 0;
    Stack->Size = INITIAL_SIZE;

    return 0;
}

int StPushStack(CC_STACK *Stack, CC_STACK *StackToPush)
{
    if (Stack == NULL || StackToPush == NULL)
    {
        return -1;
    }

    if (Stack->Count + StackToPush->Count > Stack->Size)
    {
        if (StArrayRealloc(Stack, Stack->Count + StackToPush->Count + 20) < 0)
        {
            return -1;
        }
    }

    for (int i = 0; i < StackToPush->Count; i++)
    {
        Stack->Array[Stack->Count + i] = StackToPush->Array[i];
    }
    Stack->Count += StackToPush->Count;

    if (StClear(StackToPush) < 0)
    {
        return -2;
    }

    return 0;
}

static int StArrayRealloc(CC_STACK* Stack, int Size)
{
    int* newArray = NULL;

    if (Stack == NULL)
    {
        return -1;
    }
    if (Size < 0)
    {
        return -1;
    }

    newArray = (int*)realloc(Stack->Array, sizeof(int) * Size);
    if (newArray == NULL)
    {
        return -1;
    }
    Stack->Array = newArray;
    Stack->Size = Size;

    return 0;
}