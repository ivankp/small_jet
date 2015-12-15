#ifndef SMALL_JET_HH
#define SMALL_JET_HH

#include <array>
#include <cmath>
#include <type_traits>
#include <limits>

namespace small_jet {

constexpr double twopi   = M_PI*2;
constexpr double max_rap = 1e5; // From FastJet
constexpr double max_pt  = 1e100;

template <typename T> inline T sq(T x) noexcept { return x*x; }
template <typename T, typename... TT>
inline T sq(T x, TT... xx) noexcept { return sq(x)+sq(xx...); }

#define small_fcn(name, expr) \
  template <typename T> inline auto \
    name(const T& p) noexcept -> decltype(expr) \
    { return expr; }

small_fcn( px, p[0] )
small_fcn( py, p[1] )
small_fcn( pz, p[2] )
small_fcn( E , p[3] )

small_fcn( pt2, sq(px(p),py(p)) )

#undef small_fcn

template <typename T>
inline double phi(const T& p) noexcept {
  double _phi = atan2(py(p),px(p));
  if (__builtin_expect(_phi < 0.,0)) _phi += twopi;
  else if (__builtin_expect(_phi >= twopi,0)) _phi -= twopi;
  return _phi;
}
template <typename T>
inline double phi(const T& p, double pt2) noexcept {
  return ( __builtin_expect(pt2==0.,0) ? 0. : phi(p) );
}

template <typename T>
double rap(const T& p, double pt2) noexcept {
  // Replicated FastJet rapidity calculation
  // for compatibility in cases of unphysical 4-momenta

  double _rap;
  const double abs_pz = abs(pz(p));

  if (E(p) == abs_pz && pt2 == 0.) {
    // Point has infinite rapidity -- convert that into a very large
    // number, but in such a way that different 0-pt momenta will have
    // different rapidities (so as to lift the degeneracy between
    // them) [this can be relevant at parton-level]
    _rap = max_rap + abs_pz;
    if (pz(p) < 0.) _rap = -_rap;
  } else {
    // get the rapidity in a way that's modestly insensitive to roundoff
    // error when things pz,E are large (actually the best we can do without
    // explicit knowledge of mass) and force non tachyonic mass
    double m2_pt2 = (E(p)+pz(p))*(E(p)-pz(p));
    if (m2_pt2 < pt2) m2_pt2 = pt2;
    _rap = 0.5*log(m2_pt2/sq(E(p)+abs_pz));
    if (pz(p) > 0.) _rap = -_rap;
  }
  return rap;
}

template <int P> inline std::enable_if<(P > 1),double>::type
diB(double pt2) noexcept { return pow(pt2,P); }

template <int P> inline std::enable_if<(P < -1),double>::type
diB(double pt2) noexcept
{ return ( __builtin_expect(pt2==0.,0) ? max_pt : pow(pt2,P) ); }

template <int P> inline std::enable_if<(P == 1),double>::type
diB(double pt2) noexcept { return pt2; }

template <int P> inline std::enable_if<(P == -1),double>::type
diB>(double pt2) noexcept
{ return ( __builtin_expect(pt2==0.,0) ? max_pt : 1./pt2 ); }

template <int P> inline std::enable_if<(P == 0),double>::type
diB(double pt2) noexcept { return 1.; }

// Class definition -------------------------------------------------

template <unsigned MaxNPart>
class jet_alg {
public:
  enum { size = MaxNPart };

private:
  std::array<bool,size> _ok;
  std::array<double,size> _phi, _rap, _diB;
  std::array<std::array<double,4>,size> _pp; // particles and pseudo-jets
  std::array<std::array<double,size>,size> _dij;

public:
  // P = -1 : anti-kt
  // P =  0 : Cambridge/Aachen
  // P =  1 : kt
  template <int Kt2Pow, typename InputIterator>
  cluster(InputIterator first, InputIterator last, OutputIterator result) {
    unsigned i = 0;
    for (auto it=first; it!=last; ++it, ++i) {
      if (i==size) throw std::out_of_range(
        "small_jet::jet_alg::particles(): allocated number of particles exceeded"
      );
      _ok[i]  = true;
      _pp[i]  = { px(*it), py(*it), pz(*it), E(*it) };
      const double _pt2 = pt2(_pp[i]);
      _phi[i] = phi(_pp[i],pt2);
      _rap[i] = rap(_pp[i],pt2);
      _diB[i] = diB<Kt2Pow>(pt2);
    }
    for (; i<size; ++i) _ok[i] = false;

    while (i>0) {
      // find dij
      
    }
  }

};

} // end namespace

#endif
