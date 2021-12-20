#include <iostream>
#include <ctime>
using namespace std;
#define PARENS ()
typedef short pt;

#define CONCAT_(A, B) A ## B
#define CONCAT(A, B) CONCAT_(A, B)

#define PP_NARG(...) \
         PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) \
         PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,N,...) N
#define PP_RSEQ_N() \
         16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0

#define MY_VA_OPT_HELP(y) CONCAT(UGLY_HELP_, y)
#define MY_VA_OPT(x, ...) MY_VA_OPT_HELP(PP_NARG(__VA_ARGS__))x
#define UGLY_HELP_0(...)
#define UGLY_HELP_1(...) 
#define UGLY_HELP_2(...) __VA_ARGS__ 
#define UGLY_HELP_3(...) __VA_ARGS__ 
#define UGLY_HELP_4(...) __VA_ARGS__ 
#define UGLY_HELP_5(...) __VA_ARGS__  
#define UGLY_HELP_6(...) __VA_ARGS__ 
#define UGLY_HELP_7(...) __VA_ARGS__ 
#define UGLY_HELP_8(...) __VA_ARGS__ 
#define UGLY_HELP_9(...) __VA_ARGS__  
#define UGLY_HELP_10(...) __VA_ARGS__ 
#define UGLY_HELP_11(...) __VA_ARGS__ 
#define UGLY_HELP_12(...) __VA_ARGS__ 
#define UGLY_HELP_13(...) __VA_ARGS__  
#define UGLY_HELP_14(...) __VA_ARGS__ 
#define UGLY_HELP_15(...) __VA_ARGS__ 
#define UGLY_HELP_16(...) __VA_ARGS__ 
#define UGLY_HELP_17(...) __VA_ARGS__

#define EXPAND(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define HNUS_ZNOVA() HNUS

#define HNUS(stop, ab, ugly, ar, ...)                           \
    if (*(ab) != 1) {                                           \
        pt* abb ## ugly = ab;                                   \
        time_t* epravedpo ## ugly = acb;                 \
        BEGIN_PT(*(ab))                                         \
        ar;                                                     \
        END_PT(*(ab), ugly)                                     \
        stop = true;                                            \
    }                                                           \
    MY_VA_OPT((HNUS_ZNOVA), __VA_ARGS__) MY_VA_OPT((PARENS (stop, ab+1, ugly ## a, __VA_ARGS__)), __VA_ARGS__)

#define PARALEL(n, ...) {                                       \
    pt ab[n] = { };                                             \
    time_t acb[n] = { };                                 \
    bool stop = true;                                           \
    while(stop){                                                \
        stop = false;                                           \
        EXPAND(HNUS(stop, ab, a, __VA_ARGS__, 1))               \
    }                                                           \
}

#define BEGIN_PT(p) switch(p){case 0: case 1:

#define LET_OTHERS(x) LET_OTHERS1(*(abb ## x), x)

#define LET_OTHERS1(p, ugly) p = __LINE__; goto left_to_end ## ugly; case __LINE__: a

#define END_PT(p, ugly) }p = 1; left_to_end ## ugly: 

#define WAIT_UNTIL(x, y) WAIT_WHILE(x, !(y))

#define WAIT_WHILE(x, y) while(y){LET_OTHERS(x)}

#define WAIT_FOR(x, y) *(epravedpo ## x) = time(nullptr); WAIT_WHILE(x, time(nullptr) - *(epravedpo ## x) < y)

int main(){
    static int a = 2;
    static int b = 1;
    static int c = 3;
    PARALEL(3, {
        while(a--){
            LET_OTHERS(a);
            cout << 1 << endl;
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