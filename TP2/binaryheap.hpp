// A C++ program to demonstrate common Binary Heap Operations
#include <iostream>
#include <climits>
#include <map>

using namespace std;
 
// Prototype of a utility function to swap two integers
void swap(float *x, float *y);
 
// A class for Min Heap
class MinHeap
{
    float *harr; // pointer to array of elements in heap
    int capacity; // maximum possible size of min heap
    int heap_size; // Current number of elements in min heap

public:

    // Constructor
    MinHeap(int capacity);
 
    // Searchs heap for index of element with given value k
    int findIndex(float k);

    int parent(int i) { return (i-1)/2; }
 
    // to get index of left child of node at index i
    int left(int i) { return (2*i + 1); }
 
    // to get index of right child of node at index i
    int right(int i) { return (2*i + 2); }
 
    // to extract the root which is the minimum element
    int extractMin();
 
    // Decreases key value of key at index i to new_val
    int decreaseKey(int i, float new_val);
 
    // Returns the minimum key (key at root) from min heap
    float getMin() { return harr[0]; }
 
    // to heapify a subtree with root at given index
    void MinHeapify(int );

    // Deletes a key stored at index i
    void deleteKey(int i);
 
    // Inserts a new key 'k'
    void insertKey(float k);

    // Prints heap array
    void dump() { for(int i=0;i<heap_size;i++) cout << harr[i] << ' '; cout << endl; }

    // Returns true if heap is empty
    bool isEmpty() { return heap_size < 1; }

};
 
// Constructor: Builds a heap from a given array a[] of given size
MinHeap::MinHeap(int cap)
{
    heap_size = 0;
    capacity = cap;
    harr = new float[cap];
}
 
// Inserts a new key 'k'
void MinHeap::insertKey(float k)
{
    if (heap_size == capacity)
    {
        cout << "\nOverflow: Could not insertKey\n";
        return;
    }
 
    // First insert the new key at the end
    heap_size++;
    int i = heap_size - 1;
    harr[i] = k;

    // Fix the min heap property if it is violated
    while (i != 0 && harr[parent(i)] > harr[i])
    {
       swap(&harr[i], &harr[parent(i)]);
       i = parent(i);
    }
}
 
// Decreases value of key at index 'i' to new_val and return its new parent.  It is assumed that
// new_val is smaller than harr[i].
int MinHeap::decreaseKey(int i, float new_val)
{
    int j = i;
    harr[i] = new_val;
    while (i != 0 && harr[parent(i)] > harr[i])
    {
       swap(&harr[i], &harr[parent(i)]);
       i = parent(i);
    }

    return i;
}
 
// Method to remove minimum element (or root) from min heap
int MinHeap::extractMin()
{
    if (heap_size <= 0)
        return INT_MAX;
    if (heap_size == 1)
    {
        heap_size--;
        return harr[0];
    }
 
    // Store the minimum vaLue, and remove it from heap
    int root = harr[0];
    harr[0] = harr[heap_size-1];
    heap_size--;
    MinHeapify(0);
 
    return root;
}
  
// Searchs heap for index of element with given value k
int MinHeap::findIndex(float k) 
{
    for(int i=0;i<heap_size;i++)
        if(harr[i] == k) return i;
    return -1;
}

// This function deletes key at index i. It first reduced value to minus
// infinite, then calls extractMin()
void MinHeap::deleteKey(int i)
{
    decreaseKey(i, INT_MIN);
    extractMin();
}
 
// A recursive method to heapify a subtree with root at given index
// This method assumes that the subtrees are already heapified
void MinHeap::MinHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < heap_size && harr[l] < harr[i])
        smallest = l;
    if (r < heap_size && harr[r] < harr[smallest])
        smallest = r;
    if (smallest != i)
    {
        swap(&harr[i], &harr[smallest]);
        MinHeapify(smallest);
    }
}


// A utility function to swap two elements
void swap(float *x, float *y)
{
    float temp = *x;
    *x = *y;
    *y = temp;
}
 
