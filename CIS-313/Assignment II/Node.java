/* ***********************************************
\\ File: Node.java
// Name: Nick G. Toth
\\ Email: ngt@uoregon.edu
// Date: Nov. 1, 2018
\\ Class: CIS 313
//
\\ File Overview: This file contains the BST Node
// for the second lab of CIS313.
\\
// ***********************************************/


public class Node<E extends Comparable<E>>
{
  public Node( E d
  ){
    data = d;
    leftChild = null;
    rightChild = null;
    parent = null;
  }

  public void setData( E d
  ){ data = d; }

  public void setLeftChild( Node<E> lc
  ){ leftChild = lc; }

  public void setRightChild( Node<E> rc
  ){ rightChild = rc; }

  public void setParent( Node<E> p
  ){ parent = p; }

  public E getData(
  ){ return data; }

  public Node<E> getLeftChild(
  ){ return leftChild; }

  public Node<E> getRightChild(
  ){ return rightChild; }

  public Node<E> getParent(
  ){ return parent; }

  private E data;

  private Node<E> leftChild, rightChild
                 , parent;
}