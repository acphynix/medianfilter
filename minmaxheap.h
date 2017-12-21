#ifndef MINMAXHEAPH
#define MINMAXHEAPH

#include <iostream>

enum HeapFunc { MAX, MIN };

struct MHNode{
  int k;
  int v;
};
class MHeap{
public:
  int ind;
  int size;
  MHeap();
  MHeap(int size);
  MHNode *data;  
  MHNode *head();

  void min_insert(MHNode x, int *minToMax, int *maxToMin);
  void min_replace(MHNode x, int *minToMax, int *maxToMin);
  MHNode *min_second(); 

  void max_insert(MHNode x, int *minToMax, int *maxToMin);
  void max_replace(MHNode x, int *minToMax, int *maxToMin);
  MHNode *max_second();
  
  void reset();
  void print();
};
class CorrHeap{
private:
public:
  CorrHeap(int size);

  MHeap minheap;
  MHeap maxheap;

  int* minToMax;
  int* maxToMin;

  void insert(MHNode vmin, MHNode vmax);
  void replacemin(MHNode vmin, MHNode vmax);
  void replacemax(MHNode vmin, MHNode vmax);
};


#endif