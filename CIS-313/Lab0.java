/* ****************************************************
\\ File: lab0.java
// Name: Nick G. Toth
\\ Email: ngt@uoregon.edu
// Date: Oct. 4, 2018
\\ Class: CIS 313
//
\\ Program Overview: lab0.java constitute my submission
// for the first lab of CS 313 (Intermediate Data
\\ Structures). The purpose of this program is to read
// in a set of pairs of integers, and display their
\\ greatest common divisors and least common multiples.
//
\\ Instructions: Define some data file (e.g., data.txt)
// such that each line contains a pair of integers
\\ separated by a space.
//
\\   Compile: javac lab0.java
//   Execute: java lab0 < INSERT_DATA_FILENAME.txt
\\
// ****************************************************/


// For reading lab data.
import java.util.Scanner;
// In case there are fewer inputs in the data file than specified.
import java.util.NoSuchElementException;
import java.lang.NullPointerException;


public class lab0
{
  public static void main( String[] args
  ){
    // Read in the lab results
    labResults results = scanLabData();

    // If the program executed successfully, display the results.
    if(!results.isEmpty()) { results.printLabResults(); }
    else { System.out.println("Failed to print...\nInput Quantity Label: 0..\n"); }
  }


  /* *************************************
  \\ scanLabData() reads in a set of pairs
  // of integers from an input data file,
  \\ and constructs a labResults object
  // with, you guessed it, lab results.
  \\ *************************************/
  private static labResults scanLabData()
  {
    // Create a new scanner to read in lab data.
    Scanner scanner = new Scanner(System.in);

    // Determine the number of inputs for memory allocation.
    final int line_cnt = scanner.nextInt();

    // Initialize variables:
    int i = 0  // Index
      , m = 0 , n = 0; // i.e., mod m n.

    // Storage for pending lab data scan and analysis.
    labResults res = new labResults(line_cnt);

    // Read in line_cnt pairs of integers.
    for(; i < line_cnt; ++i)
      try { res.newResult(scanner.nextInt(), scanner.nextInt()); }
      catch (NoSuchElementException e) { System.out.println("\nError in scanLabData." + e); }

    scanner.close();
    return res;
  }


	public static class labResults
	{
  	public labResults( int num_results
  	){
    	total_results = num_results;
    	current_results = 0;
    	results = new labResult[num_results];
  	}


  	/* ***************************************
  	\\ newResult pushes a new lab result onto
  	// the stack, assuming the stack is not
  	\\ full.
  	// ***************************************/
  	public boolean newResult( int m
  	                        , int n
  	){
    	if (current_results + 1 > total_results) return false;
    	results[current_results++] = new labResult(m, n);
    	return true;
  	}


  	/* ***************************************
  	\\ printLabResults attempts to display the
  	// results of lab0. See printLabResult()
  	\\ for printing-format of individual.
  	// datum.
  	\\ ***************************************/
  	public void printLabResults(
  	){
    	// Display a header to denote the start of the lab results.
    	//System.out.println("\nPrinting " + Integer.toString(total_results) + " Lab Results:");

    	// Display each of the lab results in the order they were scanned.
    	for(int i = 0; i < total_results; ++i)
      	// If there are fewer pairs of numbers in the data file
      	// than specified, results may contain a null entry.
      	try { results[i].printLabResult(); }
      	// Changed from code below to code above for parsing purposes.
      	//try { results[i].printLabResult("\n  Result #" + Integer.toString(i+1) + ":\n"); }
      	catch (NullPointerException e) { System.out.println("Error in printLabResults()." + e); }

    	// Display footer message.
    	//System.out.print("\nEnd of Lab Results!\n\n");

  	}


  	// Returns false when there is at least one printable result.
  	public boolean isEmpty() { return current_results == 0; }


  	/* ****************************************
  	\\ Intuitively, labResult is to labResults 
  	// as a is to {a}. In other words, this
  	\\ class stores a single lab result; the 
  	// gcd and lcm for some pair m n of integers.
  	\\ ****************************************/
  	public class labResult
  	{
    	public labResult( int m_
    	                , int n_
    	){
      	// Initialize input data fields.
      	this.m = m_; this.n = n_;
      	// Store the absolute values of the inputs for computation.
      	int M = abs(m_), N = abs(n_);

      	// If the input is valid (no division by zero), compute the gcd and lcm.
      	if(m_ != 0 && n_ != 0)
        	// Warning! gcd must precede lcm; rather than using the gcd
        	// method, lcm uses this.g to reduce overall computation.
        	this.g = gcd(M,N);
      	else this.g = m_;
      	this.l = this.g==0? 0 : lcm(M,N);
    	}


    	/* *************************************
    	\\ Default method for printing a SINGLE
    	// lab result; i.e. prints the input and
    	\\ either the gcd and lcm, or an error
    	// message when m=0 or n=0.
    	\\ *************************************/
    	public void printLabResult(
    	){
      	// Create numeric-strings to display lab data.
      	String m_str = Integer.toString(this.m),
               n_str = Integer.toString(this.n),
               g_str = Integer.toString(this.g), // Happy accident.
               l_str = Integer.toString(this.l);

      	System.out.println(g_str + " " + l_str);
      	//System.out.println( "\tgcd(" + m_str + "," + n_str + ") = " + g_str +
                            	//"\n\tlcm(" + m_str + "," + n_str + ") = " + l_str );

    	}


    	/* *************************************
    	\\ Wrapper for default printLabResult
    	// method which prints a header label.
    	\\ *************************************/
    	public void printLabResult( String label
    	){
      	// Print an output label.
      	System.out.print(label);
      	// Call the default printLabResult method.
      	printLabResult();
    	}


    	/* *********************
    	\\ Mathematical methods:
    	// *********************/

    	// Recursive gcd computer.
    	private int gcd( int m
                     , int n
    	){
      	int mod_eval = mod(m, n); // COUT (Calculate Once, Use Twice)
      	// Base Condition: m mod n == 0; return n.
      	// Recursive call: Euclidean Algorithm.
      	return (mod_eval == 0)? n : gcd(n, mod_eval);
    	}

    	// LCM computer. Note that lcm uses this.g in computation;
    	// to calculate lcm(m,n), g must already be equal to gcd(m,n).
    	private int lcm( int m
                     , int n
    	){ return (m * n) / g; }

    	// Donald Knuth's definition of mod. Actual formula is m-n*(floor(m/n)),
    	// but integer division takes care of the floor function already.
    	private int mod( int m
                     , int n
    	){ return m - n * (m / n); }

    	// Absolute value method for preprocessing.
    	private int abs( int n
    	){ return n<0? -n : n; }


    	/* ************
    	\\ Lab IO Data:
    	// ************/

    	private final int m, n, // Input data.
                        g, l; // Output data.

    	// True when m and n are both non-zero.
    	//private final boolean valid_input;

  	}


  	/* **************
  	\\ Lab Meta-Data:
  	// **************/

  	public final int total_results;
  	private int current_results;
  	private labResult[] results;
	}
}
