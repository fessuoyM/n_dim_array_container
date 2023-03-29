#include <iostream>
#include "Arr.h"

using namespace std;
using namespace chrono;

template <typename T>
void testing (Arr<T>& a){
  // cout << a.size() << endl;
  cout << a[1] << endl;
  // return b;
}

template <typename T>
void add (Arr<T>& a, Arr<T>& b){
  a+=b;
}

template <typename T>
Arr<T> add_r (Arr<T>& a, Arr<T>& b){
  Arr res = a+b;
  return res;
}

template <typename T>
void add (T *a, T *b, int siz){
  for (int i=0; i<siz; i++){
    a[i]+=b[i];
  }
}

template <typename T>
T* add_r (T *a, T *b, int siz){
  T c[siz];
  for (int i=0; i<siz; i++){
    c[i]=a[i]+b[i];
  }
  return move(&c[0]);
}


int main(){
  cout << "Hello World" << endl;
  int tt[5] = {1,2,5,4,3};
  Arr<int> t(tt, 5);
  // array<T, n> ans;
  testing(t);
  tt[1] = 10;
  testing(t);
  t[1] = 50;
  testing(t);
  int a = t[1];
  cout << a << endl;
  int bb[5] = {5,6,5,6,5};
  Arr<int> b(5);
  for(int i=0; i<b.size; i++){b[i]=bb[i];}
  Arr<int> c = b+5;
  cout << c << endl;
  c=c+0;
  cout << "test" << endl;
  c[2]=100;
  cout << "test" << endl;
  cout << endl << t << endl << b << endl << c << endl;
  cout << pow(8,2) << endl;
  cout << c*b << endl << t+c*b << endl << t*c*b+2 << endl << t+c*b+2+pow(b,2) << endl << 10-(t+c*b+2+pow(b,2)) << endl;
  cout << 10-(t+c*b+2+pow(b,2)) << endl;
  cout << (10-(t+c*b+2+pow(b,2))>t) << endl;
  cout << (t==t) << endl;
  Arr<bool> comp(5);
  comp=(t>b);
  cout << (b==5) << endl << (t>3) <<endl << ((b==5)&&(t>3)) << endl;

  cout << t << " | " << b << endl;
  t.Add(b);
  cout << t << endl;
  cout << Arr<int>::Sub(b, t) << endl;
  t.Sub(b);

  cout << endl << t << endl << b << endl;
  Arr<bool> comp2(5);
  comp2 = Arr<int>::Greater(t,b);
  comp2 = t.Greater(3);
  cout << comp2 << endl;
  cout << Arr<bool>::And(Arr<int>::Equals(b, 5), comp2) << endl;

  Arr<double> d(5);
  double dd[5] = {2.3,3.4,2.3,1.2,0.1};
  d=dd;
  cout << d << endl;

  cout << d+4 << endl << d+t << endl;
  d.Add(4);
  cout << d << endl;
  cout << Arr<double>::Add(d,t) << endl;
  cout << endl;
  t=d;
  cout<<t<<endl;

  // int d2[2][3] = {{1,2,3},{4,5,6}};
  // Arr<int> d2_(d2[0],6);
  // cout << endl << d2_ << endl;
  // cout << d2_[1] << endl << d2_ [1][2] << endl;

cout << endl << endl << endl;

  size_t N_array = 10000;
  size_t N_repeat = 10000;
  int num1 = 5, num2 = 3;
  Arr<int> t1 (N_array, num1);
  Arr<int> t2 (N_array, num2);
  int* t3 = new int[N_array];
  for(int i=0; i<N_array; i++){t3[i]=num1;}
  int* t4 = new int[N_array];
  for(int i=0; i<N_array; i++){t4[i]=num2;}
  

  Arr<int> r1(N_array);
  int r2[N_array];
  int* temp;

  auto t_start = high_resolution_clock::now();
  auto t_end = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;
  auto tempDur = duration;

  cout << endl << "N_array: " << N_array << "    |    N_repeat: " << N_repeat << endl;
  cout << endl << "          Operations Type               | Arr object  |  built-in Arr  |  Diffrence  |             " << endl;
  cout << endl << "----------------------------------------|-------------|----------------|-------------|-------------" << endl;

  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat; i++){
    t1+=t2;
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;
  cout << "overloaded operation in_place:          |   " << duration.count();
  tempDur=duration;

  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat; i++){
    for (int j=0; j<N_array; j++){
      t3[j]+=t4[j];
    }
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;
  cout << "    |     " << duration.count() << "     |    " << (tempDur-duration).count() << "    |   microseconds." << endl;


  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat; i++){
    add(t1, t2);
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;
  cout << "Passed to function in_place:            |   " << duration.count();
  tempDur=duration;

  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat; i++){
    add(t3, t4, N_array);
  }

  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;
  cout << "    |     " << duration.count() << "     |    " << (tempDur-duration).count() << "    |   microseconds." << endl;


  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat; i++){
    r1 = add_r(t1, t2);
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;
  cout << "Passed to function return:              |   " << duration.count();
  tempDur=duration;

  t_start = high_resolution_clock::now();
  for (int i=0; i<N_repeat; i++){
    temp = add_r(t3, t4, N_array);

    // for (int j=0; j<N_array; j++){
    //   r2[j]=temp[j];
    // }
  }
  t_end = high_resolution_clock::now();
  duration = duration_cast<microseconds>(t_end - t_start)/(double)N_repeat;
  cout << "    |     " << duration.count() << "     |    " << (tempDur-duration).count() << "    |   microseconds." << endl;


  cout << endl;
  int dim_arr[3][5] = {{2,2,3,2,1},{6,5,4,6,4},{66,7,7,9,7}};
  cout << endl;
  size_t shap[2] = {3,5};
  cout << endl;
  Arr<int, 2> dimArr(dim_arr[0],shap);
  cout << endl;
  cout << dimArr << endl;
  cout << dimArr[0]+5 << endl;
  cout << (dimArr+5) << endl;
  cout << (dimArr>4) << endl;
  dimArr[1]/=2.5;
  cout << dimArr << endl;

  Arr<double> dimArrD = dimArr[1];
  cout << dimArr << endl << dimArrD << endl;
  dimArrD[2] = 0.021;
  cout << dimArr << endl << dimArrD << endl;
} 