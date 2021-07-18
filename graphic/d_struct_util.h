#ifndef D_STRUCT_UTIL_H
#define D_STRUCT_UTIL_H

// =====================================================================
// because the linker is so picky about templates, I have to move ALL
// of the dataStructUtility code into one .h file and include it into
// main.cpp
// =====================================================================

// I did this myself, so feel free to copy paste without worrying about
// copyright claims coming to ruin your day

#define LEAF 0x400
#define BRANCH 0x100

namespace dataStructUtility {
	
	template <class T>
	struct node {
		T data;
		node<T> * next;
		node<T>* before;
	};

	template<class T>
	class linkedList {
		node<T>* head;
		node<T>* tail;
		int numOfNodes;
	public:
		linkedList();
		linkedList(T headData);
		linkedList(std::vector<T> list); // yes yes... I know, using vector is stupid and will eat up the stack, but I can't find a better alternative
		
		~linkedList();

		node<T>* getNode(int index);
		node<T>* deleteNode(int index);
		node<T>* addNode(T data);
		node<T>* addNodePos(T data, int index);
		int clearTree();
		void printList();

		node<T>* operator[](int index) {
			return getNode(index);
		}

	};

// ============================================================================

	template<class T>
	struct treeNode {
		T* data;
		short type;
		treeNode<T>* parent;
		unsigned short node_ptr[2]; // node 0 is left node, node 1 is right node
	};

	// there is only a node for the bianary tree because I will mainly use this for bsp tree construction.

};

// ========================= definition code ===============================

// note: I use templates to I can use more than one data type, using void* might be a better idea because it uses less space, but templates are easier for me to use

template<class T>
dataStructUtility::linkedList<T>::linkedList() {
	numOfNodes = 1;

	head = new node<T>;
	head->next = nullptr;
	head->before = nullptr;
}

template<class T>
dataStructUtility::linkedList<T>::linkedList(T headData) {
	numOfNodes = 1;

	head = new node<T>();
	head->data = headData;
	head->next = nullptr;
	head->before = nullptr;
}

template<class T>
dataStructUtility::linkedList<T>::linkedList(std::vector<T> list) {
	numOfNodes = list.size();
	head = new node<T>;
	head->data = list[0];
	head->before = nullptr;

	node<T>* currentNode = head;

	for (int i=1; i<list.size(); i++) {
		currentNode->next = new node<T>;
		currentNode->next->before = currentNode;
		currentNode->next->data = list[i];
		currentNode = currentNode->next;
	}

	tail = currentNode;
}

template<class T>
dataStructUtility::linkedList<T>::~linkedList() {
	clearTree(); // destructor
}

template<class T>
dataStructUtility::node<T>* 
dataStructUtility::linkedList<T>::getNode(int index) {
// gets the node at the specfied index ex. index = 1 would get the first node
	if (index < 0 || index > numOfNodes) {return nullptr;}

	node<T>* currentNode = head;
	for (int i = 0; i<index-1; i++) {
		currentNode = currentNode->next;
	}

	return currentNode;
}

template<class T>
dataStructUtility::node<T>* // delete a node at index and return it
dataStructUtility::linkedList<T>::deleteNode(int index) {
	if (index <= 0 || index > numOfNodes) {return nullptr;}

	node<T>* deleteNode = getNode(index); // get node to be deleted
	node<T>* beforeNode = getNode(index-1); // get node before deleted node
	node<T>* returnNode = (node<T>*)malloc(sizeof(node<T>)); 
	// copy the info of the deleted node into a value to be returned
	memcpy(returnNode, deleteNode, sizeof(node<T>));

	beforeNode->next = deleteNode->next; // delete node
	deleteNode->next->before = beforeNode;
	delete deleteNode;
	
	numOfNodes--;

	return returnNode; // return node deleted
	 // if return value is not used, it will have to be manually deleted
}

template<class T>
dataStructUtility::node<T>* // adds a node at the end of the list
dataStructUtility::linkedList<T>::addNode(T data) {

	tail->next = new node<T>; // pretty simple, explanation not needed
	tail->next->before = tail;
	tail = tail->next;
	tail->data = data;
	tail->next = nullptr;

	numOfNodes++;

	return tail;
}

template<class T>
dataStructUtility::node<T>*
dataStructUtility::linkedList<T>::addNodePos(T data, int index) {
	//node1 <-> node2 <-> node3 <-> node4 index = 2
	//node1 <-> newNode - node2 <-> node3 <-> node4
	//  ^                   |
	//  <------------------- *node2 still points back to node1
	//node1 <-> newNode <-> node2 <-> node3 <-> node4
	//node1 <-> node2 <-> node3 <-> node4 <-> node5



	if (index >= numOfNodes) {
		return nullptr;
	}

	node<T>* currentNode = getNode(index-1);


	node<T>* savePtr; // create pointer to save node
	savePtr = currentNode->next; // save node
	currentNode->next = new node<T>; // add node
	currentNode->next->data = data; // add data
	currentNode->next->before = currentNode; // new node before points to currentNode
	currentNode->next->next = savePtr; // put saved node after new node
	savePtr->before = currentNode->next; // set saveNode before pointer to next currentNode

	numOfNodes++;

	return currentNode->next;
}

template<class T>
void dataStructUtility::linkedList<T>::printList() {
	if (numOfNodes == 0) {return;}
	
	node<T>* currentNode = head;
	
	for (int i=0; i<numOfNodes; i++) {
		std::cout << currentNode->data << ' ';
		currentNode = currentNode->next;
	}
	std::cout << std::endl;
}

template<class T>
int dataStructUtility::linkedList<T>::clearTree() {
	if (numOfNodes == 0) {return 1;}

	node<T>* currentNode = tail;
	node<T>* beforeNode = tail->before;

	while (beforeNode != nullptr) {
		delete currentNode; // free memory and currentNode
		currentNode = beforeNode; // currentNode and beforeNode now point to same memory
		beforeNode = beforeNode->before; // set beforeNode to node before
	}

	delete currentNode;
	numOfNodes = 0;

	return 0;
}

// ==================== bianarytree =====================


#endif