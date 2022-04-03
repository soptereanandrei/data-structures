#include "cctree.h"
#include "common.h"
#include "stdio.h"
struct _NODE
{
	KEY		Value;
	PNODE	Parent;
	PNODE	Left;
	PNODE	Right;
};

static PNODE SearchInsertPosition(PNODE Node, KEY Value);
static PNODE SearchNodesToRemove(PNODE Node, KEY Value);
static int SearchNodeAfterKey(PNODE Node, KEY Value);
static int GetMaxHeight(PNODE Node);
static int FreeNodes(PNODE Node);
static int GetNodeByIndexInPreorder(PNODE Node, int Index, PNODE* NthNode);
static int GetNodeByIndexInInorder(PNODE Node, int Index, PNODE* NthNode);
static int GetNodeByIndexInPostorder(PNODE Node, int Index, PNODE* NthNode);

int TreeCreate(CC_TREE** Tree)
{
	CC_TREE* newTree = NULL;

	if (Tree == NULL)
	{
		return -1;
	}

	newTree = (CC_TREE*)malloc(sizeof(CC_TREE));
	if (newTree == NULL)
	{
		return -1;
	}

	memset(newTree, 0, sizeof(*newTree));

	newTree->Root = NULL;
	newTree->Count = 0;

	*Tree = newTree;

	return 0;
}

int TreeDestroy(CC_TREE** Tree)
{
	if (Tree == NULL)
	{
		return -1;
	}

	if (TreeClear(*Tree) < 0)
	{
		return -1;
	}

	free(*Tree);

	*Tree = NULL;

	return 0;
}

int TreeInsert(CC_TREE* Tree, int Value)
{
	PNODE newNode;
	PNODE position;

	if (Tree == NULL)
	{
		return -1;
	}

	newNode = (PNODE)malloc(sizeof(struct _NODE));
	if (newNode == NULL)
	{
		return -1;
	}

	newNode->Value = Value;
	newNode->Parent = NULL;
	newNode->Left = NULL;
	newNode->Right = NULL;

	if (Tree->Root == NULL)
	{
		Tree->Root = newNode;
		Tree->Count = 1;
	}
	else
	{
		position = SearchInsertPosition(Tree->Root, Value);
		newNode->Parent = position;
		if (Value < position->Value)
		{
			position->Left = newNode;
		}
		else
		{
			position->Right = newNode;
		}
		Tree->Count++;
	}

	return 0;
}

int TreeRemove(CC_TREE* Tree, int Value)
{
	if (Tree == NULL)
	{
		return -1;
	}

	while (TreeContains(Tree, Value) > 0)
	{
		Tree->Root = SearchNodesToRemove(Tree->Root, Value);
		Tree->Count--;
	}

	return 0;
}

int TreeContains(CC_TREE* Tree, int Value)
{
	if (Tree == NULL)
	{
		return -1;
	}

	return SearchNodeAfterKey(Tree->Root, Value);
}

int TreeGetCount(CC_TREE* Tree)
{
	if (Tree == NULL)
	{
		return -1;
	}

	return Tree->Count;
}

int TreeGetHeight(CC_TREE* Tree)
{
	if (Tree == NULL)
	{
		return -1;
	}

	return GetMaxHeight(Tree->Root);
}

int TreeClear(CC_TREE* Tree)
{
	if (Tree == NULL)
	{
		return -1;
	}

	FreeNodes(Tree->Root);
	Tree->Count = 0;

	return 0;
}

int TreeGetNthPreorder(CC_TREE* Tree, int Index, int* Value)
{
	PNODE	nthNode = NULL;

	if (Tree == NULL)
	{
		return -1;
	}
	if (Index < 0)
	{
		return -1;
	}
	if (Value == NULL)
	{
		return -1;
	}

	if (TreeGetCount(Tree) < Index - 1)
	{
		return -1;
	}

	GetNodeByIndexInPreorder(Tree->Root, Index, &nthNode);
	if (nthNode == NULL)
	{
		return -1;
	}

	*Value = nthNode->Value;

	return 0;
}

int TreeGetNthInorder(CC_TREE* Tree, int Index, int* Value)
{
	PNODE	nthNode = NULL;

	if (Tree == NULL)
	{
		return -1;
	}
	if (Index < 0)
	{
		return -1;
	}
	if (Value == NULL)
	{
		return -1;
	}

	if (TreeGetCount(Tree) < Index - 1)
	{
		return -1;
	}

	GetNodeByIndexInInorder(Tree->Root, Index, &nthNode);
	if (nthNode == NULL)
	{
		return -1;
	}

	*Value = nthNode->Value;

	return 0;
}

int TreeGetNthPostorder(CC_TREE* Tree, int Index, int* Value)
{
	PNODE	nthNode = NULL;

	if (Tree == NULL)
	{
		return -1;
	}
	if (Index < 0)
	{
		return -1;
	}
	if (Value == NULL)
	{
		return -1;
	}

	if (TreeGetCount(Tree) < Index - 1)
	{
		return -1;
	}

	GetNodeByIndexInPostorder(Tree->Root, Index, &nthNode);
	if (nthNode == NULL)
	{
		return -1;
	}

	*Value = nthNode->Value;

	return 0;
}

static PNODE SearchInsertPosition(PNODE Node, KEY Value)
{
	if (Node == NULL)
	{
		return NULL;
	}

	if (Node->Left != NULL)
	{
		if (Value < Node->Value)
		{
			return SearchInsertPosition(Node->Left, Value);
		}
	}
	if (Node->Right != NULL)
	{
		if (Value >= Node->Value)//atentie adaugare noduri cu key egale
		{
			return SearchInsertPosition(Node->Right, Value);
		}
	}

	return Node;
}

//find minim node in the right sub-tree
static PNODE FindMinInRight(PNODE Node)
{
	if (Node == NULL)
	{
		return NULL;
	}

	if (Node->Left != NULL)
	{
		return FindMinInRight(Node->Left);
	}

	return Node;
}

static PNODE SearchNodesToRemove(PNODE Node, KEY Value)
{
	PNODE temp = NULL;

	if (Node == NULL)
	{
		return NULL;
	}
	
	if (Value > Node->Value)
	{
		Node->Right = SearchNodesToRemove(Node->Right, Value);
		if (Node->Right != NULL)
		{
			Node->Right->Parent = Node;
		}
	}
	else if (Value < Node->Value)
	{
		Node->Left = SearchNodesToRemove(Node->Left, Value);
		if (Node->Left != NULL)
		{
			Node->Left->Parent = Node;
		}
	}
	else
	{
		if (Node->Left != NULL && Node->Right != NULL)
		{
			temp = FindMinInRight(Node->Right);
			Node->Value = temp->Value;
			Node->Right = SearchNodesToRemove(Node->Right, temp->Value);
		}
		else
		{
			temp = Node;
			if (Node->Left != NULL)
			{
				Node = Node->Left;
			}
			else if (Node->Right != NULL)
			{
				Node = Node->Right;
			}
			else
			{
				Node = NULL;
			}
			free(temp);
		}
	}

	return Node;
}

static int SearchNodeAfterKey(PNODE Node, KEY Value)
{
	if (Node == NULL)
	{
		return 0;
	}

	if (Node->Value == Value)
	{
		return 1;
	}
	if (Value >= Node->Value)
	{
		if (Node->Right != NULL)
		{
			return SearchNodeAfterKey(Node->Right, Value);
		}
	}
	else
	{
		if (Node->Left != NULL)
		{
			return SearchNodeAfterKey(Node->Left, Value);
		}
	}

	return 0;
}

static int GetMaxHeight(PNODE Node)
{
	int maxLeft = 0;
	int maxRight = 0;

	if (Node == NULL)
	{
		return 0;
	}

	maxLeft = 1 + GetMaxHeight(Node->Left);
	maxRight = 1 + GetMaxHeight(Node->Right);

	if (maxRight >= maxLeft)
	{
		return maxRight;
	}
	return maxLeft;
}

static int FreeNodes(PNODE Node)
{
	if (Node != NULL)
	{
		FreeNodes(Node->Left);
		FreeNodes(Node->Right);
		free(Node);
	}

	return 0;
}

static int GetNodeByIndexInPreorder(PNODE Node, int Index, PNODE *NthNode)
{
	if (Node == NULL)
	{
		return -1;
	}

	if (Index >= 0)
	{
		if (Index == 0)
		{
			*NthNode = Node;
		}
		Index--;
		if (Node->Left != NULL)
		{
			Index = GetNodeByIndexInPreorder(Node->Left, Index, NthNode);
		}
		if (Node->Right != NULL)
		{
			Index = GetNodeByIndexInPreorder(Node->Right, Index, NthNode);
		}
	}

	return Index;
}

static int GetNodeByIndexInInorder(PNODE Node, int Index, PNODE *NthNode)
{
	if (Node == NULL)
	{
		return -1;
	}

	if (Index >= 0) 
	{
		if (Node->Left != NULL)
		{
			Index = GetNodeByIndexInInorder(Node->Left, Index, NthNode);
		}
		if (Index == 0)
		{
			*NthNode = Node;
		}
		Index--;
		if (Node->Right != NULL)
		{
			Index = GetNodeByIndexInInorder(Node->Right, Index, NthNode);
		}
	}
	
	return Index;
}

static int GetNodeByIndexInPostorder(PNODE Node, int Index, PNODE* NthNode)
{
	if (Node == NULL)
	{
		return 0;
	}

	if (Index >= 0)
	{
		if (Node->Left != NULL)
		{
			Index = GetNodeByIndexInPostorder(Node->Left, Index, NthNode);
		}
		if (Node->Right != NULL)
		{
			Index = GetNodeByIndexInPostorder(Node->Right, Index, NthNode);
		}
		if (Index == 0)
		{
			*NthNode = Node;
		}
		Index--;
	}

	return Index;
}
	