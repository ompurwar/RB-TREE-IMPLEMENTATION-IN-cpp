//============================================================================
// Name        : RB-TREE.cpp
// Author      : Om Purwar
// Version     :
// Copyright   : Your copyright notice
// Description :  RB-Tree Implementation in C++ as per the book
//				" Introduction to Algorithms, Third Edition" By - T H O M A S H. CORMEN, C H A R L E S E. LEISERSON, R O N A L D L . RIVEST, CLIFFORD STEIN
//============================================================================

#include <iostream>
#include <math.h>
#include <conio.h>
#include <string.h>
#include <assert.h>

#define RED true
#define BLACK false

using namespace std;

template<class T>
class Node {
private:
	T value;

public:
	Node *parent;
	Node *left;
	Node *right;
	bool color;
	bool setValue(T val) {
		this->value = val;
		return (true);
	}

	Node<T> *getValue() {
		return (this->value);
	}

};

template<class T>
class RB_TREE {
private:
	Node<T> *root;
	Node<T> * nil;
public:
	RB_TREE() {
		this->root = new Node<T>();
		root->parent = NULL;
		root->left = NULL;
		root->right = NULL;
		root->color = BLACK;
		nil = NULL;
	}

	Node<T> *getGrandparent(Node<T> *n) {
		Node<T> *parent = n->parent;
		if (parent == NULL)
			return (NULL); 						//No parent NO grandParent
		return (parent->parent);
	}

	Node<T> *getSibiling(Node<T> *n) {
		Node<T> * parent = n->parent;
		if (parent == NULL)
			return (NULL); 						//No parent means no sibiling
		if (n != parent->left)
			return (parent->right);
		else
			return (parent->left);
	}

	Node<T> *getUncle(Node<T> *n) {
		Node<T> * parent = n->parent;
		Node<T> * grandParent = this->getGrandparent(n);
		if (grandParent == NULL) {
			return (NULL); 						//NO grand parent means no uncle
		}
		return (this->getSibiling(parent));
	}

	void left_rotate(Node<T> *x) {
		Node<T> *y = x->right;
		x->right = y->left; // turn nnew’s left subtree into n’s right subtree
		if (y->left != this->nil)
			y->left->parent = x;
		y->parent = x->parent; // link n’s parent to nnew
		if (x->parent == this->nil)
			this->root = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right == y;
		y->left = x; // put n on nnew’s left
		x->parent = y;
	}

	void right_rotate(Node<T> *y) {
		Node<T> *x = y->left;
		y->left = x->right; // turn nnew’s left subtree into n’s right subtree
		if (x->right != this->nil)
			x->right->parent = y;
		x->parent = y->parent; // link n’s parent to nnew
		if (y->parent == this->nil)
			this->root = x;
		else if (y == y->parent->right)
			y->parent->right = x;
		else
			y->parent->left == x;
		x->left = y; // put n on nnew’s left
		y->parent = x;
	}

	void RB_insert(Node<T> *z) {
		Node<T> *y = this->nil;
		Node<T> *x = this->root;
		while (x != this->nil) {
			y = x;
			if (z->getValue() < x->getValue())
				x = x->left;
			else
				x = x->right;
		}
		z->parent = y;
		if (y == this->nil)
			this->root = z;
		else if (z->getValue() < y->getValue())
			y->left = z;
		else
			y->right = z;
		z->left = this->nil;
		z->right = this->nil;
		z->color = RED;
		RN_insert_fixup(z);
	}

	void RB_insert_fixup(Node<T> *z) {
		while (z->parent->color == RED) {
			if (z->parent == z->parent->parent.left) {
				Node<T> *y = z->parent->parent->right;
				if (y->color == RED) {
					z->parent->color = BLACK;				//case 1
					y->color = BLACK;						//case 1
					z->parent->parent->color = RED;			//case 1
					z = z->parent->parent;					//case 1
				} else if (z == z->parent->right) {
					z = z->parent;							//case 2
					this->left_rotate(z);					//case 2
				}
				z->p->color = BLACK;						//case 3
				z->parent->parent->color = RED;				//case 3
				this->right_rotate(z->parent->parent);
				//case 3
			}
		}
	}
	void RB_transplant(Node<T> *u, Node<T> *v) {
		if (u->parent == this->nil)
			this->root = v;
		else if (u == u->parent->left)
			u->parent.left = v;
		else
			u->parent->right = v;
		v->parent = u->parent;
	}

	void RB_delete(Node<T> *z) {
		Node<T> *y = z;
		Node<T> *x;
		bool y_original_color = y->color;
		if (z->left == this->nil) {
			x = z->right;
			this->RB_transplant(z, z->right);
		} else if (z->right == this->nil) {
			x = z->left;
			this->RB_transplant(z, z->left);
		} else {
			y = this->Tree_minimum(z->right);
			y_original_color = y->color;
			x = y->right;
			if (y->parent == z) {
				x->parent = y;
			} else {
				this->RB_transplant(y, y->right);
				y->right = z->right;
				y->right.parent = y;
			}
			this->RB_transplant(z, y);
			y->left = z->left;
			y->left->present = y;
			y->color = z->color;
		}
		if (y_original_color == BLACK) {
			this->RB_delete_fixup(x);
		}
	}

	Node<T> *Tree_minimum(Node<T> * x) {
		while (x->left != NULL) {
			x = x->left;
		}
		return (x);
	}

	Node<T> *Tree_maximum(Node<T> * x) {
		while (x->right != NULL) {
			x = x->right;
		}
		return (x);
	}

	void RB_delete_fixup(Node<T> *x) {
		while (x != this->root && x->color == BLACK) {
			if (x == x->parent->left) {
				Node<T> *w = x->parent->right;
				if (w->color == RED) {
					w->color = BLACK;
					x->parent->color = RED;
					this->left_rotate(x->parent);
					w = x->parent->right;
				}
				if (w->left.color == BLACK && w->right.color == BLACK) {
					w->color = RED;
					x = x->parent;
				} else if (w->left.color == BLACK) {
					w->left->color = BLACK;
					w->color = RED;
					this->right_rotate(w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				this->left_rotate(x->parent);
				x = this->root;
			} else {
				Node<T> *w = x->parent->left;
				if (w->color == RED) {
					w->color = BLACK;
					x->parent->color = RED;
					this->left_rotate(x->parent);
					w = x->parent->left;
				}
				if (w->right->color == BLACK && w->left->color == BLACK) {
					w->color = RED;
					x = x->parent;
				} else if (w->right->color == BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					this->right_rotate(w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				this->left_rotate(x->parent);
				x = this->root;
			}
		}
		x->color = BLACK;
	}
};

int main() {
 // do here what ever you want
	return (0);
}
