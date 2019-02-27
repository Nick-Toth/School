/* ****************************************************
\\ File: RBT.java
// Name: Nick G. Toth
\\ Email: ngt@uoregon.edu
// Date: Nov. 30, 2018
\\ Class: CIS 313
//
\\ Overview: This file contains my red-black tree
// class definition for the fifth lab (lab 4) of
\\ CIS-313 (Intermediate Data Structures).
//
\\ See Note.java for the node class being used in this
// tree implementation, and see lab4.java for testing
\\ instructions.
//
\\ ****************************************************/

public class RBT<E extends Comparable<E>>
{

  // Initialize a new red black tree.
  public RBT(
  ){ root = null; }

  // Initialize a new red black tree give the root of some initial tree.
  public RBT( Node<E> new_root
  ){ root = new_root; }


  /* ************************************
  \\ Inserts a new node into the rb tree.
  //
  \\ ************************************/
  public boolean insert( E data
  ){

    Node<E> parent = null // The left-left grand child of the new node. Just kidding.
          , temp = root;  // Traversal node.

    // Search for a position to insert the new data.
    while (temp != null && !temp.isEmpty())
    {
      // Keep track fo the node which will serve as the new node's parent.
      parent = temp;
      // Compare the current traversal node's data to the data being inserted.
      int compare = data.compareTo(parent.getData());

      // If the data already exists in the tree, BTFO.
      if(compare == 0)
        return false;

      // Update the traversal node so that we may continue
      // searching for the correct position for insertion.
      temp = (compare < 0)
        ? parent.getLeftChild()
        : parent.getRightChild();
    }

    // If the new node is the root
    // of the tree, update root.
    if(parent == null)
    {
      temp = new Node<E>(data, null);
      root = temp;
    }
    // When the new node is not the root of
    // the tree, initialize it as usual.
    else
    {
      temp.setParent(parent);
      temp.setData(data);
      temp.setEmpty(false);
      temp.setColor('r');
    }

    // Once the node has been successfully inserted, initialize
    // its color, and correct any violated invariants.
    temp.setColor('r');
    insertFixup(temp);
    return true;
  }


  /* ************************************
  \\ At the end of insert, insertFixup is
  // called to resolve any violations of
  \\ the rb tree invariants.
  //
  \\ ************************************/
  private void insertFixup( Node<E> subtree // The root of the subtree being corrected.
  ){
    // Traverse until the
    while ( subtree.getParent() != null &&
            subtree.getGrandparent() != null &&
            subtree.getParent().getColor() == 'r')
    {

      // Case A - subtree's parent is the left child of subtree's grandparent.
      if(subtree.getParent() == subtree.getGrandparent().getLeftChild())
      {
        Node<E> uncle = subtree.getGrandparent().getRightChild();

        // Case A1 - Recolor the tree.
        if(uncle.getColor() == 'r')
        {
          subtree.getParent().setColor('b');
          uncle.setColor('b');
          subtree = subtree.getGrandparent();
          subtree.setColor('r');
        }
        else
        {
          // Case A2 - subtree is the right child of its parent => Left-rotation.
          if(subtree == subtree.getParent().getRightChild())
          {
            subtree = subtree.getParent();
            leftRotate(subtree);
          }

          // Case A2 (cont) and A3 - subtree is the right child of its parent => Right-rotation.
          subtree.getParent().setColor('b');
          subtree.getGrandparent().setColor('r');
          rightRotate(subtree.getGrandparent());
        }
      }

      // Case B - subtree's parent is the right child of subtree's grandparent.
      else if(subtree.getParent() == subtree.getGrandparent().getRightChild())
      {
        Node<E> uncle = subtree.getGrandparent().getLeftChild();

        // Case B1 - Recolor the tree.
        if(uncle.getColor() == 'r')
        {
          subtree.getParent().setColor('b');
          uncle.setColor('b');
          subtree = subtree.getGrandparent();
          subtree.setColor('r');
        }
        else
        {
          // Case B2 - subtree is the left child of its parent => Right-rotation.
          if(subtree == subtree.getParent().getLeftChild())
          {
            subtree = subtree.getParent();
            rightRotate(subtree);
          }

          // Case B2 (cont) and B3 - subtree is the right child of its parent => Left-rotation.
          subtree.getParent().setColor('b');
          subtree.getGrandparent().setColor('r');
          leftRotate(subtree.getGrandparent());
        }
      }
    }

    // Ensure that the 'root is black' property is preserved.
    root.setColor('b');
  }

  /* **************************************
  \\ Structure-correcting algorithm;
  // Performs the following transformation:
  \\
  //         |                |
  \\        (y)              (x)
  //       /   \            /   \
  \\     (x)    c    <-    a   (y)
  //    /   \                 /   \
  \\   a     b               b     c
  //
  \\ **************************************/
  private void leftRotate( Node<E> x
  ){
    // See model in function comment!
    Node<E> y = x.getRightChild(); // (y) on the right side of the model.
    x.setRightChild(y.getLeftChild()); // set y to b.

    // If y is not a null node, then x becomes its parent.
    if(y.getLeftChild() != null)
      y.getLeftChild().setParent(x);

    // Reset y's parent to x's now-previous parent.
    y.setParent(x.getParent());

    // If x happened to be the root, then the root is now y.
    if(x.getParent() == null)
        root = y;
    // Typically, though, x will not be the root. In that case..
    else
    {
      // Set up relations between the new subtree-root, y,
      // and its new parents (x's old parents).
      if(x == x.getParent().getLeftChild())
        x.getParent().setLeftChild(y);
      else x.getParent().setRightChild(y);
    }

    // Finish by setting the new relation of y and x,
    // namely, x is now the left child of y.
    y.setLeftChild(x);
    x.setParent(y);
  }


  /* **************************************
  \\ Structure-correcting algorithm;
  // Performs the following transformation:
  \\
  //         |                |
  \\        (y)              (x)
  //       /   \            /   \
  \\     (x)    c    ->    a    (y)
  //    /   \                  /   \
  \\   a     b                 b    c
  //
  \\ **************************************/
  private void rightRotate( Node<E> x
  ){

    // See model in function comment!
    Node<E> y = x.getLeftChild(); // (y) on the left side of the model.
    x.setLeftChild(y.getRightChild()); // set a to c.

    // If y is not a null node, then x becomes its parent.
    if(y.getRightChild() != null)
      y.getRightChild().setParent(x);

    // Reset y's parent to x's now-previous parent.
    y.setParent(x.getParent());

    // If x happened to be the root, then the root is now y.
    if(x.getParent() == null)
      root = y;

    // Typically, though, x will not be the root. In that case..
    else
    {
      // Set up relations between the new subtree-root, y,
      // and its new parents (x's old parents).
      if(x == x.getParent().getLeftChild())
        x.getParent().setLeftChild(y);
      else x.getParent().setRightChild(y);
    }

    // Finish by setting the new relation of y and x,
    // namely, x is now the right child of y.
    y.setRightChild(x);
    x.setParent(y);
  }


  /* ************************************
  \\ Searches for and removes the given
  // data, if it exists.
  \\
  // ************************************/
  public void delete( E data
  ){

    Node<E> temp = search(data) // Node containing the data to be deleted.
            // Store the inorder successor to temp,
            // if it exists. otherwise, store a copy of temp.
          , y = (temp.getLeftChild().isEmpty() || temp.getRightChild().isEmpty())
              ? temp
              : ios(temp)
            // If y has a left child, store it in x.
            // Otherwise, store y's left child in x.
          , x = (y.getLeftChild() != null && !y.getLeftChild().isEmpty())
              ? y.getLeftChild()
              : y.getRightChild();

    // Replace x's parent with its grandparent (y's parent).
    x.setParent(y.getParent());

    // Handle case where y is the root; update root.
    if(y.getParent() == null)
      root = x;
    // Ordinarily, though, y will not be the root. In this case..
    else
    {
      // Set up relations between the new subtree-root,
      // y, and its new parents (x's old parents).
      if(y == y.getParent().getLeftChild())
        y.getParent().setLeftChild(x);
      else y.getParent().setRightChild(x);
    }
    // Update deleted node with successor data.
    if(y != temp)
      temp.setData(y.getData());

    // Correct any invariant violations.
    if(y.getColor() == 'b')
      deleteFixup(x);
  }


  /* ************************************
  \\ deleteFixup is called at the end of
  // delete to correct any violations of
  \\ the rbtree invariants.
  //
  \\ ************************************/
  private void deleteFixup( Node<E> temp
  ){

    // Traverse the tree, and correct invariant-violations.
    while (temp != root && temp.getColor() == 'b')
    {
      // Case A; Note: In the event that this is false,
      // execute the else statement after the block
      // comment half way down. Read the block comment
      // for information regarding comments.
      if(temp == temp.getParent().getLeftChild())
      {
        // Get temp's sibling.
        Node<E> w = temp.getParent().getRightChild();

        // Case A.A; x’s sibling w is red.
        if(w.getColor() == 'r')
        {
          w.setColor('b');
          temp.getParent().setColor('r');
          leftRotate(temp.getParent());
        }
        // Case A.B; x’s sibling w is black,
        // and both of w’s children are black.
        if(w.getLeftChild().getColor() == 'b' &&
            w.getRightChild().getColor() == 'b')
        {
          w.setColor('r');
          temp = temp.getParent();
        }
        else
        {
          // Case A.C; x’s sibling w is black, w’s left
          // child is red, and w’s right child is black.
          if(w.getRightChild().getColor() == 'b')
          {
            w.getLeftChild().setColor('b');
            w.setColor('r');
            rightRotate(w);
            w = temp.getParent().getRightChild();
          }

          // Case A.D; x’s sibling w is black,
          // and w’s right child is red.
          w.setColor(temp.getParent().getColor());
          temp.getParent().setColor('b');
          w.getRightChild().setColor('b');
          leftRotate(temp.getParent());
          temp = root;
        }
      }

      /* *********************************
      \\ The code within the else statement
      // is the same as the code within the
      \\ if statement, with right and left
      // permuted. Thus, I will not comment
      \\ it. i.e. See comments from if
      // statement and swap left/right.
      \\ *********************************/
      else // Case B
      {
        Node<E> w = temp.getParent().getLeftChild();

        if(w.getColor() == 'r')
        {
          w.setColor('b');
          temp.getParent().setColor('r');
          rightRotate(temp.getParent());
        }

        if(  w.getRightChild().getColor() == 'b'
          && w.getLeftChild().getColor() == 'b')
        {
          w.setColor('r');
          temp = temp.getParent();
        }
        else
        {
          if(w.getLeftChild().getColor() == 'b')
          {
            w.getRightChild().setColor('b');
            w.setColor('r');
            leftRotate(w);
            w = temp.getParent().getLeftChild();
          }
          w.setColor(temp.getParent().getColor());
          temp.getParent().setColor('b');
          w.getLeftChild().setColor('b');
          rightRotate(temp.getParent());
          temp = root;
        }
      }
    }

    // Ensure that the 'root is black' property is preserved.
    temp.setColor('b');
  }


  /* ************************************
  \\ Returns the in-order-successor of
  // a given node.
  \\
  // ************************************/
  private Node<E> ios( Node<E> subtree
  ){
    // Find the main-subtree-root's successor as the
    // minimum node in its right subtree.
    if(subtree.getRightChild() != null && !subtree.isEmpty())
      return getMin(subtree.getRightChild());

    // Otherwise, resolve by searching for ancestor-successor.
    Node<E> successor = subtree.getParent();
    while ( successor != null
         && !successor.isEmpty()
         && subtree == successor)
    {
      subtree = successor;
      successor = subtree.getParent();
    }

    return successor;
  }


  /* ************************************
  \\ Returns the left-most element of the
  // given subtree. Used in locating ios.
  \\
  // ************************************/
  private Node<E> getMin( Node<E> subtree
  ){
    while (!subtree.getLeftChild().isEmpty() && !subtree.isEmpty())
      subtree = subtree.getLeftChild();
    return subtree;
  }


  /* ************************************
  \\ Initializes the recursive search
  // method
  \\ ************************************/
  public Node<E> search( E data
  ){ return (root == null)
        ? null
        : search(data, root); }


  /* ************************************
  \\ Recursively traverses the tree,
  // stopping and returning if the
  \\ desired data is found, or when the
  // whole tree has been traversed.
  \\
  // ************************************/
  private Node<E> search( E data
                        , Node<E> subtree
  ){
    // Edge condition I.
    // If the current path has been traversed,
    // and the data has not been found, exit.
    if(subtree.isEmpty()) return null;

    // Edge condition II. Data found in current node.
    if(data == subtree.getData())
        return subtree;

    // If the current node's data is greater
    // than the desired data, search to the left.
    if(data.compareTo( subtree.getData() ) < 0)
      return (subtree.getLeftChild().isEmpty())
        ? null
        : search(data, subtree.getLeftChild());

    // If the current node's data is less than
    // the desired data, search to the right.
    if(data.compareTo( subtree.getData() ) >= 0)
      return(subtree.getRightChild().isEmpty())
        ? null
        : search(data, subtree.getRightChild());

    // This should be dead code, unless
    // something has gone horribly wrong!!
    return null;
  }


  /* ************************************
  \\ Delegates to the pre, in and post
  // order traversal methods.
  \\
  // ************************************/
  public void traverse( String order
                      , Node<E> subtree
  ){
    switch(order)
    {
      case "preorder":
        preorder(subtree);
        break;
      case "inorder":
        inorder(subtree);
        break;
      case "postorder":
        postorder(subtree);
        break;
      default:
        return; // Leave as return, not break, for printing a new line at the end!!
    }

    // Traversal methods print w/o new line, so
    // this ends the traversal with a new line.
    System.out.println();
  }


  /* ************************************
  \\ Performs a preorder traversal of the
  // tree, printing elements along the
  \\ way.
  //
  \\ ************************************/
  private void preorder( Node<E> temp
  ){
    if(temp.isEmpty()) return;
    System.out.print(temp.getData() + " ");
    preorder(temp.getLeftChild());
    preorder(temp.getRightChild());
  }


  /* ************************************
  \\ Performs an inorder traversal of the
  // tree, printing elements along the
  \\ way.
  //
  \\ ************************************/
  private void inorder( Node<E> temp
  ){
    if(temp.isEmpty()) return;
    inorder(temp.getLeftChild());
    System.out.print(temp.getData() + " ");
    inorder(temp.getRightChild());
  }


  /* ************************************
  \\ Performs a post order traversal of
  // the tree, printing elements along
  \\ the way.
  //
  \\ ************************************/
  private void postorder( Node<E> temp
  ){
    if(temp.isEmpty()) return;
    postorder(temp.getLeftChild());
    postorder(temp.getRightChild());
    System.out.print(temp.getData() + " ");
  }


  /* ************************************
  \\ Returns true if the given node is
  // the root of a valid rb tree.
  \\
  // ************************************/
  public boolean isRBT(
  ){
    return ( root == null
          || root.isEmpty()
          || root.getColor() != 'b'
           ) ? false
             : isRBT(root); }


  /* *************************************
  \\ Returns true if the given subtree
  // satisfies the 'has a color' property
  \\ and the 'black height' property.
  //
  \\*************************************/
  private boolean isRBT( Node<E> subtree
  ){
    // If the subtree is null/empty, it is also balanced!
    if(subtree == null || subtree.isEmpty()) return true;

    // Get the subtree-root's color.
    char subtree_clr = subtree.getColor();

    // It must be red or black.
    if(subtree_clr != 'r' && subtree_clr != 'b') return false;

    // Check black-height property!
    return blkHeight(subtree.getLeftChild()) == blkHeight(subtree.getRightChild());
  }


  /* ************************************
  \\ Returns the black height starting
  // at some subtree's root.
  \\
  // ************************************/
  private int blkHeight( Node<E> subtree
  ){
    // If the current subtree is empty, then it is also balanced; add zero to the sum.
    if (subtree == null || subtree.isEmpty()) return 0;

    // Get the black height of the current subtree-root's children.
    int left = blkHeight(subtree.getLeftChild()),
        right = blkHeight(subtree.getRightChild()),
        add = subtree.getColor() == 'b' ? 1 : 0;

    return (right == -1 || left == -1 || right != left)? -1 : left + add;
  }


  // ************* |
  // Class Fields  |
  // ************* V

  private Node<E> root;

  // Returns the root of the red black tree.
  public Node<E> getRoot(
  ){ return root; }

}
