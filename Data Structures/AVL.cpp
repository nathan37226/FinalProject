/*
Nathan Obert
AVL tree implementations
Writing to and building from text files brougt up the need for special delimiter characters.
Since this is essentially a dictionary, mapping a key to an entry, there exists a need for
a delimiter between the key and entry as well as consecutive entry information.
*/

template <class T>
AVLTree<T>::AVLTree()
{
    root = nullptr;
    nodeCount = 0;
}

template <class T>
AVLTree<T>::AVLTree(string filename)
{
    //initial constructor
    root = nullptr;
    nodeCount = 0;

    //adding everything from file, if possible
    vector<T> valueList = {};
    ifstream inFile(filename);

    if (inFile)
    {
        string fileLine;

        while (getline(inFile, fileLine))
        {
            T newItem(fileLine); //creates the item from the line line in file
            valueList.push_back(newItem); //adds to a vector from which we will create the minimum height tree
        }
        
        addMidpoints(valueList, 0, valueList.size() - 1); //adding items such that tree has min height
    }
    else
    {
        cout << "No prior records were found" << endl;
    }
    inFile.close();
}

template <class T>
void AVLTree<T>::addMidpoints(vector<T> &valueList, int start, int end)
{
    if (end - start >= 0) //as soon as the distance between the upper and lower number is negative, we have finished finding all midpoints
    {
        int midpoint = start + ( (end - start) / 2); //this will round down, if needed

        insert( valueList[midpoint] ); 

        addMidpoints(valueList, start, midpoint- 1); //all values to the left of given mid
        addMidpoints(valueList, midpoint + 1, end); //all values to the right of given mid
    }
}

template <class T>
void AVLTree<T>::destroySubRoot(node<T> *&subRoot)
{
    if (subRoot)
    {
        node<T> *leftNode = subRoot->left;
        node<T> *rightNode = subRoot->right;
        delete subRoot;

        if (leftNode) //if not nullptr, then destroy that subtree
            destroySubRoot(leftNode);

        if (rightNode)
            destroySubRoot(rightNode);
    } 
}

template <class T>
AVLTree<T>::~AVLTree()
{
    destroySubRoot(root);
}

template <class T>
void AVLTree<T>::display(int option) const
{
    if (nodeCount == 0)
    {
        cout << "There are no values to display." << endl;
    }
    else
    {    
        switch (option)
        {
            case 1:
            {
                displayInOrder(root);
                break;
            }
            case 2:
            {
                displayPreOrder(root);
                break;
            }
            case 3:
            {
                displayPostOrder(root);
                break;
            }
            case 4:
            {
                displayInOrderWithHeight(root);
                break;
            }
            case 5:
            {
                displayInOrderWithMappedItems(root);
                break;
            }
        }
    }
}

template <class T>
void AVLTree<T>::displayInOrder(node<T> *subRoot) const
{
    if (subRoot) //not nullptr
    {
        displayInOrder(subRoot->left);
        cout << subRoot->value << endl;
        displayInOrder(subRoot->right);
    }
}

template <class T>
void AVLTree<T>::displayPreOrder(node<T> *subRoot) const
{
    if (subRoot)
    {
        cout << subRoot->value << endl;
        displayPreOrder(subRoot->left);
        displayPreOrder(subRoot->right);
    }
}

template <class T>
void AVLTree<T>::displayPostOrder(node<T> *subRoot) const
{
    if (subRoot)
    {
        displayPostOrder(subRoot->left);
        displayPostOrder(subRoot->right);
        cout << subRoot->value << endl;
    }
}

template <class T>
void AVLTree<T>::displayInOrderWithHeight(node<T> *subRoot) const
{
    if (subRoot) //not nullptr
    {
        displayInOrderWithHeight(subRoot->left);
        cout << subRoot->value << " Height: " << subRoot->height << endl;
        displayInOrderWithHeight(subRoot->right);
    }
}

template <class T>
void AVLTree<T>::displayInOrderWithMappedItems(node<T> *subRoot) const
{
    if (subRoot)
    {
        displayInOrderWithMappedItems(subRoot->left);
        cout << subRoot->value << endl;
        for (int i = 0; i < subRoot->list.size(); i++)
        {
            cout << subRoot->list[i] << " ";
        }
        cout << endl;
        displayInOrderWithMappedItems(subRoot->right);
    }
}

template <class T>
int AVLTree<T>::height(node<T> *&subRoot) const
{
    if (!subRoot)
    {
        return 0;
    }
    else
    {
        return subRoot->height;
    }
}

template <class T>
int AVLTree<T>::getNodeBalance(node<T> *subRoot) const
{
    if (!subRoot)
    {
        return 0;
    }
    else
    {
        return height(subRoot->left) - height(subRoot->right);
    }    
}

template <class T>
node<T>* AVLTree<T>::rightRotation(node<T> *&x)
{
    //getting points to nodes being moved                           //         x                   y
    node<T> *y = x->left;                                           //    y         z    =>    q       x    
    node<T> *q = y->right;                                          // p     q                       q    z
                                                                    
    //performing rotation                                           
    y->right = x;
    x->left = q;

    //updating heights of nodes
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return y; //new root
}

template <class T>
node<T>* AVLTree<T>::leftRotation(node<T> *&y)
{
    node<T> *x = y->right;                                          //      y                     x
    node<T> *q = x->left;                                           //  p       x     =>     y         z
 //                                                                 //        q   z       p     q  
    y->right = q;
    x->left = y;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    
    return x; //new root
}

template<class T>
node<T>* AVLTree<T>::insertHelper(node<T> *&subRoot, node<T> *&newNode)
{
    //inserting the newNode
    if (!subRoot) 
    {
        return newNode; 
    } 
    else if (newNode->value < subRoot->value)
    {
        subRoot->left = insertHelper(subRoot->left, newNode);  
    }
    else if (newNode->value > subRoot->value) 
    { 
        subRoot->right = insertHelper(subRoot->right, newNode);  
    }
    else //equal values 
    {
        nodeCount--; //counteract node count increment in insert
        cout << "Duplicate entries are not allowed. Insertion Terminated." << endl; //no return here, just let program continue and backtrack up the tree
    }
    
    //setting new height of each subRoot
    subRoot->height = max(height(subRoot->left), height(subRoot->right)) + 1;

    //now to do rotations
    performRotations(subRoot);

    return subRoot; //returning root node
}

template <class T>
void AVLTree<T>::insert(T value)
{
    node<T> *newNode = new node<T>(value);
    root = insertHelper(root, newNode); //puts node into list
    nodeCount++;
}

template <class T>
node<T>* AVLTree<T>::insertWithItemHelper(node<T> *&subRoot, node<T> *&newNode)
{
    //inserting the newNode
    if (!subRoot) 
    {
        return newNode; 
    } 
    else if (newNode->value < subRoot->value)
    {
        subRoot->left = insertWithItemHelper(subRoot->left, newNode);  
    }
    else if (newNode->value > subRoot->value) 
    { 
        subRoot->right = insertWithItemHelper(subRoot->right, newNode);  
    }
    else //equal values, so new mappedItem into list, if not there. If there, say that
    {
        bool isNewValue = true;
        for (int i = 0; i < subRoot->list.size(); i++)
        {
            if (newNode->list[0] == subRoot->list[i]) //newNode will only have the one value, subRoot could have multiple
            {
                cout << "Duplicate entries inside a node's list are not allowed. Insertion terminated." << endl;
                isNewValue = false;
                break; //terminate loop since found to be there
            }
        }
        if (isNewValue)
        {
            subRoot->list.push_back(newNode->list[0]); //is not inside subRoot's list already, then 
            mappedItemCount++; //increase counter
        }       

        nodeCount--; //counteract node count increment in insert
    }
    
    //setting new height of each subRoot
    subRoot->height = max(height(subRoot->left), height(subRoot->right)) + 1;

    //now to do rotations
    performRotations(subRoot);

    return subRoot; //returning root node
}

template <class T>
void AVLTree<T>::insertWithItem(T value, T mappedItem)
{
    node<T> *newNode = new node<T>(value, mappedItem);
    root = insertWithItemHelper(root, newNode);
    nodeCount++;
}

template <class T>
bool AVLTree<T>::isBalanced() const
{
    int bal = getNodeBalance(root);
    bool status = ( (bal >= -1) && (bal <= 1) ) ? true : false;
    return status;
}

template <class T>
void AVLTree<T>::del(T value)
{
    root = delHelper(root, value);
    nodeCount--;
}

template <class T>
node<T>* AVLTree<T>::rightmost(node<T> *&subRoot)
{
    while (subRoot->right)
    {
        subRoot = subRoot->right;
    }
    return subRoot;
}

template <class T>
node<T>* AVLTree<T>::delHelper(node<T> *&subRoot, T value)
{
    if (value < subRoot->value)
    {
        if (subRoot->left)
        {
            subRoot->left = delHelper(subRoot->left, value);
        }
        else
        {
            cout << "No value found to delete" << endl;
            nodeCount++;
        }
    }
    else if (value > subRoot->value)
    {
        if (subRoot->right)
        {
            subRoot->right = delHelper(subRoot->right, value);
        }
        else
        {
            cout << "No value found to delete" << endl;
            nodeCount++; //counter act lowering of node count in del
        }
    }
    else  //the values are equal, so need to start deleting
    {
        //at this point, need to see which case is being dealt with

        if (subRoot->left && subRoot->right) //two children present
        {
            node<T> *largestLeftNode = rightmost(subRoot->left); //finding max value in left subtree of del node
            subRoot->value = largestLeftNode->value; //copying value over
            subRoot->left = delHelper(subRoot->left, largestLeftNode->value); //delete max value in left subtree
        }

        else if (!subRoot->left && !subRoot->right) //is a leaf
        {
            delete subRoot;  //deallocating memory
            subRoot = nullptr; //node takes on nullptr since no further nodes down the tree exist
            return subRoot; //since leaf, we just need to return back to previous function call
        }

        else  //only one child exists
        {
            node<T>* child = (subRoot->right) ? subRoot->right : subRoot->left; //makes child either left or right, since we don't know
            delete subRoot; //deallocating parent
            subRoot = child; //takes on child's value so that returning will be correct
        }
        cout << "The value has been deleted" << endl;
    }

    //updating height of each node we went to, from bottom up
    subRoot->height = max(height(subRoot->left), height(subRoot->right)) + 1;

    //performing rotations, if need be
    performRotations(subRoot);

    //at the very end, this will return the root, the first node entered this function
    return subRoot;
}

template <class T>
void AVLTree<T>::delMappedItem(T value, T mappedItem)
{
    root = delMappedItemHelper(root, value, mappedItem);
    nodeCount--;
}

template <class T>
node<T>* AVLTree<T>::delMappedItemHelper(node<T> *&subRoot, T value, T mappedItem)
{
    //NOTE: most of this code is the same as delHelper. The exception being if there is more than one item in node's list
    if (value < subRoot->value)
    {
        if (subRoot->left)
        {
            subRoot->left = delMappedItemHelper(subRoot->left, value, mappedItem);
        }
        else
        {
            cout << "No value found to delete" << endl;
            nodeCount++;
        }
    }
    else if (value > subRoot->value)
    {
        if (subRoot->right)
        {
            subRoot->right = delMappedItemHelper(subRoot->right, value, mappedItem);
        }
        else
        {
            cout << "No value found to delete" << endl;
            nodeCount++; //counter act lowering of node count in del
        }
    }
    else  //the values are equal, so need to start deleting either mappedItem or entire node
    {
        bool isPresent = false;
        int index = -1;

        for (int i = 0; i < subRoot->list.size(); i++) //checking to see if the mapped item exists in the entry
        {
            if (mappedItem == subRoot->list[i])
            {
                isPresent = true;
                index = i; //which index the mappedValue is at
                break;
            }
        }

        if (isPresent) 
        {
            if (subRoot->list.size() == 1) //only the single mappedItem present in list, so need to delete the entire node
            {
                //NOTE: this following code is straight out of deleting the entire node from delHelper
                //Not necessary to read through again

                if (subRoot->left && subRoot->right) //two children present
                {
                    node<T> *largestLeftNode = rightmost(subRoot->left); //finding max value in left subtree of del node
                    subRoot->value = largestLeftNode->value; //copying value over
                    subRoot->left = delMappedItemHelper(subRoot->left, largestLeftNode->value, mappedItem); //delete max value in left subtree
                }

                else if (!subRoot->left && !subRoot->right) //is a leaf
                {
                    delete subRoot;  //deallocating memory
                    subRoot = nullptr; //node takes on nullptr since no further nodes down the tree exist
                    return subRoot; //since leaf, we just need to return back to previous function call
                }

                else  //only one child exists
                {
                    node<T>* child = (subRoot->right) ? subRoot->right : subRoot->left; //makes child either left or right, since we don't know
                    delete subRoot; //deallocating parent
                    subRoot = child; //takes on child's value so that returning will be correct
                }

            }
            
            else //there are more than one entry inside the list, so can just remove item from vector
            {
                subRoot->list.erase( subRoot->list.begin() + index ); //iterator for which position the mappedValue is at
                nodeCount++; //counteracting decrement earlier on
            }
            mappedItemCount--;
        }

        else
        {
            nodeCount++;
            cout << "The mapped item could not be found to delete." << endl;
        }
        
    }

    //updating height of each node we went to, from bottom up
    subRoot->height = max(height(subRoot->left), height(subRoot->right)) + 1;

    //performing rotations, if need be
    performRotations(subRoot);

    //at the very end, this will return the root, the first node entered this function
    return subRoot;
}

template <class T>
void AVLTree<T>::performRotations(node<T> *&subRoot)
{
    int bal = getNodeBalance(subRoot);
    int leftBal = getNodeBalance(subRoot->left);
    int rightBal = getNodeBalance(subRoot->right);

    if ( (bal > 1) && (leftBal >= 0) ) //Left heavy, Right case
    {
        subRoot = rightRotation(subRoot);
    }
    else if ( (bal > 1) && (leftBal < 0) ) //Left heavy, Left Right case
    {
        subRoot->left = leftRotation(subRoot->left);
        subRoot = rightRotation(subRoot);
    }
    else if ( (bal < -1) && (rightBal > 0) ) //Right heavy, Right Left case
    {
        subRoot->right = rightRotation(subRoot->right);
        subRoot = leftRotation(subRoot);
    }
    else if ( (bal < -1) && (rightBal <= 0) ) //Right heavy, Left case
    {
        subRoot = leftRotation(subRoot);
    }
}

template <class T>
void AVLTree<T>::search(T value) const
{
    node<T> *nodePtr = searchHelper(value, root);
    if (nodePtr)
    {
        cout << "That record exists!" << endl;
        for (int i = 0; i < nodePtr->list.size(); i++)
        {
            cout << nodePtr->list[i] << " ";
        }
        cout << endl; 
    }
    else
    {
        cout << "There is no record of that entry." << endl;
    }
}

template <class T>
node<T>* AVLTree<T>::searchHelper(T val, node<T> *subRoot) const
{
    if (subRoot) //if nullptr is ever reached, returns a nullptr
    {
        if (val < subRoot->value)
        {
            return searchHelper(val, subRoot->left);
        }
        else if (val > subRoot->value)
        {
            return searchHelper(val, subRoot->right);
        }
        else
        {
            return subRoot;
        }
    }
    else
    {
        return nullptr;
    }
}

template <class T>
void AVLTree<T>::saveHelper(node<T> *traversalNode, ofstream &outFile) const
{
    if (traversalNode)
    {
        saveHelper(traversalNode->left, outFile);
        outFile << traversalNode->value << "^|";
        for (int i = 0; i < traversalNode->list.size(); i++)
        {
            outFile << traversalNode->list[i];
            if (i < traversalNode->list.size() - 1)
            {
                outFile << "^_";
            }
        }
        outFile << endl;
        saveHelper(traversalNode->right, outFile);
    }
}

template <class T>
void AVLTree<T>::saveInfo(string filename) const
{
    ofstream outFile;
    outFile.open(filename);

    node<T> *traversalNode = root;
    saveHelper(traversalNode, outFile); //prints to .txt file with in order traversal starting at root

    outFile.close();
}

template <class T>
void AVLTree<T>::buildTree(string filename)
{
    ifstream inFile(filename);
    if (inFile)
    {
        string text = "";
        while (getline(inFile, text))
        {
            string keyDelim = "^|";
            string entryDelim = "^_";

            text = text.substr(0, text.rfind("\r")); //getting rid of carriage return at the end from .txt file
            string key = text.substr(0, text.find(keyDelim));
            text = text.substr(key.length() + keyDelim.length(), string::npos);

            vector<string> entries = {};
            string entry = "";
            int pos1 = -entryDelim.length();
            int pos2 = 0;

            while (pos2 != string::npos) //while not reached end of string
            {
                //essentially, pos1 and pos2 move thru the string to get each entry out
                pos2 = text.find(entryDelim, pos1 + entryDelim.length()); 
                entry = text.substr(pos1 + entryDelim.length(), pos2 - pos1 - entryDelim.length()); 
                pos1 = pos2; 
                entries.push_back(entry); 
                cout << entry << endl;
            }

            //vector entries is now ready to be inserted along with the key into the tree!
            node<T> *toBeInserted = new node<T>(key, entries); //creating node to store key and assoiated entries
            root = insertHelper(root, toBeInserted); //does the actual insertion
        }
    }
    else
    {
        cout << "Could not access file at:" << endl << filename << endl;
    }
}

template <class T>
int AVLTree<T>::leafCount(node<T> *subRoot) const
{
    if (!subRoot) //if a nullptr is reached, not a leaf, so 0
    {
        return 0;
    }
    else if (!subRoot->left && !subRoot->right) //if a leaf is reached, then add 1
    {
        return 1;
    }
    else
    {
        return leafCount(subRoot->left) + leafCount(subRoot->right); //will recursivly add up 1's and 0's to find leaf count
    }
}

template <class T>
int AVLTree<T>::getLeaves() const
{
    return leafCount(root);
}

template <class T>
int AVLTree<T>::getHeight() const
{
    node<T> *temp = root; //since root is a member, put inside height function
    return height(temp);
}

template <class T>
int AVLTree<T>::getNodeCount() const
{
    return nodeCount;
}
