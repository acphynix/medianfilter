#include "red_black_tree.h"
#include "Perreault.h"
#include <stdio.h>
#include <ctype.h>


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
  RBRTree* tree = new RBRTree(49, new RBRNode[49+2+1]);

  tree->insert(4);
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


  // std::vector<RBRTree*> trees;
  // for(int i=0;i<7;++i)
  // trees.push_back(new RBRTree(7));
  // for(int i=0,j=0,k=0;i<7;i++,j+=2,k+=3){
  //   trees[0]->insert(i*i);
  //   trees[1]->insert(i*j);
  //   trees[2]->insert(j*k+1);
  //   trees[3]->insert(i*i+2);
  //   trees[4]->insert(i*j+3);
  //   trees[5]->insert(j*k+4);
  //   trees[6]->insert(i*j+5);
  // }
  // for(int i=0;i<trees.size();i++){
  //   trees[i]->printall();
  //   printf("\n");
  // }
  // trees[0]->printall();
  // trees[1]->printall();
  // trees[2]->printall();

  // printf("\n");
  // printf("trees 0:\n");
  // trees[0]->print();
  // RBRNode *b = trees[0]->nth(9);
  // for(int i=0;i<4;++i){
  //   printf("%p - ", b);
  //   printf("%d\n", b->key);
  //   b = b->succ(-3);
  // }



  // return 1;

  // int median = full_median<7>(trees);

  int dims[3] = {50,50,50};
  int fmin[3] = {0,0,0};
  int fsiz[3] = {50,50,50};
  int fmax[3] = {50,50,50};
  int *data = (int*)malloc(dims[0]*dims[1]*dims[2]*sizeof(int));
  int * out = (int*)malloc(dims[0]*dims[1]*dims[2]*sizeof(int));

  int ind = 0;
  for(int i=0;i<dims[0];++i){
    for(int j=0;j<dims[1];++j){
      for(int k=0;k<dims[2];++k){
        data[ind++] = ind-1;
      }
    }
  }
  printf("go!\n");

  // print(data,dims);

  median_filter_3D<3>(data, dims, out, fmin, fsiz, fmax);

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