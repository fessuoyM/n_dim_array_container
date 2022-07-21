#include <iostream>
#include "Arr.h"

using namespace std;

template <typename T>
void testing (Arr<T> a){
  // cout << a.size() << endl;
  cout << a[1] << endl;
  // return b;
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
  for(int i=0; i<b.siz(); i++){b[i]=bb[i];}
  Arr<int> c = b+5;
  c=c+0;
  c[2]=100;
  cout << t << endl << b << endl << c << endl;
  cout << pow(8,2) << endl;
  cout << 10-(t+c*b+2+pow(b,2)) << endl;
  cout << (10-(t+c*b+2+pow(b,2))>t) << endl;
  cout << (t==t) << endl;
  Arr<bool> comp(5);
  comp=(t>b);
  cout << (b==5) << endl << (t>3) <<endl << ((b==5)&&(t>3)) << endl;

  // Arr<int> c = t+b;
  // cout << c << endl;
  // cout << t << endl << b <<endl;
  // cout << t + b << endl;
  // for(int i=0; i<ans.size(); i++){
  //   cout << ans[i] << ", ";
  // }
  // cout << endl;
}
