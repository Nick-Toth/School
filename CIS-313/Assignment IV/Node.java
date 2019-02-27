/* ****************************************************
\\ File: Node.java
// Name: Nick G. Toth
\\ Email: ngt@uoregon.edu
// Date: Nov. 30, 2018
\\ Class: CIS 313
//
\\ Overview: This file contains the Node class definition
// to be used in the red-black tree in the fifth lab
\\ (lab 4) of CIS-313 (Intermediate Data Structures).
//
\\ See RBT.java for the red-black tree implementation,
// and see lab4.java for testing instructions.
\\
// ****************************************************/

public class Node<E extends Comparable<E>> implements Comparable<Node<E>>
{

  /* *******************
  \\ Node constructors.
  \\ *******************/
  public Node(E d)
  {
    this.data = d;
    this.left = null;
    this.right = null;
    this.parent = null;
  }

  public Node( E data
             , Node<E> parent
  ){
    this.data = data;
    this.parent = parent;
    this.color = 'r';
    this.setEmpty(false);
  }

  private Node( Node<E> parent
  ){
    this.parent = parent;
    this.color = 'b';
    this.setEmpty(true);
  }


  /* ************************************
  \\ Node data getter and setter methods.
  //
  \\ ************************************/
  public E getData(
  ){ return data; }
  public void setData( E data
  ){ this.data = data; }


  /* ************************************
  \\ Getter and setter methods for node
  // descendents.
  \\
  // ************************************/
  public Node<E> getLeftChild(
  ){
    if (left == null)
      left = new Node(this);
    return left;
  }
  public void setLeftChild( Node<E> left
  ){ this.left = left; }

  public Node<E> getRightChild(
  ){
    if (right == null)
      right = new Node(this);
    return right;
  }
  public void setRightChild( Node<E> right
  ){ this.right = right; }



  /* ************************************
  \\ Returns the parent of the node for
  // which getParent is called.
  \\
  // ************************************/
  public Node<E> getParent(
  ){ return parent; }
  public void setParent( Node<E> parent
  ){ this.parent = parent; }


  /* ************************************
  \\ Returns the parent of the parent of
  \\ the node for which getGrandparent is
  // called.
  \\
  // ************************************/
  public Node<E> getGrandparent(
  ){ return (parent != null && parent.getParent() != null)
              ? parent.getParent()
              : null; }


  /* ************************************
  \\ Returns a character representing the
  // color of the node for which getColor
  \\ is called.
  //
  \\ 'r' => red
  // 'b' => black
  \\
  // ************************************/
  public char getColor(
  ){ return color; }


  /* ************************************
  \\ Sets the color field to the given
  // color.
  \\
  // 'r' => red
  \\ 'b' => black
  //
  \\ ************************************/
  public void setColor( char color
  ){ this.color = color; }


  /* ************************************
  \\ True if the node has been marked as
  \\ empty. i.e. rather than using
  // node == null, use isEmpty(node).
  \\
  // ************************************/
  public boolean isEmpty(
  ){ return isEmpty; }


  /* ************************************
  \\ Sets the isEmpty field to the given
  // boolean.
  \\
  // ************************************/
  public final void setEmpty( boolean isEmpty
  ){ this.isEmpty = isEmpty; }


  /* ************************************
  \\ Define the compareTo function for,
  // you guessed it, comparing elements
  \\ of the rb tree
  //
  \\ ************************************/
  @Override
  public int compareTo( Node<E> cmp_node
  ){ return data.compareTo(cmp_node.getData()); }



  // ************* |
  // Class Fields  |
  // ************* V

  private Node<E> parent
  //                 |
  //                THIS
  //               /    \
              , left , right ;

  private E data;
  private boolean isEmpty;
  private char color;

}
