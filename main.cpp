#include<iostream>
#include<complex>
import hyp2f1;

int main()
{
    using namespace std::literals;
    std::cout << hyp2f1(1. + 1i, 0.75, 10i, 1. + 5i) << std::endl;
}

//g++ -std=c++2a -O2 -march=native -ftree-vectorize -ffast-math
//clang++-libc++ -std=c++2a -O2 -march=native -fno-math-errno
