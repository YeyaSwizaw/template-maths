#include <iostream>

/*
 * Number Definition
 * 0 = Zero
 * 1 = Succ<Zero>
 * 2 = Succ<Succ<Zero>>
 * ...
 */
struct Zero { };

template<typename N>
struct Succ {
    typedef N n;

}; // struct Succ;

/* Equality (N == M) */
template<typename N, typename M>
struct Eq {
    static const bool value = Eq<typename N::n, typename M::n>::value;

}; // struct Eq;

template<>
struct Eq<Zero, Zero> {
    static const bool value = true;

}; // struct Eq<Zero, Zero>;

template<typename N>
struct Eq<N, Zero> {
    static const bool value = false;

}; // struct Eq<N, Zero>;

template<typename M>
struct Eq<Zero, M> {
    static const bool value = false;

}; // struct Eq<Zero, M>;

/* N <= M */
template<typename N, typename M>
struct Le {
    static const bool value = Le<typename N::n, typename M::n>::value;

}; // struct Le;

template<>
struct Le<Zero, Zero> {
    static const bool value = true;

}; // struct Le<Zero, Zero>;

template<typename N>
struct Le<N, Zero> {
    static const bool value = false;

}; // struct Le<N, Zero>;

template<typename M>
struct Le<Zero, M> {
    static const bool value = true;

}; // struct Le<Zero, M>;

/* N >= M */
template<typename N, typename M>
struct Ge {
    static const bool value = Ge<typename N::n, typename M::n>::value;

}; // struct Ge;

template<>
struct Ge<Zero, Zero> {
    static const bool value = true;

}; // struct Ge<Zero, Zero>;

template<typename N>
struct Ge<N, Zero> {
    static const bool value = true;

}; // struct Ge<N, Zero>;

template<typename M>
struct Ge<Zero, M> {
    static const bool value = false;

}; // struct Ge<Zero, M>;

/* N < M */
template<typename N, typename M>
struct Lt {
    static const bool value = !Ge<N, M>::value;

}; // struct Lt;

/* N > M */
template<typename N, typename M>
struct Gt {
    static const bool value = !Le<N, M>::value;

}; // struct Gt;

/* Addition (N + M) */
template<typename N, typename M>
struct Add {
    typedef typename Add<typename N::n, Succ<M>>::result result;

}; // struct Add;

template<typename M>
struct Add<Zero, M> {
    typedef M result;

}; // struct Add<Zero>;

/* Multiplication (N * M) */
template<typename N, typename M>
struct Mult {
    typedef typename Add<M, typename Mult<typename N::n, M>::result>::result result;

}; // struct Mult;

template<typename M>
struct Mult<Zero, M> {
    typedef Zero result;

}; // struct Mult<Zero, M>;

/* Subtraction (N - M) */
template<typename N, typename M>
struct Sub {
    typedef typename Sub<typename N::n, typename M::n>::result result;

}; // struct Sub;

template<typename N>
struct Sub<N, Zero> {
    typedef N result;

}; // struct Sub<N, Zero>;

template<typename M>
struct Sub<Zero, M> {
    typedef Zero result;

}; // struct Sub<Zero, M>;

template<>
struct Sub<Zero, Zero> {
    typedef Zero result;

}; // struct Sub<Zero, Zero>;

/* Division (N / M) */
template<typename N, typename M, typename _NM, typename _MN>
struct Div_ {
    typedef Succ<typename Div_<_NM, M, typename Sub<_NM, M>::result, typename Sub<M, _NM>::result>::result> result;
    typedef typename Div_<_NM, M, typename Sub<_NM, M>::result, typename Sub<M, _NM>::result>::rem rem;

}; // struct Div;

template<typename N, typename M>
struct Div_<N, M, Zero, Zero> {
    typedef Succ<Zero> result;
    typedef Zero rem;

}; // struct Div_<N, M, Zero, Zero>;

template<typename N, typename M, typename _MN>
struct Div_<N, M, Zero, _MN> {
    typedef Zero result;
    typedef N rem;

}; // struct Div_<N, M, Zero, _MN>;

template<typename N, typename _NM, typename _MN>
struct Div_<N, Zero, _NM, _MN> {
    typedef void result;
    typedef void rem;

}; // struct Div_;

template<typename N, typename M>
using Div = Div_<N, M, typename Sub<N, M>::result, typename Sub<M, N>::result>;

/* Template Template Parameter Test */
template<typename N, typename M, template<typename, typename> class F>
struct Func {
    typedef typename F<N, M>::result result;

}; // struct Func;

/* Number to C++ int */
template<typename N>
struct ToInt {
    static const int value = 1 + ToInt<typename N::n>::value;

}; // struct ToInt;

template<>
struct ToInt<Zero> {
    static const int value = 0;

}; // struct ToInt<Zero>;

/* -----------------------------------------------------*/

/* Lists */
struct Nil { };

template<typename Data, typename Tail>
struct Cons {
    typedef Data data;
    typedef Tail tail;

}; // struct Cons;

/* Fold */
template<typename List, template<typename, typename> class Func, typename Acc>
struct Fold {
    typedef typename Fold<typename List::tail, Func, typename Func<Acc, typename List::data>::result>::result result;

}; // struct Fold;

template<template<typename, typename> class Func, typename Acc>
struct Fold<Nil, Func, Acc> {
    typedef Acc result;

}; // struct Fold<Nil, Func, Acc>;

template<typename List>
using Sum = Fold<List, Add, Zero>;

/* Map */
template<typename List, template<typename> class Func>
struct Map {
    typedef Cons<typename Func<typename List::data>::result, typename Map<typename List::tail, Func>::result> result;

}; // struct Map;

template<template<typename> class Func>
struct Map<Nil, Func> {
    typedef Nil result;

}; // struct Map<Nil, Func>;

template<typename N>
using Double = Add<Succ<Succ<Zero>>, N>;

template<typename List>
using DoubleList = Map<List, Double>;

/* Filter */
template<typename List, template<typename> class Func>
struct Filter;

template<typename List, template<typename> class Func, bool matches>
struct FilterCheck { };

template<typename List, template<typename> class Func>
struct FilterCheck<List, Func, true> {
    typedef Cons<typename List::data, typename Filter<typename List::tail, Func>::result> result;

}; // struct FilterCheck<List, Func, true>;

template<typename List, template<typename> class Func>
struct FilterCheck<List, Func, false> {
    typedef typename Filter<typename List::tail, Func>::result result;

}; // struct FilterCheck<List, Func, false>;

template<typename List, template<typename> class Func>
struct Filter {
    typedef typename FilterCheck<List, Func, Func<typename List::data>::value>::result result;

}; // struct Filter;

template<template<typename> class Func>
struct Filter<Nil, Func> {
    typedef Nil result;

}; // struct Filter<Nil, Func>;

template<typename N>
using Eq2 = Eq<Succ<Succ<Zero>>, N>;

template<typename List>
using ListEq2 = Filter<List, Eq2>;

/* Append */
template<typename List1, typename List2>
struct Append {
    typedef Cons<typename List1::data, typename Append<typename List1::tail, List2>::result> result;

}; // struct Append;

template<typename List2>
struct Append<Nil, List2> {
    typedef List2 result;

}; // struct Append<Nil, List2>;

/* Pair */
template<typename A, typename B>
struct Pair {
    typedef A first;
    typedef B second;

}; // struct Pair;

/* Mergesort! (Why not) */

template<typename List, typename P = Pair<Nil, Nil>>
struct Split {
    typedef typename Split<typename List::tail::tail, Pair<Cons<typename List::data, typename P::first>, Cons<typename List::tail::data, typename P::second>>>::result result; 

}; // struct Split;

template<typename P, typename D>
struct Split<Cons<D, Nil>, P> {
    typedef Pair<Cons<D, typename P::first>, typename P::second> result;

}; // struct Split<Cons<D, Nil>, P>;

template<typename P>
struct Split<Nil, P> {
    typedef P result;

}; // struct Split<Nil, P>;

template<typename List1, typename List2, template<typename, typename> class Func>
struct Merge;

template<typename List1, typename List2, template<typename, typename> class Func, bool match>
struct MergeCheck { };

template<typename List1, typename List2, template<typename, typename> class Func>
struct MergeCheck<List1, List2, Func, true> {
    typedef Cons<typename List1::data, typename Merge<typename List1::tail, List2, Func>::result> result;

}; // struct MergeCheck<List1, List2, Func, true>;

template<typename List1, typename List2, template<typename, typename> class Func>
struct MergeCheck<List1, List2, Func, false> {
    typedef Cons<typename List2::data, typename Merge<List1, typename List2::tail, Func>::result> result;

}; // struct MergeCheck<List1, List2, Func, true>;

template<typename List1, typename List2, template<typename, typename> class Func>
struct Merge {
    typedef typename MergeCheck<List1, List2, Func, Func<typename List1::data, typename List2::data>::value>::result result;

}; // struct Merge;

template<typename List1, template<typename, typename> class Func>
struct Merge<List1, Nil, Func> {
    typedef List1 result;

}; // struct Merge<List1, Nil, Func>;

template<typename List2, template<typename, typename> class Func>
struct Merge<Nil, List2, Func> {
    typedef List2 result;

}; // struct Merge<List1, Nil, Func>;

template<template<typename, typename> class Func>
struct Merge<Nil, Nil, Func> {
    typedef Nil result;

}; // struct Merge<List1, Nil, Func>;

template<typename List, template<typename, typename> class Func>
struct Mergesort {
    typedef typename Merge<typename Mergesort<typename Split<List>::result::first, Func>::result, typename Mergesort<typename Split<List>::result::second, Func>::result, Func>::result result;

}; // struct Mergesort;

template<typename D, template<typename, typename> class Func>
struct Mergesort<Cons<D, Nil>, Func> {
    typedef Cons<D, Nil> result;
};

template<template<typename, typename> class Func>
struct Mergesort<Nil, Func> {
    typedef Nil result;
};

int main(int argc, char* argv[]) {
    std::cout << ToInt<Zero>::value << std::endl;
    std::cout << ToInt<Succ<Succ<Succ<Succ<Succ<Zero>>>>>>::value << std::endl;
    std::cout << std::endl;

    std::cout << "1 == 2? " << Eq<Succ<Zero>, Succ<Succ<Zero>>>::value << std::endl;
    std::cout << "2 == 2? " << Eq<Succ<Succ<Zero>>, Succ<Succ<Zero>>>::value << std::endl;
    std::cout << "2 == 1? " << Eq<Succ<Succ<Zero>>, Succ<Zero>>::value << std::endl;
    std::cout << std::endl;

    std::cout << "2 + 3 = " << ToInt<Add<Succ<Succ<Zero>>, Succ<Succ<Succ<Zero>>>>::result>::value << std::endl;
    std::cout << "3 * 4 = " << ToInt<Mult<Succ<Succ<Succ<Zero>>>, Succ<Succ<Succ<Succ<Zero>>>>>::result>::value << std::endl;
    std::cout << "5 - 2 = " << ToInt<Sub<Succ<Succ<Succ<Succ<Succ<Zero>>>>>, Succ<Succ<Zero>>>::result>::value << std::endl;
    std::cout << "2 - 3 = " << ToInt<Sub<Succ<Succ<Zero>>, Succ<Succ<Succ<Zero>>>>::result>::value << std::endl;
    std::cout << "6 / 2 = " << ToInt<Div<Succ<Succ<Succ<Succ<Succ<Succ<Zero>>>>>>, Succ<Succ<Zero>>>::result>::value << " rem "
                                << ToInt<Div<Succ<Succ<Succ<Succ<Succ<Succ<Zero>>>>>>, Succ<Succ<Zero>>>::rem>::value << std::endl;
    std::cout << "5 / 2 = " << ToInt<Div<Succ<Succ<Succ<Succ<Succ<Zero>>>>>, Succ<Succ<Zero>>>::result>::value << " rem "
                                << ToInt<Div<Succ<Succ<Succ<Succ<Succ<Zero>>>>>, Succ<Succ<Zero>>>::rem>::value << std::endl;
    std::cout << std::endl;

    std::cout << "3 + 1 = " << ToInt<Func<Succ<Succ<Succ<Zero>>>, Succ<Zero>, Add>::result>::value << std::endl;
    std::cout << std::endl;

    std::cout << "Sum [1;2;3] = " << ToInt<Sum<Cons<Succ<Zero>, Cons<Succ<Succ<Zero>>, Cons<Succ<Succ<Succ<Zero>>>, Nil>>>>::result>::value << std::endl;
    std::cout << "Sum Double [1;2;3] = " << ToInt<Sum<DoubleList<Cons<Succ<Zero>, Cons<Succ<Succ<Zero>>, Cons<Succ<Succ<Succ<Zero>>>, Nil>>>>::result>::result>::value << std::endl;
    std::cout << "Sum (Filter Eq2) [1;2;2] = " << ToInt<Sum<ListEq2<Cons<Succ<Zero>, Cons<Succ<Succ<Zero>>, Cons<Succ<Succ<Zero>>, Nil>>>>::result>::result>::value << std::endl;
    std::cout << "Sum [1;2]++[2;3] = " << ToInt<Sum<Append<Cons<Succ<Zero>, Cons<Succ<Succ<Zero>>, Nil>>, Cons<Succ<Succ<Zero>>, Cons<Succ<Succ<Succ<Zero>>>, Nil>>>::result>::result>::value << std::endl;
    std::cout << std::endl;

    #define ListTest Cons<Succ<Succ<Zero>>, Cons<Succ<Succ<Succ<Zero>>>, Cons<Succ<Zero>, Nil>>>
    std::cout << ToInt<Split<ListTest>::result::first::data>::value << " " << ToInt<Split<ListTest>::result::first::tail::data>::value << std::endl;
    std::cout << ToInt<Split<ListTest>::result::second::data>::value << std::endl;
    std::cout << std::endl;

    std::cout << ToInt<Mergesort<ListTest, Le>::result::data>::value << std::endl;
    std::cout << ToInt<Mergesort<ListTest, Le>::result::tail::data>::value << std::endl;
    std::cout << ToInt<Mergesort<ListTest, Le>::result::tail::tail::data>::value << std::endl;
    std::cout << std::endl;

    std::cout << ToInt<Mergesort<ListTest, Gt>::result::data>::value << std::endl;
    std::cout << ToInt<Mergesort<ListTest, Gt>::result::tail::data>::value << std::endl;
    std::cout << ToInt<Mergesort<ListTest, Gt>::result::tail::tail::data>::value << std::endl;
    std::cout << std::endl;

    /* Causes compilation error :) */
    //std::cout << "1 / 0 = " << ToInt<Div<Succ<Zero>, Zero>::result>::value << std::endl; 
    
} // int main(int argc, char* argv[]);
