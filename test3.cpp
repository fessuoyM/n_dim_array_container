#include <iostream>
#include "Arr.h"

using namespace std;
using namespace chrono;

int main(){
  cout << "Hello World" << endl;
  int tt[5] = {1,2,5,4,1};
  Arr<int> t(tt, 5);
  cout << "built in arr:";
  for (int i=0; i<5; i++){cout << " " << tt[i];}
  cout << endl;
  cout << t << endl;

  cout << "2d Indexing: ";
  cout << t(4) << endl;


  size_t N_repeat = 10000;
  cout << endl <<"N_repeat: " << N_repeat << endl;

  auto t_start = high_resolution_clock::now();
  auto t_end = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;
  auto tempDur = duration;

  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat;){
    i+=t(4);
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;
  cout << "    " << 1000*duration.count();
  tempDur=duration;

  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat;){
    i+=tt[4];
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;

  cout << "    |     " << 1000*duration.count();
  
  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat;){
    i+=t[4];
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;

  cout << "    |     " << 1000*duration.count();

  int ccc[3][5] = {{1,2,3,4,5},{5,6,1,8,5},{1,3,6,9,5}};
  
  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat;){
    i+=ccc[1][2];
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;

  cout << "    |     " << 1000*duration.count();

  int f1[5] = {2,2,5,4,6};
  int f2[5] = {4,8,1,5,2};
  int f3[5] = {6,2,3,7,9};
  Arr<int> t1(f1, 5);
  Arr<int> t2(f2, 5);
  Arr<int> t3(f3, 5);
  Arr<int> ttt[3] = {t1,t2,t3};
  Arr<Arr<int>> tt2(ttt, 3);
  
  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat;){
    i+=tt2[1][2];
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;

  
  cout << "    |     " << 1000*duration.count();


  int dd[3][4] = {{1,2,3,4},{5,6,7,8},{1,3,6,9}};
  size_t shap[2] = {3,4};
  Arr<int,2> d2 (dd[0], shap);

t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat;){
    i+=d2[2][0];
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;

    cout << "    |     " << 1000*duration.count();

  int ddd[2][3][4] = {{{1,1,1,1},{1,1,1,1},{1,1,1,1}},{{2,2,2,2},{2,2,2,2},{2,2,2,2}}};
  size_t shap2[3] = {2,3,4};
  Arr<int,3> d3 (ddd[0][0], shap2);

  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat;){
    i+=d3[0][2][3];
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;

  cout << "     |    " << 1000*duration.count() << "    |   nanoseconds." << endl;


  cout << d2 << endl;
  cout << d3 << endl;
  cout << d2+d3[1] << endl;
  cout << d2+d2 << endl;

  d2=d2+d2;
  cout<<d2<<endl;
  int b[3] = {3,4,5};
  int c[3] = {1,2,3};
  Arr<int> bb(b, 3);
  Arr<int> cc(c, 3);
  cout << bb+cc << endl;
}