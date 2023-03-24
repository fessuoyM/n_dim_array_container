#include <iostream>
#include <math.h>
#include <type_traits>

using namespace std;

constexpr int loopRollMin = 4;

template <typename T1, int nDim = 1>
class Arr {
//~~~~Define a type T for the underlying type of the array data using metaprogramming
  template<int N> struct getType{Arr<T1, N-1> typ;};
  template<>struct getType<1>{T1 typ;};
  using T = remove_reference_t<decltype(getType<nDim>::typ)>;
  
  private:
    bool memAlloc {false};
  protected:
    T1* memoryBlock_ {nullptr};
    T* arr_ {nullptr};
    size_t size_ {0};
    size_t* shap_ {nullptr};
  public:
    T1* const& memoryBlock {memoryBlock_};
    T* const& arr {arr_};
    size_t const& size {size_};
    size_t* const& shap {shap_};

//~~~~Constructor and Destructors
  //~~~~1D functions
    Arr(){}
    template <typename T2, typename = enable_if_t<is_convertible_v<remove_pointer_t<T2>, remove_pointer_t<T1>>>>
    Arr(const T2* a, const size_t& n):size_{n}, memAlloc{true}{shap_ = new size_t[1]; shap_[0] = n; memoryBlock_ = new T1[n]; for(int i=0; i<n; i++){memoryBlock_[i]=a[i];} arr_=memoryBlock_;}
    template <typename T2, typename = enable_if_t<is_convertible_v<remove_pointer_t<T2>, remove_pointer_t<T1>>>>
    Arr(const size_t& n, const T2& num):size_{n}, memAlloc{true}{shap_ = new size_t[1]; shap_[0] = n; memoryBlock_ = new T1[n]; for(int i=0; i<n; i++){memoryBlock_[i]=num;} arr_=memoryBlock_;}
    Arr(const size_t& n):size_{n}, memAlloc{true}{shap_ = new size_t[1]; shap_[0] = n; memoryBlock_ = new T1[n]; arr_=memoryBlock_;}

  //~~~~nD functions
    template <typename T2, typename = enable_if_t<is_convertible_v<remove_pointer_t<T2>, remove_pointer_t<T1>>>>
    Arr(const T2* a, const size_t* sh):size_{1}, memAlloc{true}{
        shap_ = new size_t[nDim];
        for (int i=0; i<nDim; i++){
            shap_[i] = sh[i]; size_*=shap_[i];
        } 
        memoryBlock_ = new T1[size_];
        for(int i=0; i<size_; i++){
            memoryBlock_[i]=a[i];
        }
        if constexpr(nDim>1){
            arr_ = new T[shap_[0]];
            for (int i=0; i<shap_[0]; i++){
                arr_[i] = T(&memoryBlock_[i*size_/sh[0]], &sh[1], nDim-1);
            }
        }else{
            arr_=memoryBlock_;
        }
    }

    template <typename T2, typename = enable_if_t<is_convertible_v<remove_pointer_t<T2>, remove_pointer_t<T1>>>>
    Arr(const T2& num, const size_t* sh):size_{1}, memAlloc{true}{
        shap_ = new size_t[nDim]; 
        for (int i=0; i<nDim; i++){
            shap_[i] = sh[i]; size_*=shap_[i];
        } 
        memoryBlock_ = new T1[size_];
        for(int i=0; i<size_; i++){
            memoryBlock_[i]=num;
        }
        if constexpr(nDim>1){
            arr_ = new T[shap_[0]];
            for (int i=0; i<shap_[0]; i++){
                arr_[i] = T(&memoryBlock_[i*size_/sh[0]], &sh[1], nDim-1);
            }
        }else{
            arr_=memoryBlock_;
        }
    }

    Arr(const size_t* sh):size_{1}, memAlloc{true}{
        shap_ = new size_t[nDim]; 
        for (int i=0; i<nDim; i++){
            shap_[i] = sh[i]; size_*=shap_[i];
        } 
        memoryBlock_ = new T1[size_];
        if constexpr(nDim>1){
            arr_ = new T[shap_[0]];
            for (int i=0; i<shap_[0]; i++){
                arr_[i] = T(&memoryBlock_[i*size_/sh[0]], &sh[1], nDim-1);
            }
        }else{
            arr_=memoryBlock_;
        }
    }

    template <typename T2, typename = enable_if_t<is_convertible_v<remove_pointer_t<T2>, remove_pointer_t<T1>>>>
    Arr(T2* a, const size_t* sh, const size_t dims):size_{1}{
      shap_ = new size_t[nDim]; 
      memoryBlock_ = a;
      for (int i=0; i<nDim; i++){
        shap_[i] = sh[i]; size_*=shap_[i];
      }
      if constexpr(nDim==1){
        arr_ = a;
      }else{
        arr_ = new T[shap_[0]];
        for (int i=0; i<shap_[0]; i++){
          arr_[i] = T(&a[i*size_/sh[0]], &sh[1], dims-1);
        }
      }
    }

  //~~~~Copy with implicit casting constructors
    template <typename T2, typename = enable_if_t<is_convertible_v<remove_pointer_t<T2>, remove_pointer_t<T1>>>>
    Arr(const Arr<T2, nDim>& a):size_{a.size}, memAlloc{true}{
        memoryBlock_ = new T1[size_]; 
        for(int i=0; i<size_; i++){
            memoryBlock_[i]=a.memoryBlock[i];
        }
        shap_ = new size_t[nDim]; 
        for (int i=0; i<nDim; i++){
            shap_[i] = a.shap[i];
        } 
        if constexpr(nDim==1){
            arr_=memoryBlock_;
        }else{
            arr_ = new T[shap_[0]];
            for (int i=0; i<shap_[0]; i++){
                arr_[i] = T(&memoryBlock_[i*size_/shap_[0]], &shap_[1], nDim-1);
            }
        }
    }
    //Copy Constructor
    Arr(const Arr<T1, nDim>& a):size_{a.size_}, memAlloc{true}{
        memoryBlock_ = new T1[size_]; 
        for(int i=0; i<size_; i++){
            memoryBlock_[i]=a.memoryBlock_[i];
        }
        shap_ = new size_t[nDim]; 
        for (int i=0; i<nDim; i++){
            shap_[i] = a.shap_[i];
        } 
        if constexpr(nDim==1){
            arr_=memoryBlock_;
        }else{
            arr_ = new T[shap_[0]];
            for (int i=0; i<shap_[0]; i++){
                arr_[i] = T(&memoryBlock_[i*size_/shap_[0]], &shap_[1], nDim-1);
            }
        }
    }
    //Move Constructor
    Arr(Arr<T1, nDim>&& a) noexcept :Arr<T1, nDim>(){
        swap(*this, a);
    }

    //Swap Function
    friend void swap(Arr<T1, nDim>& lhs, Arr<T1, nDim>& rhs){
        swap(lhs.memoryBlock_, rhs.memoryBlock_);
        swap(lhs.memAlloc, rhs.memAlloc);
        swap(lhs.arr_, rhs.arr_);
        swap(lhs.size_, rhs.size_);
        swap(lhs.shap_, rhs.shap_);
    }

  //~~~~Destructor
    ~Arr(){
        delete[] shap_;
        if constexpr(nDim>1)
            delete[] arr_;
        if (memAlloc)
            delete [] memoryBlock_;
    }

//~~~~Access private members directly
    size_t getSize() const{return size_;}
    size_t* getShap() const{return shap_;}
    T* getArr() const{return arr_;}
    T1* getMemoryBlock() const{return memoryBlock_;}
    bool getMemAlloc() const{return memAlloc;}


//~~~~Overloading access operators
    T& operator[](const int& ind){return arr_[ind];}
    const T& operator[](const int& ind) const{return arr_[ind];}

    template<typename... Ints, typename = enable_if_t<conjunction_v<is_same<Ints, int>...>>>
    T& operator()(Ints... inds){
      static_assert(sizeof...(inds)==nDim, "Numbere of argumnerts must match number of dimeensions, otherwise use the [] operators conecutivley.");
      int ind[] = {forward<Ints>(inds)...};
      int multiplier=1, offset=0;
      for (int i=sizeof...(inds)-1; i>=0; i--){
        offset+=multiplier*ind[i];
        multiplier*=shap_[i];
      }
      return memoryBlock_[offset];
    }

//~~~~Overloading copy operators
    template <typename T2, typename = enable_if_t<is_convertible_v<remove_pointer_t<T2>, remove_pointer_t<T1>>>>
    Arr<T1, nDim>& operator=(T2* arr2){
        for(int i=0; i<size_; i++){
            memoryBlock_[i]=arr2[i];
        } 
        return *this;
    }
    template <typename T2, typename = enable_if_t<is_convertible_v<remove_pointer_t<T2>, remove_pointer_t<T1>>>>
    Arr<T1, nDim>& operator=(const T2& num){
        for(int i=0; i<size_; i++){
            memoryBlock_[i]=num;
        } 
        return *this;
    }
    Arr<T1, nDim>& operator=(Arr<T1, nDim>&& arr2){
        swap(*this, arr2);
        return *this;
    }
    Arr<T1, nDim>& operator=(const Arr<T1, nDim>& arr2){
        Arr<T1, nDim> temp{arr2};
        swap(*this, temp);
        return *this;
    }
    template <typename T2, typename = enable_if_t<is_convertible_v<remove_pointer_t<T2>, remove_pointer_t<T1>>>>
    Arr<T1, nDim>& operator=(const Arr<T2, nDim>& arr2){
        Arr<T1, nDim> temp{arr2};
        swap(*this, temp);
        return *this;
    }

//~~~~Overloading arithmatic operators
  //Addition
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& operator+=(const Arr<T2, nDim>& arr2){
        if (size_>=loopRollMin) {
            T2* ptr = arr2.memoryBlock;
            for (int i=0; i+3<size_; i += 4) {
                memoryBlock_[i] += ptr[i];
                memoryBlock_[i+1] += ptr[i+1];
                memoryBlock_[i+2] += ptr[i+2];
                memoryBlock_[i+3] += ptr[i+3];
            }
        }
      for (int i = (size_/4)*4; i<size_; ++i) {
          memoryBlock_[i] += arr2.memoryBlock[i];
      }
      return *this;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim> operator+(const Arr<T2, nDim>& arr2) const{
      Arr<T1, nDim> res(shap_);
      if (size_>=loopRollMin) {
        T2* ptr = arr2.memoryBlock;
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock_[i]=memoryBlock_[i]+ptr[i];
              res.memoryBlock_[i+1]=memoryBlock_[i+1]+ptr[i+1];
              res.memoryBlock_[i+2]=memoryBlock_[i+2]+ptr[i+2];
              res.memoryBlock_[i+3]=memoryBlock_[i+3]+ptr[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock_[i]=memoryBlock_[i]+arr2.memoryBlock[i];
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& operator+=(const T2& num){
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              memoryBlock_[i] += num;
              memoryBlock_[i+1] += num;
              memoryBlock_[i+2] += num;
              memoryBlock_[i+3] += num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          memoryBlock_[i] += num;
      }
      return *this;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim> operator+(const T2& num) const{
      Arr<T1, nDim> res(shap_);
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock_[i]=memoryBlock_[i]+num;
              res.memoryBlock_[i+1]=memoryBlock_[i+1]+num;
              res.memoryBlock_[i+2]=memoryBlock_[i+2]+num;
              res.memoryBlock_[i+3]=memoryBlock_[i+3]+num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock_[i]=memoryBlock_[i]+num;
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    friend Arr<T1, nDim> operator+(const T2& num, const Arr<T1, nDim>& arr){
      Arr<T1, nDim> res(arr.size_);
      if (arr.size_>=4) {
          for (int i=0; i<arr.size_; i += 4) {
              res.memoryBlock_[i]=arr.memoryBlock_[i]+num;
              res.memoryBlock_[i+1]=arr.memoryBlock_[i+1]+num;
              res.memoryBlock_[i+2]=arr.memoryBlock_[i+2]+num;
              res.memoryBlock_[i+3]=arr.memoryBlock_[i+3]+num;
          }
      }
      for (int i = (arr.size_/4)*4; i<arr.size_; ++i) {
          res.memoryBlock_[i]=arr.memoryBlock_[i]+num;
      }
      return res;
    }

  //Subtraction
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& operator-=(const Arr<T2, nDim>& arr2){
      if (size_>=loopRollMin) {
        T2* ptr = arr2.memoryBlock;
          for (int i=0; i+3<size_; i += 4) {
              memoryBlock_[i] -= ptr[i];
              memoryBlock_[i+1] -= ptr[i+1];
              memoryBlock_[i+2] -= ptr[i+2];
              memoryBlock_[i+3] -= ptr[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          memoryBlock_[i] -= arr2.memoryBlock[i];
      }
      return *this;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim> operator-(const Arr<T2, nDim>& arr2) const{
      Arr<T1, nDim> res(shap_);
      if (size_>=loopRollMin) {
        T2* ptr = arr2.memoryBlock;
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock_[i]=memoryBlock_[i]-ptr[i];
              res.memoryBlock_[i+1]=memoryBlock_[i+1]-ptr[i+1];
              res.memoryBlock_[i+2]=memoryBlock_[i+2]-ptr[i+2];
              res.memoryBlock_[i+3]=memoryBlock_[i+3]-ptr[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock_[i]=memoryBlock_[i]-arr2.memoryBlock[i];
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& operator-=(const T2& num){
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              memoryBlock_[i] -= num;
              memoryBlock_[i+1] -= num;
              memoryBlock_[i+2] -= num;
              memoryBlock_[i+3] -= num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          memoryBlock_[i] -= num;
      }
      return *this;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim> operator-(const T2& num) const{
      Arr<T1, nDim> res(shap_);
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock_[i]=memoryBlock_[i]-num;
              res.memoryBlock_[i+1]=memoryBlock_[i+1]-num;
              res.memoryBlock_[i+2]=memoryBlock_[i+2]-num;
              res.memoryBlock_[i+3]=memoryBlock_[i+3]-num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock_[i]=memoryBlock_[i]-num;
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    friend Arr<T1, nDim> operator-(const T2& num, const Arr<T1, nDim>& arr){
      Arr<T1, nDim> res(arr.size_);
      if (arr.size_>=4) {
          for (int i=0; i<arr.size_; i += 4) {
              res.memoryBlock_[i]=arr.memoryBlock_[i]-num;
              res.memoryBlock_[i+1]=arr.memoryBlock_[i+1]-num;
              res.memoryBlock_[i+2]=arr.memoryBlock_[i+2]-num;
              res.memoryBlock_[i+3]=arr.memoryBlock_[i+3]-num;
          }
      }
      for (int i = (arr.size_/4)*4; i<arr.size_; ++i) {
          res.memoryBlock_[i]=arr.memoryBlock_[i]-num;
      }
      return res;
    }

  //Multiply
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& operator*=(const Arr<T2, nDim>& arr2){
      if (size_>=loopRollMin) {
        T2* ptr = arr2.memoryBlock;
          for (int i=0; i+3<size_; i += 4) {
              memoryBlock_[i] *= ptr[i];
              memoryBlock_[i+1] *= ptr[i+1];
              memoryBlock_[i+2] *= ptr[i+2];
              memoryBlock_[i+3] *= ptr[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          memoryBlock_[i] *= arr2.memoryBlock[i];
      }
      return *this;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim> operator*(const Arr<T2, nDim>& arr2) const{
      Arr<T1, nDim> res(shap_);
      if (size_>=loopRollMin) {
        T2* ptr = arr2.memoryBlock;
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock_[i]=memoryBlock_[i]*ptr[i];
              res.memoryBlock_[i+1]=memoryBlock_[i+1]*ptr[i+1];
              res.memoryBlock_[i+2]=memoryBlock_[i+2]*ptr[i+2];
              res.memoryBlock_[i+3]=memoryBlock_[i+3]*ptr[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock_[i]=memoryBlock_[i]*arr2.memoryBlock[i];
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& operator*=(const T2& num){
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              memoryBlock_[i] *= num;
              memoryBlock_[i+1] *= num;
              memoryBlock_[i+2] *= num;
              memoryBlock_[i+3] *= num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          memoryBlock_[i] *= num;
      }
      return *this;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim> operator*(const T2& num) const{
      Arr<T1, nDim> res(shap_);
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock_[i]=memoryBlock_[i]*num;
              res.memoryBlock_[i+1]=memoryBlock_[i+1]*num;
              res.memoryBlock_[i+2]=memoryBlock_[i+2]*num;
              res.memoryBlock_[i+3]=memoryBlock_[i+3]*num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock_[i]=memoryBlock_[i]*num;
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    friend Arr<T1, nDim> operator*(const T2& num, const Arr<T1, nDim>& arr){
      Arr<T1, nDim> res(arr.size_);
      if (arr.size_>=4) {
          for (int i=0; i<arr.size_; i += 4) {
              res.memoryBlock_[i]=arr.memoryBlock_[i]*num;
              res.memoryBlock_[i+1]=arr.memoryBlock_[i+1]*num;
              res.memoryBlock_[i+2]=arr.memoryBlock_[i+2]*num;
              res.memoryBlock_[i+3]=arr.memoryBlock_[i+3]*num;
          }
      }
      for (int i = (arr.size_/4)*4; i<arr.size_; ++i) {
          res.memoryBlock_[i]=arr.memoryBlock_[i]*num;
      }
      return res;
    }

  //Divide
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& operator/=(const Arr<T2, nDim>& arr2){
      if (size_>=loopRollMin) {
        T2* ptr = arr2.memoryBlock;
          for (int i=0; i+3<size_; i += 4) {
              memoryBlock_[i] /= ptr[i];
              memoryBlock_[i+1] /= ptr[i+1];
              memoryBlock_[i+2] /= ptr[i+2];
              memoryBlock_[i+3] /= ptr[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          memoryBlock_[i] /= arr2.memoryBlock[i];
      }
      return *this;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim> operator/(const Arr<T2, nDim>& arr2) const{
      Arr<T1, nDim> res(shap_);
      if (size_>=loopRollMin) {
        T2* ptr = arr2.memoryBlock;
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock_[i]=memoryBlock_[i]/ptr[i];
              res.memoryBlock_[i+1]=memoryBlock_[i+1]/ptr[i+1];
              res.memoryBlock_[i+2]=memoryBlock_[i+2]/ptr[i+2];
              res.memoryBlock_[i+3]=memoryBlock_[i+3]/ptr[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock_[i]=memoryBlock_[i]/arr2.memoryBlock[i];
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& operator/=(const T2& num){
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              memoryBlock_[i] /= num;
              memoryBlock_[i+1] /= num;
              memoryBlock_[i+2] /= num;
              memoryBlock_[i+3] /= num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          memoryBlock_[i] /= num;
      }
      return *this;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim> operator/(const T2& num) const{
      Arr<T1, nDim> res(shap_);
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock_[i]=memoryBlock_[i]/num;
              res.memoryBlock_[i+1]=memoryBlock_[i+1]/num;
              res.memoryBlock_[i+2]=memoryBlock_[i+2]/num;
              res.memoryBlock_[i+3]=memoryBlock_[i+3]/num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock_[i]=memoryBlock_[i]/num;
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    friend Arr<T1, nDim> operator/(const T2& num, const Arr<T1, nDim>& arr){
      Arr<T1, nDim> res(arr.size_);
      if (arr.size_>=4) {
          for (int i=0; i<arr.size_; i += 4) {
              res.memoryBlock_[i]=arr.memoryBlock_[i]/num;
              res.memoryBlock_[i+1]=arr.memoryBlock_[i+1]/num;
              res.memoryBlock_[i+2]=arr.memoryBlock_[i+2]/num;
              res.memoryBlock_[i+3]=arr.memoryBlock_[i+3]/num;
          }
      }
      for (int i = (arr.size_/4)*4; i<arr.size_; ++i) {
          res.memoryBlock_[i]=arr.memoryBlock_[i]/num;
      }
      return res;
    }

  //Exp
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    friend Arr<T1, nDim> pow(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){
      Arr<T1, nDim> res(arr1.size_);
      if (arr1.size_>=4) {
        T2* ptr = arr2.memoryBlock;
          for (int i=0; i<arr1.size_; i += 4) {
              res.memoryBlock_[i]=pow(arr1.memoryBlock_[i],ptr[i]);
              res.memoryBlock_[i+1]=pow(arr1.memoryBlock_[i+1],ptr[i+1]);
              res.memoryBlock_[i+2]=pow(arr1.memoryBlock_[i+2],ptr[i+2]);
              res.memoryBlock_[i+3]=pow(arr1.memoryBlock_[i+3],ptr[i+3]);
          }
      }
      for (int i = (arr1.size_/4)*4; i<arr1.size_; ++i) {
          res.memoryBlock_[i]=pow(arr1.memoryBlock_[i],arr2.memoryBlock[i]);;
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    friend Arr<T1, nDim> pow(const Arr<T1, nDim>& arr1, const T2& num){
      Arr<T1, nDim> res(arr1.size_);
      if (arr1.size_>=4) {
          for (int i=0; i<arr1.size_; i += 4) {
              res.memoryBlock_[i]=pow(arr1.memoryBlock_[i],num);
              res.memoryBlock_[i+1]=pow(arr1.memoryBlock_[i+1],num);
              res.memoryBlock_[i+2]=pow(arr1.memoryBlock_[i+2],num);
              res.memoryBlock_[i+3]=pow(arr1.memoryBlock_[i+3],num);
          }
      }
      for (int i = (arr1.size_/4)*4; i<arr1.size_; ++i) {
          res.memoryBlock_[i]=pow(arr1.memoryBlock_[i],num);;
      }
      return res;
    }
  //Mod
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& operator%=(const Arr<T2, nDim>& arr2){
      if (size_>=loopRollMin) {
        T2* ptr = arr2.memoryBlock;
          for (int i=0; i+3<size_; i += 4) {
              memoryBlock_[i] %= ptr[i];
              memoryBlock_[i+1] %= ptr[i+1];
              memoryBlock_[i+2] %= ptr[i+2];
              memoryBlock_[i+3] %= ptr[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          memoryBlock_[i] %= arr2.memoryBlock[i];
      }
      return *this;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim> operator%(const Arr<T2, nDim>& arr2) const{
      Arr<T1, nDim> res(shap_);
      if (size_>=loopRollMin) {
        T2* ptr = arr2.memoryBlock;
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock_[i]=memoryBlock_[i]%ptr[i];
              res.memoryBlock_[i+1]=memoryBlock_[i+1]%ptr[i+1];
              res.memoryBlock_[i+2]=memoryBlock_[i+2]%ptr[i+2];
              res.memoryBlock_[i+3]=memoryBlock_[i+3]%ptr[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock_[i]=memoryBlock_[i]%arr2.memoryBlock[i];
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& operator%=(const T2& num){
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              memoryBlock_[i] %= num;
              memoryBlock_[i+1] %= num;
              memoryBlock_[i+2] %= num;
              memoryBlock_[i+3] %= num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          memoryBlock_[i] %= num;
      }
      return *this;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim> operator%(const T2& num) const{
      Arr<T1, nDim> res(shap_);
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock_[i]=memoryBlock_[i]%num;
              res.memoryBlock_[i+1]=memoryBlock_[i+1]%num;
              res.memoryBlock_[i+2]=memoryBlock_[i+2]%num;
              res.memoryBlock_[i+3]=memoryBlock_[i+3]%num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock_[i]=memoryBlock_[i]%num;
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    friend Arr<T1, nDim> operator%(const T2& num, const Arr<T1, nDim>& arr){
      Arr<T1, nDim> res(arr.size_);
      if (arr.size_>=4) {
          for (int i=0; i<arr.size_; i += 4) {
              res.memoryBlock_[i]=arr.memoryBlock_[i]%num;
              res.memoryBlock_[i+1]=arr.memoryBlock_[i+1]%num;
              res.memoryBlock_[i+2]=arr.memoryBlock_[i+2]%num;
              res.memoryBlock_[i+3]=arr.memoryBlock_[i+3]%num;
          }
      }
      for (int i = (arr.size_/4)*4; i<arr.size_; ++i) {
          res.memoryBlock_[i]=arr.memoryBlock_[i]%num;
      }
      return res;
    }


//~~~~Overloading comparrison operators for array return type
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> operator==(const Arr<T2, nDim>& arr2) const{
      Arr<bool, nDim> res(shap_);
      if (size_>=loopRollMin) {
        T2* ptr = arr2.memoryBlock;
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock[i]=memoryBlock_[i]==ptr[i];
              res.memoryBlock[i+1]=memoryBlock_[i+1]==ptr[i+1];
              res.memoryBlock[i+2]=memoryBlock_[i+2]==ptr[i+2];
              res.memoryBlock[i+3]=memoryBlock_[i+3]==ptr[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock[i]=memoryBlock_[i]==arr2.memoryBlock[i];
      }
      return res;
    }
    Arr<bool, nDim> operator!() const{
      Arr<bool, nDim> res(shap_);
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock[i]=!memoryBlock_[i];
              res.memoryBlock[i+1]=!memoryBlock_[i+1];
              res.memoryBlock[i+2]=!memoryBlock_[i+2];
              res.memoryBlock[i+3]=!memoryBlock_[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock[i]=!memoryBlock_[i];
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> operator!=(const Arr<T2, nDim>& arr2) const{
      Arr<bool, nDim> res(shap_);
      if (size_>=loopRollMin) {
        T2* ptr = arr2.memoryBlock;
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock[i]=memoryBlock_[i]!=ptr[i];
              res.memoryBlock[i+1]=memoryBlock_[i+1]!=ptr[i+1];
              res.memoryBlock[i+2]=memoryBlock_[i+2]!=ptr[i+2];
              res.memoryBlock[i+3]=memoryBlock_[i+3]!=ptr[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock[i]=memoryBlock_[i]!=arr2.memoryBlock[i];
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> operator>=(const Arr<T2, nDim>& arr2) const{
      Arr<bool, nDim> res(shap_);
      if (size_>=loopRollMin) {
        T2* ptr = arr2.memoryBlock;
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock[i]=memoryBlock_[i]>=ptr[i];
              res.memoryBlock[i+1]=memoryBlock_[i+1]>=ptr[i+1];
              res.memoryBlock[i+2]=memoryBlock_[i+2]>=ptr[i+2];
              res.memoryBlock[i+3]=memoryBlock_[i+3]>=ptr[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock[i]=memoryBlock_[i]>=arr2.memoryBlock[i];
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> operator>(const Arr<T2, nDim>& arr2) const{
      Arr<bool, nDim> res(shap_);
      if (size_>=loopRollMin) {
        T2* ptr = arr2.memoryBlock;
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock[i]=memoryBlock_[i]>ptr[i];
              res.memoryBlock[i+1]=memoryBlock_[i+1]>ptr[i+1];
              res.memoryBlock[i+2]=memoryBlock_[i+2]>ptr[i+2];
              res.memoryBlock[i+3]=memoryBlock_[i+3]>ptr[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock[i]=memoryBlock_[i]>arr2.memoryBlock[i];
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> operator<=(const Arr<T2, nDim>& arr2) const{
      Arr<bool, nDim> res(shap_);
      if (size_>=loopRollMin) {
        T2* ptr = arr2.memoryBlock;
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock[i]=memoryBlock_[i]<=ptr[i];
              res.memoryBlock[i+1]=memoryBlock_[i+1]<=ptr[i+1];
              res.memoryBlock[i+2]=memoryBlock_[i+2]<=ptr[i+2];
              res.memoryBlock[i+3]=memoryBlock_[i+3]<=ptr[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock[i]=memoryBlock_[i]<=arr2.memoryBlock[i];
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> operator<(const Arr<T2, nDim>& arr2) const{
      Arr<bool, nDim> res(shap_);
      if (size_>=loopRollMin) {
        T2* ptr = arr2.memoryBlock;
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock[i]=memoryBlock_[i]<ptr[i];
              res.memoryBlock[i+1]=memoryBlock_[i+1]<ptr[i+1];
              res.memoryBlock[i+2]=memoryBlock_[i+2]<ptr[i+2];
              res.memoryBlock[i+3]=memoryBlock_[i+3]<ptr[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock[i]=memoryBlock_[i]<arr2.memoryBlock[i];
      }
      return res;
    }

    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> operator==(const T2& num) const{
      Arr<bool, nDim> res(shap_);
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock[i]=memoryBlock_[i]==num;
              res.memoryBlock[i+1]=memoryBlock_[i+1]==num;
              res.memoryBlock[i+2]=memoryBlock_[i+2]==num;
              res.memoryBlock[i+3]=memoryBlock_[i+3]==num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock[i]=memoryBlock_[i]==num;
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> operator!=(const T2& num) const{
      Arr<bool, nDim> res(shap_);
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock[i]=memoryBlock_[i]!=num;
              res.memoryBlock[i+1]=memoryBlock_[i+1]!=num;
              res.memoryBlock[i+2]=memoryBlock_[i+2]!=num;
              res.memoryBlock[i+3]=memoryBlock_[i+3]!=num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock[i]=memoryBlock_[i]!=num;
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> operator>=(const T2& num) const{
      Arr<bool, nDim> res(shap_);
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock[i]=memoryBlock_[i]>=num;
              res.memoryBlock[i+1]=memoryBlock_[i+1]>=num;
              res.memoryBlock[i+2]=memoryBlock_[i+2]>=num;
              res.memoryBlock[i+3]=memoryBlock_[i+3]>=num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock[i]=memoryBlock_[i]>=num;
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> operator>(const T2& num) const{
      Arr<bool, nDim> res(shap_);
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock[i]=memoryBlock_[i]>num;
              res.memoryBlock[i+1]=memoryBlock_[i+1]>num;
              res.memoryBlock[i+2]=memoryBlock_[i+2]>num;
              res.memoryBlock[i+3]=memoryBlock_[i+3]>num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock[i]=memoryBlock_[i]>num;
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> operator<=(const T2& num) const{
      Arr<bool, nDim> res(shap_);
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock[i]=memoryBlock_[i]<=num;
              res.memoryBlock[i+1]=memoryBlock_[i+1]<=num;
              res.memoryBlock[i+2]=memoryBlock_[i+2]<=num;
              res.memoryBlock[i+3]=memoryBlock_[i+3]<=num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock[i]=memoryBlock_[i]<=num;
      }
      return res;
    }
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> operator<(const T2& num) const{
      Arr<bool, nDim> res(shap_);
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock[i]=memoryBlock_[i]<num;
              res.memoryBlock[i+1]=memoryBlock_[i+1]<num;
              res.memoryBlock[i+2]=memoryBlock_[i+2]<num;
              res.memoryBlock[i+3]=memoryBlock_[i+3]<num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock[i]=memoryBlock_[i]<num;
      }
      return res;
    }


//~~~~Safe functions for the overloaded operators
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& Add(const Arr<T2, nDim>& arr2){CheckCompatability((*this),arr2); return (*this)+=arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& Sub(const Arr<T2, nDim>& arr2){CheckCompatability((*this),arr2); return (*this)-=arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& Mult(const Arr<T2, nDim>& arr2){CheckCompatability((*this),arr2); return (*this)*=arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& Div(const Arr<T2, nDim>& arr2){CheckCompatability((*this),arr2); return (*this)/=arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& Mod(const Arr<T2, nDim>& arr2){CheckCompatability((*this),arr2); return (*this)%=arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& Pow(const Arr<T2, nDim>& arr2){CheckCompatability((*this),arr2); return pow((*this),arr2);}

    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> Equals(const Arr<T2, nDim>& arr2) const{CheckCompatability((*this),arr2); return (*this)==arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> NotEquals(const Arr<T2, nDim>& arr2) const{CheckCompatability((*this),arr2); return (*this)!=arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> Greater(const Arr<T2, nDim>& arr2) const{CheckCompatability((*this),arr2); return (*this)>arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> GreaterEquals(const Arr<T2, nDim>& arr2) const{CheckCompatability((*this),arr2); return (*this)>=arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> Less(const Arr<T2, nDim>& arr2) const{CheckCompatability((*this),arr2); return (*this)<arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> LessEquals(const Arr<T2, nDim>& arr2) const{CheckCompatability((*this),arr2); return (*this)<=arr2;}

//~~~~Safe Functions for number input
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& Add(const T2& num){CheckCompatability((*this),num); return (*this)+=num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& Sub(const T2& num){CheckCompatability((*this),num); return (*this)-=num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& Mult(const T2& num){CheckCompatability((*this),num); return (*this)*=num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& Div(const T2& num){CheckCompatability((*this),num); return (*this)/=num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& Mod(const T2& num){CheckCompatability((*this),num); return (*this)%=num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<T1, nDim>& Pow(const T2& num){CheckCompatability((*this),num); return pow((*this),num);}

    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> Equals(const T2& num) const{CheckCompatability((*this),num); return (*this)==num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> NotEquals(const T2& num) const{CheckCompatability((*this),num); return (*this)!=num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> Greater(const T2& num) const{CheckCompatability((*this),num); return (*this)>num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> GreaterEquals(const T2& num) const{CheckCompatability((*this),num); return (*this)>=num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> Less(const T2& num) const{CheckCompatability((*this),num); return (*this)<num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    Arr<bool, nDim> LessEquals(const T2& num) const{CheckCompatability((*this),num); return (*this)<=num;}



//~~~~Safe Static functions for the overloaded operators
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<T1, nDim> Add(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1+arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<T1, nDim> Sub(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1-arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<T1, nDim> Mult(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1*arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<T1, nDim> Div(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1/arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<T1, nDim> Mod(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1%arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<T1, nDim> Pow(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return pow(arr1,arr2);}

    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<bool, nDim> Equals(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1==arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<bool, nDim> NotEquals(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1!=arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<bool, nDim> Greater(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1>arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<bool, nDim> GreaterEquals(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1>=arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<bool, nDim> Less(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1<arr2;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<bool, nDim> LessEquals(const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1<=arr2;}

//~~~~Static safe function for number input
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<T1, nDim> Add(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1+num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<T1, nDim> Sub(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1-num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<T1, nDim> Mult(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1*num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<T1, nDim> Div(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1/num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<T1, nDim> Mod(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1%num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<T1, nDim> Pow(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return pow(arr1,num);}

    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<bool, nDim> Equals(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1==num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<bool, nDim> NotEquals(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1!=num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<bool, nDim> Greater(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1>num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<bool, nDim> GreaterEquals(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1>=num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<bool, nDim> Less(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1<num;}
    template <typename T2, typename = enable_if_t<is_arithmetic_v<T2>>>
    static Arr<bool, nDim> LessEquals(const Arr<T1, nDim>& arr1, const T2& num){CheckCompatability(arr1,num); return arr1<=num;}




//~~~~Case specific comparison operator overload
    template<typename v3 = T1, typename = enable_if_t<is_same<v3, bool>::value>>
    Arr<bool, nDim>& operator&=(const Arr<bool, nDim>& arr2){
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              memoryBlock_[i]=memoryBlock_[i]&&arr2.memoryBlock_[i];
              memoryBlock_[i+1]=memoryBlock_[i+1]&&arr2.memoryBlock_[i+1];
              memoryBlock_[i+2]=memoryBlock_[i+2]&&arr2.memoryBlock_[i+2];
              memoryBlock_[i+3]=memoryBlock_[i+3]&&arr2.memoryBlock_[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          memoryBlock_[i]=memoryBlock_[i]&&arr2.memoryBlock_[i];
      }
      return *this;
    }
    template<typename v3 = T1, typename = enable_if_t<is_same<v3, bool>::value>>
    Arr<bool, nDim> operator&&(const Arr<bool, nDim>& arr2) const{
      Arr<T1, nDim> res(shap_);
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock_[i]=memoryBlock_[i]&&arr2.memoryBlock_[i];
              res.memoryBlock_[i+1]=memoryBlock_[i+1]&&arr2.memoryBlock_[i+1];
              res.memoryBlock_[i+2]=memoryBlock_[i+2]&&arr2.memoryBlock_[i+2];
              res.memoryBlock_[i+3]=memoryBlock_[i+3]&&arr2.memoryBlock_[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock_[i]=memoryBlock_[i]&&arr2.memoryBlock_[i];
      }
      return res;
    }
    template<typename v3 = T1, typename = enable_if_t<is_same<v3, bool>::value>>
    Arr<bool, nDim>& operator&=(const bool& num){
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              memoryBlock_[i]=memoryBlock_[i]&&num;
              memoryBlock_[i+1]=memoryBlock_[i+1]&&num;
              memoryBlock_[i+2]=memoryBlock_[i+2]&&num;
              memoryBlock_[i+3]=memoryBlock_[i+3]&&num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          memoryBlock_[i]=memoryBlock_[i]!=num;
      }
      return *this;
    }
    template<typename v3 = T1, typename = enable_if_t<is_same<v3, bool>::value>>
    Arr<bool, nDim> operator&&(const bool& num) const{
      Arr<T1, nDim> res(shap_);
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock_[i]=memoryBlock_[i]&&num;
              res.memoryBlock_[i+1]=memoryBlock_[i+1]&&num;
              res.memoryBlock_[i+2]=memoryBlock_[i+2]&&num;
              res.memoryBlock_[i+3]=memoryBlock_[i+3]&&num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock_[i]=memoryBlock_[i]!=num;
      }
      return res;
    }

    template<typename v3 = T1, typename = enable_if_t<is_same<v3, bool>::value>>
    Arr<bool, nDim>& operator|=(const Arr<bool, nDim>& arr2){
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              memoryBlock_[i]=memoryBlock_[i]||arr2.memoryBlock_[i];
              memoryBlock_[i+1]=memoryBlock_[i+1]||arr2.memoryBlock_[i+1];
              memoryBlock_[i+2]=memoryBlock_[i+2]||arr2.memoryBlock_[i+2];
              memoryBlock_[i+3]=memoryBlock_[i+3]||arr2.memoryBlock_[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          memoryBlock_[i]=memoryBlock_[i]||arr2.memoryBlock_[i];
      }
      return *this;
    }
    template<typename v3 = T1, typename = enable_if_t<is_same<v3, bool>::value>>
    Arr<bool, nDim> operator||(const Arr<bool, nDim>& arr2) const{
      Arr<T1, nDim> res(shap_);
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock_[i]=memoryBlock_[i]||arr2.memoryBlock_[i];
              res.memoryBlock_[i+1]=memoryBlock_[i+1]||arr2.memoryBlock_[i+1];
              res.memoryBlock_[i+2]=memoryBlock_[i+2]||arr2.memoryBlock_[i+2];
              res.memoryBlock_[i+3]=memoryBlock_[i+3]||arr2.memoryBlock_[i+3];
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock_[i]=memoryBlock_[i]||arr2.memoryBlock_[i];
      }
      return res;
    }
    template<typename v3 = T1, typename = enable_if_t<is_same<v3, bool>::value>>
    Arr<bool, nDim>& operator|=(const bool& num){
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              memoryBlock_[i]=memoryBlock_[i]||num;
              memoryBlock_[i+1]=memoryBlock_[i+1]||num;
              memoryBlock_[i+2]=memoryBlock_[i+2]||num;
              memoryBlock_[i+3]=memoryBlock_[i+3]||num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          memoryBlock_[i]=memoryBlock_[i]!=num;
      }
      return *this;
    }
    template<typename v3 = T1, typename = enable_if_t<is_same<v3, bool>::value>>
    Arr<bool, nDim> operator||(const bool& num) const{
      Arr<T1, nDim> res(shap_);
      if (size_>=loopRollMin) {
          for (int i=0; i+3<size_; i += 4) {
              res.memoryBlock_[i]=memoryBlock_[i]||num;
              res.memoryBlock_[i+1]=memoryBlock_[i+1]||num;
              res.memoryBlock_[i+2]=memoryBlock_[i+2]||num;
              res.memoryBlock_[i+3]=memoryBlock_[i+3]||num;
          }
      }
      for (int i = (size_/4)*4; i<size_; ++i) {
          res.memoryBlock_[i]=memoryBlock_[i]!=num;
      }
      return res;
    }

//~~~~Safe boolean comparison
    template<typename v3 = T1, typename = enable_if_t<is_same<v3, bool>::value>>
    Arr<bool, nDim>& And(const Arr<bool, nDim>& arr2){CheckCompatability((*this),arr2); return (*this)&=arr2;}
    template<typename v3 = T1, typename = enable_if_t<is_same<v3, bool>::value>>
    Arr<bool, nDim>& Or(const Arr<bool, nDim>& arr2){CheckCompatability((*this),arr2); return (*this)|=arr2;}

    template<typename v3 = T1, typename = enable_if_t<is_same<v3, bool>::value>>
    Arr<bool, nDim>& And(const bool& num){CheckCompatability((*this),num); return (*this)&=num;}
    template<typename v3 = T1, typename = enable_if_t<is_same<v3, bool>::value>>
    Arr<bool, nDim>& Or(const bool& num){CheckCompatability((*this),num); return (*this)|=num;}

//~~~~Static safe boolean comparison
    static Arr<bool, nDim> And(const Arr<bool, nDim>& arr1, const Arr<bool, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1&&arr2;}
    static Arr<bool, nDim> Or(const Arr<bool, nDim>& arr1, const Arr<bool, nDim>& arr2){CheckCompatability(arr1,arr2); return arr1||arr2;}

    static Arr<bool, nDim> And(const Arr<bool, nDim>& arr1, const bool& num){CheckCompatability(arr1,num); return arr1&&num;}
    static Arr<bool, nDim> Or(const Arr<bool, nDim>& arr1, const bool& num){CheckCompatability(arr1,num); return arr1||num;}

//~~~~Overloading output operators
    friend ostream& operator<<(ostream& os, const Arr<T1, nDim>& a){
     if (a.shap_[0]>0) {
       os << "[";
       for(int i=0; i<a.shap_[0]-1; i++){
         os << a.arr_[i] << ", ";
       }
       os << a[a.shap_[0]-1] << "]";
     }else{
       os << "[]";
     }
     return os;
    }

//~~~~Functions to check compatability
    template <typename T2, typename = enable_if_t<is_convertible_v<remove_pointer_t<T2>, remove_pointer_t<T1>>>>
    static inline void CheckCompatability (const Arr<T1, nDim>& arr1, const Arr<T2, nDim>& arr2){
      if (arr1.size!=arr2.size)
        throw invalid_argument(string("Arrays must have the same length, arr1_size: ") + to_string(arr1.size) + string(", and arr2_size: ") + to_string(arr2.size));
      if (!is_convertible<T1, T2>::value)
        throw invalid_argument(string("Arrays must have the same typr, arr1_size: ") + string(typeid(T).name()) + string(", and arr2_size: ") + string(typeid(T2).name()));
    }

    template <typename T2, typename = enable_if_t<is_convertible_v<remove_pointer_t<T2>, remove_pointer_t<T1>>>>
    static inline void CheckCompatability (const Arr<T1, nDim>& arr1, const T2& num){
      if (!is_convertible<T, T2>::value)
        throw invalid_argument(string("Arrays must have the same typr, arr1_size: ") + string(typeid(T).name()) + string(", and arr2_size: ") + string(typeid(T2).name()));
    }
};