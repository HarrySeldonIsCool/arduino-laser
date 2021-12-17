#include <iostream>
using namespace std;
#define PARENS ()
typedef short pt;

#define EXPAND(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define HNUS_ZNOVA() HNUS

#define HNUS(stop, ab, ugly, ar, ...)                           \
    if (*(ab) != 1) {                                                \
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

#define LET_OTHERS(x) LET_OTHERS1(*(ab), x)

#define LET_OTHERS1(p, ugly) p = __LINE__; goto left_to_end ## ugly; case __LINE__: 

#define END_PT(p, ugly) }p = 1; left_to_end ## ugly: 

int main(){
    static int d = 4;
    static int b = 3;
    static int c = 2;
    PARALEL(3, {
        while(d--){
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

/*{ pt ab[3] = { }; bool stop = true; while(stop){ stop = false; if (*(ab) != 1) { switch(*(ab)){case 0: case 1: { while(d--){ cout << 1 << endl; *(ab) = 45; goto left_to_enda; case 45:; } }; }*(ab) = 1; left_to_enda: stop = true; } if (*(ab+1) != 1) { switch(*(ab+1)){case 0: case 1: { while(b--){ cout << 2 << endl; *(ab) = 50; goto left_to_endaa; case 50:; } }; }*(ab+1) = 1; left_to_endaa: stop = true; } if (*(ab+1 +1) != 1) { switch(*(ab+1 +1)){case 0: case 1: { while(c--){ cout << 3 << endl; *(ab) = 55; goto left_to_endaaa; case 55:; } }; }*(ab+1 +1) = 1; left_to_endaaa: stop = true; } } }*/