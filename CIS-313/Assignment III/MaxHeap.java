/* ****************************************************
\\ File: MaxHeap.java
// Name: Nick G. Toth
\\ Email: ngt@uoregon.edu
// Date: Nov. 16, 2018
\\ Class: CIS 313
//
\\ Overview: This file contains a max heap implementation
// for use in the fourth lab in CIS-313.
\\
// ****************************************************/

import java.util.Comparator;
import java.lang.Math;


public class MaxHeap<E extends Comparable>
{
    // Heap contents.
    private E[] myArray;

    private int maxSize, // Capacity of myArray.
                length; // Current size of myArray.

    // Initialize a new s-element max heap.
    public MaxHeap( int s
    ){
        // Initialize private fields.
        myArray = (E[]) new Comparable[s];
        maxSize = s;
        length = 0;
    }


    /* **************************************
    \\ Returns a copy of the array containing
    // the heap.
    \\
    // **************************************/
    public E[] getArray(
    ){ return myArray; }


    /* **************************************
    \\ Sets newArray as the array stored by
    // MaxHeap.
    \\
    // **************************************/
    public void setArray( E[] newArray
    ){
        // Index counter.
        int i = 1,
        // Store the current length of my Array
        old_len = length;

        // Determine the number of non-null elements in newArray.
        while(null != newArray[i++]);

        // Update the heap-dimensions
        length = i-1;
        maxSize = newArray.length;

        // If the length of newArray exceeds the maximum
        // size of myArray, increase the size of myArray.
        if(length > maxSize)
        myArray = (E[]) new Comparable[length];
        // Nullify any existing contents of myArray.
        else
            for(i = 0; i < old_len; ++i)
                myArray[i] = null;

        // Move newArray into myArray.
        i = 0;
        while(i < length-1)
        myArray[i] = newArray[i++];
    }


    /* ************************************
    \\ Methods for managing heap size.
    //
    \\ ************************************/
    public int getaxSize(
    ){ return maxSize; }

    public void setMaxSize( int ms
    ){ maxSize = ms; }

    public int getLength(
    ){ return length; }

    public void setLength( int l
    ){ length = l; }


    /* ************************************
    \\ Insert a new element into the heap.
    //
    \\ ************************************/
    public void insert( E data
    ){
        // double size of array if necessary
        if (length >= maxSize - 1) return; //resize(2 * maxSize);

        // add x, and percolate it up to maintain heap invariant
        myArray[++length] = data;
        bubbleUp(length);
    }


    // Utility of insert; maintains heap property.
    private void bubbleUp(int k)
    {
        while (k > 1 && less(k/2, k))
            swap(k, k=k/2);
    }


    /* ************************************
    \\ Returns the greatest element in the
    // heap.
    \\
    // ************************************/
    public Comparable<E> maximum(
    ){ return myArray[1]; }


    /* ************************************
    \\ Removes and returns the greatest
    // element in the heap.
    \\
    // ************************************/
    public Comparable<E> extractMax(
    ){
        // Check that the heap is fruitful.
        if(length == 0) return null;

        // Store the largest element in the heap.
        E max = myArray[1];
        // Update myArray s.t. the next largest
        // element appears at the 'root'; index 1.
	      myArray[1] = myArray[length--];
        myArray = heapify(myArray, 1);

        // Return the heap-fruit.
        return max;
    }


    /* ************************************
    \\ heapify recursively adjusts a list
    // to satisfy the heap poperty.
    \\
    // ************************************/
    private E[] heapify( E[] list
                       , int i
    ){
        // Store the indices of relatives of i
        int l = getLeftChild(i),
            r = getRightChild(i);

        // Check that i, and heap[i]'s child-indices are valid.
        if(!validateIndex(list.length-1, (new int[] {i,l,r}))) return null;

        // Check that the relevant heap elements exist.
        if(  list[i] == null
          || list[r] == null
          || list[l] == null)
            return null;

        // Compare i's left child with i.
        int cmp = myArray[l].compareTo(list[i]),
        // Determine the largest element between i and its left child.
        largest = (l <= length && cmp > 0)? l : i;

        // Update comparison variable with relation of i's
        // right child to the larger element between l and i.
        cmp = list[r].compareTo(list[largest]);

        // If the i's right child is larger than i and
        // i's left child, update largest accordingly.
        if(r <= length && cmp > 0)
            largest = r;

        // If i is larger than both of i's children,
        // then the local heap at i is valid.

        // Otherwise...
        if(largest != i)
        {
            // Update the tree s.t. the heap property is preserved.
            swap(i,largest,list);
            // Then make sure nothing else is fucked up.
            heapify( list, largest);
        }

        return list; // heapify-fruit?
    }


    /* ************************************
    \\ I'll bet you can't guess what swap
    // does.
    \\
    // ************************************/
    private void swap( int a
                     , int b
    ){ myArray = swap(a, b, myArray); } // Delegate to more gneral swap method.

    // General swap method for arrays beyond myArray.
    private E[] swap( int a
                    , int b
                    , E[] list
    ){
        E temp;
        temp = list[a];
        list[a] = list[b];
        list[b] = temp;
        return list;
    }


    /* ************************************
    \\ Access descendents for xxxtreeem OOP.
    //
    \\ ************************************/
    private int getLeftChild( int i
    ){ return 2 * i; }

    private int getRightChild( int i
    ){ return 2 * i + 1; }


    /* **************************************
    \\ Returns true if the given array, list,
    // satisfies the max-heap property.
    \\
    // **************************************/
    public boolean isMaxHeap( E[] list
                            , int n
    ){
        if (n > length) return true; // Edge condition.
        // Get n's children for comparison.
        int left = getLeftChild(n)
          , right = getRightChild(n);

        // Check whether the current node follows the rules.
        if (left  <= length && less(list, n, left))  return false;
        if (right <= length && less(list, n, right)) return false;

        // The local heap at n is valid.. Keep checking.
        return isMaxHeap(left) && isMaxHeap(right);
    }

    // Shortcut-wrapper for isMaxHeap(E[],int)
    // for calls on myArray, with root=1.
    public boolean isMaxHeap(
    ){ return isMaxHeap(1); }

    // Shortcut-wrapper for isMaxHeap(E[],int)
    // for calls on myArray, with root=k.
    private boolean isMaxHeap( int n
    ){ return isMaxHeap(myArray, n); }


    // Check that i satisfies 0 <= i <= len - for safe array access.
    private boolean validateIndex( int len
                                 , int i
    ){ return 0 <= i && i < len; }


    // Validate a list of indices
    private boolean validateIndex( int len
                                 , int[] N
    ){
        for(int i = 0; i < N.length; ++i)
            if(!validateIndex(len, N[i])) return false;
        return true;
    }


    // True if myArray[i] < myArray[j]
    private boolean less( int i
                        , int j
    ){ return less(myArray, i , j); }

    // General less function.
    private boolean less( E[] list
                        , int i
                        , int j
    ){ return list[i].compareTo(list[j]) < 0; }

}
