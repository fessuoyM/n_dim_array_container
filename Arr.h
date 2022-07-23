#include <iostream>
#include <math.h>

using namespace std;

template <typename T, typename v = typename enable_if<is_arithmetic<T>::value, bool>::type>
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
    // void operator=(const Arr<T2> arr2){(*this) = new Arr<T>(arr2);}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void operator=(const Arr<T2> arr2){this->size=arr2.siz(); for(int i=0; i<this->size; i++){this->arr[i]=arr2[i];}}
    void operator=(const T* arr2){for(int i=0; i<this->size; i++){this->arr[i]=arr2[i];}}

//~~~~Overloading arithmatic operators
  //Addition
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void operator+=(const Arr<T2> arr2){for(int i=0; i<this->size; i++){this->arr[i]+=arr2[i];}}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator+(const Arr<T2> arr2) const{Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]+arr2[i];} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void operator+=(const T2 num){for(int i=0; i<this->size; i++){this->arr[i]+=num;}}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator+(const T2 num) const{Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]+num;} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T> operator+(const T2 num, const Arr<T> arr){Arr<T> res = arr; for(int i=0; i<arr.size; i++){res[i]=num+arr[i];} return res;}
  //Subtraction
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void operator-=(const Arr<T2> arr2){for(int i=0; i<this->size; i++){this->arr[i]-=arr2[i];}}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator-(const Arr<T2> arr2) const{Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]-arr2[i];} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void operator-=(const T2 num){for(int i=0; i<this->size; i++){this->arr[i]-=num;}}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator-(const T2 num) const{Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]-num;} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T> operator-(const T2 num, const Arr<T> arr){Arr<T> res = arr; for(int i=0; i<arr.size; i++){res[i]=num-arr[i];} return res;}
  //Multiply
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void operator*=(const Arr<T2> arr2){for(int i=0; i<this->size; i++){this->arr[i]*=arr2[i];}}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator*(const Arr<T2> arr2) const{Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]*arr2[i];} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void operator*=(const T2 num){for(int i=0; i<this->size; i++){this->arr[i]*=num;}}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator*(const T2 num) const{Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]*num;} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T> operator*(const T2 num, const Arr<T> arr){Arr<T> res = arr; for(int i=0; i<arr.size; i++){res[i]=num*arr[i];} return res;}
  //Divide
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void operator/=(const Arr<T2> arr2){for(int i=0; i<this->size; i++){this->arr[i]/=arr2[i];}}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator/(const Arr<T2> arr2) const{Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]/arr2[i];} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void operator/=(const T2 num){for(int i=0; i<this->size; i++){this->arr[i]/=num;}}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator/(const T2 num) const{Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]/num;} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T> operator/(const T2 num, const Arr<T> arr){Arr<T> res = arr; for(int i=0; i<arr.size; i++){res[i]=num/arr[i];} return res;}
  //Exp
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T> pow(const Arr<T> arr1, const Arr<T2> arr2){Arr<T> res = arr1; for(int i=0; i<arr1.size; i++){res[i]=pow(arr1[i],arr2[i]);} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T> pow(const Arr<T> arr1, const T2 num){Arr<T> res = arr1; for(int i=0; i<arr1.size; i++){res[i]=pow(arr1[i],num);}return res;}
  //Mod
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void operator%=(const Arr<T2> arr2){for(int i=0; i<this->size; i++){this->arr[i]%=arr2[i];}}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator%(const Arr<T2> arr2) const{Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]%arr2[i];} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void operator%=(const T2 num){for(int i=0; i<this->size; i++){this->arr[i]%=num;}}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<T> operator%(const T2 num) const{Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]%num;} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    friend Arr<T> operator%(const T2 num, const Arr<T> arr){Arr<T> res = arr; for(int i=0; i<arr.size; i++){res[i]=num%arr[i];} return res;}

//~~~~Overloading comparrison operators for array return type
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator==(const Arr<T2> arr2) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]==arr2[i];} return res;}
    Arr<bool> operator!() const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=!this->arr[i];} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator!=(const Arr<T2> arr2) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]!=arr2[i];} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator>(const Arr<T2> arr2) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]>arr2[i];} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator>=(const Arr<T2> arr2) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]>=arr2[i];} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator<(const Arr<T2> arr2) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]<arr2[i];} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator<=(const Arr<T2> arr2) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]<=arr2[i];} return res;}

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator==(const T2 num) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]==num;} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator!=(const T2 num) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]!=num;} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator>(const T2 num) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]>num;} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator>=(const T2 num) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]>=num;} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator<(const T2 num) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]<num;} return res;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> operator<=(const T2 num) const{Arr<bool> res(this->size); for(int i=0; i<this->size; i++){res[i]=this->arr[i]<=num;} return res;}


//~~~~Safe functions for the overloaded operators
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void Add(const Arr<T2> arr2){CheckCompatability((*this),arr2); (*this)+=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void Sub(const Arr<T2> arr2){CheckCompatability((*this),arr2); (*this)-=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void Mult(const Arr<T2> arr2){CheckCompatability((*this),arr2); (*this)*=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void Div(const Arr<T2> arr2){CheckCompatability((*this),arr2); (*this)/=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void Mod(const Arr<T2> arr2){CheckCompatability((*this),arr2); (*this)%=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void Pow(const Arr<T2> arr2){CheckCompatability((*this),arr2); pow((*this),arr2);}

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> Equals(const Arr<T2> arr2) const{CheckCompatability((*this),arr2); return (*this)==arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> NotEquals(const Arr<T2> arr2) const{CheckCompatability((*this),arr2); return (*this)!=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> Greater(const Arr<T2> arr2) const{CheckCompatability((*this),arr2); return (*this)>arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> GreaterEquals(const Arr<T2> arr2) const{CheckCompatability((*this),arr2); return (*this)>=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> Less(const Arr<T2> arr2) const{CheckCompatability((*this),arr2); return (*this)<arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> LessEquals(const Arr<T2> arr2) const{CheckCompatability((*this),arr2); return (*this)<=arr2;}

//~~~~Safe Functions for number input
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void Add(const T2 num){CheckCompatability((*this),num); (*this)+=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void Sub(const T2 num){CheckCompatability((*this),num); (*this)-=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void Mult(const T2 num){CheckCompatability((*this),num); (*this)*=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void Div(const T2 num){CheckCompatability((*this),num); (*this)/=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void Mod(const T2 num){CheckCompatability((*this),num); (*this)%=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    void Pow(const T2 num){CheckCompatability((*this),num); pow((*this),num);}

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> Equals(const T2 num) const{CheckCompatability((*this),num); return (*this)==num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> NotEquals(const T2 num) const{CheckCompatability((*this),num); return (*this)!=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> Greater(const T2 num) const{CheckCompatability((*this),num); return (*this)>num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> GreaterEquals(const T2 num) const{CheckCompatability((*this),num); return (*this)>=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> Less(const T2 num) const{CheckCompatability((*this),num); return (*this)<num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    Arr<bool> LessEquals(const T2 num) const{CheckCompatability((*this),num); return (*this)<=num;}



//~~~~Safe Static functions for the overloaded operators
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Add(const Arr<T> arr1, const Arr<T2> arr2){CheckCompatability(arr1,arr2); return arr1+arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Sub(const Arr<T> arr1, const Arr<T2> arr2){CheckCompatability(arr1,arr2); return arr1-arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Mult(const Arr<T> arr1, const Arr<T2> arr2){CheckCompatability(arr1,arr2); return arr1*arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Div(const Arr<T> arr1, const Arr<T2> arr2){CheckCompatability(arr1,arr2); return arr1/arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Mod(const Arr<T> arr1, const Arr<T2> arr2){CheckCompatability(arr1,arr2); return arr1%arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Pow(const Arr<T> arr1, const Arr<T2> arr2){CheckCompatability(arr1,arr2); return pow(arr1,arr2);}

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> Equals(const Arr<T> arr1, const Arr<T2> arr2){CheckCompatability(arr1,arr2); return arr1==arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> NotEquals(const Arr<T> arr1, const Arr<T2> arr2){CheckCompatability(arr1,arr2); return arr1!=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> Greater(const Arr<T> arr1, const Arr<T2> arr2){CheckCompatability(arr1,arr2); return arr1>arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> GreaterEquals(const Arr<T> arr1, const Arr<T2> arr2){CheckCompatability(arr1,arr2); return arr1>=arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> Less(const Arr<T> arr1, const Arr<T2> arr2){CheckCompatability(arr1,arr2); return arr1<arr2;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> LessEquals(const Arr<T> arr1, const Arr<T2> arr2){CheckCompatability(arr1,arr2); return arr1<=arr2;}

//~~~~Static safe function for number input
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Add(const Arr<T> arr1, const T2 num){CheckCompatability(arr1,num); return arr1+num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Sub(const Arr<T> arr1, const T2 num){CheckCompatability(arr1,num); return arr1-num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Mult(const Arr<T> arr1, const T2 num){CheckCompatability(arr1,num); return arr1*num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Div(const Arr<T> arr1, const T2 num){CheckCompatability(arr1,num); return arr1/num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Mod(const Arr<T> arr1, const T2 num){CheckCompatability(arr1,num); return arr1%num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<T> Pow(const Arr<T> arr1, const T2 num){CheckCompatability(arr1,num); return pow(arr1,num);}

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> Equals(const Arr<T> arr1, const T2 num){CheckCompatability(arr1,num); return arr1==num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> NotEquals(const Arr<T> arr1, const T2 num){CheckCompatability(arr1,num); return arr1!=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> Greater(const Arr<T> arr1, const T2 num){CheckCompatability(arr1,num); return arr1>num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> GreaterEquals(const Arr<T> arr1, const T2 num){CheckCompatability(arr1,num); return arr1>=num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> Less(const Arr<T> arr1, const T2 num){CheckCompatability(arr1,num); return arr1<num;}
    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static Arr<bool> LessEquals(const Arr<T> arr1, const T2 num){CheckCompatability(arr1,num); return arr1<=num;}




//~~~~Case specific comparison operator overload
    template <bool>
    void operator&=(const Arr<bool> arr2){for(int i=0; i<this->size; i++){this->arr[i]&&arr2[i];}}
    Arr<bool> operator&&(const Arr<bool> arr2) const{Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]&&arr2[i];} return res;}
    void operator&=(const bool num){for(int i=0; i<this->size; i++){this->arr[i]&&num;}}
    Arr<bool> operator&&(const bool num) const{Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]&&num;} return res;}

    void operator|=(const Arr<bool> arr2){for(int i=0; i<this->size; i++){this->arr[i]||arr2[i];}}
    Arr<bool> operator||(const Arr<bool> arr2) const{Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]||arr2[i];} return res;}
    void operator|=(const bool num){for(int i=0; i<this->size; i++){this->arr[i]||num;}}
    Arr<bool> operator||(const bool num) const{Arr<T> res = *this; for(int i=0; i<this->size; i++){res[i]=this->arr[i]||num;} return res;}

//~~~~Safe boolean comparison
    void And(const Arr<bool> arr2){CheckCompatability((*this),arr2); (*this)&=arr2;}
    void Or(const Arr<bool> arr2){CheckCompatability((*this),arr2); (*this)|=arr2;}

    void And(const bool num){CheckCompatability((*this),num); (*this)&=num;}
    void Or(const bool num){CheckCompatability((*this),num); (*this)|=num;}

//~~~~Static safe boolean comparison
    static Arr<bool> And(const Arr<bool> arr1, const Arr<bool> arr2){CheckCompatability(arr1,arr2); return arr1&&arr2;}
    static Arr<bool> Or(const Arr<bool> arr1, const Arr<bool> arr2){CheckCompatability(arr1,arr2); return arr1||arr2;}

    static Arr<bool> And(const Arr<bool> arr1, const bool num){CheckCompatability(arr1,num); return arr1&&num;}
    static Arr<bool> Or(const Arr<bool> arr1, const bool num){CheckCompatability(arr1,num); return arr1||num;}

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
    static inline void CheckCompatability (const Arr<T> arr1, const Arr<T2> arr2){
      if (arr1.siz()!=arr2.siz())
        throw invalid_argument(string("Arrays must have the same length, arr1_size: ") + to_string(arr1.siz()) + string(", and arr2_size: ") + to_string(arr2.siz()));
      if (!is_convertible<T, T2>::value)
        throw invalid_argument(string("Arrays must have the same typr, arr1_size: ") + string(typeid(T).name()) + string(", and arr2_size: ") + string(typeid(T2).name()));
    }

    template <typename T2, typename v2 = typename enable_if<is_arithmetic<T2>::value, bool>::type>
    static inline void CheckCompatability (const Arr<T> arr1, const T2 num){
      if (!is_convertible<T, T2>::value)
        throw invalid_argument(string("Arrays must have the same typr, arr1_size: ") + string(typeid(T).name()) + string(", and arr2_size: ") + string(typeid(T2).name()));
    }

};
