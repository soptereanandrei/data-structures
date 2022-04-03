#include "ccheap.h"
#include "common.h"

static int HpCreate(CC_HEAP** Heap);
static int IsGreater(int x, int y);
static int IsLower(int x, int y);
static int Crossing(CC_HEAP* Heap, int Node, int Value, int (*Comp)(int, int));

int HpCreateMaxHeap(CC_HEAP **MaxHeap, CC_VECTOR* InitialElements)
{
    CC_HEAP *newHeap = NULL;

    if (MaxHeap == NULL)
    {
        return -1;
    }

    if (HpCreate(&newHeap) < 0)
    {
        return -1;
    }

    newHeap->IsMaxHeap = 1;

    if (InitialElements != NULL)
    {
        for (int i = 0; i < InitialElements->Count; i++)
        {
            if (HpInsert(newHeap, InitialElements->Array[i]) < 0)
            {
                HpDestroy(&newHeap);
                return -1;
            }
        }
    }

    *MaxHeap = newHeap;

    return 0;
}

int HpCreateMinHeap(CC_HEAP **MinHeap, CC_VECTOR* InitialElements)
{
    CC_HEAP* newHeap = NULL;

    if (MinHeap == NULL)
    {
        return -1;
    }

    if (HpCreate(&newHeap) < 0)
    {
        return -1;
    }

    newHeap->IsMaxHeap = 0;

    if (InitialElements != NULL)
    {
        for (int i = 0; i < InitialElements->Count; i++)
        {
            if (HpInsert(newHeap, InitialElements->Array[i]) < 0)
            {
                HpDestroy(&newHeap);
                return -1;
            }
        }
    }

    *MinHeap = newHeap;

    return 0;
}

int HpDestroy(CC_HEAP **Heap)
{

    if (Heap == NULL)
    {
        return -1;
    }

    if (VecDestroy(&(*Heap)->Vector) < 0)
    {
        return -1;
    }

    free(*Heap);
    *Heap = NULL;

    return 0;
}

int HpInsert(CC_HEAP *Heap, int Value)
{
    int node = 0;
    int parentNode = 0;
    int temp = 0;
    int (*comp)(int, int) = NULL;
    
    if (Heap == NULL)
    {
        return -1;
    }

    if (Heap->IsMaxHeap)
    {
        comp = IsGreater;
    }
    else
    {
        comp = IsLower;
    }

    if (VecInsertTail(Heap->Vector, Value) < 0)
    {
        return -1;
    }

    node = Heap->Vector->Count - 1;
    parentNode = (node - 2) / 2;
    while (parentNode >= 0 && comp(Heap->Vector->Array[node], Heap->Vector->Array[parentNode]))
    {
        temp = Heap->Vector->Array[node];
        Heap->Vector->Array[node] = Heap->Vector->Array[parentNode];
        Heap->Vector->Array[parentNode] = temp;
        node = parentNode;
        parentNode = (node - 2) / 2;
    }

    return 0;
}

int HpRemove(CC_HEAP *Heap, int Value)
{
    int (*comp)(int, int) = NULL;

    if (Heap == NULL)
    {
        return -1;
    }

    if (Heap->IsMaxHeap)
    {
        comp = IsGreater;
    }
    else
    {
        comp = IsLower;
    }

    if (Crossing(Heap, 0, Value, comp) < 0)
    {
        return -1;
    }

    return 0;
}

int HpGetExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
    if (Heap == NULL)
    {
        return -1;
    }
    if (ExtremeValue == NULL)
    {
        return -1;
    }

    *ExtremeValue = Heap->Vector->Array[0];

    return 0;
}

int HpPopExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
    int node = 0;
    int leftChild = node * 2 + 1;
    int rightChild = node * 2 + 2;
    int (*comp)(int, int) = NULL;
    int temp = 0;

    if (Heap == NULL)
    {
        return -1;
    }
    if (ExtremeValue == NULL)
    {
        return -1;
    }

    *ExtremeValue = Heap->Vector->Array[0];

    if (Heap->IsMaxHeap)
    {
        comp = IsGreater;
    }
    else
    {
        comp = IsLower;
    }

    temp = Heap->Vector->Array[Heap->Vector->Count - 1];
    if (VecRemoveByIndex(Heap->Vector, Heap->Vector->Count - 1) < 0)
    {
        return -1;
    }
    Heap->Vector->Array[0] = temp;

    while (leftChild < Heap->Vector->Count && rightChild < Heap->Vector->Count &&
        (comp(Heap->Vector->Array[leftChild], Heap->Vector->Array[node]) || comp(Heap->Vector->Array[rightChild], Heap->Vector->Array[node])))
    {
        if (comp(Heap->Vector->Array[leftChild], Heap->Vector->Array[rightChild]))
        {
            temp = Heap->Vector->Array[leftChild];
            Heap->Vector->Array[leftChild] = Heap->Vector->Array[node];
            Heap->Vector->Array[node] = temp;
            node = leftChild;
        }
        else
        {
            temp = Heap->Vector->Array[rightChild];
            Heap->Vector->Array[rightChild] = Heap->Vector->Array[node];
            Heap->Vector->Array[node] = temp;
            node = rightChild;
        }
        leftChild = node * 2 + 1;
        rightChild = node * 2 + 2;
    }

    return 0;
}

int HpGetElementCount(CC_HEAP *Heap)
{
    if (Heap == NULL)
    {
        return -1;
    }

    return Heap->Vector->Count;
}

int HpSortToVector(CC_HEAP *Heap, CC_VECTOR* SortedVector)
{
    if (Heap == NULL)
    {
        return -1;
    }
    if (SortedVector == NULL)
    {
        return -1;
    }

    if (VecClear(SortedVector) < 0)
    {
        return -1;
    }
    if (VecAppend(SortedVector, Heap->Vector) < 0)
    {
        return -1;
    }

    int isSort = 1;

    do
    {
        isSort = 1;
        for (int i = 0; i < SortedVector->Count - 1; i++)
        {
            if (SortedVector->Array[i] > SortedVector->Array[i + 1])
            {
                int temp = SortedVector->Array[i];
                SortedVector->Array[i] = SortedVector->Array[i + 1];
                SortedVector->Array[i + 1] = temp;
                isSort = 0;
            }
        }
    } while (!isSort);

    return 0;
}

static int HpCreate(CC_HEAP** Heap)
{
    CC_HEAP* newHeap = NULL;

    if (Heap == NULL)
    {
        return -1;
    }

    newHeap = (CC_HEAP*)malloc(sizeof(CC_HEAP));
    if (newHeap == NULL)
    {
        return -1;
    }

    memset(newHeap, 0, sizeof(*newHeap));

    if (VecCreate(&newHeap->Vector) < 0)
    {
        free(newHeap);
        return -1;
    }

    *Heap = newHeap;

    return 0;
}

static int Crossing(CC_HEAP* Heap, int Node, int Value, int (*Comp)(int, int))
{
    int node = -1;
    int leftChild = -1;
    int rightChild = -1;

    if (Heap == NULL)
    {
        return -1;
    }
    if (Comp == NULL)
    {
        return -1;
    }
    if (Node < 0)
    {
        return -1;
    }

    if (Node >= Heap->Vector->Count)
    {
        return 0;
    }

    if (Heap->Vector->Array[Node] == Value)
    {
        int temp = 0;
        node = Node;
        leftChild = node * 2 + 1;
        rightChild = node * 2 + 2;
        
        temp = Heap->Vector->Array[Heap->Vector->Count - 1];
        if (VecRemoveByIndex(Heap->Vector, Heap->Vector->Count - 1) < 0)
        {
            return -1;
        }
        Heap->Vector->Array[Node] = temp;

        while (leftChild < Heap->Vector->Count && rightChild < Heap->Vector->Count &&
            (Comp(Heap->Vector->Array[leftChild], Heap->Vector->Array[node]) || Comp(Heap->Vector->Array[rightChild], Heap->Vector->Array[node])))
        {
            if (Comp(Heap->Vector->Array[leftChild], Heap->Vector->Array[rightChild]))
            {
                temp = Heap->Vector->Array[leftChild];
                Heap->Vector->Array[leftChild] = Heap->Vector->Array[node];
                Heap->Vector->Array[node] = temp;
                node = leftChild;
            }
            else
            {
                temp = Heap->Vector->Array[rightChild];
                Heap->Vector->Array[rightChild] = Heap->Vector->Array[node];
                Heap->Vector->Array[node] = temp;
                node = rightChild;
            }
            leftChild = node * 2 + 1;
            rightChild = node * 2 + 2;
        }
    }

    leftChild = Node * 2 + 1;
    rightChild = Node * 2 + 2;
    if (!Comp(Value, Heap->Vector->Array[leftChild]))
    {
        if (Crossing(Heap, leftChild, Value, Comp) < 0)
        {
            return -1;
        }
    }

    if (!Comp(Value, Heap->Vector->Array[rightChild]))
    {
        if (Crossing(Heap, rightChild, Value, Comp) < 0)
        {
            return -1;
        }
    }

    return 0;
}

static int IsGreater(int x, int y)
{
    if (x > y)
        return 1;
    return 0;
}

static int IsLower(int x, int y)
{
    if (x < y)
        return 1;
    return 0;
}