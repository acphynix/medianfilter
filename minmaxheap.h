class MMNode{
  MMNode* parent;
  MMNode* left;
  MMNode* right;
  RBRNode value;
}
class MMHeap{
public:
  MMHeap(int size){
    data = new MMNode[size];
  }
  MMNode *data;
  int min();
  int max();
  void insert(MMNode *v);
};