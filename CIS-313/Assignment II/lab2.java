/* ***********************************************
\\ File: Assignment II.java
// Name: Nick G. Toth
\\ Email: ngt@uoregon.edu
// Date: Nov. 1, 2018
\\ Class: CIS 313
//
\\ File Overview: This file is for testing my BST
// implementation for the third lab of CIS313.
\\
// java lab2 < inSample.txt
\\
// ***********************************************/

import java.util.Scanner;

public class lab2
{
  public static void main(String[] args)
  {
    Scanner scan = new Scanner(System.in);

    String input, task;

    BST tree = new BST<Integer>();

    while (scan.hasNext())
    {
      input = scan.nextLine();
      String[] phrases = input.split(" ");
      task = phrases[0];
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
        default:
          tree.traverse(task, tree.getRoot());
          break;
      }
    }
    scan.close();
  }
}
