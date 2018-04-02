#include <iostream>
#include<stdexcept>
#include<algorithm>
#include<numeric>
#include "vector.h"
#include<functional>
using namespace std;
int main(){
    Vector x;
    
    Vector::Const_Iterator();
    
    //    Test der Absicherung
    x.push_back(1);
    auto e = x.end();
    auto b = x.begin();
    
    try{
        cout << *e << endl;
    }catch(runtime_error &e){
        cout << e.what() << endl;
    }
    cout << *b << endl;
    x.pop_back();
    try{
        cout << *b << endl;
    }catch(runtime_error &e){
        cout << e.what() << endl;
    }
}

    //    Test von Max
    Vector y{5,10,8};
 /*
    auto i = max_element(y.begin(),y.end());
    cout << "Max: " << *i << endl;
    
    
    //    Testen sie Min, Accumulate, count_if, ..., Sort
  
    //    Test von reserve, ==/!=, ++ postfix/prefix, operator->, insert/erase und range based for
    y.reserve(20);
    i = y.begin();
    ++i;
    Vector::const_iterator j{i++};
    if (i == j) cout << "should not happen";
    if (++j != i) cout << "should not happen too";
    cout << *(j.operator->()) << '\n';
    i = y.insert(i,7);
    j = i = y.insert(i,4);
    y.insert(++i,9);
    y.erase(j);
    for (const auto& val : y) cout << val << " ";
    cout << '\n';
    
    return 0;
}

 [Fehlermeldung]
 1
 [Fehlermeldung]
 Max: 10
 8
 5 10 9 7 8
 */
