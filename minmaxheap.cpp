#include <limits.h>
#include <stdio.h>
#include <cstdlib>
#include "minmaxheap.h"

#define LCHILD(x) ((x<<1) + 1)
#define RCHILD(x) ((x<<1) + 2)
#define PARENT(x) ((x - 1) / 2)

#define min(x,y) (((x)<(y))?(x):(y))
#define max(x,y) (((x)>(y))?(x):(y))

MHeap::MHeap(int size){
  this->data = new MHNode[size];
  this->size = size;
  this->ind = 0;
}
MHeap::MHeap(){}
MHNode *MHeap::head(){
  return data;
}

void MHeap::min_insert(MHNode xv, int *minToMax, int *maxToMin){
  data[ind] = xv;
  MHNode *p;
  int     xi=ind, pi;

  // the parent must be leq its children.
  for(;;){
    pi = PARENT(xi);
    p  = data+pi;

    // if the child is less than the
    // parent, then swap them. else, break.
    if(xi && xv.v < p->v){
      data[xi] = *p;
      xi = pi;
      continue;
    }
    data[xi] = xv;
    break;
  }
  ++ind;
}
void MHeap::max_insert(MHNode xv, int *minToMax, int *maxToMin){
  data[ind] = xv;
  MHNode *p;
  int     xi=ind, pi;

  // the parent must be geq its children.
  for(;;){
    pi = PARENT(xi);
    p  = data+pi;

    // if the child is less than the
    // parent, then swap them. else, break.
    if(xi && xv.v > p->v){
      data[xi] = *p;
      xi = pi;
      continue;
    }
    data[xi] = xv;
    break;
  }
  ++ind;
}

void MHeap::min_replace(MHNode xv, int *minToMax, int *maxToMin){
  static MHNode MH_MAX{0, INT_MAX};
  int xi = 0, li, ri;
  MHNode* x=&xv, *l, *r;
  bool xl, xr, lr;
  for(;;){
    li=LCHILD(xi);
    ri=RCHILD(xi);

    // prevent overflow when there are
    // no more children.
    l=(li<size)?(data+li):&MH_MAX;
    r=(ri<size)?(data+ri):&MH_MAX;
    
    // used to correctly swap elements.
    xl=(xv.v <= l->v);
    xr=(xv.v <= r->v);
    lr=(l->v <= r->v);
    
    if(xl && xr){
      data[xi] = xv;
      break;
    }else if(!xl && lr){
      // swap with left.
      data[xi] = data[li];
      xi = li;
    }else{
      // swap with right.
      data[xi] = data[ri];
      xi = ri;
    }
  }
}

void MHeap::max_replace(MHNode xv, int *minToMax, int *maxToMin){
  static MHNode MH_MIN{0, INT_MIN};
  int xi = 0, li, ri;
  MHNode* x=&xv, *l, *r;
  bool xl, xr, lr;
  for(;;){
    li=LCHILD(xi);
    ri=RCHILD(xi);

    // prevent overflow when there are
    // no more children.
    l=(li<size)?(data+li):&MH_MIN;
    r=(ri<size)?(data+ri):&MH_MIN;
    
    // used to correctly swap elements.
    xl=(xv.v < l->v);
    xr=(xv.v < r->v);
    lr=(l->v < r->v);
    
    if(!xl && !xr){
      // save and exit.
      data[xi] = xv;
      break;
    }else if(xl && !lr){
      // swap with left.
      data[xi] = data[li];
      xi = li;
    }else{
      // swap with right.
      data[xi] = data[ri];
      xi = ri;
    }
  }
}

MHNode* MHeap::min_second(){
  if(data[1].v <= data[2].v)return data+1;
  return data+2;
}
MHNode* MHeap::max_second(){
  if(data[1].v >= data[2].v)return data+1;
  return data+2;
} 
void MHeap::reset(){
  ind = 0;
}
void MHeap::print(){
  for(int i=0;i<size;++i){
    printf("%d ", data[i].v);
  }
}


CorrHeap::CorrHeap(int size){
  minheap = MHeap(size);
  maxheap = MHeap(size);

  minToMax = new int[size];
  maxToMin = new int[size];
}
void CorrHeap::insert(MHNode vmin, MHNode vmax){
  minToMax[minheap.ind] = maxheap.ind;
  maxToMin[maxheap.ind] = minheap.ind;
  minheap.min_insert(vmin, minToMax, maxToMin);
  maxheap.max_insert(vmax, minToMax, maxToMin);
}