#include <limits.h>
#include <stdio.h>
#include <cstdlib>
#include "minmaxheap.h"
#include "red_black_tree.h"

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

void MHeap::min_insert(MHNode xv){
  data[ind] = xv;
  MHNode *p;
  int xi=ind, pi;

  // the parent must be leq its children.
  for(;;){
    pi = PARENT(xi);
    p  = data+pi;

    // if the child is less than the
    // parent, then swap them. else, break.
    if(xi && xv.v < p->v){
      data[xi] = *p;
      data[xi].corr->corr = data+xi;
      // printf("%d <-> %d <-> %d\n", data[xi].v, data[xi].corr->v, data[xi].corr->corr->v);
      xi = pi;
      continue;
    }
    xv.corr->corr = data+xi;
    data[xi] = xv;
    // printf("%d <-> %d <-> %d\n", data[xi].v, data[xi].corr->v, data[xi].corr->corr->v);

    break;
  }
  ++ind;
}
void MHeap::max_insert(MHNode xv){
  data[ind] = xv;
  // printf("insert %d <-> %d <-> %d\n", data[ind].v, data[ind].corr->v, data[ind].corr->corr->v);
  MHNode *p;
  int     xi=ind, pi;

  // the parent must be geq its children.
  for(;;){
    pi = PARENT(xi);
    p  = data+pi;

    // if the child is less than the
    // parent, then swap them. else, break.
    if(xi && xv.v > p->v){
      // printf("MAXINS\n");
      data[xi] = *p;
      data[xi].corr->corr = data+xi;
      // printf("%d <-> %d <-> %d\n", data[xi].v, data[xi].corr->v, data[xi].corr->corr->v);
      xi = pi;
      continue;
    }
    xv.corr->corr = data+xi;
    data[xi] = xv;
    // printf("%d <-> %d <-> %d\n", data[xi].v, data[xi].corr->v, data[xi].corr->corr->v);
    break;
  }
  ++ind;
}

/* Move *xv around so that the heap
 * property is satisfied. Assumed that
 * *xv is the only node that breaks
 * the heap property, and that
 *   xv = data[ind] for some ind.
 *
 */
MHNode *MHeap::min_update(MHNode *x){
  // print();
  // printf("\nminheap[%d] = %d\n",(int)(x-data), x->v);
  // printf("minupdate %d\n",x->v);
  static MHNode MH_MAX{0, INT_MAX, 0};
  int    xi,    pi, li, ri;
  MHNode xv=*x, pv, lv, rv;
  MHNode        *p, *l, *r;
  bool   xl,        xr, lr;

  xi = (x-data);
  pi = PARENT(ind);
  
  // check parent
  if(xi && x->v < data[pi].v){
    // percolate up and then exit.
    for(;;){
      pi = PARENT(xi);
      p  = data+pi;

      // if the child is less than the
      // parent, then swap them. else, break.
      if(xi && xv.v < p->v){
        data[xi] = *p;
        data[xi].corr->corr = data+xi;

        xi = pi;
        continue;
      }
      data[xi] = xv;
      data[xi].corr->corr = data+xi;
      return data+xi;
    }
  }

  // check children
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
      data[xi].corr->corr = data+xi;
      return data+xi;
    }else if(!xl && lr){
      // swap with left.
      data[xi] = data[li];
      data[xi].corr->corr = data+xi;
      xi = li;
    }else{
      // swap with right.
      data[xi] = data[ri];
      data[xi].corr->corr = data+xi;
      xi = ri;
    }
  }
}

MHNode *MHeap::max_update(MHNode *x){
  // print();
  // printf("\nmaxheap[%d] = %d\n",(int)(x-data), x->v);
  static MHNode MH_MIN{0, INT_MIN, 0};
  int    xi,    pi, li, ri;
  MHNode xv=*x, pv, lv, rv;
  MHNode        *p, *l, *r;
  bool   xl,        xr, lr;

  xi = (x-data);
  pi = PARENT(ind);
  
  // check parent
  if(xi && x->v > data[pi].v){
    // percolate up and then exit.
    for(;;){
      pi = PARENT(xi);
      p  = data+pi;

      // if the child is less than the
      // parent, then swap them. else, break.
      if(xi && xv.v > p->v){
        data[xi] = *p;
        data[xi].corr->corr = data+xi;

        xi = pi;
        continue;
      }
      data[xi] = xv;
      data[xi].corr->corr = data+xi;
      return data+xi;
    }
  }

  // check children
  for(;;){
    li=LCHILD(xi);
    ri=RCHILD(xi);

    // prevent overflow when there are
    // no more children.
    l=(li<size)?(data+li):&MH_MIN;
    r=(ri<size)?(data+ri):&MH_MIN;
    
    // used to correctly swap elements.
    xl=(xv.v <= l->v);
    xr=(xv.v <= r->v);
    lr=(l->v <= r->v);
    
    if(!xl && !xr){
      // printf(" stay\n");
      data[xi] = xv;
      data[xi].corr->corr = data+xi;
      return data+xi;
    }else if(xl && !lr){
      // printf("left ");
      // swap with left.
      data[xi] = data[li];
      data[xi].corr->corr = data+xi;
      xi = li;
    }else{
      // printf("right ");
      // swap with right.
      data[xi] = data[ri];
      data[xi].corr->corr = data+xi;
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
    // dprint("%d ", data[i].v);
  }
}


CorrHeap::CorrHeap(int size){
  minheap = MHeap(size);
  maxheap = MHeap(size);
}
void CorrHeap::insert(MHNode vmin, MHNode vmax){
  // printf("insert %d, %d\n", vmin.v, vmax.v);
  minheap.data[minheap.ind] = vmin;
  maxheap.data[maxheap.ind] = vmax;

  vmin.corr = maxheap.data+maxheap.ind;
  vmax.corr = minheap.data+minheap.ind;
  // printf("    ~ min insert\n");
  minheap.min_insert(vmin);
  // for(int i=0;i<minheap.ind;++i){
  //   // printf("min %d <-> %d <-> %d\n", minheap.data[i].v, minheap.data[i].corr->v, minheap.data[i].corr->corr->v);
  //   if(minheap.data[i].v != minheap.data[i].corr->corr->v)exit(0);
  // }
  // for(int i=0;i<maxheap.ind;++i){
  //   // printf("max %d <-> %d <-> %d\n", maxheap.data[i].v, maxheap.data[i].corr->v, maxheap.data[i].corr->corr->v);
  //   if(maxheap.data[i].v != maxheap.data[i].corr->corr->v)exit(0);
  // }
  // printf("    ~ max insert\n");
  maxheap.max_insert(maxheap.data[maxheap.ind]);
  // for(int i=0;i<minheap.ind;++i){
  //   // printf("min %d <-> %d <-> %d\n", minheap.data[i].v, minheap.data[i].corr->v, minheap.data[i].corr->corr->v);
  //   if(minheap.data[i].v != minheap.data[i].corr->corr->v)exit(0);
  // }
  // for(int i=0;i<maxheap.ind;++i){
  //   // printf("max %d <-> %d <-> %d\n", maxheap.data[i].v, maxheap.data[i].corr->v, maxheap.data[i].corr->corr->v);
  //   if(maxheap.data[i].v != maxheap.data[i].corr->corr->v)exit(0);
  // }
}

void CorrHeap::minmax_distinct(MHNode **rmin, MHNode **rmax){
  // printf("a");
  MHNode *minv = minheap.head();
  MHNode *maxv = maxheap.head();
  // printf("b");

  if(minv->k == maxv->k){
    MHNode *min_second = minheap.min_second();
    if(min_second->v == minv->v){
      *rmin = min_second;
      *rmax = maxv;
      return;
    }
    MHNode *max_second = maxheap.max_second();
    if(max_second->v == maxv->v){
      *rmin = minv;
      *rmax = max_second;
      return;
    }
  }
  *rmin = minv;
  *rmax = maxv;
  // printf("c");
}

void CorrHeap::updateminmax(MHNode *umin, MHNode *umax, int minv, int minc, int maxv, int maxc){
  // umin and umax are nodes in the data arrays.
  MHNode *umincorr = umin->corr;
  MHNode *umaxcorr = umax->corr;
  // printf("uminmax %d %d\n",umin->v, umax->v);
  // maxheap.print();printf("\n");

  // update min, but now maxcorr has changed.
  umin->v = minv;
  umin     = minheap.min_update(umin);
  umaxcorr = umax->corr;

  // update max, but now mincorr has changed.
  umax->v = maxv;
  umax     = maxheap.max_update(umax);
  umincorr = umin->corr;

  umincorr->v = minc;
  umaxcorr->v = maxc;
  maxheap.max_update(umincorr);
  minheap.min_update(umaxcorr);
}

void CorrHeap::reset(){
  minheap.reset();
  maxheap.reset();
}
void CorrHeap::print(){
  minheap.print();
  // dprint(" + ");
  maxheap.print();
}