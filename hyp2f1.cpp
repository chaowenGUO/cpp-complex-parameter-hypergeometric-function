module;
#include <boost/numeric/odeint/stepper/bulirsch_stoer.hpp>
#include <boost/numeric/odeint/integrate/integrate_adaptive.hpp>
export module hyp2f1;

std::array<std::complex<double>, 2> hypser(std::complex<double> const &A, std::complex<double> const &B, std::complex<double> const &C, std::complex<double> const &Z)
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

export auto hyp2f1(std::complex<double> const &A, std::complex<double> const &B, std::complex<double> const &C, std::complex<double> const &Z)
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
}
