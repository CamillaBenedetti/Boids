// opzioni di compilazione: g++ -Wall -Wextra -fsanitize=address flock.test.cpp
// flock.cpp boids.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "Flock.hpp"

#include "doctest.h"

TEST_CASE("Testing Flock") {
  SUBCASE("Testing flock of one") {
    Coordinate pos1{2., 1.};
    Coordinate vel1{2., 0.};
    Boids b1(pos1, vel1);
    Flock f({.7, .5, .2}, {800, 600});
    f.push_back(b1);
    f.evolve(0.1);
    CHECK(f.state()[0].p().x == doctest::Approx(2.2));
    CHECK(f.state()[0].p().y == doctest::Approx(1.));
    CHECK(f.state()[0].v().x == doctest::Approx(2.));  // velocità costante
    CHECK(f.state()[0].v().y == doctest::Approx(0.));
    f.evolve(0.1);
    CHECK(f.state()[0].p().x == doctest::Approx(2.4));
    CHECK(f.state()[0].p().y == doctest::Approx(1.));
    CHECK(f.state()[0].v().x == doctest::Approx(2.));  // velocità costante
    CHECK(f.state()[0].v().y == doctest::Approx(0.));
  }

  SUBCASE("Testing flock of two") {
    Coordinate pos1{2., 1.};
    Coordinate vel1{2., 0.};
    Boids b1(pos1, vel1);
    Coordinate pos2{1., 2.};
    Coordinate vel2{0., 2.};
    Boids b2(pos2, vel2);
    Flock f({.7, .5, .2}, {800, 600});
    f.push_back(b1);
    f.push_back(b2);
    f.evolve(0.1);
    CHECK(b1.find_boids(b2) == true);
    CHECK(b2.find_boids(b1) == true);
    CHECK(f.state()[0].v().x == doctest::Approx(1.5));
    CHECK(f.state()[0].v().y == doctest::Approx(0.5));
    CHECK(f.state()[1].v().x == doctest::Approx(0.5));
    CHECK(f.state()[1].v().y == doctest::Approx(1.5));
  }

  SUBCASE("Testing flock of two, evolving twice") {
    Coordinate pos1{1., 1.};
    Coordinate vel1{1., 1.};
    Boids b1(pos1, vel1);
    Coordinate pos2{2., 1.};
    Coordinate vel2{1., 0.};
    Boids b2(pos2, vel2);
    Flock flock{{0.7, 0.5, 0.2}, {200., 200.}};
    flock.push_back(b1);
    flock.push_back(b2);
    flock.evolve(1.);
    CHECK(flock.state()[0].p().x == doctest::Approx(2.));
    CHECK(flock.state()[0].p().y == doctest::Approx(2.));
    CHECK(flock.state()[0].v().x == doctest::Approx(0.5));
    CHECK(flock.state()[0].v().y == doctest::Approx(0.5));
    CHECK(flock.state()[1].p().x == doctest::Approx(3.));
    CHECK(flock.state()[1].p().y == doctest::Approx(1.));
    CHECK(flock.state()[1].v().x == doctest::Approx(1.7));
    // il secondo boid non vede il primo, usa solo separation
    CHECK(flock.state()[1].v().y == doctest::Approx(0.));
  }

  SUBCASE("Testing flock of three") {
    Coordinate pos1{-2., -2.};
    Coordinate vel1{1., 1.};
    Boids b1(pos1, vel1);
    Coordinate pos2{-2., 3.};
    Coordinate vel2{1., 0.};
    Boids b2(pos2, vel2);
    Coordinate pos3{2., 2.};
    Coordinate vel3{0., 1.};
    Boids b3{pos3, vel3};
    Flock flock({.3, .4, .5}, {800, 600});
    CHECK(b1.find_boids(b2) == true);
    CHECK(b3.find_boids(b2) == true);
    flock.push_back(b1);
    flock.push_back(b2);
    flock.push_back(b3);
    flock.evolve(1.);
    CHECK(flock.state()[0].v().x == doctest::Approx(0.6));
    CHECK(flock.state()[0].v().y == doctest::Approx(0.35));
    CHECK(flock.state()[1].v().x ==
          doctest::Approx(
              -0.4));  // comportamento dovuto alla posizione negativa al bordo
    CHECK(flock.state()[1].v().y ==
          doctest::Approx(
              2.2));  // comportamento dovuto alla posizione negativa al bordo
    CHECK(flock.state()[2].v().x == doctest::Approx(0.8));
    CHECK(flock.state()[2].v().y == doctest::Approx(0.95));
  }
}

TEST_CASE("Testing borders") {
  SUBCASE("Testing left") {
    Coordinate pos1{-1., 2.};
    Coordinate vel1{-1., 0.};
    Boids b1(pos1, vel1);
    Flock flock{{0.7, 0.5, 0.2}, {200., 200.}};
    flock.push_back(b1);
    flock.evolve(1.);
    CHECK(flock.state()[0].p().x == doctest::Approx(198.));
    CHECK(flock.state()[0].p().y == doctest::Approx(2.));
    CHECK(flock.state()[0].v().x == doctest::Approx(-1.));
    CHECK(flock.state()[0].v().y == doctest::Approx(0.));
  }

  SUBCASE("Testing right") {
    Coordinate pos1{201., 2.};
    Coordinate vel1{1., 0.};
    Boids b1(pos1, vel1);
    Flock flock{{0.7, 0.5, 0.2}, {200., 200.}};
    flock.push_back(b1);
    flock.evolve(1.);
    CHECK(flock.state()[0].p().x == doctest::Approx(2.));
    CHECK(flock.state()[0].p().y == doctest::Approx(2.));
    CHECK(flock.state()[0].v().x == doctest::Approx(1.));
    CHECK(flock.state()[0].v().y == doctest::Approx(0.));
  }

  SUBCASE("Testing down-right") {
    Coordinate pos1{201., -1.};
    Coordinate vel1{1., -1.};
    Boids b1(pos1, vel1);
    Flock flock{{0.7, 0.5, 0.2}, {200., 200.}};
    flock.push_back(b1);
    flock.evolve(1.);
    CHECK(flock.state()[0].p().x == doctest::Approx(2.));
    CHECK(flock.state()[0].p().y == doctest::Approx(198.));
    CHECK(flock.state()[0].v().x == doctest::Approx(1.));
    CHECK(flock.state()[0].v().y == doctest::Approx(-1.));
  }

  SUBCASE("Testing up-left") {
    Coordinate pos1{-5., -10.};
    Coordinate vel1{1., -1.};
    Boids b1(pos1, vel1);
    Flock flock{{0.7, 0.5, 0.2}, {200., 200.}};
    flock.push_back(b1);
    flock.evolve(1.);
    CHECK(flock.state()[0].p().x == doctest::Approx(196.));
    CHECK(flock.state()[0].p().y == doctest::Approx(189.));
    CHECK(flock.state()[0].v().x == doctest::Approx(1.));
    CHECK(flock.state()[0].v().y == doctest::Approx(-1.));
  }

  SUBCASE("Testing val_control") {
    Coordinate pos1{0., 0.};
    Coordinate vel1{500, 700};
    Boids b1{pos1, vel1};
    double dir1 = b1.direction();
    Coordinate pos2{1000., 0.};
    Coordinate vel2{700, 500};
    Boids b2{pos2, vel2};
    double dir2 = b2.direction();
    Coordinate pos3{2000., 0.};
    Coordinate vel3{-500, 700};
    Boids b3{pos3, vel3};
    double dir3 = b3.direction();
    Coordinate pos4{3000., 0.};
    Coordinate vel4{-700, 500};
    Boids b4{pos4, vel4};
    double dir4 = b4.direction();
    Coordinate pos5{4000., 0.};
    Coordinate vel5{500, -700};
    Boids b5{pos5, vel5};
    double dir5 = b5.direction();
    Coordinate pos6{5000., 0.};
    Coordinate vel6{700, -500};
    Boids b6{pos6, vel6};
    double dir6 = b6.direction();
    Flock flock{{0.7, 0.5, 0.2}, {10000., 10000.}};
    flock.push_back(b1);
    flock.push_back(b2);
    flock.push_back(b3);
    flock.push_back(b4);
    flock.push_back(b5);
    flock.push_back(b6);
    flock.evolve(1.);
    double d_f1 = flock.state()[0].direction();
    double d_f2 = flock.state()[1].direction();
    double d_f3 = flock.state()[2].direction();
    double d_f4 = flock.state()[3].direction();
    double d_f5 = flock.state()[4].direction();
    double d_f6 = flock.state()[5].direction();
    CHECK((d_f1) == doctest::Approx(dir1));
    CHECK(module(flock.get_()[0].v()) == doctest::Approx(700.));
    CHECK((d_f2) == doctest::Approx(dir2));
    CHECK(module(flock.state()[1].v()) == doctest::Approx(700.));
    CHECK((d_f3) == doctest::Approx(dir3));
    CHECK(module(flock.state()[2].v()) == doctest::Approx(700.));
    CHECK((d_f4) == doctest::Approx(dir4));
    CHECK(module(flock.state()[3].v()) == doctest::Approx(700.));
    CHECK((d_f5) == doctest::Approx(dir5));
    CHECK(module(flock.state()[4].v()) == doctest::Approx(700.));
    CHECK((d_f6) == doctest::Approx(dir6));
    CHECK(module(flock.state()[5].v()) == doctest::Approx(700.));
  }
}
