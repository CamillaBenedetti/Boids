#ifndef BOIDS_HPP
#define BOIDS_HPP

#include <cmath>

struct Coordinate {
  double x{};
  double y{};

  auto& operator+=(Coordinate const& rhs) { // per fare operatori simmetrici, non sono inline perché dentro la struct
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
  
  auto& operator*=(double const value) { // questo serve?
    x *= value;
    y *= value;
    return *this;
  }

};

template<class T>
inline auto operator+(T const& lhs, T const& rhs) { // Lo fa anche per i coordinate
  auto result{lhs};
  result += rhs;
  return result;
}

inline auto operator*(double const value, Coordinate const c1) {
  auto result{c1};
  result *= value;
  return result;
}

inline Coordinate operator-(Coordinate const c1, Coordinate const c2) {
  return Coordinate{c1 + (-1.) * c2};
}

inline bool operator==(Coordinate const c1, Coordinate const c2) {
  return (c1.x == c2.x && c1.y == c2.y);
}

inline bool operator!=(Coordinate const c1, Coordinate const c2) {
  return !(c1 == c2);
}

inline double module(Coordinate const c1) {
  return std::sqrt(c1.x * c1.x + c1.y * c1.y);
}

struct FOV {  // = Field Of View
  double const angle;
  double const radius;
};

class Boids {
  Coordinate position_;
  Coordinate velocity_;
  FOV const fov_{(M_PI * 3 / 4), 150.};

 public:
  Boids(Coordinate p = {0, 0}, Coordinate v = {0, 0})
      : position_{p}, velocity_{v} {}
  Boids(Boids const& boid) : position_{boid.p()}, velocity_{boid.v()} {} // copy constructor, vanno definiti gli altri 4?
    auto& operator+=(Boids const& rhs) { 
    p() += rhs.p();
    v() += rhs.v();
    return *this;
  }

  Coordinate p() const { return position_; } 
  Coordinate v() const { return velocity_; }

  // Calcola la distanza tra due boids
  double distance(Boids const& b) const;

  // Verifica se un altro boid è nel suo FOV
  bool find_boids(Boids const& b) const;

  // restituisce angolo di rotazione adeguato a
  // sistema di riferimento di SFML [angolo 0 asse y verso l'alto
  // (coordinate negative) crescente in senso orario]
  double direction() const;

  void operator=(Boids const& b);
};

inline bool operator==(Boids const& b1, Boids const& b2) {
  return (b1.p() == b2.p()); // vogliamo verificare solo se la posizione coincide, la velocità può essere diversa
}

inline bool operator!=(Boids const& b1, Boids const& b2) { return !(b1 == b2); }

#endif