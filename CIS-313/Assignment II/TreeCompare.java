/* ***********************************************
\\ File: TreeCompare.java
// Name: Nick G. Toth
\\ Email: ngt@uoregon.edu
// Date: Nov. 1, 2018
\\ Class: CIS 313
//
\\ File Overview: This file contains an extra
// credit main file. The difference between
\\ TreeCompare and lab2 is that TreeCompare
// builds two binary trees, and then tests
\\ whether or not they are equivalent.
// (Extra Credit)
\\
// java TreeCompare < inSample.txt
\\
// ***********************************************/

import java.util.Scanner;

public class TreeCompare
{
  public static void main(String[] args)
  {
    Scanner scan = new Scanner(System.in);
    String input;
    String task;

    BST tree1 = new BST<Integer>(),
        tree2 = new BST<Integer>(),
        tree = null; // For switching trees.

    // tree_switch needs to be false initially, but the first line of the
    // data file will invert tree_switch, so we set tree_switch to false.
    boolean tree_switch = true;

    while (scan.hasNext())
    {
      input = scan.nextLine();
      String[] phrases = input.split(" ");
      task = phrases[0];

      tree = tree_switch? tree1 : tree2;

      switch(task)
      {
        case "insert":
          tree.insert(Integer.parseInt(phrases[1]));
          break;
        case "delete":
          tree.delete(Integer.parseInt(phrases[1]));
          break;
        case "min":
          tree.getMin(tree.getRoot());
          break;
        case "find":
          if(tree.find(Integer.parseInt(phrases[1])) != null)
            System.out.println("Found: " + phrases[1]);
          else
            System.out.println("Not Found: " + phrases[1]);
          break;
        case "white": // Print a line of whitesspace.
          int wsp = Integer.parseInt(phrases[1]);
          while(0 < wsp--)
            System.out.println("");
          break;
        case "label": // Print a label
          System.out.println(phrases[1]);
          break;
        case "preorder":
        case "inorder":
        case "postorder":
          tree.traverse(task, tree.getRoot());
          break;
        default:
          tree_switch = isInteger(task)? !tree_switch : tree_switch;
          break;
      }
    }

    // Tree compare.
    if(compareTrees(tree1.getRoot(), tree2.getRoot()))
      System.out.println("The trees have the same shape.");
    else
      System.out.println("The trees do not have the same shape.");

    scan.close();
  }


  // Test whether or not two trees are equivalent.
  public static boolean compareTrees( Node<Integer> root1
                                    , Node<Integer> root2
  ){
    // Shortcut for reference equality; also handles equals(null, null)
    if (root1 == root2)
        return true;
    if (root1 == null || root2 == null)
        return false;
    return root1.getData() == root2.getData() &&
           compareTrees(root1.getLeftChild(), root2.getLeftChild()) &&
           compareTrees(root1.getRightChild(), root2.getRightChild());
  }


  public static boolean isInteger( String input
  ){
    try { Integer.parseInt( input ); return true; }
    catch( Exception e ) { return false; }
  }
}



