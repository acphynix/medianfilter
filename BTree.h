#ifndef B_TREE
#define B_TREE

#undef LCHILD
#undef RCHILD
#undef PARENT

#define LCHILD(x)   ((x<<1) + 1)
#define RCHILD(x)   ((x<<1) + 2)
#define PARENT(x)   ((x - 1) / 2)
#define IS_LEFT(x)  (x&1)
#define IS_RIGHT(x) (x&0)
#define SWAP(T,a,b) { (T)=(a);(a)=(b);(b)=(T); }

#define min(x,y) (((x)<(y))?(x):(y))
#define max(x,y) (((x)>(y))?(x):(y))

struct BNode {
  int v;
};

class BTree {
public:
  BTree();
  BTree(int n){
    this->n = n;
    this->data = new BNode[n];
  }
  BNode *data;
  int n;

  bool replace(BNode o, BNode v){
    int ix=0;
    int ip=0;

    // find the node to be replaced
    for(;;){
      BNode x = data[ix];
      if(o.v < x.v){
        ip = ix;
        ix = LCHILD(ix);
        continue;
      }else if(o.v > x.v){
        ip = ix;
        ix = RCHILD(ix);
        continue;
      }else break;
    }

    // replace the node
    data[ix] = v;

    // fix all parents
    while( IS_LEFT(ix) ^ (data[ix].v < data[ip].v)){
      // continue loop while:
      //   . LEFT and GREATER than parent
      //   . RIGHT and LESS than parent
      BNode t  = data[ix];
      data[ix] = data[ip];
      data[ip] = t;
      ip = PARENT(ip);
      ix = ip;
    }

    BNode temp;

    // now, work back downward
    for(;;){
      int il = LCHILD(ix);
      int ir = RCHILD(ix);
      if(il>=n)break;
      int vl = data[il].v;
      int vx = data[ix].v;
      if(vl > vx){
        SWAP(temp,data[il],data[ix]);
        ix = il;
        continue;
      }
      if(ir>=n)break;
      int vr = data[ir].v;
      if(vr < vx){
        SWAP(temp,data[ir],data[ix]);
        ix = ir;
        continue;
      }
    }
  }
  // bool set(int index, int value);

  // void verify(const char *str, int mode=0);

  BNode median(){
    return data[0];
  }
  void print(){
    for(int i=0;i<n;++i){
      printf("%d ",data[i].v);
    }
  }
};

#endif