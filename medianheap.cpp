#include "medianheap.h"

int mains(){
  MedianHeap mh(9);
  mh.replace(0,10);
  mh.replace(1,20);
  mh.replace(2,30);
  mh.replace(3,40);
  mh.replace(4,50);
  mh.replace(5,60);
  mh.replace(6,70);
  mh.replace(7,80);
  mh.replace(8,90);
  mh.print();
}