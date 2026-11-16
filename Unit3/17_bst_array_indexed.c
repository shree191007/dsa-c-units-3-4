/* FILE: 17_bst_array_indexed.c
 * BST with array node records and integer child indices.
 * Includes insertion, inorder, node/leaf counts, copy and equality.
 * -1 means no child. Unlike sequential storage, skewed trees use consecutive records.
 * Standalone C11 program. Compile this file alone.
 */
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 200
struct node 
{
	int key_;
	int left_;
	int right_;
};
typedef struct node node_t;

struct tree 
{
	int root_;
	int count_;
	node_t elem_[MAXSIZE];
};
typedef struct tree tree_t; 

void init(tree_t *ptr_tree);
void deinit(tree_t *ptr_tree);
void disp(tree_t *ptr_tree);
void insert(tree_t *ptr_tree, int key);

// adding 
// 1. find # of nodes 
int number_of_nodes(tree_t *ptr_tree);
// 2. find # of leaves 
int number_of_leaves(tree_t *ptr_tree);
// 3. copy tree 
void copy(tree_t* ptr_src, tree_t* ptr_dst);
// 4. compare tree
int are_same(tree_t* tree1, tree_t* tree2);


// node functions 
static void disp_recursive(node_t elem[], int temp)
{
	if(temp != -1)
	{
		disp_recursive(elem, elem[temp].left_);
		printf("%d ", elem[temp].key_);
		disp_recursive(elem, elem[temp].right_);
	}
}

static int insert_recursive(node_t elem[], int root, int temp)
{
	if(root == -1)
	{
		root = temp;
	}
	else if(elem[root].key_ > elem[temp].key_)
	{
		elem[root].left_ = insert_recursive(elem, elem[root].left_, temp);
	}
	else 
	{
		elem[root].right_ = insert_recursive(elem, elem[root].right_, temp);
	}
	return root;
}


// tree functions 
void init(tree_t *ptr_tree)
{
	ptr_tree->root_ = -1;
	ptr_tree->count_ = 0;
}
void deinit(tree_t *ptr_tree)
{
	ptr_tree->root_ = -1;
	ptr_tree->count_ = 0;
}
void disp(tree_t *ptr_tree)
{
	printf("Tree : ");
	disp_recursive(ptr_tree->elem_, ptr_tree->root_);
	printf("\n");
}
void insert(tree_t *ptr_tree, int key)
{
	if(ptr_tree->count_  == MAXSIZE)
	{
		printf("tree full\n");
		exit(1);
	}
	int temp = ptr_tree->count_++;
	ptr_tree->elem_[temp].key_ = key;
	ptr_tree->elem_[temp].left_ = ptr_tree->elem_[temp].right_ = -1;
	
	ptr_tree->root_ = insert_recursive(ptr_tree->elem_, ptr_tree->root_, temp);
}
//--------------------------------------
// adding 

// node 

int number_of_nodes_recursive(node_t elem[], int temp)
{
	if(temp == -1)
	{
		return 0;
	}
	else 
	{
		return 1 + number_of_nodes_recursive(elem, elem[temp].left_) 
			+ number_of_nodes_recursive(elem, elem[temp].right_);
	}
}

int number_of_leaves_recursive(node_t elem[], int temp)
{
	if(temp == -1)
	{
		return 0;
	}
	else if(elem[temp].left_ == -1 && elem[temp].right_ == -1)
	{
		return 1;
	}
	else
	{
		return number_of_leaves_recursive(elem, elem[temp].left_) 
			+ number_of_leaves_recursive(elem, elem[temp].right_);
	}
}

// tree 
int number_of_nodes(tree_t *ptr_tree)
{
	return number_of_nodes_recursive(ptr_tree->elem_, ptr_tree->root_);
}

int number_of_leaves(tree_t *ptr_tree)
{
	return number_of_leaves_recursive(ptr_tree->elem_, ptr_tree->root_);
}
// -----------------
// node 


// tree 
void copy(tree_t* ptr_src, tree_t* ptr_dst)
{
	ptr_dst->root_ = ptr_src->root_;
	ptr_dst->count_ = ptr_src->count_;
	for(int i = 0; i < ptr_src->count_; ++i)
	{
		ptr_dst->elem_[i] = ptr_src->elem_[i];
	}
}
// node 
int are_same_recursive(node_t elem1[], int root1, node_t elem2[], int root2)
{
	if(root1 == -1 && root2 == -1)
		return 1;
	else if(root1 == -1 || root2 == -1)
		return 0;
	else 
		return elem1[root1].key_ == elem2[root2].key_ &&
			are_same_recursive(elem1, elem1[root1].left_, 
				elem2, elem2[root2].left_)  &&
			are_same_recursive(elem1, elem1[root1].right_, 
				elem2, elem2[root2].right_);
}
// tree 
int are_same(tree_t* ptr_tree1, tree_t* ptr_tree2)
{
	return are_same_recursive(ptr_tree1->elem_, ptr_tree1->root_, 
		ptr_tree2->elem_, ptr_tree2->root_);
}

static void build(tree_t *t, const int a[], int n)
{
	init(t);
	for(int i = 0; i < n; ++i)
		insert(t, a[i]);
}

static void report(const char *name, tree_t *t)
{
	printf("%s ", name);
	disp(t);
	printf("  # of nodes  : %d\n", number_of_nodes(t));
	printf("  # of leaves : %d\n", number_of_leaves(t));
}

int main(void)
{
	int a1[] = {40, 20, 60, 10, 70, 30, 50};   /* balanced shape */
	int a2[] = {10, 20, 30, 40, 50, 60, 70};   /* skewed (right) shape */
	tree_t t1, t2, t3, empty;

	build(&t1, a1, 7);
	build(&t2, a2, 7);
	init(&empty);

	report("balanced:", &t1);
	report("skewed  :", &t2);
	report("empty   :", &empty);

	copy(&t1, &t3);
	printf("copy of balanced, same as original : %d\n", are_same(&t1, &t3));
	printf("balanced vs skewed (same keys)     : %d\n", are_same(&t1, &t2));
	printf("balanced vs empty                  : %d\n", are_same(&t1, &empty));

	deinit(&t1); deinit(&t2); deinit(&t3); deinit(&empty);
	return 0;
}
