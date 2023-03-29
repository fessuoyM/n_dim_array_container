#include <iostream>
#include "Arr.h"

using namespace std;
using namespace chrono;

int main(){
  int ind1 = 3; 
  int ind2 = 2;
  int ind3 = 1;
  int num = 1;
  cout << "Hello World" << endl;
  int a1[5] = {num,2,4,num,1};
  Arr<int> aa1{a1, 5};
  cout << "built in arr:";
  for (int i=0; i<5; i++){cout << " " << a1[i];}
  cout << endl;
  cout << aa1 << endl;

  cout << "() Indexing: ";
  cout << aa1(4) << endl;


  size_t N_repeat = 100000;
  cout << endl <<"N_repeat: " << N_repeat << endl;
  cout << endl <<" built-in[] |   Arr []   |   Arr ()   |   Arr [] x |   Arr () x |   () vs [] |   units    " << endl;

  auto t_start = high_resolution_clock::now();
  auto t_end = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;
  auto tempDur = duration.count()*1000;
  auto tempDur2 = duration.count()*1000;


  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat;){
    i+=a1[ind1];
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;
  cout << "    " << 1000*duration.count();
  tempDur=duration.count()*1000;

  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat;){
    i+=aa1[ind1];
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;
  tempDur2=duration.count()*1000;

  cout << "    |     " << 1000*duration.count();
  
  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat;){
    i+=aa1(ind1);
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;

  cout << "    |     " << 1000*duration.count();
  cout << "    |     " << tempDur2/tempDur;
  cout << "    |     " << 1000*duration.count()/tempDur;
  cout << "    |     " << 1000*duration.count()/tempDur2 << "    |   nanoseconds." << endl;

  int a2[3][5] = {{num,2,3,4,5},{5,9,1,8,5},{1,3,6,num,5}};
  size_t shap2[2] = {3,5};
  Arr<int, 2> aa2{a2[0], shap2};
  
  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat;){
    i+=a2[ind2][ind1];
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;
  cout << "    " << 1000*duration.count();
  tempDur=duration.count()*1000;

  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat;){
    i+=aa2[ind2][ind1];
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;
  tempDur2=duration.count()*1000;

  cout << "    |     " << 1000*duration.count();
  
  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat;){
    i+=aa2(ind2, ind1);
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;

  cout << "    |     " << 1000*duration.count();
  cout << "    |     " << tempDur2/tempDur;
  cout << "    |     " << 1000*duration.count()/tempDur;
  cout << "    |     " << 1000*duration.count()/tempDur2 << "    |   nanoseconds." << endl;

  int a3[2][3][4] = {{{num,1,1,1},{1,1,1,1},{1,1,1,1}},{{2,2,2,2},{2,2,2,2},{2,2,2,num}}};
  size_t shap3[3] = {2,3,4};
  Arr<int,3> aa3{a3[0][0], shap3};

  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat;){
    i+=a3[ind3][ind2][ind1];
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;
  cout << "    " << 1000*duration.count();
  tempDur=duration.count()*1000;

  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat;){
    i+=aa3[ind3][ind2][ind1];
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;
  tempDur2=duration.count()*1000;

  cout << "    |     " << 1000*duration.count();
  
  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat;){
    i+=aa3(ind3, ind2, ind1);
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;

  cout << "    |     " << 1000*duration.count();
  cout << "    |     " << tempDur2/tempDur;
  cout << "    |     " << 1000*duration.count()/tempDur;
  cout << "    |     " << 1000*duration.count()/tempDur2 << "    |   nanoseconds." << endl;

  int* b1 = new int[4]{num, 1,1,1};
  int* b2 = new int[4]{1,1,1,1};
  int* b3 = new int[4]{1,1,1,1};
  int* b4 = new int[4]{2,2,2,2};
  int* b5 = new int[4]{2,2,2,2};
  int* b6 = new int[4]{2,2,2,num};
  int** bb1 = new int*[4]{b1, b2, b3};
  int** bb2 = new int*[4]{b4, b5, b6};
  int*** bbb1 = new int**[2]{bb1, bb2};


  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat;){
    i+=bbb1[ind3][ind2][ind1];
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;
  cout << "\n    " << 1000*duration.count() << endl;;

  cout << aa2(aa2<7) << endl;
}