#include <iostream>
#include <math.h>

using namespace std;

template <typename T1, int dim = 1>
struct Arr2 {
//~~~~Define a type T for the underlying type of the array data using metaprogramming
  template<int N> struct getType{Arr2<T1, N-1> typ;};
  template<>struct getType<1>{T1 typ;};
  using T = typename remove_reference<decltype(getType<dim>::typ)>::type;

  private:
    T1* memoryBlock;
  public:
    mutable T* arr;
    mutable size_t* shap;
    mutable size_t nDim;
    mutable size_t size;
//~~~~Constructor and Destructors
  //~~~~1D functions
    Arr2():arr{nullptr}, size(0), nDim{0}, shap{&size}, memoryBlock{nullptr}{}
    template <typename T2>
    Arr2(const T2* a, const size_t& n):size{n}, nDim{2}, shap{&size}, memoryBlock{nullptr}{shap = new size_t[2]; shap[0]=3; shap[1]=2; arr = new T[n]; for(int i=0; i<n; i++){arr[i]=a[i];}}
    template <typename T2>
    Arr2(const size_t& n, const T2& num):size{n}, nDim{1}, shap{&size}, memoryBlock{nullptr}{arr = new T[n]; for(int i=0; i<n; i++){arr[i]=num;}}
    Arr2(const size_t& n):size{n}, nDim{1}, shap{&size}, memoryBlock{nullptr}{arr = new T[n];}

  //~~~~nD functions
    template <typename T2>
    Arr2(const T2* a, const size_t* sh):nDim{dim}, size{1}{
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
    Arr2(T2* a, const size_t* sh, const size_t dims):nDim{dims}, size{1}{
      shap = new size_t[nDim]; 
      for (int i=0; i<nDim; i++){
        shap[i] = sh[i]; size*=shap[i];
      }
      if constexpr(dim==1){
        arr = a;
      }else{
        arr = new T[shap[0]];
        for (int i=0; i<shap[0]; i++){
          arr[i] = T(&a[i*size/sh[0]], &sh[1], dims-1);
        }
      }
    }

    // template <typename T2>
    // Arr2(const Arr2<T2> a, const size_t* sh, const size_t& dims):nDim{dims}, size{1}{
    //   T2 lol = 5;
    //   cout << lol << "<--proof" << endl;
    // }

  //~~~~Destructor
    // ~Arr2(){
    //   delete[] memoryBlock;
    //   delete[] arr;
    //   delete[] shap;
    // }

  //~~~~Copy and move constructor
    // template <typename T2>
    // Arr2(const Arr2<T2>& a):size{a.siz()}, nDim{a.nDim}, shap{a.shap}{arr = new T[a.siz()]; T2* p = a.ptr(); for(int i=0; i<size; i++){arr[i]=p[i];}}
    // Arr2(const Arr2<T>& a):size{a.size}, nDim{a.nDim}, shap{a.shap}{arr = new T[a.size]; T* p = a.arr; for(int i=0; i<size; i++){arr[i]=p[i];}}
    // Arr2(Arr2<T>&& a):size{a.size}, arr{a.arr}, nDim{a.nDim}, shap{a.shap}{a.arr = nullptr; a.size = 0; a.nDim=0; a.shap=&a.size;}


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
      return arr[offset];
    }

    // template<typename... Ints, typename = enable_if_t<conjunction<is_same<Ints, int>...>::value>>
    // T& operator()(Ints... inds){
    //   return arr[get<0>(helper(inds...))];
    // }

    // tuple <int,int,int> helper(int ind){
    //   return make_tuple(ind,shap[nDim-1],nDim-2);
    // }

    // template<typename... Ints, typename = enable_if_t<conjunction<is_same<Ints, int>...>::value>>
    // tuple <int,int,int> helper(int ind, Ints... inds){
    //   auto [offset, multiplier, count] = helper(inds...);
    //   return make_tuple(ind*multiplier+offset,multiplier*shap[count],count-1);
    // }
    


//~~~~Overloading output operators
      friend ostream& operator<<(ostream& os, const Arr2<T1,dim>& a){
        if (a.shap[0]>0) {
          os << "[";
          for(int i=0; i<a.shap[0]-1; i++){
            os << a[i] << ", ";
          }
          os << a[a.shap[0]-1] << "]";
        }else{
          os << "[]";
        }
        return os;
      }
};
 


//I think i need to use either template metaprogramming or a constexp function to generate the template type based on the number passed in.
//Also need to make use of decltype which will allow us to generate the type of a variable at compile time i believe.
//typename remove_reference<decltype(__object__)>::type temp;

//It works, use the following to specify the type for T, note that the number of dimensions must be passed in through the brackets and not the constructor
//typename remove_reference<decltype(getType<__Dim__>::typ)>::type
