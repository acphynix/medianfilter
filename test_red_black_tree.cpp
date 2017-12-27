#include "red_black_tree.h"
#ifdef PERREAULT
#include "Perreault.h"
#elif HUANG
#include "Huang.h"
#elif PERREAULTQS
#include "PerreaultQS.h"
#elif HUANGQS
#include "HuangQS.h"
#endif
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "quickselect.h"

#ifndef WINDOW_SIZE
#define WINDOW_SIZE (1)
#endif

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
void brute_force(int *data, int *dims, int *out, int R){
  int N = R*2 + 1;
  int size = N*N*N;
  int *buffer = new int[size];
  int w = dims[0], l=dims[1], h=dims[2];
  int index = 0;
  int total = (dims[0]-2*R)*(dims[1]-2*R)*(dims[2]-2*R);

  int copy_ct = 0;
  int qs_ct = 0;

  struct timespec spec;
  clock_gettime(CLOCK_MONOTONIC, &spec);
  double sec_start = double(spec.tv_sec) + (spec.tv_nsec)/1000000000.0;
  double sec;

  for(int x=R;x<dims[0]-R;++x){
    for(int y=R;y<dims[1]-R;++y){
      for(int z=R;z<dims[2]-R;++z){
        if(index % 1000 == 0 || index+1 == total){
          clock_gettime(CLOCK_MONOTONIC, &spec);
          float p = ((float)index)/total;
          sec =  double(spec.tv_sec) + (spec.tv_nsec)/1000000000.0;
          sec -= sec_start;
          if(sec > TIMELIMIT || index+1 == total){
            // printf("Done %d = %.8f\n", index, p);
            // printf("Took %.3fs\n", sec);
            float e = (1.f/p)*sec;
            printf("bruteforce\twindow,time,total\t%d\t%.3f\t%.3f\n",WINDOW_SIZE,sec,e);
            // printf("Total Time: %.3f seconds\n", e);
            // exit(0);
          }
        }
        int i=0;
        for(int xx=x-R; xx<=x+R; ++xx){
          for(int yy=y-R; yy<=y+R; ++yy){
            for(int zz=z-R; zz<=z+R; ++zz){
              // printf("%d %d %d %d\n",xx,yy,zz,i);
              buffer[i] = data[zz*w*l + yy*w + xx];
              ++i;
              ++copy_ct;
            }
          }
        }
        // printf("%d %d %d %d\n",x,y,z,index);
        int oi = z*w*l + y*w + x;
        out[oi] = quick_select(buffer, size);
        // printf("out[%d] = %d\n", index, out[index]);
        ++qs_ct;
        ++index;
      }
    }
  }
  // printf("copy: %d\n",copy_ct);
  // printf("qs: %d\n", qs_ct);
  // printf("final: %d\n",index);
}
int main(int argc, char** argv) {
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

  int dims[3] = {496,258,190};
  int fmin[3] = {0,0,0};
  int fsiz[3] = {496,258,190};
  int fmax[3] = {496,258,190};
  int *data = (int*)malloc(dims[0]*dims[1]*dims[2]*sizeof(int));
  int * out = (int*)malloc(dims[0]*dims[1]*dims[2]*sizeof(int));

  int ind = 0;
  for(int i=0;i<dims[0];++i){
    for(int j=0;j<dims[1];++j){
      for(int k=0;k<dims[2];++k){
        int v = ((((ind*ind + 354)<<(ind%10))*ind*ind+7)*2) % 100 + 101;
        data[ind++] = v;
        // printf("%d ",v);
      }
    }
  }

  // print(data,dims);

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

#ifdef BRUTEFORCE
  brute_force(data, dims, out, WINDOW_SIZE);
#else
  median_filter_3D<WINDOW_SIZE>(data, dims, out, fmin, fsiz, fmax);
#endif

  int sum =0 ;
  int size = dims[0]*dims[1]*dims[2];
  for(int i=0;i<size;++i){
    // printf("%d ",out[i]);
    sum += out[i] * out[size-i];
  }

  // print(out,dims);

  printf("result: %d\n", sum);

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
