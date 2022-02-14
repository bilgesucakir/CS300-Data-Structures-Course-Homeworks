#include "BVHTree.h"

//CS300HW3 Bilgesu Cakir 27889

BVHTree::BVHTree(){ //constructor
	root = NULL;
}

BVHTree::~BVHTree(){ //destructor
	map.clear();
	deleteTree(root);

}

bool BVHTree::isItLeftChild(BVHTreeNode * node){ //returns true if given node is left child of its parent
	return (node->parent->leftChild == node);
}

void BVHTree::printNode(std::ostream &out, BVHTreeNode *node, int level) {
	if (root == nullptr) return;
	for (int i = 0; i < level; i++) {
		out << "  ";
	}
	if (!node->isLeaf) {
		out << "+ branch || ";
		node->aabb.printAABB(out);
		out << std::endl;
		printNode(out, node->rightChild, level + 1);
		printNode(out, node->leftChild, level + 1);
	}
	else {
		out << "- ";
		if (node->parent) {
			if (node->parent->rightChild == node)
				out << "R ";
			else
				out << "L ";
		}
		out << "- leaf: " << node->name << " || ";
		node->aabb.printAABB(out);
		out << std::endl;
	}
}
std::ostream &operator<<(std::ostream &out, BVHTree &tree) {
	tree.printNode(out, tree.root, 0);
	return out;
}

void BVHTree::addBVHMember(AABB objectArea, std::string name){
	BVHTreeNode * newNode = new BVHTreeNode(objectArea, name, true);
	map[name] = newNode;

	if(root == NULL){ //if the tree is empty
		newNode->parent = NULL;
		root = newNode;
	}
	else{
		if(root->isLeaf){//if there is only the root in the tree
			BVHTreeNode * oldRoot = root;

			AABB oldRootAABB = oldRoot->aabb;
			AABB newNodeAABB = newNode->aabb;
			AABB newAABB = oldRootAABB + newNodeAABB;

			root = new BVHTreeNode(newAABB, "root", false);
			root->leftChild = newNode;
			newNode->parent = root;
			root->rightChild = oldRoot;
			oldRoot->parent = root;
		}
		else{
			BVHTreeNode * existingLeaf = pickExistingLeaf(newNode);

			AABB existingLeafAABB = existingLeaf->aabb;
			AABB newNodeAABB = newNode->aabb;
			AABB newAABB = existingLeafAABB + newNodeAABB;

			BVHTreeNode * newBranchNode = new BVHTreeNode(newAABB, "branch", false);

			newBranchNode->parent = existingLeaf->parent;

			if(isItLeftChild(existingLeaf)){
				existingLeaf->parent->leftChild = newBranchNode;
			}
			else{
				existingLeaf->parent->rightChild = newBranchNode;
			}

			newBranchNode->leftChild = newNode;
			newNode->parent = newBranchNode;
			newBranchNode->rightChild = existingLeaf;
			existingLeaf->parent = newBranchNode;

			fixParentAreas(newNode->parent); //to fix the areas of the upper nodes in the tree according to this newly added node
		}
	}
}

BVHTreeNode * BVHTree::pickExistingLeaf(BVHTreeNode * newNode){ //this function returns the leaf node that increases the area of the tree less when new node is added

	BVHTreeNode * branchNode = root;
	while(! branchNode->isLeaf){

		int increaseInRightTreeSize = AABB::unionArea(newNode->aabb, branchNode->rightChild->aabb) - branchNode->rightChild->aabb.getArea();
		int increaseInLeftTreeSize = AABB::unionArea(newNode->aabb, branchNode->leftChild->aabb) - branchNode->leftChild->aabb.getArea();

		if(increaseInRightTreeSize < increaseInLeftTreeSize){
			branchNode = branchNode->rightChild;
		}
		else{
			branchNode = branchNode->leftChild;
		}
	}
	return branchNode;
}

void BVHTree::fixParentAreas(BVHTreeNode * node){ //node is the parent of the newly added node
	if(node == root){
		return;
	}
	else{
		BVHTreeNode * nodeCopy = node;
		while(nodeCopy->parent != NULL){
			AABB newAABB = nodeCopy->aabb;

			if(isItLeftChild(nodeCopy)){
				newAABB = newAABB + nodeCopy->parent->rightChild->aabb;
			}
			else{
				newAABB = newAABB + nodeCopy->parent->leftChild->aabb;
			}

			nodeCopy->parent->aabb = newAABB;
			nodeCopy = nodeCopy->parent;
		}
	}
}

void BVHTree::deleteTree(BVHTreeNode * node){ //recursive function for deleting the nodes of a tree
	if(! node->isLeaf){
		deleteTree(node->leftChild);
		deleteTree(node->rightChild);
	}
	delete node;
	node = NULL;
}

void BVHTree::moveBVHMember(std::string name, AABB newLocation){
	if(map.find(name) != map.end()){
		BVHTreeNode * nodeToMove = map[name];
		if(newLocation.maxX <= nodeToMove->parent->aabb.maxX && newLocation.maxY <= nodeToMove->parent->aabb.maxY && newLocation.minX >= nodeToMove->parent->aabb.minX && newLocation.minY >= nodeToMove->parent->aabb.minY){
			nodeToMove->aabb = newLocation;
		}
		else{
			std::string theName = nodeToMove->name;

			removeBVHMember(nodeToMove->name);
			addBVHMember(newLocation, theName);
		}
	}
	else{
		return;
	}
}


void BVHTree::removeBVHMember(std::string name){
	if(map.find(name) != map.end()){

		BVHTreeNode * toRemove = map[name];
		BVHTreeNode * siblingOfToRemove;
		BVHTreeNode * branchNode = toRemove->parent;
		bool isParentRoot = false;

		if(branchNode == root){
			isParentRoot = true;
		}
		if(isItLeftChild(toRemove)){
			siblingOfToRemove = branchNode->rightChild;
		}
		else{
			siblingOfToRemove = branchNode->leftChild;
		}
	
		siblingOfToRemove->parent = branchNode->parent;

		std::string branchName = branchNode->name;

		if(isParentRoot == true){
			root = siblingOfToRemove;

			delete branchNode;
			branchNode = NULL;
			map.erase(branchName);
			map.erase(name);
			delete toRemove;
			toRemove = NULL;
		}
		else{

			bool isParentLeft = false;
			if(isItLeftChild(branchNode)){
				isParentLeft = true;
			}

			delete branchNode;
			branchNode = NULL;
			map.erase(branchName);
			map.erase(name);
			delete toRemove;
			toRemove = NULL;

			if(isParentLeft){
				siblingOfToRemove->parent->leftChild = siblingOfToRemove;
			}
			else{
				siblingOfToRemove->parent->rightChild = siblingOfToRemove;
			}

			fixParentAreas(siblingOfToRemove); //again we need to fix the areas of the parents when we remove a node
		}
	}
	else{
		return;
	}
}

void BVHTree::collidingRecursiveInner(BVHTreeNode * node, std::vector<std::string> & vec, AABB object){ //to use recursion in the getCollidingObjects function
	if(object.collide(node->aabb)){
		if(node->isLeaf){
			vec.push_back(node->name); //if collides with given object an if it is a leaf node, then add to the vector (its name)
		}
		else{
			collidingRecursiveInner(node->leftChild, vec, object); //if a branch, then check the children.
			collidingRecursiveInner(node->rightChild, vec, object);
		}
	}
}

std::vector<std::string> BVHTree::getCollidingObjects(AABB object){
	std::vector<std::string> vec;
	collidingRecursiveInner(root, vec, object);

	return vec;
}