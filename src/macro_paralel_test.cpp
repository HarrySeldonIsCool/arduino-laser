#include <iostream>
using namespace std;
#define PARENS ()
typedef short pt;

#define EXPAND(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define HNUS_ZNOVA() HNUS

#define HNUS(stop, ab, ugly, ar, ...)                           \
    pt abb ## ugly = *(ab);                                     \
    if (*(ab) != 1) {                                           \
        BEGIN_PT(*(ab))                                         \
        ar;                                                     \
        END_PT(*(ab), ugly)                                     \
        stop = true;                                            \
    }                                                           \
    __VA_OPT__(HNUS_ZNOVA PARENS (stop, ab+1, ugly ## a, __VA_ARGS__))

#define PARALEL(n, ...) {                                       \
    pt ab[n] = { };                                             \
    bool stop = true;                                           \
    while(stop){                                                \
        stop = false;                                           \
        EXPAND(HNUS(stop, ab, a, __VA_ARGS__))                  \
    }                                                           \
}

#define BEGIN_PT(p) switch(p){case 0: case 1:

#define LET_OTHERS(x) LET_OTHERS1(abb ## x, x)

#define LET_OTHERS1(p, ugly) p = __LINE__; goto left_to_end ## ugly; case __LINE__: 

#define END_PT(p, ugly) }p = 1; left_to_end ## ugly: 

int main(){
    static int a = 2;
    static int b = 1;
    static int c = 3;
    PARALEL(3, {
        while(a--){
            cout << 1 << endl;
            LET_OTHERS(a);
        }
    },{
        while(b--){
            cout << 2 << endl;
            LET_OTHERS(aa);
        }
    },{
        while(c--){
            cout << 3 << endl;
            LET_OTHERS(aaa);
        }
    });
}