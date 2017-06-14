/*****************************************************************
// File Name:  Table.cpp
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       June 2nd, 2017
// Class:      CS163
// Program #:  4
//
// Overview: This is the implementation file for the Table,
// implemented as a binary search tree, for use in the fourth
// programming assignment. This file also contains the definitions
// for the Node struct's constructor and destructor. Information
// about the data (Events) being stored in the Nodes can be found
// in the Event.cpp/h files. For more information about the Table
// and Node, see the function comments below, and the comments in
// the Table.h file.
//
// ***************************************************************/

#include "Table.h"


// ***************** |
// Node Functions *  |
// ***************** V


/* ********************************************
// Initializes node pointers to NULL.
//
// ********************************************/
Node :: Node(void)
{
  // Nullify node fields.
  left = NULL;
  right = NULL;
}



/* ********************************************
// Sets node pointers to NULL.
//
// ********************************************/
Node :: ~Node(void)
{
  // Nullify node fields.
  left = NULL;
  right = NULL;
}



// ***************** |
// Table Functions * |
// ***************** V


/* ********************************************
// Initialize the tree fields and pointers to 0,
// NULL.
//
// ********************************************/
Table :: Table(void)
{
  // Init fields to 0.
  height = 0;
  current_size = 0;

  // Init the tree to NULL.
  root = NULL;

  return;
}



/* ********************************************
// Copy constructor for future use.
//
// ********************************************/
Table :: Table(const Table & table)
{ return; }



/* ********************************************
// Deallocate all events in the tree. Reset
// class fields and pointers to 0, NULL.
//
// ********************************************/
Table :: ~Table(void)
{
  // Deallocate and nullify the tree.
  deleteTree(root);

  // Reset fields to 0.
  height = 0;
  current_size = 0;

  return;
}



/* ********************************************
// Public insert - Calls the private insert
// function on the root of the tree.
//
// ********************************************/
int Table :: insert(const Event & new_event)
{
  // If the height has not been
  // initialized, set it to 1.
  if(height == 0) ++height;

  // Create a new node with the given data.
  Node * new_node = new Node;
  new_node->data.copy(new_event);

  // Insert the new event, return the
  // level at which it's inserted.
  return insert(new_node, root);
}



/* ********************************************
// Private insert - Inserts the new event into
// the tree in alphabetical order.
//
// ********************************************/
int Table :: insert( Node *& new_node,
                     Node *& root)
{
  // The value returned from calling
  // insert on a subtree of root.
  unsigned int return_val = 0;

  // Pointer to the right or left subtree of root,
  // depending on the alphabetical difference between
  // root->data and new_event.
  Node ** subtree = NULL;

  // If root is NULL..
  if(!root)
  {
    // Set root to the new node.
    root = new_node;
    // Increment the total number of nodes.
    ++current_size;
    // Add 1 to the return value.
    return 1;
  }

         // The name of the event in root.
  char * root_name = root->data.name,
         // The name of the new event.
       * new_name = new_node->data.name;

  // If the new event's name alphabetically
  // precedes the name in root's data..
  if(Table::min(new_name, root_name) == new_name)
    // Temporarily store root's left subtree.
    subtree = &root->left;
  // Temporarily store root's right subtree.
  else subtree = &root->right;

  // Call insert on subtree, store the returned value.
  return_val = 1 + insert(new_node, *subtree);

  // Update the height.
  height = std::max(height, return_val);

  // Return the level at which the insertion took place.
  return return_val;
}



/* ********************************************
// Public retrieve - Calls / returns the private
// retrieve function with root.
//
// ********************************************/
bool Table :: retrieve( char * name,
                        Event & event_match ) const
{
  // If the name is NULL, return false.
  if(!name) return false;

  // Try to locate the event with
  // the private retrieve function.
  return retrieve(name, event_match, root);
}



/* ********************************************
// Private retrieve - Searches for the specified
// name. If data with the specified name exists
// in the tree, it's copied into the Event param.
//
// ********************************************/
bool Table :: retrieve( char * name,
                        Event & event_match,
                        Node * root ) const
{
  // If the tree does not contain the
  // specified event, report failure.
  if(!root) return false;

  // If the name matches the name of
  // the event in the current node..
  if(!strcmp(root->data.name, name))
  {
    // Copy the matched event into event_match.
    event_match.copy(root->data);

    // Report success.
    return true;
  }

  // If the name alphabetically preceeds the name of the event
  // in the current node, call retrieve on it's left subtree.
  if(!strcmp(name, Table::min(root->data.name, name)))
    return retrieve(name, event_match, root->left);

  // If the name alphabetically exceeds the name of the event
  // in the current node, call retrieve on it's right subtree.
  return retrieve(name, event_match, root->right);
}


/* ********************************************
// Public display - Calls the private display
// function on the root.
//
// ********************************************/
int Table :: display(void) const
{
	// std::cout << "\n\tHeight => " << getHeight() << std::endl;

  // Display the tree, return the number of displayed Nodes.
  return display(root);
}



/* ********************************************
// Displays the event in every Node. Returns
// the number of displayed events.
//
// ********************************************/
int Table :: display(Node * root) const
{
  // If the current Node is NULL, return 0.
  if(!root) return 0;

  // If the left subtree of the current Node is
  // not NULL, call the display function on it.
  int nodes = display(root->left) + 1;
  // Display a line of whitespace.
  std::cout << std::endl;
  // Display the contents of the current Node.
  root->data.display();

  // Display the right subtree. Return
  // the number of Nodes in the tree.
  return nodes + display(root->right);
}



/* ********************************************
// Public removeEventName - As long as the
// given name is valid.. calls the private
// removeEventName with root.
//
// ********************************************/
bool Table :: removeEventName(char * name)
{
  // If the name is NULL, return error code -1.
  if(!name) return false;

  // Call the private removeEventName function on root.
  return removeEventName(name, root);
}



/* ********************************************
// Private removeEventName - Traverses the tree
// in order, until an event is found with the
// supplied name. If and when a match is found,
// the matching event is removed, and that node
// is deleted / replaced by the in order
// successor where possible.
//
// ********************************************/
bool Table :: removeEventName( char * name,
                               Node *& root )
{
  // If the event could not be found, return 0.
  if(!root) return false;

  // If the name matches the name of
  // the event in the current node..
  if(!strcmp(root->data.name, name))
  {
           // Temporarily store the
           // inorder successor of root.
    Node * temp = root->right,
           // Will store the parent, wherever possible
           // of root's inorder successor's parent.
         * temp_parent = root;

    // If root has no children..
    if(isLeaf(root))
    {
      // Delete and nullify root.
      delete root;
      root = NULL;
    }
    // If root has only one (right) subtree..
    else if(!root->left)
    {
      // Temporarily store root's right subtree
      temp = root->right;
      // Delete root, and replace with its right subtree.
      delete root;
      root = temp;
    }
    // If root has only one (left) subtree..
    else if(!root->right)
    {
      // Temporarily store root's left subtree
      temp = root->left;
      // Delete root, and replace with its left subtree.
      delete root;
      root = temp;
    }
    // If root has both subtrees..
    else
    {
      // Find root's inorder successor.
      while(temp->left)
      {
        // Store the parent of the current
        // most left node of root.
        temp_parent = temp;
				// Set temp to root's new most left node.
        temp = temp->left;
      }

      // Copy the inorder successor's data into root's data.
      root->data.copy(temp->data);

			// Set the inorder successor's parent's left subtree
			// to the right subtree of the inorder successor.
			temp_parent->left = temp->right;

			// Delete the inorder successor.
      delete temp;
    }

    // Report success.
    return true;
  }

  // If the name alphabetically preceeds the name of the event
  // in the current node, call remove on it's left subtree.
  if(!strcmp(name, Table::min(root->data.name, name)))
    return removeEventName(name, root->left);

  // If the name alphabetically exceeds the name of the event
  // in the current node, call remove on it's right subtree.
  return removeEventName(name, root->right);
}



/* ********************************************
// Public removeEventKey - As long as the given
// keyword is valid.. calls the private
// removeEventKey with root.
//
// ********************************************/
int Table :: removeEventKey(const char * keyword)
{
  // If the keyword is NULL, return error code -1.
  if(!keyword) return -1;

  // Call the private removeEventKey function on root.
  return removeEventKey(keyword, root);
}



/* ********************************************
// Private removeEventKey - Traverses the whole
// tree, checks for a keyword match in each
// event, removes nodes with matches using the
// private removeEventName function. Returns
// the number of deleted nodes.
//
// ********************************************/
int Table :: removeEventKey( const char * keyword,
                             Node *& root )
{
  // If the tree is empty, return 0.
  if(!root) return 0;

        // Get the number of keywords in the current event.
  short keywords = root->data.keywordCount(),
        // General index counter.
        g_index = 0;

	// Sum of removeEventKey subtree values.
	int remove_sum = 0;

  // Loop control for searching for keyword matches.
  bool match = false;

  // For every keyword in the current event,
  // and while no match has been found..
  while(g_index < keywords && !match)
    // If the current event keyword matches the keyword param..
    if(!strcmp(keyword, root->data.keywords[g_index++]))
      // Match found - Update loop control variable.
      match = true;

	// Store the return values from recursive removeEventKey calls.
	remove_sum = removeEventKey(keyword, root->left) + removeEventKey(keyword, root->right);
	// Add 1 if root contains the specified keyword.
	remove_sum += match? 1 : 0;
	
	// If the current root contains the specified keyword..
	if(match)
  	// The current event does contain the specified keyword.
  	removeEventName(root->data.name, root);

	// Return the number of deleted nodes.
	return remove_sum;
}



/* ********************************************
// Deallocates and nullifys the tree, returns
// the number of deleted nodes. Postorder.
//
// ********************************************/
int Table :: deleteTree(Node *& root)
{
  // If the current Node is NULL, return 0.
  if(!root) return 0;

  // Call delete on the children of the current Node. Store
  // the return values to return to the calling function.
  int return_val = deleteTree(root->left) + deleteTree(root->right);

  // Delete and nullify the current Node.
  delete root;
  root = NULL;

  // Return the number of deleted Nodes.
  return 1 + return_val;
}



/* ********************************************
// Returns the height of the tree.
//
// ********************************************/
int Table :: getHeight(void) const
{
  // Return value of height field.
  //return height;

  // Recalculate and return height.
  return calcHeight(root);
}



/* ********************************************
// Recursively calculates and returns the
// height of the tree. (private).
//
// ********************************************/
int Table :: calcHeight(Node * root) const
{
  // If root is NULL, stop counting.
  if(!root) return 0;

  // Return the height of the taller subtree.
  return 1 + std::max(calcHeight(root->right), calcHeight(root->left));
}



/* ********************************************
// Checks whether a node is a leaf. (private).
//
// ********************************************/
bool Table :: isLeaf(Node * node) const
{
  // If node isn't NULL, and it has not
  // children, report that node is a leaf.
  if(node && !node->left && !node->right ) return true;

  // Report that node is not a leaf.
  return false;
}



/* ********************************************
// Return a pointer to the char[] with greater
// alphabetical prescedence. e.g. (a,b) => a,
// (ng, nc) => nc, etc..
// 
// If the contents of the char[]s are the same,
// returns the second.
// 
// If one of the pointers is NULL, returns the
// non NULL pointer. If both are NULL, returns
// NULL.
//
// ********************************************/
char * Table :: min(char * first, char * second) const
{
  // If the first or the second char*s are NULL, or
  // they point to equivalent character strings..
  if(!first || !second || !strcmp(first, second))
  {
    // If the first is NULL, return the
    // second (event if it, too, is NULL).
    if(!first) return second;
    // As a convention, return the first. This is done so
    // that when new events which have preexisting copies
    // in the tree, the new (copy) event is inserted into
    // the right subtree of its preexisting copy.
    return first;
  }

      // Get the length of the smaller string (so that
      // it isn't traversed beyond its '\0').
  int min_len = std::min(strlen(first), strlen(second)),
      // Character index counter.
      ch_index = 0;

  // Increment ch_index until unequivalent characters are 
  // found in the same indices of the first and second strings.
  while(ch_index < min_len && tolower(first[ch_index]) == tolower(second[ch_index]))
    ++ch_index;

  // If the first different character of the
  // first string precedes that of the second..
  if(tolower(first[ch_index]) < tolower(second[ch_index]))
    // Return the (alphabetically smaller) first.
    return first;

  // Return the (alphabetically smaller) second.
  return second;
}