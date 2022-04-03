#include "ccvector.h"
#include "common.h"

#define INITIAL_SIZE    100

static int VecRealloc(CC_VECTOR *Vector, int Size);

int VecCreate(CC_VECTOR **Vector)
{
    CC_VECTOR *vec = NULL;

    if (NULL == Vector)
    {
        return -1;
    }

    vec = (CC_VECTOR*)malloc(sizeof(CC_VECTOR));
    if (NULL == vec)
    {
        return -1;
    }

    memset(vec, 0, sizeof(*vec));

    vec->Count = 0;
    vec->Size = INITIAL_SIZE;
    vec->Array = (int*)malloc( sizeof(int) * INITIAL_SIZE );
    if (NULL == vec->Array) 
    {
        free(vec);
        return -1;
    }

    *Vector = vec;

    return 0;
}

int VecDestroy(CC_VECTOR **Vector)
{   /* original
    CC_VECTOR *vec = *Vector;

    if (NULL == Vector)
    {
        return -1;
    }

    free(vec->Array);
    free(vec);

    *Vector = NULL;

    return -1; */
    CC_VECTOR* vec = NULL;

    if (Vector == NULL)
    {
        return -1;
    }

    vec = *Vector;

    if (vec == NULL)
    {
        return -1;
    }

    free(vec->Array);
    free(vec);

    *Vector = NULL;

    return 0;
}

int VecInsertTail(CC_VECTOR *Vector, int Value)
{
    if (NULL == Vector)
    {
        return -1;
    }

    if (Vector->Count >= Vector->Size)
    {
        if (VecRealloc(Vector, Vector->Size * 2) < 0)
        {
            return -1;
        }
    }

    Vector->Array[Vector->Count] = Value;
    Vector->Count++;

    return 0;
}

int VecInsertHead(CC_VECTOR *Vector, int Value)
{
    if (NULL == Vector)
    {
        return -1;
    }

    if (Vector->Count >= Vector->Size)
    {
        if (VecRealloc(Vector, Vector->Size * 2) < 0)
        {
            return -1;
        }
    }

    for (int i = Vector->Count-1; i >= 0; i--)
    {
        Vector->Array[i + 1] = Vector->Array[i];
    }
    Vector->Array[0] = Value;
    Vector->Count++;

    return 0;
}

int VecInsertAfterIndex(CC_VECTOR *Vector, int Index, int Value)
{
    if (NULL == Vector)
    {
        return -1;
    }

    if (Index < 0 || Index >= Vector->Count)
    {
        return -1;
    }

    if (Vector->Count >= Vector->Size)
    {
        if (VecRealloc(Vector, Vector->Size * 2) < 0)
        {
            return -1;
        }
    }

    for (int i = Vector->Count - 1; i >= Index; i--)
    {
        Vector->Array[i + 1] = Vector->Array[i];
    }
    Vector->Array[Index + 1] = Value;
    Vector->Count++;

    return 0;
}

int VecRemoveByIndex(CC_VECTOR *Vector, int Index)
{
    if (NULL == Vector)
    {
        return -1;
    }

    if (Index < 0 || Index >= Vector->Count)
    {
        return -1;
    }
    
    for (int i = Index; i < Vector->Count - 1; i++)
    {
        Vector->Array[i] = Vector->Array[i + 1];
    }
    Vector->Count--;
    return 0;
}

int VecGetValueByIndex(CC_VECTOR *Vector, int Index, int *Value)
{
    if (Vector == NULL || Value == NULL)
    {
        return -1;
    }

    if (Index < 0 || Index >= Vector->Count)
    {
        return -1;
    }

    *Value = Vector->Array[Index];

    return 0;
}

int VecGetCount(CC_VECTOR *Vector)
{
    if (NULL == Vector)
    {
        return -1;
    }

    return Vector->Count;
}

int VecClear(CC_VECTOR *Vector)
{
    if (Vector == NULL)
    {
        return -1;
    }

    Vector->Count = 0;
    if (VecRealloc(Vector, INITIAL_SIZE) < 0)
    {
        return -2;
    }

    return 0;
}

int VecSort(CC_VECTOR *Vector)
{
    int temp = 0;
    int is_sort = 0;

    if (Vector == NULL)
    {
        return -1;
    }

    do
    {
        is_sort = 1;
        for (int i = 0; i < Vector->Count - 1; i++)
        {
            if (Vector->Array[i] < Vector->Array[i + 1])
            {
                is_sort = 0;
                temp = Vector->Array[i];
                Vector->Array[i] = Vector->Array[i + 1];
                Vector->Array[i + 1] = temp;
            }
        }
    } while (!is_sort);

    return 0;
}

int VecAppend(CC_VECTOR* DestVector, CC_VECTOR* SrcVector)
{
    if (DestVector == NULL || SrcVector == NULL)
    {
        return -1;
    }

    if (DestVector->Count + SrcVector->Count > DestVector->Size)
    {
        if (VecRealloc(DestVector, DestVector->Count + SrcVector->Count + 20) < 0)
        {
            return -1;
        }
    }

    for (int i = 0; i < SrcVector->Count; i++)
    {
        DestVector->Array[DestVector->Count + i] = SrcVector->Array[i];
    }
    DestVector->Count += SrcVector->Count;

    if (VecClear(SrcVector) < 0)
    {
        return -2;
    }

    return 0;
}

static int VecRealloc(CC_VECTOR* Vector, int Size)
{
    int *newArray = NULL;

    if (Vector == NULL)
    {
        return -1;
    }
    if (Size < 0)
    {
        return -1;
    }

    newArray = realloc(Vector->Array, Size);
    
    if (newArray == NULL)
    {
        return -1;
    }

    Vector->Array = newArray;
    Vector->Size = Size;
    
    return 0;
}