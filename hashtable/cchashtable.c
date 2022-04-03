#include "cchashtable.h"
#include "common.h"
#include "string.h"

#define MULTIPLIER (37)

struct _HASH_KEYS
{
    char        *Key;
    ITEM        Value;
    PHASH_KEYS  Next;
};

static char* StrDuplicate(char* s)
{
    char* newString = NULL;
    int len = 0;
    int i = 0;

    if (s == NULL)
    {
        return NULL;
    }

    while (s[len])
    {
        len++;
    }

    newString = (char*)malloc(sizeof(char) * (len + 1));
    if (newString == NULL)
    {
        return NULL;
    }

    for (i = 0; i < len; i++)
    {
        newString[i] = s[i];
    }
    newString[i] = '\0';

    return newString;
}

static int StrCmp(char* s1, char* s2)
{
    int i = 0;
    if (s1 == NULL && s2 == NULL)
    {
        return 0;
    }
    if (s1 == NULL)
    {
        return -1;
    }
    if (s2 == NULL)
    {
        return 1;
    }

    while (s1[i] || s2[i])
    {
        if (s1[i] - s2[i] != 0)
        {
            return s1[i] - s2[i];
        }
        i++;
    }

    return 0;
}

static size_t GetHash(const char *s)
{
    size_t                  hash = 0;
    unsigned const char     *str = NULL;

    str = (unsigned const char*)s;
    while (*str != '\0')
    {
        hash = hash * MULTIPLIER + *str;
        str++;
    }

    return hash;
}

int HtCreate(CC_HASH_TABLE **HashTable)
{
    CC_HASH_TABLE   *newHashTable = NULL;

    if (HashTable == NULL)
    {
        return -1;
    }

    newHashTable = (CC_HASH_TABLE*)malloc(sizeof(CC_HASH_TABLE));

    if (newHashTable == NULL)
    {
        return -1;
    }

    memset(newHashTable, 0, sizeof(*newHashTable));

    *HashTable = newHashTable;

    return 0;
}

int HtDestroy(CC_HASH_TABLE **HashTable)
{
    if (HashTable == NULL)
    {
        return -1;
    }

    if (HtClear(*HashTable) < 0)
    {
        return -1;
    }

    free(*HashTable);
    *HashTable = NULL;

    return 0;
}

int HtSetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int Value)
{
    PHASH_KEYS  newKey = NULL;
    PHASH_KEYS  node = NULL;
    PHASH_KEYS  precedentNode = NULL;
    int         index = 0;

    if (HashTable == NULL)
    {
        return -1;
    }
    if (Key == NULL)
    {
        return -1;
    }
    if (*Key == 0)
    {
        return -1;
    }

    index = GetHash(Key) % 256;
    
    node = precedentNode = HashTable->Table[index];
    while (node)
    {
        if (StrCmp(node->Key, Key) == 0)
        {
            return -1;
        }
        precedentNode = node;
        node = node->Next;
    }

    newKey = (PHASH_KEYS)malloc(sizeof(struct _HASH_KEYS));
    if (newKey == NULL)
    {
        return -1;
    }

    newKey->Key = StrDuplicate(Key);
    if (newKey->Key == NULL)
    {
        free(newKey);
        return -1;
    }
    newKey->Value = Value;
    newKey->Next = NULL;
    HashTable->Count++;

    if (precedentNode == NULL)
    {
        HashTable->Table[index] = newKey;
    }
    else
    {
        precedentNode->Next = newKey;
    }

    return 0;
}

int HtGetKeyValue(CC_HASH_TABLE *HashTable, char *Key, int *Value)
{
    PHASH_KEYS  node = NULL;
    int         index = 0;

    if (HashTable == NULL || Key == NULL || Value == NULL)
    {
        return -1;
    }

    if (*Key == 0)
    {
        return -1;
    }
    
    index = GetHash(Key) % 256;

    if (HashTable->Table[index] == NULL)
    {
        return -1;
    }

    node = HashTable->Table[index];
    while (node)
    {
        if (StrCmp(node->Key, Key) == 0)
        {
            *Value = node->Value;
            return 0;
        }
        node = node->Next;
    }

    return -1;
}

int HtRemoveKey(CC_HASH_TABLE *HashTable, char *Key)
{
    PHASH_KEYS  node = NULL;
    PHASH_KEYS  precedentNode = NULL;
    int         index = 0;

    if (HashTable == NULL || Key == NULL)
    {
        return -1;
    }
    if (*Key == 0)
    {
        return -1;
    }
    
    index = GetHash(Key) % 256;

    if (HashTable->Table[index] == NULL)
    {
        return -1;
    }

    node = precedentNode = HashTable->Table[index];
    while (node)
    {
        if (StrCmp(node->Key, Key) == 0)
        {
            if (node == precedentNode)
            {
                HashTable->Table[index] = node->Next;
            }
            else
            {
                precedentNode->Next = node->Next;
            }
            free(node->Key);
            free(node);
            HashTable->Count--;
            return 0;
        }
        node = node->Next;
    }

    return -1;
}

int HtHasKey(CC_HASH_TABLE *HashTable, char *Key)
{
    PHASH_KEYS  node = NULL;
    int         index = 0;

    if (HashTable == NULL || Key == NULL)
    {
        return -1;
    }
    if (*Key == 0)
    {
        return -1;
    }
    
    index = GetHash(Key) % 256;

    node = HashTable->Table[index];
    while (node)
    {
        if (StrCmp(node->Key, Key) == 0)
        {
            return 1;
        }
        node = node->Next;
    }

    return 0;
}

int HtGetFirstKey(CC_HASH_TABLE* HashTable, CC_HASH_TABLE_ITERATOR** Iterator, char** Key)
{
    CC_HASH_TABLE_ITERATOR  *iterator = NULL;
    int                     indexOfList = 0;

    if (NULL == HashTable)
    {
        return -1;
    }
    if (NULL == Iterator)
    {
        return -1;
    }
    if (NULL == Key)
    {
        return -1;
    }

    iterator = (CC_HASH_TABLE_ITERATOR*)malloc(sizeof(CC_HASH_TABLE_ITERATOR));

    if (iterator == NULL)
    {
        return -1;
    }

    memset(iterator, 0, sizeof(*iterator));

    iterator->HashTable = HashTable;
    while (iterator->HashTable->Table[indexOfList] == NULL && indexOfList < TABLE_SIZE)
    {
        indexOfList++;
    }

    if (indexOfList >= TABLE_SIZE)
    {
        free(iterator);
        return -2;
    }

    iterator->IndexOfList = indexOfList;
    iterator->List = iterator->HashTable->Table[indexOfList];

    *Iterator = iterator;

    *Key = iterator->List->Key;

    return 0;
}

int HtGetNextKey(CC_HASH_TABLE_ITERATOR *Iterator, char **Key)
{
    int indexOfList;

    if (Iterator == NULL || Key == NULL)
    {
        return -1;
    }

    indexOfList = Iterator->IndexOfList + 1;

    while (indexOfList < TABLE_SIZE && Iterator->HashTable->Table[indexOfList] == NULL)
    {
        indexOfList++;
    }

    if (indexOfList >= TABLE_SIZE)
    {
        return -2;
    }

    Iterator->IndexOfList = indexOfList;
    Iterator->List = Iterator->HashTable->Table[indexOfList];

    *Key = Iterator->List->Key;

    return 0;
}

int HtReleaseIterator(CC_HASH_TABLE_ITERATOR** Iterator)
{
    if (Iterator == NULL)
    {
        return -1;
    }

    if (*Iterator == NULL)
    {
        return -1;
    }

    free(*Iterator);
    *Iterator = NULL;

    return 0;
}

int HtClear(CC_HASH_TABLE *HashTable)
{
    CC_HASH_TABLE_ITERATOR  *iterator = NULL;
    char                    *keyName = NULL;
    PHASH_KEYS              node = NULL;
    PHASH_KEYS              nextNode = NULL;

    if (HashTable == NULL)
    {
        return -1;
    }
    
    if (HtGetFirstKey(HashTable, &iterator, &keyName) >= 0)
    {
        do
        {
            node = iterator->List;
            while (node)
            {
                nextNode = node->Next;
                free(node->Key);
                free(node);
                node = nextNode;
            }
        } while (HtGetNextKey(iterator, &keyName) >= 0);
    }
    HashTable->Count = 0;

    HtReleaseIterator(&iterator);

    return 0;
}

int HtGetKeyCount(CC_HASH_TABLE *HashTable)
{
    if (HashTable == NULL)
    {
        return -1;
    }
    return HashTable->Count;
}
