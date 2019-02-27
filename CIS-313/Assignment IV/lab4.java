/* ****************************************************
\\ File: lab4.java
// Name: Nick G. Toth
\\ Email: ngt@uoregon.edu
// Date: Nov. 30, 2018
\\ Class: CIS 313
//
\\ Program Overview:
//
\\ Instructions: Define some data file (e.g., data.txt)
// such that each line contains a pair of integers
\\ separated by a space.
//
\\   Compile: javac lab4.java
//   Execute: java lab4 < INSERT_DATA_FILENAME.txt
\\
// ****************************************************/

import java.util.Scanner;

public class lab4
{
  public static void main( String[] args
  ){

    Scanner sc = new Scanner(System.in);

    String input,
           task;

    int num;

    RBT<Integer> myRBT = new RBT<Integer>();
    WrongTree<Integer> wrongRBT;

    while (sc.hasNextLine())
    {
      input = sc.nextLine();
      String[] phrases = input.split(" ");
      task = phrases[0];

      switch(task)
      {
        case "insert" :

          num = Integer.parseInt(phrases[1]);
          myRBT.insert(num);
          break;

        case "delete" :
          num = Integer.parseInt(phrases[1]);
          myRBT.delete(num);
          break;

        case "search" :
          num = Integer.parseInt(phrases[1]);
          Node<Integer> found = myRBT.search(num);
          System.out.println( (found == null)
                              ?  "Not Found"
                              :  "Found" );
          break;

        case "traverse" :
          myRBT.traverse("preorder", myRBT.getRoot());
          break;

        case "exit" :
          System.out.println("Successful Exit");
          return;

        case "test" :
          wrongRBT = new WrongTree<Integer>(0);
          RBT<Integer> wrongRBT_testRBT = new RBT<Integer>(wrongRBT.getRoot());
          boolean isrbt = wrongRBT_testRBT.isRBT();
          System.out.println(wrongRBT.getTime());
          System.out.println(isrbt);
          break;

        default:
          break;
      }
    }
    sc.close();
  }
}
