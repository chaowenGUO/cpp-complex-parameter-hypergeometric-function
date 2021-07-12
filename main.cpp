/*#include<iostream>
#include<complex>

import hyp2f1;

int main()
{
    using namespace std::literals;
    std::cout << hyp2f1(1. + 1i, 0.75, 10i, 1. + 5i) << std::endl;
}*/

//compile//clang++-libc++ -std=c++2a -O2 -march=native -fmodules-ts --precompile hyp2f1.cppm
         //clang++-libc++ -std=c++2a -O2 -march=native -fmodules-ts -fprebuilt-module-path=. hyp2f1.pcm main.cpp
//test//import mpmath; mpmath.hyp2f1(1+1j, 0.75, 10j, 1+5j)

//g++ -std=c++2a -O2 -march=native -ftree-vectorize -ffast-math
//clang++-libc++ -std=c++2a -O2 -march=native -fno-math-errno

#include <boost/numeric/odeint.hpp>

int main()
{
    using namespace std;
    using namespace boost::numeric::odeint;
    state_type x(2);
    x[0] = 1.0; // start at x=1.0, p=0.0
    x[1] = 0.0;
    size_t steps = integrate( harmonic_oscillator ,
            x , 0.0 , 10.0 , 0.1 );
    harm_osc ho(0.15);
    steps = integrate( ho ,
            x , 0.0 , 10.0 , 0.1 );
    vector<state_type> x_vec;
    vector<double> times;

    steps = integrate( harmonic_oscillator ,
            x , 0.0 , 10.0 , 0.1 ,
            push_back_state_and_time( x_vec , times ) );

    /* output */
    for( size_t i=0; i<=steps; i++ )
    {
        cout << times[i] << '\t' << x_vec[i][0] << '\t' << x_vec[i][1] << '\n';
    }
}
