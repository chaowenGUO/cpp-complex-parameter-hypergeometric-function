/*module;
#include <boost/numeric/odeint.hpp>
export module hyp2f1;

namespace {

std::array<std::complex<double>, 2> hypser(std::complex<double> const &A, std::complex<double> const &B,
                                           std::complex<double> const &C, std::complex<double> const &Z)
{
    std::complex<double> series, deriv, fac{1};
    auto temp{fac}, a{A}, b{B}, c{C};
    for (std::size_t n{1}; n <= 1000; ++n)
    {
        fac *= a * b / c;
        deriv += fac;
        fac *= 1. / n * Z;
        series = temp + fac;
        if (series == temp) return {series, deriv};
        temp = series;
        a += 1;
        b += 1;
        c += 1;
    }
    throw("convergence failure in hypser");
}

} // namespace

export std::complex<double> hyp2f1(std::complex<double> const &A, std::complex<double> const &B,
                                   std::complex<double> const &C, std::complex<double> const &Z)
{
    std::complex<double> z0;
    if (std::norm(Z) <= 0.25) return hypser(A, B, C, Z).front();
    else if (std::real(Z) < 0) z0 = -0.5;
    else if (std::real(Z) <= 1) z0 = 0.5;
    else
    {
        using namespace std::literals;
        z0 = std::imag(Z) >= 0 ? 0.5i : -0.5i;
    }
    auto const Dz{Z - z0};
    auto dependentVariable{hypser(A, B, C, z0)};
    boost::numeric::odeint::integrate_adaptive(boost::numeric::odeint::bulirsch_stoer<decltype(dependentVariable)>{1e-14, 1e-14},
        [&](auto const &DependentVariable, auto &derivative, double const Variable)
        {
            derivative.front() = DependentVariable.back() * Dz;
            auto const z{z0 + Variable * Dz};
            derivative.back() = (A * B * DependentVariable.front() - (C - (A + B + 1.) * z) * DependentVariable.back()) * Dz / (z * (1. - z));
        },
        dependentVariable, 0., 1., 0.1);
    return dependentVariable.front();
}*/

#include <boost/numeric/odeint.hpp>

typedef std::vector< double > state_type;

const double gam = 0.15;

void harmonic_oscillator( const state_type &x , state_type &dxdt , const double )
{
    dxdt[0] = x[1];
    dxdt[1] = -x[0] - gam*x[1];
}

class harm_osc {

    double m_gam;

public:
    harm_osc( double gam ) : m_gam(gam) { }

    void operator() ( const state_type &x , state_type &dxdt , const double /* t */ )
    {
        dxdt[0] = x[1];
        dxdt[1] = -x[0] - m_gam*x[1];
    }
};

struct push_back_state_and_time
{
    std::vector< state_type >& m_states;
    std::vector< double >& m_times;

    push_back_state_and_time( std::vector< state_type > &states , std::vector< double > &times )
    : m_states( states ) , m_times( times ) { }

    void operator()( const state_type &x , double t )
    {
        m_states.push_back( x );
        m_times.push_back( t );
    }
};
