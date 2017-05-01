#include<boost/numeric/odeint.hpp>

namespace{

std::complex<double> hypser(std::complex<double> const &a, std::complex<double> const &b, std::complex<double> const &c, std::complex<double> const &z, std::complex<double> &series)
{
  std::complex<double> deriv=0.0;
  std::complex<double> fac=1.0;
  std::complex<double> temp=fac;
  std::complex<double> aa=a;
  std::complex<double> bb=b;
  std::complex<double> cc=c;
  for (int n=1;n<=1000;n++)
  {
    fac *= (aa*bb)/cc;
    deriv += fac;
    fac *= (1.0/n)*z;
    series=temp+fac;
    if (series == temp) return deriv;
    temp=series;
    aa += 1.0;
    bb += 1.0;
    cc += 1.0;
  }
  throw("convergence failure in hypser");
}

}//namespace

std::complex<double> hypgeo(std::complex<double> const &a, std::complex<double> const &b,std::complex<double> const &c, std::complex<double> const &z)
{
  std::complex<double> ans,dz,z0;
  std::array<std::complex<double>,2>y;
  if (std::norm(z) <= 0.25)
  {
    y.back()=hypser(a,b,c,z,ans);
    return ans;
  }
  else if (std::real(z) < 0) z0=-0.5;
  else if (std::real(z) <= 1) z0=0.5;
  else z0=std::complex<double>(0.0,std::imag(z) >= 0.0 ? 0.5 : -0.5);
  dz=z-z0;
  y.back()=hypser(a,b,c,z0,y.front());
  boost::numeric::odeint::integrate_adaptive(boost::numeric::odeint::bulirsch_stoer<std::array<std::complex<double>,2>>{1e-14,1e-14},
      [&](std::array<std::complex<double>,2> const&y,std::array<std::complex<double>,2>&dyds,double const s)
      {
	dyds.front()=y.back()*dz;
	auto const z{z0+s*dz};
	dyds.back()=(a*b*y.front()-(c-(a+b+1.)*z)*y.back())*dz/(z*(1.-z));
      }, y ,0.,1.,0.1);
  return y.front();
}

int main()
{
  using namespace std::literals;
  std::cout<<hypgeo(1.+1i,0.75,10i,1.+5i)<<std::endl;
}
