#include "red_black_tree.h"
#include "Perreault.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


void print(int *data, int *dims){
  // printf("hello! %d %d %d",dims[0],dims[1],dims[2]); 
  int ind =0 ;
  for(int i=0;i<dims[0];++i){
    for(int j=0;j<dims[1];++j){
      for(int k=0;k<dims[2];++k){
        printf("%2d ",data[ind++]);
      }
      printf("\n");
    }
    printf("\n\n");
  }
}
int main() {
  setbuf(stdout, 0);
  // RBRTree* tree = new RBRTree(49, new RBRNode[49+2+1]);

  // tree->insert(4);
  // tree->insert(5);
  // tree->insert(10);
  // tree->insert(1);
  // tree->insert(23);
  // tree->insert(19);
  // tree->insert(6);
  // tree->insert(5);
  // tree->insert(7);
  // tree->insert(10);
  // tree->insert(11);
  // tree->insert(12);
  // tree->insert(120);

  // tree->printall();

  srand (time(NULL));

  std::vector<RBRTree*> trees;
  for(int i=0;i<7;++i)
  trees.push_back(new RBRTree(7, new RBRNode[7+3]));
  for(int i=0,j=0,k=0;i<7;i++,j+=2,k+=3){
    trees[0]->insert(rand()%20);
    // trees[1]->insert(i*j);
    // trees[2]->insert(j*k+1);
    // trees[3]->insert(i*i+2);
    // trees[4]->insert(i*j+3);
    // trees[5]->insert(j*k+4);
    // trees[6]->insert(i*j+5);
  }
  // for(int i=0;i<1;i++){
  //   trees[i]->printall();
  //   printf("\n");
  // }
  // trees[0]->printall();
  // trees[1]->printall();
  // trees[2]->printall();

  // printf("\n");
  trees[0]->printall();
  printf("\n\n");
  trees[0]->print();
  // RBRNode *b = trees[0]->nth(9);
  // for(int i=0;i<4;++i){
  //   printf("%p - ", b);
  //   printf("%d\n", b->key);
  //   b = b->succ(-3);
  // }



  // return 1;

  // int median = full_median<7>(trees);

  int dims[3] = {50, 50, 50};
  int fmin[3] = {0,0,0};
  int fsiz[3] = {50, 50, 50};
  int fmax[3] = {50, 50, 50};
  int *data = (int*)malloc(dims[0]*dims[1]*dims[2]*sizeof(int));
  int * out = (int*)malloc(dims[0]*dims[1]*dims[2]*sizeof(int));

  int ind = 0;
  for(int i=0;i<dims[0];++i){
    for(int j=0;j<dims[1];++j){
      for(int k=0;k<dims[2];++k){
        data[ind++] = ((((ind*ind + 354)<<(ind%10))*ind*ind+7)*2)%1000 + 10005 ;
        // printf("%d ",data[ind-1]);
      }
    }
  }

  // int s = 9;
  // MHeap heap(s);
  // for(int i=0;i<s;i++){
  //   MHNode n{i,(i*4)%11};
  //   // printf("adding %d, %d\n", n.k, n.v);
  //   // heap.data[i] = n;
  //   heap.min_insert(n);
  //   // heap.print();
  //   // printf("\n");
  // }
  // heap.print();
  // printf("\n");
  // for(int i=0;i<s;++i){
  //   MHNode n{i,(i*4)%11};
  //   printf("replace %d\n", n.v);
  //   // heap.data[i] = n;
  //   heap.min_replace(n);
  //   heap.print();
  //   printf("\n");
  // }

  printf("go!\n");

  // print(data,dims);

  median_filter_3D<5>(data, dims, out, fmin, fsiz, fmax);

  printf("done.\n");
  // print(out, dims);

  // tree->replace(2,2);

  // printf("median: %d\n",median); 

  free(data);
  free(out);

  // tree->print();  
  // RBRNode *median = tree->median();
  // printf("median: %d\n", median->key);

  return 0;
}
