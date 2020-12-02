/*
Nathan Obert M03134502
AVL tree implementations
*/

template <class T>
AVLTree<T>::AVLTree()
{
    root = nullptr;
    nodeCount = 0;
}

template <class T>
void AVLTree<T>::addMidpoints(vector<node<T>*> &nodeList, int start, int end)
{
    if (end - start >= 0) //as soon as the distance between the upper and lower number is negative, we have finished finding all midpoints
    {
        int midpoint = start + ( (end - start) / 2); //this will round down, if needed

        root = insertHelper(root, nodeList[midpoint] );
        nodeCount++;

        addMidpoints(nodeList, start, midpoint- 1); //all values to the left of given mid
        addMidpoints(nodeList, midpoint + 1, end); //all values to the right of given mid
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
                displayInOrderWithClients(root);
                break;
            }
            case 2:
            {
                displayInOrderWithAdmins(root);
                break;
            }
            case 3:
            {
                displayInOrderWithOfficials(root);
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
void AVLTree<T>::displayInOrderWithClients(node<T> *subRoot) const
{
    if (subRoot) //not nullptr
    {
        displayInOrderWithClients(subRoot->left);
        if (subRoot->list[1] == "client")
        {
            cout << subRoot->value << endl;
        }
        displayInOrderWithClients(subRoot->right);
    }
}

template <class T>
void AVLTree<T>::displayInOrderWithAdmins(node<T> *subRoot) const
{
    if (subRoot)
    {
        displayInOrderWithAdmins(subRoot->left);
        if (subRoot->list[1] == "admin")
        {
            cout << subRoot->value << endl;
        }
        displayInOrderWithAdmins(subRoot->right);
    }
}

template <class T>
void AVLTree<T>::displayInOrderWithOfficials(node<T> *subRoot) const
{
    if (subRoot)
    {
        displayInOrderWithOfficials(subRoot->left);
        if (subRoot->list[1] == "official")
        {
            cout << subRoot->value << endl;
        }
        displayInOrderWithOfficials(subRoot->right);
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
        cout << "Key: " << subRoot->value << endl << "Value: ";
        for (int i = 0; i < subRoot->list.size(); i++)
        {
            cout << subRoot->list[i];
            if (i + 1 < subRoot->list.size())
            {
                cout << " -- ";
            }
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
        mappedItemCount += newNode->list.size();
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
    if (root)
    {
        root = delHelper(root, value);
        nodeCount--;
    }    
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
            //cout << "No value found to delete" << endl;
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
            //cout << "No value found to delete" << endl;
            nodeCount++; //counter act lowering of node count in del
        }
    }
    else  //the values are equal, so need to start deleting
    {
        mappedItemCount -= subRoot->list.size();
        //at this point, need to see which case is being dealt with

        if (subRoot->left && subRoot->right) //two children present
        {
            node<T> *largestLeftNode = rightmost(subRoot->left); //finding max value in left subtree of del node
            subRoot->value = largestLeftNode->value; //copying value over
            subRoot->list = largestLeftNode->list; //deep copy of list
            mappedItemCount += largestLeftNode->list.size(); //if largestLeftNode is deleted, then we need to counter act the decrement to mappedItems
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
        //cout << "The value has been deleted" << endl;
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
            //cout << "No value found to delete" << endl;
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
            //cout << "No value found to delete" << endl;
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
                    subRoot->list = largestLeftNode->list; //copying list over, deep
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
vector<T> AVLTree<T>::returnMappedItems(T value) const
{
    node<T> *nodePtr = searchHelper(value, root);
    if (nodePtr)
    {
        return nodePtr->list;
    }
    else
    {
        vector<T> badValue = {};
        return badValue; //will be able to determine if good or bad based on size
    }
}

template <class T>
void AVLTree<T>::insertWithList(T value, vector<T> mappedItems)
{
    del(value); //delete old node that has bad values
    node<T> *newNode = new node<T>(value, mappedItems); //create node to replace
    root = insertHelper(root, newNode); //insert node to replace
    nodeCount++;
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

//Building and saving the tree is specific to type string. Any other 'class T' will not work as designed!
//it was added for the purposes of the final project
template <class T>
void AVLTree<T>::saveHelper(node<T> *traversalNode, ofstream &outFile) const
{
    if (traversalNode)
    {
        saveHelper(traversalNode->left, outFile);
        string toWrite = traversalNode->value + "%#";
        for (int i = 0; i < traversalNode->list.size(); i++)
        {
            toWrite += traversalNode->list[i];
            if (i < traversalNode->list.size() - 1)
            {
                toWrite += "%&";
            }
        }
        outFile << EncryptionBox::encrypt(toWrite) << endl;
        saveHelper(traversalNode->right, outFile);
    }
}

template <class T>
void AVLTree<T>::saveInfo(string filename) const
{
    EncryptionBox::positionInFile = 0;
    ofstream outFile;
    outFile.open(filename);
    if (outFile)
    {
        node<T> *traversalNode = root;
        saveHelper(traversalNode, outFile); //prints to .txt file with in order traversal starting at root
    }
    else
    {
        cout << "The info could not be saved at:" << endl << filename << endl;
    }
    outFile.close();
}

template <class T>
void AVLTree<T>::buildTree(string filename)
{
    EncryptionBox::positionInFile = 0;
    ifstream inFile(filename);
    if (inFile)
    {
        //ensuring tree is destroyed before building new one
        nodeCount = 0;
        mappedItemCount = 0;
        destroySubRoot(root);

        string text = "";
        vector<node<T>*> nodes = {};

        while (getline(inFile, text))
        {
            string keyDelim = "%#";
            string entryDelim = "%&";

            text = text.substr(0, text.rfind("\r")); //getting rid of carriage return at the end from .txt file
            text = EncryptionBox::decrypt(text); //decrypting text to further parse and store inside the tree
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
            }

            //vector entries is now ready to be inserted along with the key into the tree!
            node<T> *toBeInserted = new node<T>(key, entries); //creating node to store key and assoiated entries
            nodes.push_back(toBeInserted);
        }

        addMidpoints(nodes, 0, nodes.size() - 1); //adding items such that tree has min height
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

template <class T>
void AVLTree<T>::getStatistics() const
{
    unsigned int minHeight = log2(2 * nodeCount);
    cout << endl;
    cout << "Total keys stored: " << nodeCount << endl;
    cout << "Total items mapped to keys: " << mappedItemCount << endl;
    cout << "Height of tree: " << getHeight() << endl;
    cout << "Theoretical minimum height: " << minHeight << endl;
    cout << "Total leaf count: " << getLeaves() << endl;
}
