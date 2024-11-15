#include "Boids.hpp"


double Boids::distance(Boids const &b) const {
  return std::sqrt(std::pow((position_.x - b.p().x), 2) +
                   std::pow((position_.y - b.p().y), 2));
}

bool Boids::find_boids(Boids const &b) const {
  if (distance(b) <= fov_.radius) {  // se distanza non è > raggio FOV, calcola
                                     // vettore congiungente
    Coordinate vec_temp{b.p().x - position_.x, b.p().y - position_.y};

    double ang =  // formula inversa prodotto scalare congiungente-velocità
        std::acos((velocity_.x * vec_temp.x + velocity_.y * vec_temp.y) /
                  (module(velocity_) * module(vec_temp)));

    return (ang <= fov_.angle);
  } else
    return false;
}

double Boids::direction() const {
  double alpha = (std::acos(v().x / module(v())) * (180 / M_PI));

  if ((v().x >= 0. && v().y >= 0.) || (v().x < 0 && v().y >= 0.)) {
    alpha = alpha + 90.;
  } else if ((v().x >= 0. && v().y < 0.) || (v().x < 0 && v().y < 0.)) {
    alpha = 90. - alpha;
  }
  // nota: valori sommati ad alpha sono stati ricavati empiricamente
  return alpha;
}

void Boids::operator=(
    Boids const &b) {  // copia un boid, assegnazione valore e copia  // non lo fa già operatore copia?
                       // implicita (nota del compilatore)
  position_ = b.p();
  velocity_ = b.v();
}
