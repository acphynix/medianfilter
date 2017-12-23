
#include <vector>
#include <stdio.h>
#include <cstdlib>

#include "red_black_tree.h"

template<int R>
void median_filter_3D(int *data, int *dims, int *out, int *fmin, int *fsiz, int *fmax){
  verbose = 0;
  setbuf(stdout, NULL);

  // ensure fmin + fsiz = fmax;
  for(int i=0;i<3;++i){
    if(fmin[i] + fsiz[i] != fmax[i]){
      fprintf(stderr,"malformed argument: fmin[%d] + fsiz[%d] =/= fmax[%d]\n",i,i,i);
      exit(0);
    }
  }
  
  // implement Perreault's algorithm using trees.
  const int N = R*2+1;
  RBRTree *frame = (RBRTree*)malloc(fsiz[0]*fsiz[1]*sizeof(RBRTree));
  std::vector<RBRTree*> window;

  RBRNode *all_nodes = new RBRNode[fsiz[0]*fsiz[1]*(N+2+1)];

  // 1. initialize histograms for entire frame.
  int ind = 0;
  for(int z=fmin[2];z<fmin[2]+N;++z){
    ind = 0;
    for(int y=fmin[1];y<fsiz[1];++y){
      for(int x=fmin[0];x<fsiz[0];++x){
        if(z == 0)frame[ind] = RBRTree(N, all_nodes);
        int v = data[z*dims[0]*dims[1] + y*dims[0] + x];
        frame[ind].insert(v);
        ++ind;
        all_nodes += N+3;
        // dprint("\n");
      }
    }
    // dprint("____\n");
  }
  verbose = 0;
  
  // 2. initialize histograms for window starting at top left corner.
  ind =0;
  for(int y=0;y<N;++y){
    for(int x=0;x<N;++x){
      window.push_back(frame+ind);
      ++ind;
    }
    ind += fsiz[0]-N;
  }

  // dprint("\n");
  // for(int i=0;i<N*N;++i){
    // dprint(">>%d: ",i);
    // window[i]->printall();
    // dprint("\n");
  // }
  
  // current point to be medianed
  int x = fmin[0] + R;
  int y = fmin[1] + R;
  int z = fmin[2] + R;
  
  ind = z*dims[0]*dims[1] + y*dims[0] + x;

  // direction of window's travel, x and y.
  int dir_x = 1;
  int dir_y = 1;

  // offset with which data is stored in window
  int off_x = 0;
  int off_y = 0;
  int off_z = 0;

  // position of window within frmae
  int win_x = 0;
  int win_y = 0;

  // depth of window relative to start
  int depth_z = 0;

  int total = (dims[0]-2*R)*(dims[1]-2*R)*(dims[2]-2*R);
  int iteration = 0;
  for(;;){
    // dprint("(%d %d %d %d);",x,y,z,ind);
    // for(int j=0;j<N*N;++j){
    //   window[j]->printall();
    // }
    verbose = 0;
    // dprint("index %d...\n", ind);
    // frame[89999].print();
    // int ix =0;
    // for(int j=0;j<N;++j){
    //   for(int k=0;k<N;++k){
    //     window[ix]->printall();
    //     dprint("\n");
    //     ++ix;
    //   }
    //   // dprint("\n");
    // }
    // dprint("\n");
    // verbose = 0;

    // dprint("(%d %d %d %d);",x,y,z,ind);
    // if(iteration >= 17670)verbose = 1;
    if(++iteration%1000 == 0){
      printf("Done: %d %.5f\n", iteration, (float)((float)iteration/total));
      // ++ind;
      // continue;
    }
    // dprint("offset: %d, %d\n",off_x,off_y);

    // dprint("calculating median\n");
    // verbose = 0;
    out[ind] = full_median<N*N>(window);
    // if(iteration > 9600)verbose = 1;

    ind += dir_x;
    x   += dir_x;

    if(x == fmax[0]-R || x == R-1){
      y += dir_y;
      if(y == fmax[1]-R || y == R-1){
        ++z;
        if(z == fmax[2]-R){
          // wrap x, y, z;
          break;
        }else{
          // dprint("Y\n");
          // wrap x, y; slide along z and prepare for next line.
          ind = ind + (fmax[0]*fmax[1]) - dir_x;
          x  -= dir_x;
          y  -= dir_y;

          dir_y = (dir_y==1)?-1:1;
          dir_x = (dir_x==1)?-1:1;

          ++depth_z;

          // move window down by updating each
          // of our trees.
          int to_ind, to_xx=off_x, to_yy=off_y;
          int fr_ind=ind-R-(R*dims[0])+(R*dims[0]*dims[1]), fr_xx=0, fr_yy=0;
          
          if(to_xx < 0)to_xx += N;
          if(to_xx >=N)to_xx -= N;
          if(to_yy < 0)to_yy += N;
          if(to_yy >=N)to_yy -= N;
          
          to_ind = to_xx + N*to_yy;

          int ii = 0;
          while(ii < N*N){
            // dprint("yo");
            // dprint("(%d, %d) -> %d\n", to_xx, to_yy, to_ind);
            // push the tree down to the next level
            // dprint("add data[%d] to window[%d]\n", fr_ind, to_ind);
            // dprint("set window[%d][%d] = data[%d]\n", to_ind, window[to_ind]->off, fr_ind);
            // vvvv
            // dprint("ff");
            window[to_ind]->replace(window[to_ind]->off, data[fr_ind]);
            // dprint("jj");
            window[to_ind]->off += 1;
            if(window[to_ind]->off >= N)window[to_ind]->off = 0; 
            window[to_ind]->pos = depth_z;

            // update indices
            ++to_ind;
            ++to_xx;
            ++fr_ind;
            ++fr_xx;
            if(fr_xx == N){
              // go to next line of data array
              fr_ind += dims[0]-fr_xx;
              fr_xx   = 0;
              ++fr_yy;

              to_ind += N;
              ++to_yy;
              // wrap with offset along y axis.
              if(fr_yy == N){
                fr_yy = 0;
                fr_ind -= N*(N-1);
              }
            }
            // wrap with offset along x and y axes.
            if(to_xx==N){
              to_ind -= to_xx;
              to_xx = 0;
            }
            if(to_yy==N){
              to_ind -= to_yy*(N);
              to_yy = 0;
            }
            // dprint("done\n");
            ++ii;
          }
        }
      }else{
        // dprint("X\n");
        // wrap x; slide along y and prepare for next line.
        ind = ind + ((dir_y==1)?fmax[0]:-fmax[0]) - dir_x;
        x  -= dir_x;
        dir_x = (dir_x==1)?-1:1;

        // simple y stutterstep. take the next N
        // trees along the Y axis of frame and put
        // them in window, being aware of the x and
        // y offsets and y_dir.
        int i=0, xx=0, yy=0, zz=0;
        int fr_ind = (win_y + ((dir_y==1)?N:-1))*fsiz[1] + win_x;
        int to_ind = (off_y + ((dir_y==1)?(0):(-1)) )*N + off_x;
        if(to_ind>=N*N)to_ind -= N*N;
        if(to_ind<0)to_ind += N*N;
        int to_xx  = off_x;

        if(to_xx < 0)to_xx += N;
        if(to_xx >=N)to_xx -= N;

        xx = x - R;
        yy = y + ((dir_y==1)?R:-R);
        zz = z + R;

        for(i=0;i<N;++i){
          
          // dprint("set window[%d] = frame[%d]\n",to_ind, fr_ind);
          // vvvv
          window[to_ind] = (frame+fr_ind);
          
          // push the tree down to the next level
          // if it is not already in sync.
          
          if(window[to_ind]->pos != depth_z){
            int next = zz*(dims[0]*dims[1]) + yy*(dims[0]) + (x - R + i);
            // dprint("window[%d]->replace(%d,%d);\n",to_ind,window[to_ind]->off, data[next]);
            window[to_ind]->replace(window[to_ind]->off, data[next]);
            window[to_ind]->off += 1;
            if(window[to_ind]->off >= N)window[to_ind]->off = 0; 
            window[to_ind]->pos = depth_z;
          }

          fr_ind += 1;
          to_ind += 1;
          to_xx  += 1;
          if(to_xx >= N){
            to_xx  -= N;
            to_ind -= N;
          }
        }
        // dprint("xx\n");
        off_y += dir_y;
        if(off_y==N)off_y=0;
        if(off_y<0)off_y+=N;
        win_y += dir_y;
      }
    }else{
      // dprint("C %d\n",dir_x);
      // dprint("win: %d %d\n",win_x, win_y);
      // no wrap
      // simple x stutterstep. take the next N
      // trees of frame and put them in window,
      // being aware of the values of the x and
      // y offsets and x_dir.
      // dprint("\n");
      int i=0, xx=0, yy=0, zz=0;
      int fr_ind = win_y*fsiz[0] + win_x + ((dir_x==1)?(N):(-1));
      int to_ind = off_y*N       + off_x + ((dir_x==1)?(0):(-1));

      if(to_ind>=N*N)to_ind -= N*N;
      if(to_ind<0)to_ind += N*N;

      xx = x + ((dir_x==1)?R:-R);
      yy = y - R;
      zz = z + R;

      for(i=0;i<N;++i){
        // dprint("set window[%d] = frame[%d]\n",to_ind, fr_ind);
        window[to_ind] = (frame+fr_ind);

        // push the tree down to the next level
        // if it is not already in sync.
        if(window[to_ind]->pos != depth_z){
          int next = zz*(dims[0]*dims[1]) + (y-R+i)*(dims[0]) + xx;
          window[to_ind]->replace(window[to_ind]->off, data[next]);
          window[to_ind]->off += 1;
          if(window[to_ind]->off >= N)window[to_ind]->off = 0; 
          window[to_ind]->pos = depth_z;
        }

        fr_ind += fsiz[0];
        to_ind += N;
        if(to_ind >= N*N)to_ind -= N*N;
      }
      off_x += dir_x;
      if(off_x==N)off_x=0;
      if(off_x<0)off_x+=N;

      win_x += dir_x;
      // dprint(".\n");
    }
    // dprint("VV");
  }
  // dprint("\n");
  free(frame);
}