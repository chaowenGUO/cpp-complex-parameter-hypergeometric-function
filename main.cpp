#include<iostream>
#include<complex>

import hyp2f1;

int main()
{
    using namespace std::literals;
    std::cout << hyp2f1(1. + 1i, 0.75, 10i, 1. + 5i) << std::endl;
}

//compile//clang++-libc++ -std=c++2a -O2 -march=native -fmodules-ts --precompile hyp2f1.cppm
         //clang++-libc++ -std=c++2a -O2 -march=native -fmodules-ts -fprebuilt-module-path=. hyp2f1.pcm main.cpp
//test//import mpmath; mpmath.hyp2f1(1+1j, 0.75, 10j, 1+5j)
