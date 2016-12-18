/**********************************************************
* AVL Tree Implementation file
*
*  ** riffed and modified heavily by Aaron Crandall - 2016
*  ** All implementation in header because of templating

* Lucy Tran
* 10-16-16
* PA2
**********************************************************/

#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <queue>
#include <iostream>
#include <algorithm>

using namespace std;

template<class T>
class AVLNode {
public:
	// Default blank AVLNode constructor
	AVLNode() {
		left = right = nullptr;
		height = 0;
	}

	// Constructor with provided element (data) and children
	AVLNode(const T& el, AVLNode *l = nullptr, AVLNode *r = nullptr) {
		key = el;
		left = l;
		right = r;
		height = 0;
	}

	T key;                  // Key to compare/insert on of type <T>
	AVLNode *left, *right;  // Children of this node
	int height;             // Height of this node in the tree
};

/*
* Main AVLTree class that stores AVLNode objects
*  -- REQUIRES: implementation of rotation functions
*  -- REQUIRES: implementation of node heights
*  -- REQUIRES: implementation of contains function
*  -- REQUIRES: implementation of remove function
*/
template<class T>
class AVLTree {
public:
	AVLTree() { root = nullptr; }

	void insert(const T& key) { insert(root, key); }
	void printTree() { printTree(root); }

	bool contains(const T& key) { return(contains(root, key)); }
	void remove(const T& key) { remove(root, key); }

private:
	AVLNode<T>* root;

	/*Calculate and set the height of a node*/
	void calcHeight(AVLNode<T>* &node) {
		//No children
		if (node->left == nullptr && node->right == nullptr) {
			node->height = 0;
		}

		//No left child
		else if (node->left == nullptr) {
			node->height = (node->right)->height + 1;
		}

		//No right child
		else if (node->right == nullptr) {
			node->height = (node->left)->height + 1;
		}

		//Have both children
		else {	
			node->height = max((node->left)->height, (node->right)->height) + 1;
		}
	}

	void rightRotation(AVLNode<T>* &node) {
		AVLNode<T>* left = node->left;
		AVLNode<T>* temp = node;

		node->left = left->right;
		left->right = node;
		node = left;

		/*Update heights*/
		calcHeight(temp);
		calcHeight(left);
		calcHeight(node);
	}

	/*Reverse rightRotation contents*/
	void leftRotation(AVLNode<T>* &node) {
		AVLNode<T>* right = node->right;
		AVLNode<T>* temp = node;

		node->right = right->left;
		right->left = node;
		node = right;

		/*Update heights*/
		calcHeight(temp);
		calcHeight(right);
		calcHeight(node);
	}

	/*Balance factor to decide what rotation is to be made*/
	int calcBalanceFactor(AVLNode<T>* node) {
		/*Nothing there to calculate*/
		if (node == nullptr) {
			return 0;
		}

		/*At least one node present in tree*/
		else {
			//No children
			if (node->left == nullptr && node->right == nullptr) {
				return 0;
			}

			//No left child
			else if (node->left == nullptr) {
				return (-1) - (node->right)->height;
			}

			//No right child
			else if (node->right == nullptr) {
				return (node->left)->height - (-1);
			}

			//Both children
			else {
				/*Left node height - right node height*/
				return (node->left)->height - (node->right)->height;
			}
		}
	}

	/*Does the rotations to balance the tree*/
	void balance(AVLNode<T>* &node, int& balanceFactor) {

		// Left-left case : right rotation
		if (balanceFactor > 1 && calcBalanceFactor(node->left) >= 0) {
			rightRotation(node);
		}

		// Right-right case : left rotation
		if (balanceFactor < -1 && calcBalanceFactor(node->right) <= 0) {
			leftRotation(node);
		}

		// Left-right case : left-right rotation
		if (balanceFactor > 1 && calcBalanceFactor(node->left) < 0)
		{
			leftRotation(node->left);
			rightRotation(node);
		}

		// Right-left case : right-left rotation
		if (balanceFactor < -1 && calcBalanceFactor(node->right) > 0)
		{
			rightRotation(node->right);
			leftRotation(node);
		}
	}

	/*
	*  This function needs to update heights as it returns from recursive insert.
	*  If the heights of a node are more than 2 different, rotate to fix
	*  After fixing, repair the heights of all nodes in the rotated tree
	*/
	void insert(AVLNode<T>* &node, const T& key) {
		if (node == nullptr) {
			node = new AVLNode<T>(key);
		}
		else if (key > node->key) {
			insert(node->right, key);
		}
		else {
			insert(node->left, key);
		}

		/*Update height*/
		calcHeight(node);

		/*Calculate balance factor*/
		int balanceFactor = calcBalanceFactor(node);

		/*Balance the tree*/
		balance(node, balanceFactor);
	}

	//Searching for a node with a certain key value
	bool contains(AVLNode<T>* root, const T& key) {
		
		/*Base case: key not found*/
		if (root == nullptr) {
			return false;
		}

		/*Second base case: key found*/
		else if (root->key == key) {
			return true;
		}

		// Move to left subtree
		if (key < root->key) {
			contains(root->left, key);
		}

		// Move to right subtree
		else if (key > root->key) {
			contains(root->right, key);
		}
	}

	//Removes a node and rebalances
	void remove(AVLNode<T>* &root, const T& key) {

		//If element is not present in tree, you cannot delete it!
		//if (!contains(root, key)) {
		//	return;
		//}

		/*Empty*/
		if (root == nullptr) {
			return;
		}

		/*Move to left subtree*/
		if (key < root->key) {
			remove(root->left, key);
		}

		/*Move to right subtree*/
		else if (key > root->key) {
			remove(root->right, key);
		}

		/*Same as the root : deletion of root*/
		else {

			//No child
			if ((root->left == nullptr) && (root->right == nullptr)) {
				root = nullptr;
			}

			//Only right child
			else if (root->left == nullptr) {
				AVLNode<T>* temp = root; //This node will be lost
				root = root->right;
				delete temp; //Free the lost node 
			}

			//Only left child
			else if (root->right == nullptr) {
				AVLNode<T>* temp = root;
				root = root->left;
				delete temp; //Free lost node
			}

			//Both children
			else
			{
				/*Leftmost node of the right subtree*/
				AVLNode<T>* temp = root->right;

				/*Loop to find leftmost leaf*/
				while (temp->left != nullptr) {
					temp = temp->left;
				}

				/*Replace root key*/
				root->key = temp->key;
				
				/*Remove duplicate*/
				remove(root->right, temp->key);
			}
		}

		/*Only one node and deleted*/
		if (root == nullptr) {
			return;
		}

		// Update height
		calcHeight(root);

		// Calculate balance factor
		int balanceFactor = calcBalanceFactor(root);

		// Balance tree after deletion
		balance(root, balanceFactor);
	}

	// Should do a level order printout with actual depth (no alignment)
	void printTree(AVLNode<T>* node) {
		queue<AVLNode<T>*> bufQueue;
		int curr_height = node->height;
		bufQueue.push(node);
		while (!bufQueue.empty()) {
			AVLNode<T>* curr = bufQueue.front();
			if (curr->left != nullptr) { bufQueue.push(curr->left); }
			if (curr->right != nullptr) { bufQueue.push(curr->right); }
			if (curr->height < curr_height) { //change < to <= ???
				//cout << endl;
				curr_height = curr->height;
			}
			cout << curr->key << " ";
			bufQueue.pop();
		}
		cout << endl;
	}
	// end private
};

#endif
