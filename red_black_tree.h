#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#ifdef DMALLOC
#include <dmalloc.h>
#endif
#include "misc.h"
#include "stack.h"

#include "minmaxheap.h"
#include <limits.h>

#include <vector>
#include <stdarg.h>
using std::vector;

// colored output for inline verbose-optional debug messages

#define KYEL  "\x1B[33m"
#define KRED  "\x1B[31m"
#define KNRM  "\x1B[0m"

// associate verbosity with filename (but, because we don't have strcmp
// just yet, just look at the first letter of the filename).
//
// this is a bit of a hack; a better way of doing this would involve
// #defining a VERBOSE_PRECEDENCE flag in each of the associated files,
// and relying on that.

extern int verbose;

int dprint(const char *format, ...);

/*  CONVENTIONS:  All data structures for red-black trees have the prefix */
/*                "rb_" to prevent name conflicts. */
/*                                                                      */
/*                Function names: Each word in a function name begins with */
/*                a capital letter.  An example funcntion name is  */
/*                CreateRedTree(a,b,c). Furthermore, each function name */
/*                should begin with a capital letter to easily distinguish */
/*                them from variables. */
/*                                                                     */
/*                Variable names: Each word in a variable name begins with */
/*                a capital letter EXCEPT the first letter of the variable */
/*                name.  For example, int newLongInt.  Global variables have */
/*                names beginning with "g".  An example of a global */
/*                variable name is gNewtonsConstant. */

/* comment out the line below to remove all the debugging assertion */
/* checks from the compiled code.  */
#define DEBUG_ASSERT 1


#define min(x,y) (((x)<(y))?(x):(y))
#define max(x,y) (((x)>(y))?(x):(y))

class RBRNode {
public:
  RBRNode();
  int set;
  int key;
  void* info;
  int red; /* if red=0 then the node is black */
  int count; // number of children;
  RBRNode* left;
  RBRNode* right;
  RBRNode* parent;

  RBRNode* succ(int n);
};


/* Compare(a,b) should return 1 if *a > *b, -1 if *a < *b, and 0 otherwise */
/* Destroy(a) takes a pointer to whatever key might be and frees it accordingly */
class RBRTree {
public:
  RBRTree();
  RBRTree(int n, RBRNode *data);
  RBRNode* root;
  RBRNode* nil;
  RBRNode* data;
  RBRNode* min;
  RBRNode* max;
  int n;

  int off;
  int pos;

  bool insert(int value);
  bool remove(int index);
  bool replace(int index, int value);

  RBRNode* median();
  RBRNode* nth(int n);

  void print();
  void printall();
};


RBRNode* TreePredecessor(RBRTree*,RBRNode*);
RBRNode* TreeSuccessor(RBRTree*,RBRNode*);

template<int N>
int full_median(std::vector<RBRTree*> &trees){

  static MHeap medians_lo(N);
  static MHeap medians_up(N);
  medians_lo.reset();
  medians_up.reset();

  // verbose = vb;
  // properties of type RBRNode
  const int rangelow   = 0;
  const int medianlo   = 1;
  const int medianup   = 2;
  const int rangehigh  = 3;
  RBRNode *p[N][5];
 
  // properties of type int
  const int size       = 0;
  const int medct      = 1;
  const int alive      = 2;
  int v[N][4];

  int ct = N;

  // initialize min, max, medians.
  RBRNode *medmin=0, *medmin_alt=0;
  RBRNode *medmax=0, *medmax_alt=0;

  int medmini, medmin_alti;
  int medmaxi, medmax_alti;

  for(int i=0;i<N;++i){
    v[i][medct]     = (N%2 == 0);
    v[i][size]      = trees[i]->n;
    v[i][alive]     = 1;
    p[i][medianup]  = trees[i]->median();
    p[i][medianlo]  = TreePredecessor(trees[i],p[i][medianup]);
    p[i][rangelow]  = trees[i]->min;
    p[i][rangehigh] = trees[i]->max;

    medians_lo.min_insert(MHNode{i,p[i][medianlo]->key});
    medians_up.max_insert(MHNode{i,p[i][medianup]->key});
  }
  // while we have >2 trees...
  int itr = 0;
  for(;;){
    // find the minimum and maximum medians.
    int i=0;
    /*
    while(!v[i][alive])++i;
    medmin  = medmin_alt  = p[i][medianlo];
    medmax  = medmax_alt  = p[i][medianup];
    medmini = medmin_alti = i;
    medmaxi = medmax_alti = i;
    for(;i<N;++i){
      if(!v[i][alive])continue;
      if(p[i][medianlo]->key < medmin->key){
        medmin = p[i][medianlo];
        medmini     = i;

        medmin_alt  = 0;
        medmin_alti = 0;
      }else if(medmin->key == p[i][medianlo]->key){
        medmin_alt  = p[i][medianlo];
        medmin_alti = i;
      }
      if(p[i][medianup]->key > medmax->key){
        medmax = p[i][medianup];
        medmaxi = i;

        medmax_alt  = 0;
        medmax_alti = 0;
      }else if(medmax->key == p[i][medianup]->key){
        medmax_alt  = p[i][medianup];
        medmax_alti = i;
      }
    }

    // try to make the min and max be in different trees.
    if(medmaxi == medmini){
      if(medmin_alti){
        medmin  = medmin_alt;
        medmini = medmin_alti;
      }else if(medmax_alti){
        medmax  = medmax_alt;
        medmaxi = medmax_alti;
      }
    }
    */

    medmini = medians_lo.head()->k;
    medmaxi = medians_up.head()->k;

    if(medmini == medmaxi){
      MHNode *low_second, *high_second;
      if((low_second = medians_lo.min_second())->v == p[medmini][medianlo]->key){
        medmini = low_second->k;
      }else if((low_second = medians_lo.max_second())->v == p[medmini][medianup]->key){
        medmaxi = high_second->k;
      }
    }

    // dprint debug info.

    bool on = false;
    for(int i=0;i<N;++i){
      RBRNode *cur = trees[i]->min;
      char flair = 'v';
      if (i == medmini && i == medmaxi)dprint(" xxx ");
      else if(i == medmini)dprint(" <<< ");
      else if (i==medmaxi)dprint(" >>> ");
      else dprint("     ");
      for(;;){
        if(cur == p[i][rangelow]){
          on = true;
        }
        if(!v[i][alive]){
          on = false;
        }
        if      (cur == p[i][medianup]) flair = '|';
        else if (cur == p[i][medianlo]) flair = '|';
        else                    flair=' ';
        if(on)dprint("%c%4d%c", flair, cur->key, flair);
        else  dprint("      ");
        // for(int i=0,j=0;i<100000000;++i)j=j*i;
        if(cur == p[i][rangehigh]){
          on = false;
        }
        if(cur == trees[i]->max)break;
        cur = cur->succ(1);
      }
      if (i == medmini && i == medmaxi)dprint(" xxx ");
      else if(i == medmini)dprint(" <<< ");
      else if (i==medmaxi)dprint(" >>> ");
      else dprint("     ");

      if(!v[i][alive])dprint("   DEAD %d", v[i][size]);
      else dprint("        %d", v[i][size]);

      dprint("\n");
    }

    medians_lo.print();
    printf("\n");
    medians_up.print();
    printf("\n");

    // delete the same number of elements from both arrays,
    // delete elements e, e > medmax OR e < medmin.

    RBRNode* left  = p[medmini][rangelow];
    RBRNode* right = p[medmaxi][rangehigh];

    int cut = min((v[medmini][size])/2, (v[medmaxi][size])/2);
    cut = max(cut, 1);

    // dprint("cut %d\n",cut);

    int medadjmin = 0;
    int medadjmax = 0;
    if(cut%2 == 1){
      v[medmini][medct] += 1;
      v[medmaxi][medct] -= 1;
      if(v[medmini][medct] == 2){
        v[medmini][medct] = 0;
        medadjmin = 1;
      }
      if(v[medmaxi][medct] == 2){
        v[medmaxi][medct] = 0;
        medadjmax = -1;
      }
    }

    // dprint("v[%d][size] = %d\n",medmini, v[medmini][size]);
    // dprint("v[%d][size] = %d\n",medmaxi, v[medmaxi][size]);

    v[medmini][size]  -= cut;
    v[medmaxi][size]  -= cut;


    p[medmini][rangelow]    = p[medmini][rangelow]->succ(cut);
    p[medmaxi][rangehigh]   = p[medmaxi][rangehigh]->succ(-cut);

    p[medmini][medianlo]    = p[medmini][medianlo]->succ(cut/2  + medadjmin);
    p[medmini][medianup]    = p[medmini][medianlo]->succ(1);

    p[medmaxi][medianlo]    = p[medmaxi][medianlo]->succ(-cut/2 + medadjmax);
    p[medmaxi][medianup]    = p[medmaxi][medianlo]->succ(1);


    // dprint("move medmin by %d\n", cut/2  + medadjmin);
    // dprint("move medmax by %d\n", -cut/2  + medadjmax);

    // dprint("set min median = %d %d\n", p[medmini][medianlo]->key, p[medmini][medianup]->key);
    // dprint("set max median = %d %d\n", p[medmaxi][medianlo]->key, p[medmaxi][medianup]->key);

    if(v[medmini][alive] && v[medmini][size]<=0){
      v[medmini][alive] = 0;
      --ct;
      medians_lo.min_replace(MHNode{medmini, INT_MAX});
    }else{
      medians_lo.min_replace(MHNode{medmini, p[medmini][medianlo]->key});
    }
    if(v[medmaxi][alive] && v[medmaxi][size]<=0){
      v[medmaxi][alive] = 0;
      --ct;
      medians_up.max_replace(MHNode{medmaxi, INT_MIN});
    }else{
      medians_up.max_replace(MHNode{medmaxi, p[medmaxi][medianup]->key});
    }
    if(ct <= 0){
      return p[medmini][medianlo]->key;
    }
    dprint("\n\n");
  }
  for(int i=0;i<N;++i){
    if( v[i][alive] ){
      return p[i][medianup]->key;
    }
  }
  return 0;
}

#endif