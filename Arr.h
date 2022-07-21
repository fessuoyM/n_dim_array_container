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
