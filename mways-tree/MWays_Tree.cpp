#include <stdlib.h>
#include <stdio.h>

#define NODES_MAX 1000
#define PADDING 3

/*
	TREE FIGURED BY PARENTS VECTOR
*/
typedef struct _ParentTree
{
	int parents[NODES_MAX + 1];
	int keys[NODES_MAX + 1];
	int nodes;
}PARENT_TREE, *PPARENT_TREE;

PPARENT_TREE parentTree;

void PPrint(PPARENT_TREE tree, int col, int parent)
{
	for (int i = 1; i <= tree->nodes; i++)
	{
		if (tree->parents[i] == parent)
		{
			printf("%*d\n", col * PADDING, i);
			PPrint(tree, col + 1, i);
		}
	}
}

void DemoParentTree()
{
	PPARENT_TREE tree = (PPARENT_TREE)malloc(sizeof(PARENT_TREE));

	tree->nodes = 9;
	tree->parents[1] = 2;
	tree->parents[2] = 7;
	tree->parents[3] = 5;
	tree->parents[4] = 2;
	tree->parents[5] = 7;
	tree->parents[6] = 7;
	tree->parents[7] = -1;
	tree->parents[8] = 5;
	tree->parents[9] = 2;

	printf("Print parents tree:\n");
	PPrint(tree, 0, -1);
	printf("\n");

	parentTree = tree;
}

/*
	MULTI-WAYS TREE
*/
typedef struct _MultiWayNode* PMULTI_WAY_NODE;
typedef struct _MultiWayNode {
	int keys;
	PMULTI_WAY_NODE children[NODURI_MAX + 1];
	int nr_children;
}MULTI_WAY_NODE;

typedef struct _MultiWayTree {
	PMULTI_WAY_NODE root;
}MULTI_WAY_TREE, *PMULTI_WAY_TREE;

PMULTI_WAY_NODE CreateNode(int keys)
{
	PMULTI_WAY_NODE nod = (PMULTI_WAY_NODE)malloc(sizeof(MULTI_WAY_NODE));

	nod->keys = keys;
	for (int i = 0; i < NODES_MAX + 1; i++)
		nod->children[i] = NULL;
	nod->nr_children = 0;

	return nod;
}

PMULTI_WAY_TREE TransformToMultiWayTree(PPARENT_TREE parentTree)
{
	PMULTI_WAY_TREE tree = (PMULTI_WAY_TREE)malloc(sizeof(tree_MULTI_CAI));

	PMULTI_WAY_NODE* nodes = (PMULTI_WAY_NODE*)calloc(NODES_MAX + 1, sizeof(PMULTI_WAY_NODE));

	for (int i = 1; i <= parentTree->nodes; i++)
	{
		int parent = parentTree->parents[i];

		if (nodes[i] == NULL) //Yet node i is not created
			nodes[i] = CreateNode(i);

		if (parent == -1)
			tree->root = nodes[i];
		else if (nodes[parent] == NULL) //Yet parent node is not created
			nodes[parent] = CreateNode(parent);

		if (parent != -1)
		{
			(nodes[parent]->nr_children)++;
			nodes[parent]->children[nodes[parent]->nr_children] = nodes[i]; //Create link between parent and child
		}
	}

	free(nodes);

	return tree;
}

void PPrint(PMULTI_WAY_NODE nod, int col)
{
	printf("%*d\n", col * PADDING, nod->keys);

	for (int i = 1; i <= nod->nr_children; i++)
		PPrint(nod->children[i], col + 1);
}

void DemoMultiWayTree()
{
	PMULTI_WAY_TREE multiWayTree = TransformToMultiWayTree(parentTree);
	printf("Print tree multi-cai:\n");
	PPrint(multiWayTree->root, 0);
	printf("\n");
}

/*
	BINARY TREE
*/
typedef struct _BinaryNode* PBINARY_NODE;
typedef struct _BinaryNode {
	int keys;
	PBINARY_NODE child;
	PBINARY_NODE firstBrother;
	PBINARY_NODE lastBrother;
}BINARY_NODE;

typedef struct _BinaryTree {
	PBINARY_NODE root;
}BINARY_TREE, *PBINARY_TREE;

PBINARY_NODE CreateBinaryNode(int keys)
{
	PBINARY_NODE nod = (PBINARY_NODE)malloc(sizeof(BINARY_NODE));

	nod->keys = keys;
	nod->child = NULL;
	nod->firstBrother = NULL;
	nod->lastBrother = NULL;

	return nod;
}

PBINARY_TREE TransformInBinaryTree(PPARENT_TREE parentTree)
{
	PBINARY_TREE tree = (PBINARY_TREE)malloc(sizeof(BINARY_TREE));

	PBINARY_NODE* nodes = (PBINARY_NODE*)calloc(NODES_MAX + 1, sizeof(PBINARY_NODE));

	for (int i = 1; i <= parentTree->nodes; i++)
	{
		int parent = parentTree->parents[i];

		if (nodes[i] == NULL) //Yet node i is not created
			nodes[i] = CreateBinaryNode(i);

		if (parent == -1)
			tree->root = nodes[i];
		else if (nodes[parent] == NULL) //Yet parent node is not created
			nodes[parent] = CreateBinaryNode(parent);

		if (parent != -1)
		{
			PBINARY_NODE child = nodes[parent]->child;
			if (child == NULL)
			{
				nodes[parent]->child = nodes[i];
			}
			else
			{
				if (child->lastBrother == NULL) // have no brothers
				{
					child->firstBrother = child->lastBrother = nodes[i];
				}
				else
				{
					child->lastBrother->firstBrother = nodes[i];
					child->lastBrother = nodes[i];
				}
			}
		}
	}

	free(nodes);

	return tree;
}

void PPrint(PBINARY_NODE node, int col)
{
	if (node == NULL)
		return;

	printf("%*d\n", col * PADDING, node->keys);

	PPrint(node->child, col + 1);
	PPrint(node->firstBrother, col);
}

void DemoBinaryTree()
{
	PBINARY_TREE tree = TransformInBinaryTree(parentTree);
	printf("Print binary tree:\n");
	PPrint(tree->root, 0);
	printf("\n");
}

int main()
{
	DemoParentTree();

	DemoMultiWayTree();

	DemoBinaryTree();

	return 0;
}