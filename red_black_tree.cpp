#include "red_black_tree.h"

/***********************************************************************/
/*  FUNCTION:  RBTreeCreate */
/**/
/*  INPUTS:  All the inputs are names of functions.  CompFunc takes to */
/*  void pointers to keys and returns 1 if the first arguement is */
/*  "greater than" the second.   DestFunc takes a pointer to a key and */
/*  destroys it in the appropriate manner when the node containing that */
/*  key is deleted.  InfoDestFunc is similiar to DestFunc except it */
/*  recieves a pointer to the info of a node and destroys it. */
/*  PrintFunc recieves a pointer to the key of a node and prints it. */
/*  PrintInfo recieves a pointer to the info of a node and prints it. */
/*  If RBTreePrint is never called the print functions don't have to be */
/*  defined and NullFunction can be used.  */
/**/
/*  OUTPUT:  This function returns a pointer to the newly created */
/*  red-black tree. */
/**/
/*  Modifies Input: none */
/***********************************************************************/

static void RBReplace(RBRTree* tree, int index, int value);
static bool RBTreeInsert(RBRTree*, int key, void* info);
static RBRNode * RBTreeInsertN(RBRTree*, RBRNode*);
static void RBTreePrint(RBRTree*);
static void RBDelete(RBRTree* , RBRNode* );
static void RBTreeDestroy(RBRTree*);
RBRNode* TreePredecessor(RBRTree*,RBRNode*);
RBRNode* TreeSuccessor(RBRTree*,RBRNode*);
static RBRNode* RBExactQuery(RBRTree*, int);
static stk_stack * RBEnumerate(RBRTree* tree,int low, int high);
static void NullFunction(void*);
static void InorderTreePrint(RBRTree* tree, RBRNode* x);

int verbose = 1;
inline int dprint(const char *format, ...){
  // return 0;
  // if(!verbose)return 0;
  va_list args;
  va_start(args, format);

  vprintf(format, args);

  va_end(args);
}

RBRNode::RBRNode(){
  set = 0;
  key = 0;
  info = 0;
  red = 0; /* if red=0 then the node is black */
  left = 0;
  right = 0;
  parent = 0;
  count = 1;
}


RBRNode* RBRNode::succ(int j){
  RBRNode *n = this;
  RBRNode *nc;
  int c;
  while(n){
    if(!j )return n;
    if(j>0){
      c = n->right->count; // entire right subtree
      if(j <= c){
        n = n->right;
        j -= n->left->count + 1;
      }else for(;;){
        nc = n;
        n = n->parent;
        if(n->parent == n->parent->parent){
          // we have hit the root.
          // no way to go higher.
          return NULL;
        }
        if(n->key >= nc->key){
          j -= 1 + c;
          break;
        }
      }
    }else{
      c = n->left->count; // entire left subtree
      if(j >= -c){
        n = n->left;
        j += n->right->count + 1;
      }else for(;;){
        nc = n;
        n = n->parent;
        if(n->parent == n->parent->parent){
          // we have hit the root.
          // no way to go higher.
          return NULL;
        }
        if(n->key <= nc->key){
          j += 1 + c;
          break;
        }
      }
    }
  }
  return NULL;
}

RBRTree::RBRTree(){ }
RBRTree::RBRTree(int size, RBRNode *data) {
  RBRNode* temp; 

  /*  see the comment in the RBRTree structure in red_black_tree.h */
  /*  for information on nil and root */

  RBRNode *all_nodes = data;
  nil=all_nodes;

  nil->parent=nil;
  nil->left=nil;
  nil->right=nil;
  nil->red=0;
  nil->key=-1;
  nil->set=1;
  nil->count = 0;

  root=(all_nodes+1);
  root->parent=nil;
  root->left=nil;
  root->right=nil;
  root->key=0;
  root->red=0;
  root->set=1;

  min=NULL;
  max=NULL;

  off = 0;
  pos = 0;


  this->n = size;
  this->data = (all_nodes+2);
}

void RBRTree::print(){
  for(int i=-2;i<n;++i){
    RBRNode *v =data+i;
    if(v->set){
      dprint("%d: (%d) -> %d [%d] -> (%d, %d)\n", i, (v->parent)?(v->parent->key):-1, v->key, v->count, (v->left)?(v->left->key):-1, (v->right)?(v->right->key):-1);
    }
  }
}

void RBRTree::printall(){
  InorderTreePrint(this, root);
}

bool RBRTree::insert(int v){
  RBTreeInsert(this, v, 0);
}

bool RBRTree::remove(int v){
  RBDelete(this, (data+v));
}

bool RBRTree::replace(int index, int v){
  RBReplace(this, index, v);
}

RBRNode* RBRTree::nth(int t){
  // returns the element n such that there
  // are (c/2) elements less than it (ie. the upper median)
  RBRNode *n = root->left;
  int a = 0;
  while(n){
    int l = n->left->count;
    int r = n->right->count;
    if(a+l==t)return n;
    if(a+l <t){
      a = a+l+1;
      n = n->right;
    }else{
      n = n->left;
    }
  }
  return NULL;
}

RBRNode* RBRTree::median(){
  // returns the element n such that there
  // are (c/2) elements less than it (ie. the upper median)
  RBRNode *n = root->left;
  int c = n->count;
  return nth(c/2);
}

/***********************************************************************/
/*  FUNCTION:  LeftRotate */
/**/
/*  INPUTS:  This takes a tree so that it can access the appropriate */
/*           root and nil pointers, and the node to rotate on. */
/**/
/*  OUTPUT:  None */
/**/
/*  Modifies Input: tree, x */
/**/
/*  EFFECTS:  Rotates as described in _Introduction_To_Algorithms by */
/*            Cormen, Leiserson, Rivest (Chapter 14).  Basically this */
/*            makes the parent of x be to the left of x, x the parent of */
/*            its parent before the rotation and fixes other pointers */
/*            accordingly. */
/***********************************************************************/

void LeftRotate(RBRTree* tree, RBRNode* x) {
  RBRNode* y;
  RBRNode* nil=tree->nil;

  /*  I originally wrote this function to use the sentinel for */
  /*  nil to avoid checking for nil.  However this introduces a */
  /*  very subtle bug because sometimes this function modifies */
  /*  the parent pointer of nil.  This can be a problem if a */
  /*  function which calls LeftRotate also uses the nil sentinel */
  /*  and expects the nil sentinel's parent pointer to be unchanged */
  /*  after calling this function.  For example, when RBDeleteFixUP */
  /*  calls LeftRotate it expects the parent pointer of nil to be */
  /*  unchanged. */

  y=x->right;
  x->right=y->left;
  x->count = x->count - y->count + x->right->count;

  if (y->left != nil) y->left->parent=x; /* used to use sentinel here */
  /* and do an unconditional assignment instead of testing for nil */
  
  y->parent=x->parent;   

  /* instead of checking if x->parent is the root as in the book, we */
  /* count on the root sentinel to implicitly take care of this case */
  if( x == x->parent->left) {
    x->parent->left=y;
  } else {
    x->parent->right=y;
  }
  y->count -= y->left->count;
  y->left=x;
  y->count += x->count;
  x->parent=y;



#ifdef DEBUG_ASSERT
  Assert(!tree->nil->red,"nil not red in LeftRotate");
#endif
}


/***********************************************************************/
/*  FUNCTION:  RighttRotate */
/**/
/*  INPUTS:  This takes a tree so that it can access the appropriate */
/*           root and nil pointers, and the node to rotate on. */
/**/
/*  OUTPUT:  None */
/**/
/*  Modifies Input?: tree, y */
/**/
/*  EFFECTS:  Rotates as described in _Introduction_To_Algorithms by */
/*            Cormen, Leiserson, Rivest (Chapter 14).  Basically this */
/*            makes the parent of x be to the left of x, x the parent of */
/*            its parent before the rotation and fixes other pointers */
/*            accordingly. */
/***********************************************************************/

void RightRotate(RBRTree* tree, RBRNode* y) {
  RBRNode* x;
  RBRNode* nil=tree->nil;

  /*  I originally wrote this function to use the sentinel for */
  /*  nil to avoid checking for nil.  However this introduces a */
  /*  very subtle bug because sometimes this function modifies */
  /*  the parent pointer of nil.  This can be a problem if a */
  /*  function which calls LeftRotate also uses the nil sentinel */
  /*  and expects the nil sentinel's parent pointer to be unchanged */
  /*  after calling this function.  For example, when RBDeleteFixUP */
  /*  calls LeftRotate it expects the parent pointer of nil to be */
  /*  unchanged. */

  x=y->left;
  y->count -= y->left->count;
  y->left=x->right;
  y->count += y->left->count;

  if (nil != x->right)  x->right->parent=y; /*used to use sentinel here */
  /* and do an unconditional assignment instead of testing for nil */

  /* instead of checking if x->parent is the root as in the book, we */
  /* count on the root sentinel to implicitly take care of this case */
  x->parent=y->parent;
  if( y == y->parent->left) {
    y->parent->left=x;
  } else {
    y->parent->right=x;
  }
  x->count -= x->right->count;
  x->right=y;
  x->count += x->right->count;
  y->parent=x;

#ifdef DEBUG_ASSERT
  Assert(!tree->nil->red,"nil not red in RightRotate");
#endif
}

/***********************************************************************/
/*  FUNCTION:  TreeInsertHelp  */
/**/
/*  INPUTS:  tree is the tree to insert into and z is the node to insert */
/**/
/*  OUTPUT:  none */
/**/
/*  Modifies Input:  tree, z */
/**/
/*  EFFECTS:  Inserts z into the tree as if it were a regular binary tree */
/*            using the algorithm described in _Introduction_To_Algorithms_ */
/*            by Cormen et al.  This function is only intended to be called */
/*            by the RBTreeInsert function and not by the user */
/***********************************************************************/

void TreeInsertHelp(RBRTree* tree, RBRNode* z) {
  /*  This function should only be called by InsertRBTree (see above) */
  RBRNode* x;
  RBRNode* y;
  RBRNode* nil=tree->nil;
  
  z->left=z->right=nil;
  y=tree->root;
  x=tree->root->left;
  while( x != nil) {
    x->count++;
    y=x;
    if (x->key > z->key) { /* x.key > z.key */
      x=x->left;
    } else { /* x,key <= z.key */
      x=x->right;
    }
  }
  z->parent=y;
  if ( (y == tree->root) ||
       (y->key > z->key)) { /* y.key > z.key */
    y->left=z;
  } else {
    y->right=z;
  }

#ifdef DEBUG_ASSERT
  Assert(!tree->nil->red,"nil not red in TreeInsertHelp");
#endif
}

/*  Before calling Insert RBTree the node x should have its key set */

/***********************************************************************/
/*  FUNCTION:  RBTreeInsert */
/**/
/*  INPUTS:  tree is the red-black tree to insert a node which has a key */
/*           pointed to by key and info pointed to by info.  */
/**/
/*  OUTPUT:  This function returns a pointer to the newly inserted node */
/*           which is guarunteed to be valid until this node is deleted. */
/*           What this means is if another data structure stores this */
/*           pointer then the tree does not need to be searched when this */
/*           is to be deleted. */
/**/
/*  Modifies Input: tree */
/**/
/*  EFFECTS:  Creates a node node which contains the appropriate key and */
/*            info pointers and inserts it into the tree. */
/***********************************************************************/

RBRNode * RBTreeInsertN(RBRTree* tree, RBRNode* x){

  if(!tree->max || (x->key > tree->max->key)){
    tree->max = x;
  }
  if(!tree->min || (x->key < tree->min->key)){
    tree->min = x;
  }

  x->set = 1;
  RBRNode * y;
  RBRNode * newNode;

  TreeInsertHelp(tree,x);
  newNode=x;
  x->red=1;
  while(x->parent->red) { /* use sentinel instead of checking for root */
    if (x->parent == x->parent->parent->left) {
      y=x->parent->parent->right;
      if (y->red) {
        x->parent->red=0;
        y->red=0;
        x->parent->parent->red=1;
        x=x->parent->parent;
      } else {
        if (x == x->parent->right) {
          x=x->parent;
          LeftRotate(tree,x);
        }
        x->parent->red=0;
        x->parent->parent->red=1;
        RightRotate(tree,x->parent->parent);
      } 
    } else { /* case for x->parent == x->parent->parent->right */
      y=x->parent->parent->left;
      if (y->red) {
        x->parent->red=0;
        y->red=0;
        x->parent->parent->red=1;
        x=x->parent->parent;
      } else {
        if (x == x->parent->left) {
          x=x->parent;
          RightRotate(tree,x);
        }
        x->parent->red=0;
        x->parent->parent->red=1;
        LeftRotate(tree,x->parent->parent);
      } 
    }
  }
  tree->root->left->red=0;
  return(newNode);

#ifdef DEBUG_ASSERT
  Assert(!tree->nil->red,"nil not red in RBTreeInsert");
  Assert(!tree->root->red,"root not red in RBTreeInsert");
#endif
}

bool RBTreeInsert(RBRTree* tree, int key, void* info) {

  RBRNode *set = NULL;
  for(int i=0;i<tree->n;++i){
    if(!tree->data[i].set){
      set = (tree->data + i);
      break;
    }
  }
  if(!set){
    return false;
  }

  set->key=key;
  set->info=info;
  RBTreeInsertN(tree, set);
  return true;
}

/***********************************************************************/
/*  FUNCTION:  TreeSuccessor  */
/**/
/*    INPUTS:  tree is the tree in question, and x is the node we want the */
/*             the successor of. */
/**/
/*    OUTPUT:  This function returns the successor of x or NULL if no */
/*             successor exists. */
/**/
/*    Modifies Input: none */
/**/
/*    Note:  uses the algorithm in _Introduction_To_Algorithms_ */
/***********************************************************************/
  
RBRNode* TreeSuccessor(RBRTree* tree,RBRNode* x) { 
  RBRNode* y;
  RBRNode* nil=tree->nil;
  RBRNode* root=tree->root;

  if (nil != (y = x->right)) { /* assignment to y is intentional */
    while(y->left != nil) { /* returns the minium of the right subtree of x */
    // dprint("%p | %p\n", y->left, y->nil);
      y=y->left;
    }
    return(y);
  } else {
    y=x->parent;
    while(x == y->right) { /* sentinel used instead of checking for nil */
      x=y;
      y=y->parent;
    }
    if (y == root) return(nil);
    return(y);
  }
}

/***********************************************************************/
/*  FUNCTION:  Treepredecessor  */
/**/
/*    INPUTS:  tree is the tree in question, and x is the node we want the */
/*             the predecessor of. */
/**/
/*    OUTPUT:  This function returns the predecessor of x or NULL if no */
/*             predecessor exists. */
/**/
/*    Modifies Input: none */
/**/
/*    Note:  uses the algorithm in _Introduction_To_Algorithms_ */
/***********************************************************************/

RBRNode* TreePredecessor(RBRTree* tree, RBRNode* x) {
  RBRNode* y;
  RBRNode* nil=tree->nil;
  RBRNode* root=tree->root;

  if (nil != (y = x->left)) { /* assignment to y is intentional */
    while(y->right != nil) { /* returns the maximum of the left subtree of x */
      y=y->right;
    }
    return(y);
  } else {
    y=x->parent;
    while(x == y->left) { 
      if (y == root) return(nil); 
      x=y;
      y=y->parent;
    }
    return(y);
  }
}

/***********************************************************************/
/*  FUNCTION:  InorderTreePrint */
/**/
/*    INPUTS:  tree is the tree to print and x is the current inorder node */
/**/
/*    OUTPUT:  none  */
/**/
/*    EFFECTS:  This function recursively prints the nodes of the tree */
/*              inorder using the PrintKey and PrintInfo functions. */
/**/
/*    Modifies Input: none */
/**/
/*    Note:    This function should only be called from RBTreePrint */
/***********************************************************************/

void InorderTreePrint(RBRTree* tree, RBRNode* x) {
  RBRNode* nil=tree->nil;
  RBRNode* root=tree->root;
  if (x != tree->nil) {
    dprint("");
    InorderTreePrint(tree,x->left);
    dprint(" %2d ",x->key);
    InorderTreePrint(tree,x->right);
    dprint("");
  }
}


/***********************************************************************/
/*  FUNCTION:  TreeDestHelper */
/**/
/*    INPUTS:  tree is the tree to destroy and x is the current node */
/**/
/*    OUTPUT:  none  */
/**/
/*    EFFECTS:  This function recursively destroys the nodes of the tree */
/*              postorder using the DestroyKey and DestroyInfo functions. */
/**/
/*    Modifies Input: tree, x */
/**/
/*    Note:    This function should only be called by RBTreeDestroy */
/***********************************************************************/

void TreeDestHelper(RBRTree* tree, RBRNode* x) {
  RBRNode* nil=tree->nil;
  if (x != nil) {
    TreeDestHelper(tree,x->left);
    TreeDestHelper(tree,x->right);
    // free(x);
  }
}


/***********************************************************************/
/*  FUNCTION:  RBTreeDestroy */
/**/
/*    INPUTS:  tree is the tree to destroy */
/**/
/*    OUTPUT:  none */
/**/
/*    EFFECT:  Destroys the key and frees memory */
/**/
/*    Modifies Input: tree */
/**/
/***********************************************************************/

void RBTreeDestroy(RBRTree* tree) {
  TreeDestHelper(tree,tree->root->left);
  // free(tree->root);
  // free(tree->nil);
  // free(tree);
}


/***********************************************************************/
/*  FUNCTION:  RBTreePrint */
/**/
/*    INPUTS:  tree is the tree to print */
/**/
/*    OUTPUT:  none */
/**/
/*    EFFECT:  This function recursively prints the nodes of the tree */
/*             inorder using the PrintKey and PrintInfo functions. */
/**/
/*    Modifies Input: none */
/**/
/***********************************************************************/

void RBTreePrint(RBRTree* tree) {
  InorderTreePrint(tree,tree->root->left);
}


/***********************************************************************/
/*  FUNCTION:  RBExactQuery */
/**/
/*    INPUTS:  tree is the tree to print and q is a pointer to the key */
/*             we are searching for */
/**/
/*    OUTPUT:  returns the a node with key equal to q.  If there are */
/*             multiple nodes with key equal to q this function returns */
/*             the one highest in the tree */
/**/
/*    Modifies Input: none */
/**/
/***********************************************************************/
  
RBRNode* RBExactQuery(RBRTree* tree, int q) {
  RBRNode* x=tree->root->left;
  RBRNode* nil=tree->nil;
  int compVal;
  if (x == nil) return(0);
  compVal=x->key - q;
  while(!compVal) {/*assignemnt*/
    if (compVal > 0) { /* x->key > q */
      x=x->left;
    } else {
      x=x->right;
    }
    if ( x == nil) return(0);
    compVal= x->key - q;
  }
  return(x);
}


/***********************************************************************/
/*  FUNCTION:  RBDeleteFixUp */
/**/
/*    INPUTS:  tree is the tree to fix and x is the child of the spliced */
/*             out node in RBTreeDelete. */
/**/
/*    OUTPUT:  none */
/**/
/*    EFFECT:  Performs rotations and changes colors to restore red-black */
/*             properties after a node is deleted */
/**/
/*    Modifies Input: tree, x */
/**/
/*    The algorithm from this function is from _Introduction_To_Algorithms_ */
/***********************************************************************/

void RBDeleteFixUp(RBRTree* tree, RBRNode* x) {
  RBRNode* root=tree->root->left;
  RBRNode* w;

  while( (!x->red) && (root != x)) {
    if (x == x->parent->left) {
      w=x->parent->right;
      if (w->red) {
	w->red=0;
	x->parent->red=1;
	LeftRotate(tree,x->parent);
	w=x->parent->right;
      }
      if ( (!w->right->red) && (!w->left->red) ) { 
	w->red=1;
	x=x->parent;
      } else {
	if (!w->right->red) {
	  w->left->red=0;
	  w->red=1;
	  RightRotate(tree,w);
	  w=x->parent->right;
	}
	w->red=x->parent->red;
	x->parent->red=0;
	w->right->red=0;
	LeftRotate(tree,x->parent);
	x=root; /* this is to exit while loop */
      }
    } else { /* the code below is has left and right switched from above */
      w=x->parent->left;
      if (w->red) {
	w->red=0;
	x->parent->red=1;
	RightRotate(tree,x->parent);
	w=x->parent->left;
      }
      if ( (!w->right->red) && (!w->left->red) ) { 
	w->red=1;
	x=x->parent;
      } else {
	if (!w->left->red) {
	  w->right->red=0;
	  w->red=1;
	  LeftRotate(tree,w);
	  w=x->parent->left;
	}
	w->red=x->parent->red;
	x->parent->red=0;
	w->left->red=0;
	RightRotate(tree,x->parent);
	x=root; /* this is to exit while loop */
      }
    }
  }
  x->red=0;

#ifdef DEBUG_ASSERT
  Assert(!tree->nil->red,"nil not black in RBDeleteFixUp");
#endif
}


/***********************************************************************/
/*  FUNCTION:  RBDelete */
/**/
/*    INPUTS:  tree is the tree to delete node z from */
/**/
/*    OUTPUT:  none */
/**/
/*    EFFECT:  Deletes z from tree and frees the key and info of z */
/*             using DestoryKey and DestoryInfo.  Then calls */
/*             RBDeleteFixUp to restore red-black properties */
/**/
/*    Modifies Input: tree, z */
/**/
/*    The algorithm from this function is from _Introduction_To_Algorithms_ */
/***********************************************************************/

void RBDelete(RBRTree* tree, RBRNode* z){
  z->set = 0;
  RBRNode* y;
  RBRNode* x;
  RBRNode* nil=tree->nil;
  RBRNode* root=tree->root;

  if(tree->max == z){
    tree->max = TreePredecessor(tree, z);
  }
  if(tree->min == z){
    tree->min = TreeSuccessor(tree, z);
  }

  y= ((z->left == nil) || (z->right == nil)) ? z : TreeSuccessor(tree,z);
  x= (y->left == nil) ? y->right : y->left;
  if (root == (x->parent = y->parent)) { /* assignment of y->p to x->p is intentional */
    root->left=x;
  } else {
    if (y == y->parent->left) {
      y->parent->count -= y->parent->left->count;
      y->parent->left=x;
      y->parent->count += y->parent->left->count;
    } else {
      y->parent->count -= y->parent->right->count;
      y->parent->right=x;
      y->parent->count += y->parent->right->count;
    }
  }
  if (y != z) { /* y should not be nil in this case */

#ifdef DEBUG_ASSERT
    Assert( (y!=tree->nil),"y is nil in RBDelete\n");
#endif
    /* y is the node to splice out and x is its child */

    if (!(y->red)) RBDeleteFixUp(tree,x);
  
    y->left=z->left;
    y->right=z->right;
    y->parent=z->parent;
    y->red=z->red;
    z->left->parent=z->right->parent=y;
    if (z == z->parent->left) {
      z->parent->left=y; 
    } else {
      z->parent->right=y;
    }
    y->count = y->left->count + y->right->count;
    y->parent->count = y->parent->left->count + y->parent->right->count;
  } else {
    if (!(y->red)) RBDeleteFixUp(tree,x);
  }
  
#ifdef DEBUG_ASSERT
  Assert(!tree->nil->red,"nil not black in RBDelete");
#endif
}

void RBReplace(RBRTree* tree, int index, int value){
  RBRNode *node = tree->data + index;
  RBDelete(tree, node);
  node->key = value;
  RBTreeInsertN(tree, node);
}


/***********************************************************************/
/*  FUNCTION:  RBDEnumerate */
/**/
/*    INPUTS:  tree is the tree to look for keys >= low */
/*             and <= high with respect to the Compare function */
/**/
/*    OUTPUT:  stack containing pointers to the nodes between [low,high] */
/**/
/*    Modifies Input: none */
/***********************************************************************/

stk_stack* RBEnumerate(RBRTree* tree, int low, int high) {
  stk_stack* enumResultStack;
  RBRNode* nil=tree->nil;
  RBRNode* x=tree->root->left;
  RBRNode* lastBest=nil;

  enumResultStack=StackCreate();
  while(nil != x) {
    if ( x->key > high) { /* x->key > high */
      x=x->left;
    } else {
      lastBest=x;
      x=x->right;
    }
  }
  while ( (lastBest != nil) && (low <= lastBest->key)) {
    StackPush(enumResultStack,lastBest);
    lastBest=TreePredecessor(tree,lastBest);
  }
  return(enumResultStack);
}
      
    
  
  



