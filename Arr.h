#include <iostream>
#include <math.h>

using namespace std;

template <typename T1, int nDim = 1, typename v = typename enable_if<is_arithmetic<T1>::value, bool>::type>
struct Arr {
//~~~~Define a type T for the underlying type of the array data using metaprogramming
  template<int N> struct getType{Arr<T1, N-1> typ;};
  template<>struct getType<1>{T1 typ;};
  using T = typename remove_reference<decltype(getType<nDim>::typ)>::type;

  private:
    T1* memoryBlock;
  public:
    mutable T* arr;
    mutable size_t* shap;
    mutable size_t size;
//~~~~Constructor and Destructors
  //~~~~1D functions
    Arr():arr{nullptr}, size(0), shap{&size}, memoryBlock{nullptr}{}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr(const T2* a, const size_t& n):size{n}, shap{&size}, memoryBlock{nullptr}{arr = new T[n]; for(int i=0; i<n; i++){arr[i]=a[i];}}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr(const size_t& n, const T2& num):size{n}, shap{&size}, memoryBlock{nullptr}{arr = new T[n]; for(int i=0; i<n; i++){arr[i]=num;}}
    Arr(const size_t& n):size{n}, shap{&size}, memoryBlock{nullptr}{arr = new T[n];}

  //~~~~nD functions
    template <typename T2>
    Arr(const T2* a, const size_t* sh):size{1}{
      shap = new size_t[nDim]; 
      for (int i=0; i<nDim; i++){
        shap[i] = sh[i]; size*=shap[i];
      } 
      memoryBlock = new T1[size];
      for(int i=0; i<size; i++){
        memoryBlock[i]=a[i];
      }
      arr = new T[shap[0]];
      for (int i=0; i<shap[0]; i++){
        arr[i] = T(&memoryBlock[i*size/sh[0]], &sh[1], nDim-1);
      }
    }

    template<typename T2>
    Arr(T2* a, const size_t* sh, const size_t dims):size{1}{
      shap = new size_t[nDim]; 
      for (int i=0; i<nDim; i++){
        shap[i] = sh[i]; size*=shap[i];
      }
      if constexpr(nDim==1){
        arr = a;
      }else{
        arr = new T[shap[0]];
        for (int i=0; i<shap[0]; i++){
          arr[i] = T(&a[i*size/sh[0]], &sh[1], dims-1);
        }
      }
    }

    // template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    // Arr(const size_t* sh, const size_t& dims, const T2& num):size{1}{shap = new size_t[nDim]; for (int i=0; i<nDim; i++){shap[i] = sh[i]; size*=shap[i];} arr = new T[size]; for(int i=0; i<size; i++){arr[i]=num;}}
    // Arr(const size_t* sh, const size_t& dims):size{1}{shap = new size_t[nDim]; for (int i=0; i<nDim; i++){shap[i] = sh[i]; size*=shap[i];} arr = new T[size];}

  //~~~~Destructor
    ~Arr(){
      free(memoryBlock);
      delete[] arr;
      if constexpr(nDim > 1){
        delete[] shap;
      }
    }

  //~~~~Copy and move constructor
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr(const Arr<T2, nDim>& a):size{a.size}, shap{a.shap}{arr = new T[a.size]; for(int i=0; i<size; i++){arr[i]=a.arr[i];}}
    Arr(const Arr<T1, nDim>& a):size{a.size}, shap{a.shap}{arr = new T[a.size]; for(int i=0; i<size; i++){arr[i]=a.arr[i];}}
    Arr(Arr<T1, nDim>&& a):size{a.size}, arr{a.arr}, shap{a.shap}{a.arr = nullptr; a.size = 0; a.shap=&a.size;}


// //~~~~Access private members directly
//     size_t siz() const{return size;}
//     size_t shap() const{return shap;}
//     T* ptr() const{return arr;}

//~~~~Overloading access operators
    T& operator[](const int& ind){return arr[ind];}
    const T& operator[](const int& ind) const{return arr[ind];}

    template<typename... Ints, typename = enable_if_t<conjunction<is_same<Ints, int>...>::value>>
    T& operator()(Ints... inds){
      int ind[] = {forward<Ints>(inds)...};
      int multiplier=1, offset=0;
      for (int i=sizeof...(inds)-1; i>=0; i--){
        offset+=multiplier*ind[i];
        multiplier*=shap[i];
      }
      return memoryBlock[offset];
    }

//~~~~Overloading copy operators
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& operator=(const Arr<T2, nDim>& arr2){size=arr2.size; for(int i=0; i<size; i++){arr[i]=arr2.arr[i];} return *this;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value || is_arithmetic<remove_pointer<T2> >::value, bool>::type>
    Arr<T1, nDim>& operator=(const T2* arr2){for(int i=0; i<size; i++){arr[i]=arr2[i];} return *this;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& operator=(const T2& num){for(int i=0; i<size; i++){arr[i]=num;} return *this;}
    Arr<T1, nDim>& operator=(Arr<T1, nDim>&& arr2){delete arr; arr=arr2.arr; size=arr2.size; arr2.arr = nullptr; arr2.size = 0; return *this;}

//~~~~Overloading arithmatic operators
  //Addition
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& operator+=(const Arr<T2, nDim>& arr2){
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              arr[i] += arr2.arr[i];
              arr[i+1] += arr2.arr[i+1];
              arr[i+2] += arr2.arr[i+2];
              arr[i+3] += arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i] += arr2.arr[i];
      }
      return *this;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim> operator+(const Arr<T2, nDim>& arr2) const{
      Arr<T1, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]+arr2.arr[i];
              res.arr[i+1]=arr[i+1]+arr2.arr[i+1];
              res.arr[i+2]=arr[i+2]+arr2.arr[i+2];
              res.arr[i+3]=arr[i+3]+arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]+arr2.arr[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& operator+=(const T2& num){
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
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
    Arr<T1, nDim> operator+(const T2& num) const{
      Arr<T1, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]+num;
              res.arr[i+1]=arr[i+1]+num;
              res.arr[i+2]=arr[i+2]+num;
              res.arr[i+3]=arr[i+3]+num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]+num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T1, nDim> operator+(const T2& num, const Arr<T1, nDim>& arr){
      Arr<T1, nDim> res(arr.size);
      if (arr.size>=4) {
          for (int i=0; i<arr.size; i += 4) {
              res.arr[i]=arr.arr[i]+num;
              res.arr[i+1]=arr.arr[i+1]+num;
              res.arr[i+2]=arr.arr[i+2]+num;
              res.arr[i+3]=arr.arr[i+3]+num;
          }
      }
      for (int i = (arr.size/4)*4; i<arr.size; ++i) {
          res.arr[i]=arr[i]+num;
      }
      return res;
    }

  //Subtraction
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& operator-=(const Arr<T2, nDim>& arr2){
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              arr[i] -= arr2.arr[i];
              arr[i+1] -= arr2.arr[i+1];
              arr[i+2] -= arr2.arr[i+2];
              arr[i+3] -= arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i] -= arr2.arr[i];
      }
      return *this;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim> operator-(const Arr<T2, nDim>& arr2) const{
      Arr<T1, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]-arr2.arr[i];
              res.arr[i+1]=arr[i+1]-arr2.arr[i+1];
              res.arr[i+2]=arr[i+2]-arr2.arr[i+2];
              res.arr[i+3]=arr[i+3]-arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]-arr2.arr[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& operator-=(const T2& num){
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
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
    Arr<T1, nDim> operator-(const T2& num) const{
      Arr<T1, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]-num;
              res.arr[i+1]=arr[i+1]-num;
              res.arr[i+2]=arr[i+2]-num;
              res.arr[i+3]=arr[i+3]-num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]-num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T1, nDim> operator-(const T2& num, const Arr<T1, nDim>& arr){
      Arr<T1, nDim> res(arr.size);
      if (arr.size>=4) {
          for (int i=0; i<arr.size; i += 4) {
              res.arr[i]=arr.arr[i]-num;
              res.arr[i+1]=arr.arr[i+1]-num;
              res.arr[i+2]=arr.arr[i+2]-num;
              res.arr[i+3]=arr.arr[i+3]-num;
          }
      }
      for (int i = (arr.size/4)*4; i<arr.size; ++i) {
          res.arr[i]=arr[i]-num;
      }
      return res;
    }

  //Multiply
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& operator*=(const Arr<T2, nDim>& arr2){
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              arr[i] *= arr2.arr[i];
              arr[i+1] *= arr2.arr[i+1];
              arr[i+2] *= arr2.arr[i+2];
              arr[i+3] *= arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i] *= arr2.arr[i];
      }
      return *this;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim> operator*(const Arr<T2, nDim>& arr2) const{
      Arr<T1, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]*arr2.arr[i];
              res.arr[i+1]=arr[i+1]*arr2.arr[i+1];
              res.arr[i+2]=arr[i+2]*arr2.arr[i+2];
              res.arr[i+3]=arr[i+3]*arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]*arr2.arr[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& operator*=(const T2& num){
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
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
    Arr<T1, nDim> operator*(const T2& num) const{
      Arr<T1, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]*num;
              res.arr[i+1]=arr[i+1]*num;
              res.arr[i+2]=arr[i+2]*num;
              res.arr[i+3]=arr[i+3]*num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]*num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T1, nDim> operator*(const T2& num, const Arr<T1, nDim>& arr){
      Arr<T1, nDim> res(arr.size);
      if (arr.size>=4) {
          for (int i=0; i<arr.size; i += 4) {
              res.arr[i]=arr.arr[i]*num;
              res.arr[i+1]=arr.arr[i+1]*num;
              res.arr[i+2]=arr.arr[i+2]*num;
              res.arr[i+3]=arr.arr[i+3]*num;
          }
      }
      for (int i = (arr.size/4)*4; i<arr.size; ++i) {
          res.arr[i]=arr[i]*num;
      }
      return res;
    }

  //Divide
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& operator/=(const Arr<T2, nDim>& arr2){
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              arr[i] /= arr2.arr[i];
              arr[i+1] /= arr2.arr[i+1];
              arr[i+2] /= arr2.arr[i+2];
              arr[i+3] /= arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i] /= arr2.arr[i];
      }
      return *this;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim> operator/(const Arr<T2, nDim>& arr2) const{
      Arr<T1, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]/arr2.arr[i];
              res.arr[i+1]=arr[i+1]/arr2.arr[i+1];
              res.arr[i+2]=arr[i+2]/arr2.arr[i+2];
              res.arr[i+3]=arr[i+3]/arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]/arr2.arr[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& operator/=(const T2& num){
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
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
    Arr<T1, nDim> operator/(const T2& num) const{
      Arr<T1, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]/num;
              res.arr[i+1]=arr[i+1]/num;
              res.arr[i+2]=arr[i+2]/num;
              res.arr[i+3]=arr[i+3]/num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]/num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T1, nDim> operator/(const T2& num, const Arr<T1, nDim>& arr){
      Arr<T1, nDim> res(arr.size);
      if (arr.size>=4) {
          for (int i=0; i<arr.size; i += 4) {
              res.arr[i]=arr.arr[i]/num;
              res.arr[i+1]=arr.arr[i+1]/num;
              res.arr[i+2]=arr.arr[i+2]/num;
              res.arr[i+3]=arr.arr[i+3]/num;
          }
      }
      for (int i = (arr.size/4)*4; i<arr.size; ++i) {
          res.arr[i]=arr[i]/num;
      }
      return res;
    }

  //Exp
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T1, nDim> pow(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){
      Arr<T1, nDim> res(arr1.size);
      if (arr1.size>=4) {
          for (int i=0; i<arr1.size; i += 4) {
              res.arr[i]=pow(arr1.arr[i],arr2.arr[i]);
              res.arr[i+1]=pow(arr1.arr[i+1],arr2.arr[i+1]);
              res.arr[i+2]=pow(arr1.arr[i+2],arr2.arr[i+2]);
              res.arr[i+3]=pow(arr1.arr[i+3],arr2.arr[i+3]);
          }
      }
      for (int i = (arr1.size/4)*4; i<arr1.size; ++i) {
          res.arr[i]=pow(arr1.arr[i],arr2.arr[i]);;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T1, nDim> pow(const Arr<T1, nDim>& arr1, const T2& num){
      Arr<T1, nDim> res(arr1.size);
      if (arr1.size>=4) {
          for (int i=0; i<arr1.size; i += 4) {
              res.arr[i]=pow(arr1.arr[i],num);
              res.arr[i+1]=pow(arr1.arr[i+1],num);
              res.arr[i+2]=pow(arr1.arr[i+2],num);
              res.arr[i+3]=pow(arr1.arr[i+3],num);
          }
      }
      for (int i = (arr1.size/4)*4; i<arr1.size; ++i) {
          res.arr[i]=pow(arr1.arr[i],num);;
      }
      return res;
    }
  //Mod
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& operator%=(const Arr<T2, nDim>& arr2){
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              arr[i] %= arr2.arr[i];
              arr[i+1] %= arr2.arr[i+1];
              arr[i+2] %= arr2.arr[i+2];
              arr[i+3] %= arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i] %= arr2.arr[i];
      }
      return *this;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim> operator%(const Arr<T2, nDim>& arr2) const{
      Arr<T1, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]%arr2.arr[i];
              res.arr[i+1]=arr[i+1]%arr2.arr[i+1];
              res.arr[i+2]=arr[i+2]%arr2.arr[i+2];
              res.arr[i+3]=arr[i+3]%arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]%arr2.arr[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& operator%=(const T2& num){
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
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
    Arr<T1, nDim> operator%(const T2& num) const{
      Arr<T1, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]%num;
              res.arr[i+1]=arr[i+1]%num;
              res.arr[i+2]=arr[i+2]%num;
              res.arr[i+3]=arr[i+3]%num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]%num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T1, nDim> operator%(const T2& num, const Arr<T1, nDim>& arr){
      Arr<T1, nDim> res(arr.size);
      if (arr.size>=4) {
          for (int i=0; i<arr.size; i += 4) {
              res.arr[i]=arr.arr[i]%num;
              res.arr[i+1]=arr.arr[i+1]%num;
              res.arr[i+2]=arr.arr[i+2]%num;
              res.arr[i+3]=arr.arr[i+3]%num;
          }
      }
      for (int i = (arr.size/4)*4; i<arr.size; ++i) {
          res.arr[i]=arr[i]%num;
      }
      return res;
    }


//~~~~Overloading comparrison operators for array return type
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> operator==(const Arr<T2, nDim>& arr2) const{
      Arr<bool, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]==arr2.arr[i];
              res.arr[i+1]=arr[i+1]==arr2.arr[i+1];
              res.arr[i+2]=arr[i+2]==arr2.arr[i+2];
              res.arr[i+3]=arr[i+3]==arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]==arr2.arr[i];
      }
      return res;
    }
    Arr<bool, nDim> operator!() const{
      Arr<bool, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=!arr[i];
              res.arr[i+1]=!arr[i+1];
              res.arr[i+2]=!arr[i+2];
              res.arr[i+3]=!arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=!arr[i];
      }
      for(int i=0; i<size; i++){res.arr[i]=!arr[i];}
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> operator!=(const Arr<T2, nDim>& arr2) const{
      Arr<bool, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]!=arr2.arr[i];
              res.arr[i+1]=arr[i+1]!=arr2.arr[i+1];
              res.arr[i+2]=arr[i+2]!=arr2.arr[i+2];
              res.arr[i+3]=arr[i+3]!=arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]!=arr2.arr[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> operator>=(const Arr<T2, nDim>& arr2) const{
      Arr<bool, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]>=arr2.arr[i];
              res.arr[i+1]=arr[i+1]>=arr2.arr[i+1];
              res.arr[i+2]=arr[i+2]>=arr2.arr[i+2];
              res.arr[i+3]=arr[i+3]>=arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]>=arr2.arr[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> operator>(const Arr<T2, nDim>& arr2) const{
      Arr<bool, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]>arr2.arr[i];
              res.arr[i+1]=arr[i+1]>arr2.arr[i+1];
              res.arr[i+2]=arr[i+2]>arr2.arr[i+2];
              res.arr[i+3]=arr[i+3]>arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]>arr2.arr[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> operator<=(const Arr<T2, nDim>& arr2) const{
      Arr<bool, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]<=arr2.arr[i];
              res.arr[i+1]=arr[i+1]<=arr2.arr[i+1];
              res.arr[i+2]=arr[i+2]<=arr2.arr[i+2];
              res.arr[i+3]=arr[i+3]<=arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]<=arr2.arr[i];
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> operator<(const Arr<T2, nDim>& arr2) const{
      Arr<bool, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]<arr2.arr[i];
              res.arr[i+1]=arr[i+1]<arr2.arr[i+1];
              res.arr[i+2]=arr[i+2]<arr2.arr[i+2];
              res.arr[i+3]=arr[i+3]<arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]<arr2.arr[i];
      }
      return res;
    }

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> operator==(const T2& num) const{
      Arr<bool, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]==num;
              res.arr[i+1]=arr[i+1]==num;
              res.arr[i+2]=arr[i+2]==num;
              res.arr[i+3]=arr[i+3]==num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]==num;
      }
      for(int i=0; i<size; i++){res.arr[i]=arr[i]==num;}
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> operator!=(const T2& num) const{
      Arr<bool, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]!=num;
              res.arr[i+1]=arr[i+1]!=num;
              res.arr[i+2]=arr[i+2]!=num;
              res.arr[i+3]=arr[i+3]!=num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]!=num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> operator>=(const T2& num) const{
      Arr<bool, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]>=num;
              res.arr[i+1]=arr[i+1]>=num;
              res.arr[i+2]=arr[i+2]>=num;
              res.arr[i+3]=arr[i+3]>=num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]>=num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> operator>(const T2& num) const{
      Arr<bool, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]>num;
              res.arr[i+1]=arr[i+1]>num;
              res.arr[i+2]=arr[i+2]>num;
              res.arr[i+3]=arr[i+3]>num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]>num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> operator<=(const T2& num) const{
      Arr<bool, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]<=num;
              res.arr[i+1]=arr[i+1]<=num;
              res.arr[i+2]=arr[i+2]<=num;
              res.arr[i+3]=arr[i+3]<=num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]<=num;
      }
      return res;
    }
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> operator<(const T2& num) const{
      Arr<bool, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]<num;
              res.arr[i+1]=arr[i+1]<num;
              res.arr[i+2]=arr[i+2]<num;
              res.arr[i+3]=arr[i+3]<num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]<num;
      }
      return res;
    }


//~~~~Safe functions for the overloaded operators
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& Add(const Arr<T2, nDim>& arr2){CheckCompatability((*this),arr2); return (*this)+=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& Sub(const Arr<T2, nDim>& arr2){CheckCompatability((*this),arr2); return (*this)-=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& Mult(const Arr<T2, nDim>& arr2){CheckCompatability((*this),arr2); return (*this)*=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& Div(const Arr<T2, nDim>& arr2){CheckCompatability((*this),arr2); return (*this)/=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& Mod(const Arr<T2, nDim>& arr2){CheckCompatability((*this),arr2); return (*this)%=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& Pow(const Arr<T2, nDim>& arr2){CheckCompatability((*this),arr2); return pow((*this),arr2);}

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> Equals(const Arr<T2, nDim>& arr2) const{CheckCompatability((*this),arr2); return (*this)==arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> NotEquals(const Arr<T2, nDim>& arr2) const{CheckCompatability((*this),arr2); return (*this)!=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> Greater(const Arr<T2, nDim>& arr2) const{CheckCompatability((*this),arr2); return (*this)>arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> GreaterEquals(const Arr<T2, nDim>& arr2) const{CheckCompatability((*this),arr2); return (*this)>=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> Less(const Arr<T2, nDim>& arr2) const{CheckCompatability((*this),arr2); return (*this)<arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> LessEquals(const Arr<T2, nDim>& arr2) const{CheckCompatability((*this),arr2); return (*this)<=arr2;}

//~~~~Safe Functions for number input
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& Add(const T2& num){CheckCompatability((*this),num); return (*this)+=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& Sub(const T2& num){CheckCompatability((*this),num); return (*this)-=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& Mult(const T2& num){CheckCompatability((*this),num); return (*this)*=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& Div(const T2& num){CheckCompatability((*this),num); return (*this)/=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& Mod(const T2& num){CheckCompatability((*this),num); return (*this)%=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T1, nDim>& Pow(const T2& num){CheckCompatability((*this),num); return pow((*this),num);}

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> Equals(const T2& num) const{CheckCompatability((*this),num); return (*this)==num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> NotEquals(const T2& num) const{CheckCompatability((*this),num); return (*this)!=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> Greater(const T2& num) const{CheckCompatability((*this),num); return (*this)>num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> GreaterEquals(const T2& num) const{CheckCompatability((*this),num); return (*this)>=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> Less(const T2& num) const{CheckCompatability((*this),num); return (*this)<num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool, nDim> LessEquals(const T2& num) const{CheckCompatability((*this),num); return (*this)<=num;}



//~~~~Safe Static functions for the overloaded operators
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T1, nDim> Add(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1+arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T1, nDim> Sub(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1-arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T1, nDim> Mult(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1*arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T1, nDim> Div(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1/arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T1, nDim> Mod(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1%arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T1, nDim> Pow(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return pow(arr1,arr2);}

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool, nDim> Equals(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1==arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool, nDim> NotEquals(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1!=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool, nDim> Greater(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1>arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool, nDim> GreaterEquals(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1>=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool, nDim> Less(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1<arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool, nDim> LessEquals(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1<=arr2;}

//~~~~Static safe function for number input
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T1, nDim> Add(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1+num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T1, nDim> Sub(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1-num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T1, nDim> Mult(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1*num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T1, nDim> Div(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1/num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T1, nDim> Mod(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1%num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T1, nDim> Pow(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return pow(arr1,num);}

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool, nDim> Equals(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1==num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool, nDim> NotEquals(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1!=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool, nDim> Greater(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1>num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool, nDim> GreaterEquals(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1>=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool, nDim> Less(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1<num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool, nDim> LessEquals(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1<=num;}




//~~~~Case specific comparison operator overload
    template <bool>
    Arr<bool, nDim>& operator&=(const Arr<bool, nDim>& arr2){
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              arr[i]=arr[i]&&arr2.arr[i];
              arr[i+1]=arr[i+1]&&arr2.arr[i+1];
              arr[i+2]=arr[i+2]&&arr2.arr[i+2];
              arr[i+3]=arr[i+3]&&arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i]=arr[i]!=arr2.arr[i];
      }
      return *this;
    }
    Arr<bool, nDim> operator&&(const Arr<bool, nDim>& arr2) const{
      Arr<T1, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]&&arr2.arr[i];
              res.arr[i+1]=arr[i+1]&&arr2.arr[i+1];
              res.arr[i+2]=arr[i+2]&&arr2.arr[i+2];
              res.arr[i+3]=arr[i+3]&&arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]!=arr2.arr[i];
      }
      return res;
    }
    Arr<bool, nDim>& operator&=(const bool& num){
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
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
    Arr<bool, nDim> operator&&(const bool& num) const{
      Arr<T1, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]&&num;
              res.arr[i+1]=arr[i+1]&&num;
              res.arr[i+2]=arr[i+2]&&num;
              res.arr[i+3]=arr[i+3]&&num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]!=num;
      }
      return res;
    }

    template <bool>
    Arr<bool, nDim>& operator|=(const Arr<bool, nDim>& arr2){
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              arr[i]=arr[i]||arr2.arr[i];
              arr[i+1]=arr[i+1]||arr2.arr[i+1];
              arr[i+2]=arr[i+2]||arr2.arr[i+2];
              arr[i+3]=arr[i+3]||arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          arr[i]=arr[i]!=arr2.arr[i];
      }
      return *this;
    }
    Arr<bool, nDim> operator||(const Arr<bool, nDim>& arr2) const{
      Arr<T1, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]||arr2.arr[i];
              res.arr[i+1]=arr[i+1]||arr2.arr[i+1];
              res.arr[i+2]=arr[i+2]||arr2.arr[i+2];
              res.arr[i+3]=arr[i+3]||arr2.arr[i+3];
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]!=arr2.arr[i];
      }
      return res;
    }
    Arr<bool, nDim>& operator|=(const bool& num){
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
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
    Arr<bool, nDim> operator||(const bool& num) const{
      Arr<T1, nDim> res(size);
      if (size>=4) {
          for (int i=0; i+3<size; i += 4) {
              res.arr[i]=arr[i]||num;
              res.arr[i+1]=arr[i+1]||num;
              res.arr[i+2]=arr[i+2]||num;
              res.arr[i+3]=arr[i+3]||num;
          }
      }
      for (int i = (size/4)*4; i<size; ++i) {
          res.arr[i]=arr[i]!=num;
      }
      return res;
    }

//~~~~Safe boolean comparison
    Arr<bool, nDim>& And(const Arr<bool, nDim>& arr2){CheckCompatability((*this),arr2); return (*this)&=arr2;}
    Arr<bool, nDim>& Or(const Arr<bool, nDim>& arr2){CheckCompatability((*this),arr2); return (*this)|=arr2;}

    Arr<bool, nDim>& And(const bool& num){CheckCompatability((*this),num); return (*this)&=num;}
    Arr<bool, nDim>& Or(const bool& num){CheckCompatability((*this),num); return (*this)|=num;}

//~~~~Static safe boolean comparison
    static Arr<bool, nDim> And(const Arr<bool, nDim>& arr1, const Arr<bool, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1&&arr2;}
    static Arr<bool, nDim> Or(const Arr<bool, nDim>& arr1, const Arr<bool, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1||arr2;}

    static Arr<bool, nDim> And(const Arr<bool, nDim>& arr1, const bool& num){CheckCompatability(arr1,num); return arr1&&num;}
    static Arr<bool, nDim> Or(const Arr<bool, nDim>& arr1, const bool& num){CheckCompatability(arr1,num); return arr1||num;}

//~~~~Overloading output operators
    friend ostream& operator<<(ostream& os, const Arr<T1, nDim>& a){
        os << "hello" << endl;
     if (a.shap[0]>0) {
       os << "[";
       for(int i=0; i<a.shap[0]-1; i++){
         os << a.arr[i] << ", ";
       }
       os << a[a.shap[0]-1] << "]";
     }else{
       os << "[]";
     }
     return os;
    }

//~~~~Functions to check compatability
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static inline void CheckCompatability (const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){
      if (arr1.size!=arr2.size)
        throw invalid_argument(string("Arrays must have the same length, arr1_size: ") + to_string(arr1.size) + string(", and arr2_size: ") + to_string(arr2.size));
      if (!is_convertible<T, T2>::value)
        throw invalid_argument(string("Arrays must have the same typr, arr1_size: ") + string(typeid(T).name()) + string(", and arr2_size: ") + string(typeid(T2).name()));
    }

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static inline void CheckCompatability (const Arr<T1, nDim>& arr1, const T2& num){
      if (!is_convertible<T, T2>::value)
        throw invalid_argument(string("Arrays must have the same typr, arr1_size: ") + string(typeid(T).name()) + string(", and arr2_size: ") + string(typeid(T2).name()));
    }
};