/* ****************************************************
\\ File: pQueue.java
// Name: Nick G. Toth
\\ Email: ngt@uoregon.edu
// Date: Nov. 16, 2018
\\ Class: CIS 313
//
\\ Overview: This file contains a priority queue
// implementation based on MaxHeap. For use in the
\\ fourth assignment in CIS-313.
//
\\ ****************************************************/


public class pQueue<E extends Comparable>
{
    // Priority queue storage.
    private MaxHeap myHeap;

    public pQueue( int s
    ){ myHeap = new MaxHeap(s); }


    /* ************************************
    \\ Display the contents of the array
    // stored in myHeap.
    \\
    // ************************************/
    public void print(
    ){
        // Display header label.
        System.out.print("Current Queue: ");

        int i = 1 // index counter
          , heap_size = myHeap.getLength(); // number of elements in myHeap.

        // Get a copy of the array stored in myHeap.
        E[] heap = (E[]) myHeap.getArray();

        // Print out heap-array as csv.
        for( ; i < heap_size+1; ++i )
        {
            System.out.print(heap[i]);
            if(i+1<heap_size+1)
            System.out.print(",");
        }
        System.out.print("\n");
    }


    // *********************** |
    // Heap Wrapper Methods:   |
    // *********************** V

    public void insert( E data
    ){ myHeap.insert(data); }

    public Comparable<E> maximum(
    ){ return myHeap.maximum(); }

    public Comparable<E> extractMax(
    ){ return myHeap.extractMax(); }

    public boolean isEmpty(
    ){ return myHeap.getLength() == 0; }

}
