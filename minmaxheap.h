#ifndef MINMAXHEAPH
#define MINMAXHEAPH

#include <iostream>

enum HeapFunc { MAX, MIN };

struct MHNode{
  int k;
  int v;
  MHNode* corr;
};
class MHeap{
public:
  int ind;
  int size;
  MHeap();
  MHeap(int size);
  MHNode *data;  
  MHNode *head();

  void min_update(MHNode *x);
  void min_insert(MHNode x);
  void min_replace(MHNode x);
  MHNode *min_second(); 

  void max_update(MHNode *x);
  void max_insert(MHNode x);
  void max_replace(MHNode x);
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

  // call update on both min and max.
  void updateminmax(MHNode *umin, MHNode *umax);

  // gets min and max values from the heap such that
  // the keys are the min and max, but, if multiples
  // exist, then the values are distinct.
  void minmax_distinct(MHNode **rmin, MHNode **rmax);
  
  void reset();
  void print();
};


#endif