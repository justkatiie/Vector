

// header weil sonst keine templates möglich

#include <iostream>
#include <stdexcept>
#include <initializer_list>
using namespace std;


class Vector{
public:

    using value_type = double;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = double&;
    using const_reference = const double&;
    using pointer = double*;
    using const_pointer = const double*;
    
private:
    static constexpr size_type initial_size {30};
    size_type sz; //index -> wie voll ist das Array
    size_type max_sz; //größe des Arrays
    pointer values; //dyn Array in dem Werte gespeichert sind
    
public:
    
    class Iterator;
    class Const_Iterator;
    
    using iterator = Iterator;
    using const_iterator = Const_Iterator;
    
    
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

    
    
    //push_back - Adds Value at the End
    void push_back(value_type value){
        if(sz == max_sz){
            reserve(sz*2);
        }
        values[sz++] = value;
    }
    
    //reserve - Increases the Size of the array
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
        if(/*index < 0 ||*/ index >= sz)
            throw runtime_error("Out of Bounds");
        return values[index];
    }
    
    const_reference operator[](size_type index)const {
        if(/*index < 0 ||*/ index >= sz)
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
    
    
    // pop_back() - deletes last element
    void pop_back(){
        if(sz == 0)
            return;
        --sz;
        
    }
    
    //returns size()
    size_type size() const {return sz;}
    
    //
    size_type getmaxsz() const {return max_sz;}
    
    //returns true/false if emtpy()
    bool empty() const {return sz == 0;}
    
    //clear - clears array
    void clear() {sz = 0;}
    
    void print(){
        for (size_type i = 0; i < sz; i++) {
            cout << "["<< values[i]<< "] ";
        }
        //cout << "size():"<<size() << '\n' << "sz:" << sz  << '\n' <<"max_size: "<<max_sz<< endl;
    }
    
    
    
    
    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////
    
    
    
    
    iterator begin() { return Iterator(values,*this);}
    iterator end() {return Iterator(values+sz,*this);}
    const_iterator begin() const {return Const_Iterator(values, this);}
    const_iterator end() const {return Const_Iterator(values+sz, this);}
    
    
    iterator erase(const_iterator pos) { auto diff = pos-begin();
        if (diff<0 || static_cast<size_type>(diff)>=sz) throw runtime_error("Iterator out of bounds");
        size_type current{static_cast<size_type>(diff)}; for (size_type i{current}; i<sz-1; ++i)
            values[i]=values[i+1]; --sz;
        return iterator(values+current, *this);
        
    }
    
    iterator insert(const_iterator pos, const_reference val) {
        auto diff = pos-begin();
        if (diff<0 || static_cast<size_type>(diff)>sz)
            throw runtime_error("Iterator out of bounds");
        size_type current{static_cast<size_type>(diff)}; if (sz>=max_sz)
            reserve(max_sz*2); //max_sz*2+10, wenn Ihr Container max_sz==0 erlaubt
        for (size_t i {sz}; i-->current;) values[i+1]=values[i];
        values[current]=val;
        ++sz;
        return iterator{values+current, *this};
        
    }
    
    
    
    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////
    
    
    
    class Iterator{
    public:
        using value_type = double;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using reference = double&;
        using pointer = double*;
        using iterator_category = std::forward_iterator_tag;
        
    private:
        pointer ptr;
        const Vector * v;
        
    public:
       
        // Constructor for Iterator
        Iterator(){
            v = nullptr;
            ptr = nullptr;
        }
        
        // Constructor for Iterator with Value(in this Case pointer and vector)
        Iterator(pointer ptr, Vector  &v){
            this -> ptr = ptr;
            this -> v = &v;
        };
        
        //Destructer for Iterator
        ~Iterator() {}
        
        
        operator Const_Iterator(){ return const_iterator(ptr, v);}

        
        // Overwrites ++ operator --> prefix a++
        Iterator operator++(){
            if(this->ptr >= v->values+v->sz){
                throw runtime_error ("[Fehlermeldung]");
            } else {
                ++ptr;
                return *this;
            }
            
        }
    
       
      /*  bool operator!=(iterator &rop){        // rop -> rechter operand, "this" immer der linke Operand
            if(ptr == rop.ptr)return false;
                else return true;
        }
        
        bool operator==(iterator& rop){
            if(ptr==rop.ptr) return true;
            return false;
        }*/
        
        
        double& operator*(){
            if(this->ptr >= v->values+v->sz || this->ptr < v->values){
                throw runtime_error ("[Fehlermeldung]");
            } return *ptr;              //* vor Pointer -> Element im array, nur pointer -> adresse
        }
        
        const double& operator*() const{
            return *ptr;
        }
        
        
       
        
    };
    
    
    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////
    
    
    class Const_Iterator{
    public:
        using value_type = double;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using reference = double&;
        using pointer = double*;
        using iterator_category = std::forward_iterator_tag;
    
    private:
        pointer ptr;
        const Vector * v;
        
    public:
        Const_Iterator(){
            v = nullptr;
            ptr = nullptr;
        }
        
        Const_Iterator(pointer ptr, const Vector *v){
            this->v = v;
            this-> ptr = ptr;
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
        
        
        ///////////////////////////////////////////////////////////////////////////////////
        
        
        friend difference_type operator-(const Const_Iterator& lop, const Const_Iterator& rop){
            return lop.ptr-rop.ptr;
        }
        
        friend const_iterator operator-(const Const_Iterator& lop, size_type rop){
         return const_iterator(lop.ptr-rop,lop.v);
         }
        
        friend difference_type operator!=(const Const_Iterator& lop, const Const_Iterator& rop){
            return lop.ptr!=rop.ptr;
        }
        
        friend difference_type operator==(const Const_Iterator& lop, const Const_Iterator& rop){
            return lop.ptr==rop.ptr;
        }
        
        friend const_iterator operator+(const Const_Iterator& lop, size_type rop){
         return const_iterator(lop.ptr+rop, lop.v);
         }
        
        friend difference_type operator<(const Const_Iterator& lop, const Const_Iterator& rop){
            return lop.ptr<rop.ptr;
        }
        
        friend difference_type operator>(const Const_Iterator& lop, const Const_Iterator& rop){
            return lop.ptr>rop.ptr;
        }
        
        friend difference_type operator<=(const Const_Iterator& lop, const Const_Iterator& rop){
            return lop.ptr<=rop.ptr;
        }
        
        friend difference_type operator>=(const Const_Iterator& lop, const Const_Iterator& rop){
            return lop.ptr>=rop.ptr;
        }
        
    };
    
    
};

