

// header weil sonst keine templates möglich

#include <iostream>
#include <stdexcept>
#include <initializer_list>
using namespace std;

const size_t initial_size = 4; //Erste Größe vom Vektor

class Vector{
public:
    
    class Iterator;
    class Const_Iterator;
    using value_type = double;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = double&;
    using const_reference = const double&;
    using pointer = double*;
    using const_pointer = const double*;
    using iterator = Iterator;
    using const_iterator = Const_Iterator;
    
private:
    size_type sz; //index -> wie voll ist das Array
    size_type max_sz; //größe des Arrays
    pointer values; //dyn Array in dem Werte gespeichert sind
    
public:
    //Konstruktor ohne Werte, Immer mit 4 Elementen
    Vector():Vector(initial_size){};
    
    //Konstruktor mit mitgegebenen werten
    Vector(size_type newsz):sz{0}{
        if(newsz < initial_size)        //wollen initial_size nicht unterschreiten
            newsz= initial_size;
        values = new value_type [newsz];
        max_sz = newsz;
    }
    
    //Konstruktor mit init_list
    Vector(initializer_list<value_type> li): Vector(li.size()){
        for(auto& elem : li){
            push_back(elem);
        }
    }
    
    //Kopierkonstruktor
    Vector(const Vector& copy):Vector(copy.size()){
        for(size_type i = 0; i< copy.size(); ++i){
            push_back(copy[i]);
        }
    }
    
    //Destruktor
    ~Vector(){delete[] values;}
    
    
    //push_back
    void push_back(value_type value){
        if(sz == max_sz){
            reserve(sz*2);
        }
        values[sz++] = value;
    }
    
    //reserve
    void reserve(size_type newsz){
        pointer zwischen = new value_type[newsz];
        
        for(size_type i=0; i< sz; ++i){
            zwischen[i] = values[i];
        }
        delete[] values;
        values = zwischen;
        max_sz = newsz;
    }
    
    //shrink_to_fit
    //neues array erstellen das kleiner ist - werte hinein kopieren
    void shrink_to_fit(){
        
        pointer zwischen = new value_type[sz];
        
        for(size_type i=0; i< sz; ++i){
            zwischen[i] = values[i];
        }
        delete[] values;
        values = zwischen;
        max_sz = sz;
    }
    
    // [] Operator
    reference operator[](size_type index){
        if(index < 0 || index >= sz)
            throw runtime_error("Out of Bounds");
        return values[index];
    }
    
    const_reference operator[](size_type index)const {
        if(index < 0 || index >= sz)
            throw runtime_error("Out of Bounds");
        return values[index];
    }
    
    // = Operator
    Vector operator = (Vector src){
        delete[] values;
        sz = 0;
        max_sz = src.size();
        
        values = new value_type [src.size()];
        for(size_type i =0; i < src.size(); ++i){
            push_back(src[i]);
        }
        
        return *this;
    }
    
    
    // pop_back()
    void pop_back(){
        if(sz == 0)
            return;
        --sz;
        
    }
    
    //size()
    size_type size() const {return sz;}
    
    //
    size_type getmaxsz() const {return max_sz;}
    
    //emtpy()
    bool empty() const {return sz == 0;}
    
    //clear
    void clear() {sz = 0;}
    
    
    
    ;
    
    
    //////////////////////////////////////////////
    
    
    iterator begin() { return Iterator(values, *this);}
    iterator end() {return Iterator(values+sz, *this);}
    //const_iterator begin() const {return Const_Iterator(values, this);}
    // const_iterator end() const {return Iterator(values+sz, this);}
    
    
    class Iterator{
        
        pointer ptr;
        const Vector * v;
        
    public:
        using value_type = double;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using reference = double&;
        using pointer = double*;
        using iterator_category = std::forward_iterator_tag;
        
        Iterator(){
            v = new Vector();
            ptr = v->values;
        }
        Iterator(pointer ptr, Vector &v){
            this -> ptr = ptr;
            this -> v = &v;
        };
        
        ~Iterator() {}
        
        Iterator& operator++(){
            if(this->ptr >= v->values+v->sz){
                throw runtime_error ("[Fehlermeldung]");
            } else {
                ++ptr;
                return *this;
            }
            
            
        }
        
        /*   bool operator!=(const_iterator rop){        // rop -> rechter operand, "this" immer der linke Operand
         if(*this == rop){
         return false;
         }
         else{
         return true;
         }
         }
         */
        
        double& operator*(){
            if(this->ptr >= v->values+v->sz || this->ptr < v->values){
                throw runtime_error ("[Fehlermeldung]");
            } return *ptr;              //* vor Pointer -> Element im array, nur pointer -> adresse
        }
        
        const double& operator*() const{
            return *ptr;
        }
        
        
       operator Const_Iterator(){ return Vector::Const_Iterator(ptr, &v);}
        
        
        
    };
    
    
    ///////////////////////////////////////////////////
    
    
    class Const_Iterator{
        pointer ptr;
        const Vector * v;
        
    public:
        using value_type = double;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using reference = double&;
        using pointer = double*;
        using iterator_category = std::forward_iterator_tag;
        
        Const_Iterator(){
            v = new Vector();
            ptr = v->values;
        }
        
        Const_Iterator(pointer ptr, Vector &vec){
            ptr = ptr;
            v = &vec;
        };
        
        ~Const_Iterator() {}
        
        Const_Iterator& operator++(){
            if(this->ptr >= v->values+v->sz){
                throw runtime_error ("[Fehlermeldung]");
            } else {
                ++ptr;
                return *this;
            }
            
            
        }
        
        bool operator!=(const_iterator rop){        // rop -> rechter operand, "this" immer der linke Operand
            if(ptr == rop.ptr){
                return false;
            }
            else{
                return true;
            }
        }
        
        double& operator*(){
            if(this->ptr >= v->values+v->sz || this->ptr < v->values){
                throw runtime_error ("[Fehlermeldung]");
            } return *ptr;              //* vor Pointer -> Element im array, nur pointer -> adresse
        }
        
        const double& operator*() const{
            return *ptr;
        }
        
        
        
        
        friend difference_type operator-(const Const_Iterator& lep, const Const_Iterator& rop){
            return lep.ptr-rop.ptr;
        }
        
        /* friend difference_type operator-(const Const_Iterator& lep, size_type rop){
         return lep.ptr-rop;
         }*/
        
        friend difference_type operator!=(const Const_Iterator& lep, const Const_Iterator& rop){
            return lep.ptr!=rop.ptr;
        }
        
        friend difference_type operator==(const Const_Iterator& lep, const Const_Iterator& rop){
            return lep.ptr==rop.ptr;
        }
        
        /* friend difference_type operator+(const Const_Iterator& lep, const Const_Iterator& rop){
         return lep.ptr+rop.ptr;
         }*/
        
        friend difference_type operator<(const Const_Iterator& lep, const Const_Iterator& rop){
            return lep.ptr<rop.ptr;
        }
        
        friend difference_type operator>(const Const_Iterator& lep, const Const_Iterator& rop){
            return lep.ptr>rop.ptr;
        }
        
        friend difference_type operator<=(const Const_Iterator& lep, const Const_Iterator& rop){
            return lep.ptr<=rop.ptr;
        }
        
        friend difference_type operator>=(const Const_Iterator& lep, const Const_Iterator& rop){
            return lep.ptr>=rop.ptr;
        }
        
    };
    
    
};

