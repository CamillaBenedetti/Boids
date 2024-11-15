#ifndef FLOCK_HPP
#define FLOCK_HPP

#include <cassert>
#include <stdexcept>
#include <vector>
#include <array>

#include "Boids.hpp"

struct rule_parameters {
  double s {.7};  // parametro di separazione
  double a {.05};  // parametro di allineamento
  double c {.05};  // parametro di coesione
};

class Flock {
  std::vector<Boids> flock_;
  double const separation_d{30.};  // molto minore di 250. raggio fov
  rule_parameters rule_par;        // accettati in input
  double v_lim_{700.};   // velocità limite di un boid
  Coordinate p_lim_;  // larghezza e lunghezza schermo (presi in main.cpp)
  char species_{'G'};

  // Calcola distanza tra 2 boids per calcolo della velocità di separazione
  auto v_separation(Boids const& b, std::vector<Boids> const& flock) const {
    Coordinate v_sep{0., 0.};
    for (auto& boid : flock) {
      if (b != boid) {
        if (b.distance(boid) < 1E-3) {
          throw std::runtime_error{
              "Two boids have collided, try different parameters. TIP: the "
              "separation parameter should be at least 80 times greater than the other "
              "parameters"};
        }

        assert(b.distance(boid) >
               0.);  // il programma crasha quando i parametri
                     // dello stormo non sono ottimali

        if (b.distance(boid) <= separation_d) {
          v_sep += boid.p() - b.p();
        }
      }
    }
    return (-rule_par.s * v_sep);
  }

  auto v_alignment(Boids const& b, std::vector<Boids> const& flock) const {  // calcoli per i parametri messi private
    Coordinate v_alig{0., 0.};
    int counter{0};

    for (auto& boid : flock) {
      if (b != boid) {
        if (b.find_boids(boid)) {
          v_alig += boid.v();
          ++counter;
        }
      }
    }

    if (v_alig == Coordinate{0., 0.}) {
      return Coordinate{0., 0.};
    } else {
      double div = 1. / counter;
      return (rule_par.a * ((div * v_alig) - b.v()));
    }
  }

  auto v_cohesion(Boids const& b, std::vector<Boids> const& flock) const {
    Coordinate v_cohe{0., 0.};
    int counter{0};

    for (auto& boid : flock) {
      if (b != boid) {
        if (b.find_boids(boid)) {
          v_cohe += boid.p();
          ++counter;
        }
      }
    }

    if (v_cohe == Coordinate{0., 0.}) {
      return Coordinate{0., 0.};
    } else {
      double div = 1. / counter;
      return (rule_par.c * ((div * v_cohe) - b.p()));
    }
  }

 public:
  Flock(rule_parameters par, Coordinate const p_lim)
      : rule_par{par}, p_lim_{p_lim} {}  // costruttore

  void push_back(Boids const& b);

  int size() const;

  Boids const& back() const;

  void set_par(rule_parameters par);

  void change_species(char c);

  void generate(int n);

  auto val_control(Coordinate& vel, Coordinate& pos);

  void evolve(double const delta_t);

  std::array<double,4> const statistics() const;

  std::vector<Boids> const& state() const;
};

#endif