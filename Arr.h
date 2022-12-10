#include <iostream>
#include <math.h>

using namespace std;

template <typename T, typename v = typename enable_if<is_arithmetic<T>::value, bool>::type>
struct Arr {
  private:
    T* arr;
    size_t* shap;
    size_t nDim;
    size_t size;
  public:
//~~~~Constructor and Destructors
  //~~~~1D functions
    Arr():size(0), nDim{0}, shap{&size}{}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr(const T2* a, const size_t& n):size{n}, nDim{1}, shap{&size}{arr = new T[n]; for(int i=0; i<n; i++){arr[i]=a[i];}}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr(const size_t& n, const T2& num):size{n}, nDim{1}, shap{&size}{arr = new T[n]; for(int i=0; i<n; i++){arr[i]=num;}}
    Arr(const size_t& n):size{n}, nDim{1}, shap{&size}{arr = new T[n];}

  //~~~~nD functions
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr(const T2* a, const size_t* sh, const size_t& dims):nDim{dims}, size{1}{shap = new size_t[nDim]; for (int i=0; i<nDim; i++){shap[i] = sh[i]; size*=shap[i];} arr = new T[size]; for(int i=0; i<size; i++){arr[i]=a[i];}}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr(const size_t* sh, const size_t& dims, const T2& num):nDim{dims}, size{1}{shap = new size_t[nDim]; for (int i=0; i<nDim; i++){shap[i] = sh[i]; size*=shap[i];} arr = new T[size]; for(int i=0; i<size; i++){arr[i]=num;}}
    Arr(const size_t* sh, const size_t& dims):nDim{dims}, size{1}{shap = new size_t[nDim]; for (int i=0; i<nDim; i++){shap[i] = sh[i]; size*=shap[i];} arr = new T[size];}

  //~~~~Copy and move constructor
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr(const Arr<T2>& a):size{a.siz()}, nDim{a.nDim}, shap{a.shap}{arr = new T[a.siz()]; T2* p = a.ptr(); for(int i=0; i<size; i++){arr[i]=p[i];}}
    Arr(const Arr<T>& a):size{a.size}, nDim{a.nDim}, shap{a.shap}{arr = new T[a.size]; T* p = a.arr; for(int i=0; i<size; i++){arr[i]=p[i];}}
    Arr(Arr<T>&& a):size{a.size}, arr{a.arr}, nDim{a.nDim}, shap{a.shap}{a.arr = nullptr; a.size = 0; a.nDim=0; a.shap=&a.size;}


//~~~~Access private members directly
    size_t siz() const{return size;}
    size_t ndim() const{return ndim;}
    size_t shape() const{return shape;}
    T* ptr() const{return arr;}

//~~~~Overloading access operators
    T& operator[](const int& ind){return arr[ind];}
    const T& operator[](const int& ind) const{return arr[ind];}

//~~~~Overloading copy operators
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& operator=(const Arr<T2>& arr2){size=arr2.siz(); T2* p = arr2.ptr(); for(int i=0; i<size; i++){arr[i]=p[i];} return *this;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value || is_arithmetic<remove_pointer<T2> >::value, bool>::type>
    Arr<T>& operator=(const T2* arr2){for(int i=0; i<size; i++){arr[i]=arr2[i];} return *this;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& operator=(const T2& num){for(int i=0; i<size; i++){arr[i]=num;} return *this;}
    Arr<T>& operator=(Arr<T>&& arr2){delete arr; arr=arr2.arr; size=arr2.size; arr2.arr = nullptr; arr2.size = 0; return *this;}

//~~~~Overloading arithmatic operators
  //Addition
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& operator+=(const Arr<T2>& arr2){
      T2* p = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              arr[i] += p[i];
              arr[i+1] += p[i+1];
              arr[i+2] += p[i+2];
              arr[i+3] += p[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i] += p[i];
      }
      return *this;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator+(const Arr<T2>& arr2) const{
      Arr<T> res(size);
      T* p = res.ptr();
      T2* p2 = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]+p2[i];
              p[i+1]=arr[i+1]+p2[i+1];
              p[i+2]=arr[i+2]+p2[i+2];
              p[i+3]=arr[i+3]+p2[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]+p2[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& operator+=(const T2& num){
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              arr[i] += num;
              arr[i+1] += num;
              arr[i+2] += num;
              arr[i+3] += num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i] += num;
      }
      return *this;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator+(const T2& num) const{
      Arr<T> res(size);
      T* p = res.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]+num;
              p[i+1]=arr[i+1]+num;
              p[i+2]=arr[i+2]+num;
              p[i+3]=arr[i+3]+num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]+num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T> operator+(const T2& num, const Arr<T>& arr){
      Arr<T> res(arr.size);
      T* p = res.ptr();
      T* p2 = arr.ptr();
      if (arr.size>=4) {
          for (int i=0; i<arr.size; i += 4) {
              p[i]=p2[i]+num;
              p[i+1]=p2[i+1]+num;
              p[i+2]=p2[i+2]+num;
              p[i+3]=p2[i+3]+num;
          }
      }
      for (int i = (arr.size/4)*4; i<arr.size; ++i) {
          p[i]=arr[i]+num;
      }
      return res;
    }

  //Subtraction
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& operator-=(const Arr<T2>& arr2){
      T2* p = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              arr[i] -= p[i];
              arr[i+1] -= p[i+1];
              arr[i+2] -= p[i+2];
              arr[i+3] -= p[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i] -= p[i];
      }
      return *this;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator-(const Arr<T2>& arr2) const{
      Arr<T> res(size);
      T* p = res.ptr();
      T2* p2 = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]-p2[i];
              p[i+1]=arr[i+1]-p2[i+1];
              p[i+2]=arr[i+2]-p2[i+2];
              p[i+3]=arr[i+3]-p2[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]-p2[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& operator-=(const T2& num){
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              arr[i] -= num;
              arr[i+1] -= num;
              arr[i+2] -= num;
              arr[i+3] -= num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i] -= num;
      }
      return *this;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator-(const T2& num) const{
      Arr<T> res(size);
      T* p = res.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]-num;
              p[i+1]=arr[i+1]-num;
              p[i+2]=arr[i+2]-num;
              p[i+3]=arr[i+3]-num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]-num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T> operator-(const T2& num, const Arr<T>& arr){
      Arr<T> res(arr.size);
      T* p = res.ptr();
      T* p2 = arr.ptr();
      if (arr.size>=4) {
          for (int i=0; i<arr.size; i += 4) {
              p[i]=p2[i]-num;
              p[i+1]=p2[i+1]-num;
              p[i+2]=p2[i+2]-num;
              p[i+3]=p2[i+3]-num;
          }
      }
      for (int i = (arr.size/4)*4; i<arr.size; ++i) {
          p[i]=arr[i]-num;
      }
      return res;
    }

  //Multiply
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& operator*=(const Arr<T2>& arr2){
      T2* p = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              arr[i] *= p[i];
              arr[i+1] *= p[i+1];
              arr[i+2] *= p[i+2];
              arr[i+3] *= p[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i] *= p[i];
      }
      return *this;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator*(const Arr<T2>& arr2) const{
      Arr<T> res(size);
      T* p = res.ptr();
      T2* p2 = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]*p2[i];
              p[i+1]=arr[i+1]*p2[i+1];
              p[i+2]=arr[i+2]*p2[i+2];
              p[i+3]=arr[i+3]*p2[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]*p2[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& operator*=(const T2& num){
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              arr[i] *= num;
              arr[i+1] *= num;
              arr[i+2] *= num;
              arr[i+3] *= num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i] *= num;
      }
      return *this;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator*(const T2& num) const{
      Arr<T> res(size);
      T* p = res.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]*num;
              p[i+1]=arr[i+1]*num;
              p[i+2]=arr[i+2]*num;
              p[i+3]=arr[i+3]*num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]*num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T> operator*(const T2& num, const Arr<T>& arr){
      Arr<T> res(arr.size);
      T* p = res.ptr();
      T* p2 = arr.ptr();
      if (arr.size>=4) {
          for (int i=0; i<arr.size; i += 4) {
              p[i]=p2[i]*num;
              p[i+1]=p2[i+1]*num;
              p[i+2]=p2[i+2]*num;
              p[i+3]=p2[i+3]*num;
          }
      }
      for (int i = (arr.size/4)*4; i<arr.size; ++i) {
          p[i]=arr[i]*num;
      }
      return res;
    }

  //Divide
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& operator/=(const Arr<T2>& arr2){
      T2* p = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              arr[i] /= p[i];
              arr[i+1] /= p[i+1];
              arr[i+2] /= p[i+2];
              arr[i+3] /= p[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i] /= p[i];
      }
      return *this;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator/(const Arr<T2>& arr2) const{
      Arr<T> res(size);
      T* p = res.ptr();
      T2* p2 = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]/p2[i];
              p[i+1]=arr[i+1]/p2[i+1];
              p[i+2]=arr[i+2]/p2[i+2];
              p[i+3]=arr[i+3]/p2[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]/p2[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& operator/=(const T2& num){
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              arr[i] /= num;
              arr[i+1] /= num;
              arr[i+2] /= num;
              arr[i+3] /= num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i] /= num;
      }
      return *this;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator/(const T2& num) const{
      Arr<T> res(size);
      T* p = res.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]/num;
              p[i+1]=arr[i+1]/num;
              p[i+2]=arr[i+2]/num;
              p[i+3]=arr[i+3]/num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]/num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T> operator/(const T2& num, const Arr<T>& arr){
      Arr<T> res(arr.size);
      T* p = res.ptr();
      T* p2 = arr.ptr();
      if (arr.size>=4) {
          for (int i=0; i<arr.size; i += 4) {
              p[i]=p2[i]/num;
              p[i+1]=p2[i+1]/num;
              p[i+2]=p2[i+2]/num;
              p[i+3]=p2[i+3]/num;
          }
      }
      for (int i = (arr.size/4)*4; i<arr.size; ++i) {
          p[i]=arr[i]/num;
      }
      return res;
    }

  //Exp
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T> pow(const Arr<T>& arr1, const Arr<T2>& arr2){
      Arr<T> res(arr1.size);
      T* p = res.ptr();
      T2* p1 = arr1.ptr();
      T2* p2 = arr2.ptr();
      if (arr1.size>=4) {
          for (int i=0; i<arr1.size; i += 4) {
              p[i]=pow(p1[i],p2[i]);
              p[i+1]=pow(p1[i+1],p2[i+1]);
              p[i+2]=pow(p1[i+2],p2[i+2]);
              p[i+3]=pow(p1[i+3],p2[i+3]);
          }
      }
      for (int i = (arr1.size/4)*4; i<arr1.size; ++i) {
          p[i]=pow(p1[i],p2[i]);;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T> pow(const Arr<T>& arr1, const T2& num){
      Arr<T> res(arr1.size);
      T* p = res.ptr();
      T2* p1 = arr1.ptr();
      if (arr1.size>=4) {
          for (int i=0; i<arr1.size; i += 4) {
              p[i]=pow(p1[i],num);
              p[i+1]=pow(p1[i+1],num);
              p[i+2]=pow(p1[i+2],num);
              p[i+3]=pow(p1[i+3],num);
          }
      }
      for (int i = (arr1.size/4)*4; i<arr1.size; ++i) {
          p[i]=pow(p1[i],num);;
      }
      return res;
    }
  //Mod
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& operator%=(const Arr<T2>& arr2){
      T2* p = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              arr[i] %= p[i];
              arr[i+1] %= p[i+1];
              arr[i+2] %= p[i+2];
              arr[i+3] %= p[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i] %= p[i];
      }
      return *this;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator%(const Arr<T2>& arr2) const{
      Arr<T> res(size);
      T* p = res.ptr();
      T2* p2 = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]%p2[i];
              p[i+1]=arr[i+1]%p2[i+1];
              p[i+2]=arr[i+2]%p2[i+2];
              p[i+3]=arr[i+3]%p2[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]%p2[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& operator%=(const T2& num){
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              arr[i] %= num;
              arr[i+1] %= num;
              arr[i+2] %= num;
              arr[i+3] %= num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i] %= num;
      }
      return *this;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator%(const T2& num) const{
      Arr<T> res(size);
      T* p = res.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]%num;
              p[i+1]=arr[i+1]%num;
              p[i+2]=arr[i+2]%num;
              p[i+3]=arr[i+3]%num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]%num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T> operator%(const T2& num, const Arr<T>& arr){
      Arr<T> res(arr.size);
      T* p = res.ptr();
      T* p2 = arr.ptr();
      if (arr.size>=4) {
          for (int i=0; i<arr.size; i += 4) {
              p[i]=p2[i]%num;
              p[i+1]=p2[i+1]%num;
              p[i+2]=p2[i+2]%num;
              p[i+3]=p2[i+3]%num;
          }
      }
      for (int i = (arr.size/4)*4; i<arr.size; ++i) {
          p[i]=arr[i]%num;
      }
      return res;
    }


//~~~~Overloading comparrison operators for array return type
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator==(const Arr<T2>& arr2) const{
      Arr<bool> res(size);
      bool* p = res.ptr();
      T2* p2 = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]==p2[i];
              p[i+1]=arr[i+1]==p2[i+1];
              p[i+2]=arr[i+2]==p2[i+2];
              p[i+3]=arr[i+3]==p2[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]==p2[i];
      }
      return res;
    }
    Arr<bool> operator!() const{
      Arr<bool> res(size);
      bool* p = res.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=!arr[i];
              p[i+1]=!arr[i+1];
              p[i+2]=!arr[i+2];
              p[i+3]=!arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=!arr[i];
      }
      for(int i=0; i<size; i++){p[i]=!arr[i];}
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator!=(const Arr<T2>& arr2) const{
      Arr<bool> res(size);
      bool* p = res.ptr();
      T2* p2 = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]!=p2[i];
              p[i+1]=arr[i+1]!=p2[i+1];
              p[i+2]=arr[i+2]!=p2[i+2];
              p[i+3]=arr[i+3]!=p2[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]!=p2[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator>=(const Arr<T2>& arr2) const{
      Arr<bool> res(size);
      bool* p = res.ptr();
      T2* p2 = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]>=p2[i];
              p[i+1]=arr[i+1]>=p2[i+1];
              p[i+2]=arr[i+2]>=p2[i+2];
              p[i+3]=arr[i+3]>=p2[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]>=p2[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator>(const Arr<T2>& arr2) const{
      Arr<bool> res(size);
      bool* p = res.ptr();
      T2* p2 = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]>p2[i];
              p[i+1]=arr[i+1]>p2[i+1];
              p[i+2]=arr[i+2]>p2[i+2];
              p[i+3]=arr[i+3]>p2[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]>p2[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator<=(const Arr<T2>& arr2) const{
      Arr<bool> res(size);
      bool* p = res.ptr();
      T2* p2 = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]<=p2[i];
              p[i+1]=arr[i+1]<=p2[i+1];
              p[i+2]=arr[i+2]<=p2[i+2];
              p[i+3]=arr[i+3]<=p2[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]<=p2[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator<(const Arr<T2>& arr2) const{
      Arr<bool> res(size);
      bool* p = res.ptr();
      T2* p2 = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]<p2[i];
              p[i+1]=arr[i+1]<p2[i+1];
              p[i+2]=arr[i+2]<p2[i+2];
              p[i+3]=arr[i+3]<p2[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]<p2[i];
      }
      return res;
    }

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator==(const T2& num) const{
      Arr<bool> res(size);
      bool* p = res.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]==num;
              p[i+1]=arr[i+1]==num;
              p[i+2]=arr[i+2]==num;
              p[i+3]=arr[i+3]==num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]==num;
      }
      for(int i=0; i<size; i++){p[i]=arr[i]==num;}
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator!=(const T2& num) const{
      Arr<bool> res(size);
      bool* p = res.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]!=num;
              p[i+1]=arr[i+1]!=num;
              p[i+2]=arr[i+2]!=num;
              p[i+3]=arr[i+3]!=num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]!=num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator>=(const T2& num) const{
      Arr<bool> res(size);
      bool* p = res.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]>=num;
              p[i+1]=arr[i+1]>=num;
              p[i+2]=arr[i+2]>=num;
              p[i+3]=arr[i+3]>=num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]>=num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator>(const T2& num) const{
      Arr<bool> res(size);
      bool* p = res.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]>num;
              p[i+1]=arr[i+1]>num;
              p[i+2]=arr[i+2]>num;
              p[i+3]=arr[i+3]>num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]>num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator<=(const T2& num) const{
      Arr<bool> res(size);
      bool* p = res.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]<=num;
              p[i+1]=arr[i+1]<=num;
              p[i+2]=arr[i+2]<=num;
              p[i+3]=arr[i+3]<=num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]<=num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator<(const T2& num) const{
      Arr<bool> res(size);
      bool* p = res.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]<num;
              p[i+1]=arr[i+1]<num;
              p[i+2]=arr[i+2]<num;
              p[i+3]=arr[i+3]<num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]<num;
      }
      return res;
    }


//~~~~Safe functions for the overloaded operators
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& Add(const Arr<T2>& arr2){CheckCompatability((*this),arr2); return (*this)+=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& Sub(const Arr<T2>& arr2){CheckCompatability((*this),arr2); return (*this)-=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& Mult(const Arr<T2>& arr2){CheckCompatability((*this),arr2); return (*this)*=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& Div(const Arr<T2>& arr2){CheckCompatability((*this),arr2); return (*this)/=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& Mod(const Arr<T2>& arr2){CheckCompatability((*this),arr2); return (*this)%=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& Pow(const Arr<T2>& arr2){CheckCompatability((*this),arr2); return pow((*this),arr2);}

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> Equals(const Arr<T2>& arr2) const{CheckCompatability((*this),arr2); return (*this)==arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> NotEquals(const Arr<T2>& arr2) const{CheckCompatability((*this),arr2); return (*this)!=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> Greater(const Arr<T2>& arr2) const{CheckCompatability((*this),arr2); return (*this)>arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> GreaterEquals(const Arr<T2>& arr2) const{CheckCompatability((*this),arr2); return (*this)>=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> Less(const Arr<T2>& arr2) const{CheckCompatability((*this),arr2); return (*this)<arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> LessEquals(const Arr<T2>& arr2) const{CheckCompatability((*this),arr2); return (*this)<=arr2;}

//~~~~Safe Functions for number input
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& Add(const T2& num){CheckCompatability((*this),num); return (*this)+=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& Sub(const T2& num){CheckCompatability((*this),num); return (*this)-=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& Mult(const T2& num){CheckCompatability((*this),num); return (*this)*=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& Div(const T2& num){CheckCompatability((*this),num); return (*this)/=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& Mod(const T2& num){CheckCompatability((*this),num); return (*this)%=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T>& Pow(const T2& num){CheckCompatability((*this),num); return pow((*this),num);}

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> Equals(const T2& num) const{CheckCompatability((*this),num); return (*this)==num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> NotEquals(const T2& num) const{CheckCompatability((*this),num); return (*this)!=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> Greater(const T2& num) const{CheckCompatability((*this),num); return (*this)>num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> GreaterEquals(const T2& num) const{CheckCompatability((*this),num); return (*this)>=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> Less(const T2& num) const{CheckCompatability((*this),num); return (*this)<num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> LessEquals(const T2& num) const{CheckCompatability((*this),num); return (*this)<=num;}



//~~~~Safe Static functions for the overloaded operators
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Add(const Arr<T>& arr1, const Arr<T2>& arr2){CheckCompatability(arr1,arr2); return arr1+arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Sub(const Arr<T>& arr1, const Arr<T2>& arr2){CheckCompatability(arr1,arr2); return arr1-arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Mult(const Arr<T>& arr1, const Arr<T2>& arr2){CheckCompatability(arr1,arr2); return arr1*arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Div(const Arr<T>& arr1, const Arr<T2>& arr2){CheckCompatability(arr1,arr2); return arr1/arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Mod(const Arr<T>& arr1, const Arr<T2>& arr2){CheckCompatability(arr1,arr2); return arr1%arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Pow(const Arr<T>& arr1, const Arr<T2>& arr2){CheckCompatability(arr1,arr2); return pow(arr1,arr2);}

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> Equals(const Arr<T>& arr1, const Arr<T2>& arr2){CheckCompatability(arr1,arr2); return arr1==arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> NotEquals(const Arr<T>& arr1, const Arr<T2>& arr2){CheckCompatability(arr1,arr2); return arr1!=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> Greater(const Arr<T>& arr1, const Arr<T2>& arr2){CheckCompatability(arr1,arr2); return arr1>arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> GreaterEquals(const Arr<T>& arr1, const Arr<T2>& arr2){CheckCompatability(arr1,arr2); return arr1>=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> Less(const Arr<T>& arr1, const Arr<T2>& arr2){CheckCompatability(arr1,arr2); return arr1<arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> LessEquals(const Arr<T>& arr1, const Arr<T2>& arr2){CheckCompatability(arr1,arr2); return arr1<=arr2;}

//~~~~Static safe function for number input
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Add(const Arr<T>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1+num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Sub(const Arr<T>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1-num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Mult(const Arr<T>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1*num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Div(const Arr<T>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1/num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Mod(const Arr<T>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1%num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Pow(const Arr<T>& arr1, const T2& num){CheckCompatability(arr1,num); return pow(arr1,num);}

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> Equals(const Arr<T>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1==num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> NotEquals(const Arr<T>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1!=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> Greater(const Arr<T>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1>num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> GreaterEquals(const Arr<T>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1>=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> Less(const Arr<T>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1<num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> LessEquals(const Arr<T>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1<=num;}




//~~~~Case specific comparison operator overload
    template <bool>
    Arr<bool>& operator&=(const Arr<bool>& arr2){
      bool* p = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              arr[i]=arr[i]&&p[i];
              arr[i+1]=arr[i+1]&&p[i+1];
              arr[i+2]=arr[i+2]&&p[i+2];
              arr[i+3]=arr[i+3]&&p[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i]=arr[i]!=p[i];
      }
      return *this;
    }
    Arr<bool> operator&&(const Arr<bool>& arr2) const{
      Arr<T> res(size); T* p = res.ptr();
      bool* p2 = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]&&p2[i];
              p[i+1]=arr[i+1]&&p2[i+1];
              p[i+2]=arr[i+2]&&p2[i+2];
              p[i+3]=arr[i+3]&&p2[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]!=p2[i];
      }
      return res;
    }
    Arr<bool>& operator&=(const bool& num){
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              arr[i]=arr[i]&&num;
              arr[i+1]=arr[i+1]&&num;
              arr[i+2]=arr[i+2]&&num;
              arr[i+3]=arr[i+3]&&num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i]=arr[i]!=num;
      }
      return *this;
    }
    Arr<bool> operator&&(const bool& num) const{
      Arr<T> res(size);
      T* p = res.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]&&p[i];
              p[i+1]=arr[i+1]&&p[i+1];
              p[i+2]=arr[i+2]&&p[i+2];
              p[i+3]=arr[i+3]&&p[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]!=p[i];
      }
      return res;
    }

    template <bool>
    Arr<bool>& operator|=(const Arr<bool>& arr2){
      bool* p = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              arr[i]=arr[i]||p[i];
              arr[i+1]=arr[i+1]||p[i+1];
              arr[i+2]=arr[i+2]||p[i+2];
              arr[i+3]=arr[i+3]||p[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i]=arr[i]!=p[i];
      }
      return *this;
    }
    Arr<bool> operator||(const Arr<bool>& arr2) const{
      Arr<T> res(size); T* p = res.ptr();
      bool* p2 = arr2.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]||p2[i];
              p[i+1]=arr[i+1]||p2[i+1];
              p[i+2]=arr[i+2]||p2[i+2];
              p[i+3]=arr[i+3]||p2[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]!=p2[i];
      }
      return res;
    }
    Arr<bool>& operator|=(const bool& num){
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              arr[i]=arr[i]||num;
              arr[i+1]=arr[i+1]||num;
              arr[i+2]=arr[i+2]||num;
              arr[i+3]=arr[i+3]||num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i]=arr[i]!=num;
      }
      return *this;
    }
    Arr<bool> operator||(const bool& num) const{
      Arr<T> res(size);
      T* p = res.ptr();
      if (size>=4) {
          for (int i=0; i<size; i += 4) {
              p[i]=arr[i]||p[i];
              p[i+1]=arr[i+1]||p[i+1];
              p[i+2]=arr[i+2]||p[i+2];
              p[i+3]=arr[i+3]||p[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          p[i]=arr[i]!=p[i];
      }
      return res;
    }

//~~~~Safe boolean comparison
    Arr<bool>& And(const Arr<bool>& arr2){CheckCompatability((*this),arr2); return (*this)&=arr2;}
    Arr<bool>& Or(const Arr<bool>& arr2){CheckCompatability((*this),arr2); return (*this)|=arr2;}

    Arr<bool>& And(const bool& num){CheckCompatability((*this),num); return (*this)&=num;}
    Arr<bool>& Or(const bool& num){CheckCompatability((*this),num); return (*this)|=num;}

//~~~~Static safe boolean comparison
    static Arr<bool> And(const Arr<bool>& arr1, const Arr<bool>& arr2){CheckCompatability(arr1,arr2); return arr1&&arr2;}
    static Arr<bool> Or(const Arr<bool>& arr1, const Arr<bool>& arr2){CheckCompatability(arr1,arr2); return arr1||arr2;}

    static Arr<bool> And(const Arr<bool>& arr1, const bool& num){CheckCompatability(arr1,num); return arr1&&num;}
    static Arr<bool> Or(const Arr<bool>& arr1, const bool& num){CheckCompatability(arr1,num); return arr1||num;}

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

//~~~~Functions to check compatability
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static inline void CheckCompatability (const Arr<T>& arr1, const Arr<T2>& arr2){
      if (arr1.siz()!=arr2.siz())
        throw invalid_argument(string("Arrays must have the same length, arr1_size: ") + to_string(arr1.siz()) + string(", and arr2_size: ") + to_string(arr2.siz()));
      if (!is_convertible<T, T2>::value)
        throw invalid_argument(string("Arrays must have the same typr, arr1_size: ") + string(typeid(T).name()) + string(", and arr2_size: ") + string(typeid(T2).name()));
    }

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static inline void CheckCompatability (const Arr<T>& arr1, const T2& num){
      if (!is_convertible<T, T2>::value)
        throw invalid_argument(string("Arrays must have the same typr, arr1_size: ") + string(typeid(T).name()) + string(", and arr2_size: ") + string(typeid(T2).name()));
    }

};