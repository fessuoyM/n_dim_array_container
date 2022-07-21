#include <iostream>
#include <math.h>

using namespace std;

template <typename T>
struct Arr {
  private:
    T* arr;
    size_t size;
  public:
//~~~~Constructor and Destructors
    Arr(){this->size=0;}
    Arr(T* a, size_t n){this->arr = new T[n]; for(int i=0; i<n; i++){this->arr[i]=a[i];} this->size = n;}
    Arr(const Arr<T>& a){this->arr = new T[a.siz()]; this->size=a.siz(); for(int i=0; i<this->size; i++){this->arr[i]=a[i];}}
    Arr(size_t n){this->arr = new T[n]; this->size=n;}
    // ~Arr(){delete[] this->arr;}

//~~~~Access private members directly
    size_t siz() const{return this->size;}
    T* ptr() const{return this->arr;}

//~~~~Overloading access operators
    T& operator[](int ind){return this->arr[ind];}
    const T& operator[](int ind) const{return this->arr[ind];}

//~~~~Overloading copy operators
    // void operator=(const Arr<T> arr2){(*this) = new Arr<T>(arr2);}
    void operator=(const Arr<T> arr2){this->size=arr2.siz(); for(int i=0; i<this->size; i++){this->arr[i]=arr2[i];}}

//~~~~Overloading arithmatic operators
  //Addition
    void operator+=(Arr<T> arr2){for(int i=0; i<this->size; i++){this->arr[i]+=arr2[i];}}
    Arr<T> operator+(Arr<T> arr2){Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]+arr2[i];} return res;}
    void operator+=(T num){for(int i=0; i<this->size; i++){this->arr[i]+=num;}}
    Arr<T> operator+(T num){Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]+num;} return res;}
    friend Arr<T> operator+(T num, Arr<T> arr){Arr<T> res = arr; for(int i=0; i<arr.size; i++){res[i]=num+arr[i];} return res;}
  //Subtraction
    void operator-=(Arr<T> arr2){for(int i=0; i<this->size; i++){this->arr[i]-=arr2[i];}}
    Arr<T> operator-(Arr<T> arr2){Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]-arr2[i];} return res;}
    void operator-=(T num){for(int i=0; i<this->size; i++){this->arr[i]-=num;}}
    Arr<T> operator-(T num){Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]-num;} return res;}
    friend Arr<T> operator-(T num, Arr<T> arr){Arr<T> res = arr; for(int i=0; i<arr.size; i++){res[i]=num-arr[i];} return res;}
  //Multiply
    void operator*=(Arr<T> arr2){for(int i=0; i<this->size; i++){this->arr[i]*=arr2[i];}}
    Arr<T> operator*(Arr<T> arr2){Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]*arr2[i];} return res;}
    void operator*=(T num){for(int i=0; i<this->size; i++){this->arr[i]*=num;}}
    Arr<T> operator*(T num){Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]*num;} return res;}
    friend Arr<T> operator*(T num, Arr<T> arr){Arr<T> res = arr; for(int i=0; i<arr.size; i++){res[i]=num*arr[i];} return res;}
  //Divide
    void operator/=(Arr<T> arr2){for(int i=0; i<this->size; i++){this->arr[i]/=arr2[i];}}
    Arr<T> operator/(Arr<T> arr2){Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]/arr2[i];} return res;}
    void operator/=(T num){for(int i=0; i<this->size; i++){this->arr[i]/=num;}}
    Arr<T> operator/(T num){Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]/num;} return res;}
    friend Arr<T> operator/(T num, Arr<T> arr){Arr<T> res = arr; for(int i=0; i<arr.size; i++){res[i]=num/arr[i];} return res;}
  //Exp
    friend Arr<T> pow(Arr<T> arr, Arr<T> arr2){Arr<T> res = arr; for(int i=0; i<arr.size; i++){res[i]=pow(arr[i],arr2[i]);} return res;}
    friend Arr<T> pow(Arr<T> arr, T num){Arr<T> res = arr; for(int i=0; i<arr.size; i++){res[i]=pow(arr[i],num);}return res;}
  //Mod
    void operator%=(Arr<T> arr2){for(int i=0; i<this->size; i++){this->arr[i]%=arr2[i];}}
    Arr<T> operator%(Arr<T> arr2){Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]%arr2[i];} return res;}
    void operator%=(T num){for(int i=0; i<this->size; i++){this->arr[i]%=num;}}
    Arr<T> operator%(T num){Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]%num;} return res;}
    friend Arr<T> operator%(T num, Arr<T> arr){Arr<T> res = arr; for(int i=0; i<arr.size; i++){res[i]=num%arr[i];} return res;}

//~~~~Overloading comparrison operators for array return type
    Arr<bool> operator==(const Arr<T> arr2) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]==arr2[i];} return res;}
    Arr<bool> operator!() const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=!this->arr[i];} return res;}
    Arr<bool> operator!=(const Arr<T> arr2) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]!=arr2[i];} return res;}
    Arr<bool> operator>(const Arr<T> arr2) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]>arr2[i];} return res;}
    Arr<bool> operator>=(const Arr<T> arr2) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]>=arr2[i];} return res;}
    Arr<bool> operator<(const Arr<T> arr2) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]<arr2[i];} return res;}
    Arr<bool> operator<=(const Arr<T> arr2) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]<=arr2[i];} return res;}

    Arr<bool> operator==(const T num) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]==num;} return res;}
    Arr<bool> operator!=(const T num) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]!=num;} return res;}
    Arr<bool> operator>(const T num) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]>num;} return res;}
    Arr<bool> operator>=(const T num) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]>=num;} return res;}
    Arr<bool> operator<(const T num) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]<num;} return res;}
    Arr<bool> operator<=(const T num) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]<=num;} return res;}

//~~~~Case specific comparison operator overload
    template <bool>
    void operator&=(Arr<bool> arr2){for(int i=0; i<this->size; i++){this->arr[i]&&arr2[i];}}
    Arr<bool> operator&&(Arr<bool> arr2){Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]&&arr2[i];} return res;}
    void operator&=(bool num){for(int i=0; i<this->size; i++){this->arr[i]&&num;}}
    Arr<bool> operator&&(bool num){Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]&&num;} return res;}

    void operator|=(Arr<bool> arr2){for(int i=0; i<this->size; i++){this->arr[i]||arr2[i];}}
    Arr<bool> operator||(Arr<bool> arr2){Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]||arr2[i];} return res;}
    void operator|=(bool num){for(int i=0; i<this->size; i++){this->arr[i]||num;}}
    Arr<bool> operator||(bool num){Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]||num;} return res;}

//~~~~Overloading output operators
     friend ostream& operator<<(ostream& os, const Arr<T>& a){
       if (a.size>0) {
         os << "[";
         for(int i=0; i<a.size-1; i++){
           os << a[i] << ", ";
         }
         os << a[a.size-1] << "]";
       }else{
         os << "[]";
       }
       return os;
     }


    // const bool equals(const array arr2) const{
    //   if this->size!=arr2.size()
    //     throw invalid_argument("Arrays must have the same length, arr1_size: " + this->size + ", and arr2_size: " arr2.size());
    //   if (typeid(this-->arr).name()!=typeid(arrs.ptr()).name())
    //     throw invalid_argument("Arrays must have the same typr, arr1_size: " + typeid(this->arr).name + ", and arr2_size: " + typeid(arr2.ptr()).name());
    //   return this==arr2
    // }
};

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
