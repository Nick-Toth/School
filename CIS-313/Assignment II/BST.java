/* ***********************************************
\\ File: BST.java
// Name: Nick G. Toth
\\ Email: ngt@uoregon.edu
// Date: Nov. 1, 2018
\\ Class: CIS 313
//
\\ File Overview: This file contains my BST
// implementation for the third lab of CIS313.
\\ See lab2.java for instructions.
//
\\ ***********************************************/


public class BST< E extends Comparable<E> >
{
  private Node<E> root;

  public BST(
  ){ root = null; }


  /* ************************************
  \\ Returns the root of the BST.
  //
  \\ ************************************/
  public Node<E> getRoot(
  ){ return root; }


  /* ************************************
  \\ Initializes the recursive insert
  // function.
  \\
  // ************************************/
  public void insert( E data
  ){
    // If the tree is empty, simply initialize
    // it with the given data.
    if(root == null) root = new Node<E> (data);
    // Initialize recursive insert function.
    else insert(data, root, root);
  }


  /* ************************************
  \\ Recursively searches for a place to
  // insert.
  \\
  // ************************************/
  private void insert( E data
                      , Node<E> current
                      , Node<E> parent
  ){
    // If the current node is null, insert at its parent; insert at parent.
    if(current == null)
    {
      insertParent(data, parent);
      return;
    }

    E current_data = current.getData();
    int compare = current_data.compareTo(data);

    // If the current node's data is less than the data being
    // inserted, insert at the current node's right subtree.
    if(compare < 0) insert(data, current.getRightChild(), current);
    else if(current_data == data) return; // No duplicate entries.
    // If the current node's data is greater than the data being
    // inserted, insert at the current node's left subtree.
    else insert(data, current.getLeftChild(), current);
  }


  /* ************************************
  \\ insertParent inserts a new node when
  // insert reaches a null node, where new
  \\ data must be inserted.
  //
  \\ ************************************/
  private void insertParent( E data
                           , Node<E> top
  ){
    if(top == null) return;

    // Create new node for insertion.
    Node <E> new_node = new Node<E>(data);
    // Set top as the new node's parent.
    new_node.setParent(top);

    // Get top's data for comparison.
    E top_data = top.getData();
    // Compare top to the data being inserted.
    int compare = top.getData().compareTo(data);

    // Determine whether we must insert into the left or right of top, then insert.
    if(compare < 0)
    {
      new_node.setRightChild(top.getRightChild());
      top.setRightChild(new_node);
    }
    else if(compare > 0)
    {
      new_node.setLeftChild(top.getLeftChild());
      top.setLeftChild(new_node);
    }
    else return;
  }


  /* ************************************
  \\ Initializes the private, recursive
  // find (search) method.
  \\
  // @return: null  if data isn't found.
  \\          Node  if data is found.
  //
  \\ ************************************/
  public Node<E> find( E data // Data to locate.
  ){ return find(data, root); }

  /* ************************************
  \\ Recursive find (search) method,
  // initialized by public find.
  \\
  // @return: null  if data isn't found.
  \\          Node  if data is found.
  //
  \\  ************************************/
  private Node<E> find( E data // Data to locate.
                      , Node<E> current
  ){

    // If the data isn't found, end search in the current subtree.
    if(current == null) return null;

    // Get the current node's data.
    E current_data = current.getData();

    // If the data exists in the current node, return the current node.
    if(current_data == data)
      return current;

    // Compare the data we're searching for, with the data in the current node.
    int compare = current_data.compareTo(data);

    // If the current node's data is less than the data we're
    // searching for, continue searching in the right subtree.
    if(compare < 0)
      return find(data, current.getRightChild());

    // If the current node's data is greater than the data we're
    // searching for, continue searching in the left subtree.
    return find(data, current.getLeftChild());
  }


  /* ************************************
  \\ Initializer for private recursive
  // delete function.
  \\
  // ************************************/
  public void delete( E data
  ){ root = delete(data, root); }

  /* ************************************
  \\ Recursive delete function.
  //
  \\ ************************************/
  private Node<E> delete( E data
                        , Node<E> top
  ){

    // If the node wasn't found in the current subtree, end search.
    if(top == null) return null;

    // Relate the given data to the data at top.
    int compare = data.compareTo(top.getData());

    // If the data we're searching for is less than the data
    // in top, continue searching in the left child of top.
    if(compare < 0) top.setLeftChild(delete(data, top.getLeftChild()));
    // greater -> Right subtree
    else if(compare > 0) top.setRightChild(delete(data, top.getRightChild()));
    // If the data in top is the data we must delete, and top has two children..
    else if(top.getRightChild() != null && top.getLeftChild() != null)
    {
      // Update the current node with its successor.
      top.setData(getMin(top.getRightChild()).getData());
      // Update the successor.
      top.setRightChild(delete(top.getData(), top.getRightChild()));
    }
    // If top has less than two children, reset top to
    // whatever child it has, or null if top is a leaf.
    else top = top.getLeftChild() == null? top.getRightChild() : top.getLeftChild();

    return top;
  }



  /* ************************************
  \\ Delegates to preorder, inorder, and
  // postorder traversal functions.
  \\
  // ************************************/
  public void traverse( String order // Type of traversal to perform.
                      , Node<E> top  // Subtree to be traversed.
  ){
    if (top != null)
    {
      switch (order)
      {
        case "preorder":
          preorder(top);
          System.out.println("");
          break;

        case "inorder":
          inorder(top);
          System.out.println("");
          break;

        case "postorder":
          postorder(top);
          System.out.println("");
          break;

        default:
          System.out.println("Error :: Invalid Instruction!");
          break;
      }
    }
  }

  /* ************************************
  \\ Performs an inorder traversal of
  // a given subtree.
  \\
  // ************************************/
  private void inorder( Node<E> current // Root of subtree.
  ){
    if(current == null) return;

    inorder(current.getLeftChild());
    System.out.print(current.getData() + " ");
    inorder(current.getRightChild());
  }

  /* ************************************
  \\ Performs a preorder traversal of
  // a given subtree.
  \\
  // ************************************/
  private void preorder( Node<E> current // Root of subtree.
  ){
    if(current == null) return;

    System.out.print(current.getData() + " ");
    preorder(current.getLeftChild());
    preorder(current.getRightChild());
  }

  /* ************************************
  \\ Performs a postorder traversal of
  // a given subtree.
  \\
  // ************************************/
  private void postorder( Node<E> current // Root of subtree.
  ){
    if(current == null) return;

    postorder(current.getLeftChild());
    postorder(current.getRightChild());
    System.out.print(current.getData() + " ");
  }

  /* ************************************
  \\ Return the smallest (leftmost)
  // element of a given subtree.
  \\
  // @return: null  if tree is empty.
  \\          node  if tree is not empty.
  //
  \\ ************************************/
  public Node<E> getMin( Node<E> top // Root of subtree.
  ){
    if(top == null) return null;
    if(top.getLeftChild() != null)
      return getMin(top.getLeftChild());
    return top;
  }

}

