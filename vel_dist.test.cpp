// opzioni di compilazione: g++ -Wall -Wextra -fsanitize=address flock.test.cpp flock.cpp boids.cpp

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "Flock.hpp"
#include <numeric>

#include "doctest.h"

struct means_RMS {
  double velocity;
  double v_RMS;
  double distance;
  double d_RMS;
};

means_RMS const calculate(Flock f) {
  int s = f.size();
  double size = static_cast<double>(s);
  std::vector<Boids> flock_ = f.state();
  Boids total_sum = std::accumulate(flock_.begin(), flock_.end(), Boids{});
  double sum_v = module(total_sum.v());  // somma delle velocità
  double sum_v_square{0.};               // somma dei quadrati delle velocità

  double sum_d{0.};         // somma delle distanze
  double sum_d_square{0.};  // somma dei quadrati delle distanze
  int n_steps{0};
  // numero di distanze calcolate == sommatoria da 0 a (size-1)

  for (int i{0}; i != size; ++i) {
    sum_v += module(flock_[i].v());
    sum_v_square += std::pow(module(flock_[i].v()), 2);

    for (int j{i}; j < size - 1; ++j) {
      sum_d += flock_[i].distance(flock_[j + 1]);
      sum_d_square += std::pow((flock_[i].distance(flock_[j + 1])), 2);
      ++n_steps;
    }
  }

  double v_mean = sum_v / size;
  double v_RMS{0.};
  double d_mean{0.};
  double d_RMS{0.};
  if (size > 1) {
    v_RMS = std::sqrt((size / (size - 1)) *
                      ((sum_v_square / size) - std::pow(v_mean, 2)));
    // varianza = (n/(n-1))*(media quadrati - quadrato media)
    d_mean = sum_d / n_steps;
  }

  double steps = static_cast<double>(n_steps);
  if (n_steps > 2) {
    d_RMS = std::sqrt((steps / (steps - 1)) *
                      ((sum_d_square / steps) - std::pow(d_mean, 2)));
  }
  return {v_mean, v_RMS, d_mean, d_RMS};
}

TEST_CASE("Testing mean distance and velocity") {
  SUBCASE("One boid") {
    Coordinate pos1{4., 4.};
    Coordinate vel1{1., 1.};
    Boids b1(pos1, vel1);
    Flock flock({.3, .4, .5}, {800, 600});
    flock.push_back(b1);
    means_RMS s = calculate(flock);
    CHECK(s.velocity == doctest::Approx(1.414213562));
    CHECK(s.v_RMS == doctest::Approx(0.));
    CHECK(s.distance == doctest::Approx(0.));
    CHECK(s.d_RMS == doctest::Approx(0.));
  }

  SUBCASE("Two boids") {
    Coordinate pos1{4., 4.};
    Coordinate vel1{1., 1.};
    Boids b1(pos1, vel1);
    Coordinate pos2{2., 3.};
    Coordinate vel2{1., 0.};
    Boids b2(pos2, vel2);
    Flock flock({.3, .4, .5}, {800, 600});
    flock.push_back(b1);
    flock.push_back(b2);
    means_RMS s = calculate(flock);
    CHECK(s.velocity == doctest::Approx(1.207106781));
    CHECK(s.v_RMS == doctest::Approx(0.292893219));
    CHECK(s.distance == doctest::Approx(sqrt(5)));
    CHECK(s.d_RMS == doctest::Approx(0.));
  }

  SUBCASE("Three boids") {
    Coordinate pos1{4., 4.};
    Coordinate vel1{1., 1.};
    Boids b1(pos1, vel1);
    Coordinate pos2{2., 3.};
    Coordinate vel2{1., 0.};
    Boids b2(pos2, vel2);
    Coordinate pos3{2., 2.};
    Coordinate vel3{0., 1.};
    Boids b3{pos3, vel3};
    Flock flock({.3, .4, .5}, {800, 600});
    flock.push_back(b1);
    flock.push_back(b2);
    flock.push_back(b3);
    means_RMS s = calculate(flock);
    CHECK(s.velocity == doctest::Approx(1.138071187));
    CHECK(s.v_RMS == doctest::Approx(0.2391463117));
    CHECK(s.distance == doctest::Approx(2.021498367));
    CHECK(s.d_RMS == doctest::Approx(0.932907566));
  }
}