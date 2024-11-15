// opzioni di compilazione: g++ -Wall -Wextra -fsanitize=address boids.test.cpp
// boids.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "Boids.hpp"

#include "doctest.h"

TEST_CASE("Testing operators and module for coord") {
  Coordinate p1{1., 1.};
  Coordinate p2{20., 0.1111};
  Coordinate p3{-50., -0.0000006};
  auto p = p1 + p2;
  auto pp = p + p3;
  auto po = pp - p3;
  CHECK(p.x == doctest::Approx(21.));
  CHECK(p.y == doctest::Approx(1.1111));
  CHECK(pp.x == doctest::Approx(-29.));
  CHECK(pp.y == doctest::Approx(1.11110994));
  CHECK((p == po) == true);
  CHECK((p != po) == false);
  CHECK((0.9 * p == 0.9 * po) ==
        true); 
  CHECK((p != pp) == true);
  CHECK((p == pp) == false);
  CHECK((0. * p == 0. * pp) ==
        true);  // moltiplicazione per 0, la divisione non è prevista
  CHECK(module(p1) == doctest::Approx(std::sqrt(2)));
}

TEST_CASE("Testing boids") {
  SUBCASE("Testing distance and find_boids") {
    Coordinate pos1{1., 1.};
    Coordinate vel1{3., 4.};
    Boids b1(pos1, vel1);
    CHECK(
        b1.p().x ==
        doctest::Approx(1.));  // verifico la corrretta assegnazione dei valori
    CHECK(b1.p().y == doctest::Approx(1.));
    CHECK(b1.v().x == doctest::Approx(3.));
    CHECK(b1.v().y == doctest::Approx(4.));
    Coordinate pos2{2., 2.};
    Boids b2(pos2, vel1);
    CHECK(b1.distance(b2) == doctest::Approx(std::sqrt(2)));
    CHECK(b2.distance(b1) == doctest::Approx(std::sqrt(2)));
    CHECK(b1.find_boids(b2) == true);
    CHECK(b2.find_boids(b1) ==
          false);  // il secondo boid non vede il boid 1 che ha dietro
    Coordinate pos3{1., -3.};
    Boids b3{pos3, vel1};
    CHECK(b1.distance(b3) == doctest::Approx(4));
    CHECK(b2.distance(b3) == doctest::Approx(std::sqrt(26)));
    CHECK(b1.find_boids(b3) == false);  // il boid 3 è dietro al 1 e al 2
    CHECK(b2.find_boids(b3) == false);
  }

  SUBCASE("Testing more boids") {
    Coordinate pos1{2., 1.};
    Coordinate vel1{2., 0.};
    Boids b1(pos1, vel1);
    Coordinate pos2{1., 2.};
    Coordinate vel2{0., 2.};
    Boids b2(pos2, vel2);
    Coordinate pos3{1., -3.};
    Boids b3{pos3, vel1};
    Coordinate pos4{-1., -1.};
    Boids b4{pos4, vel1};
    Coordinate pos5{-2., 2.};
    Coordinate vel5{-2., 1.};
    Boids b5{pos5, vel5};
    Boids b6{pos5, vel2};  // 5 e 6 hanno stessa posizione
    Coordinate pos7{152., 1.};
    Coordinate vel7{-2., 0.};
    Boids b7{pos7, vel7};
    Coordinate pos8{152.001, 1.};
    Boids b8{pos8, vel7};
    CHECK(b1.distance(b2) == doctest::Approx(std::sqrt(2)));
    CHECK(b1.distance(b3) == doctest::Approx(sqrt(17)));
    CHECK(b5.distance(b3) == doctest::Approx(sqrt(34)));
    CHECK(b1.find_boids(b2) == true);   // al limite destro del FOV
    CHECK(b2.find_boids(b1) == true);   // al limite sinistro del FOV
    CHECK(b1.find_boids(b4) == false);  // casi fuori dall'angolo di FOV
    CHECK(b2.find_boids(b4) == false);
    CHECK(b5.find_boids(b3) == false);
    CHECK(b6.find_boids(b4) == false);
    CHECK(b7.find_boids(b1) == true);  // al limite di distanza del fov (250.)
    CHECK(b1.find_boids(b7) == true);  // al limite di distanza del fov
    CHECK(b1.find_boids(b8) ==
          false);  // leggermente oltre il limite di distanza (250.) del fov
  }

  SUBCASE("Testing direction") {
    Coordinate pos{2., 1.};  // per testare la direzione vario solo la velocità
    Coordinate vel1{2., 0.};
    Boids b1(pos, vel1);
    Coordinate vel2{0., 2.};
    Boids b2(pos, vel2);
    Coordinate vel3{-1., 0.};
    Boids b3(pos, vel3);
    Coordinate vel4{0., -1.};
    Boids b4(pos, vel4);
    Coordinate vel5{1., 1.};
    Boids b5(pos, vel5);
    Coordinate vel6{-2., -1.};
    Boids b6(pos, vel6);
    Coordinate vel7{-1., 3.};
    Boids b7(pos, vel7);
    CHECK(b1.direction() == doctest::Approx(90.));
    CHECK(b2.direction() == doctest::Approx(180.));
    CHECK(b3.direction() == doctest::Approx(270.));
    CHECK(b4.direction() == doctest::Approx(0.));
    CHECK(b5.direction() == doctest::Approx(135.));
    CHECK(b6.direction() ==
          doctest::Approx(
              -acos(1 / sqrt(5)) * 360 /
              (2 * M_PI)));  // angolo negativo con conversione in  gradi
    CHECK(b7.direction() ==
          doctest::Approx(acos(3 / sqrt(10)) * 360 / (2 * M_PI) + 180));
  }
}
