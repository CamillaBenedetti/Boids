#include "Flock.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <algorithm>

void Flock::push_back(Boids const& b) { flock_.push_back(b); }

int Flock::size() const {
  return flock_.size();
}  // ritorna il numero di boids dello stormo

Boids const& Flock::back() const {
  return flock_.back();
}  // ritorna ultimo boid del vector flock, serve in key-A (main.cpp)


void Flock::set_par(rule_parameters par) { rule_par = par; }

void Flock::change_species(char c) { species_ = c; }

void Flock::generate(int n) {
  // generazione randomica dei boids (velocità e posizione) e inserimento nel
  // flock
  for (int i = 0; i != n; ++i) {
    Boids boid;
    bool overlap;
    do {
    std::uniform_real_distribution<> r_velx(-500., 500.);
    std::uniform_real_distribution<> r_vely(500., -500.);
    std::uniform_real_distribution<> r_posx(50., p_lim_.x - 50.);
    std::uniform_real_distribution<> r_posy(50., p_lim_.y - 50.);
    std::random_device rd;   // generatore numeri random per il seed
    std::mt19937 gen(rd());  // generatore numeri random a partire dal seed
    double px = r_posx(gen);
    double py = r_posy(gen);
    double vx = r_velx(gen);
    double vy = r_vely(gen);
    Coordinate pos = {px, py};
    Coordinate vel = {vx, vy};
    boid = {pos, vel};
    auto find= [boid](Boids b) {return boid.distance(b)< 30.;};
    auto pointer = std::find_if (flock_.begin(), flock_.end(), find);
    overlap = (pointer != flock_.end());
    } while (overlap);
    flock_.push_back(boid);
  }
}

auto Flock::val_control(Coordinate& pos, Coordinate& vel) { 
  // effetto pacman 
  pos.x = std::fmod(pos.x + p_lim_.x, p_lim_.x);
  pos.y = std::fmod(pos.y + p_lim_.y, p_lim_.y);

  // riduce velocità troppo elevate a v_lim, modulo
  double speed_m = module(vel);
  if (speed_m > v_lim_) {
    double alpha = std::acos(vel.x / speed_m);

    if ((vel.x >= 0. && vel.y < 0) || (vel.x < 0 && vel.y < 0)) {
      alpha = -alpha;
    }

    vel.x = v_lim_ * std::cos(alpha);
    vel.y = v_lim_ * std::sin(alpha);
  }
}

void Flock::evolve(double const delta_t) {
  std::vector<Boids> const flock_copy =
      flock_;  // crea un vettore copia costante su cui operare

  for (int i = 0; i != size(); ++i) {
    auto v1 = v_separation(flock_copy[i], flock_copy);  // separazione
    auto v2 = v_alignment(flock_copy[i], flock_copy);   // allineamento
    auto v3 = v_cohesion(flock_copy[i], flock_copy);    // baricentro

    Coordinate new_v = flock_copy[i].v() + v1 + v2 + v3;
    Coordinate new_p = flock_copy[i].p() + delta_t * flock_copy[i].v();

    val_control(new_p, new_v);

    flock_[i] = Boids{new_p, new_v};
  }
}

std::array<double, 4> const Flock::statistics() const {
  Boids total_sum = std::accumulate(flock_.begin(), flock_.end(), Boids{});
  double sum_v = module(total_sum.v());  // somma delle velocità
  double sum_v_square{0.};               // somma dei quadrati delle velocità

  double sum_d{0.};         // somma delle distanze
  double sum_d_square{0.};  // somma dei quadrati delle distanze
  int n_steps{
      0};  // numero di distanze calcolate == sommatoria da 0 a (size()-1)

  double s = static_cast<double>(size());

  for (int i{0}; i != size(); ++i) {
    sum_v += module(flock_[i].v());
    sum_v_square += std::pow(module(flock_[i].v()), 2);

    for (int j{i}; j < size() - 1; ++j) {
      sum_d += flock_[i].distance(flock_[j + 1]);
      sum_d_square += std::pow((flock_[i].distance(flock_[j + 1])), 2);
      ++n_steps;
    }
  }

  double v_mean = sum_v / s;
  double v_RMS{0.};
  double d_mean{0.};
  double d_RMS{0.};

  if (size() > 1) {
    v_RMS = std::sqrt(
        (s / (s - 1.)) *
        ((sum_v_square / s) -
         std::pow(v_mean,
                  2)));  // RMS = sqrt(varianza) = sqrt((n/(n-1))*(media
                         // quadrati - quadrato media))
    d_mean = sum_d / n_steps;
  }
  double steps = static_cast<double>(n_steps);
  if (n_steps > 2) {
    d_RMS = std::sqrt((steps / (steps - 1.)) *
                      ((sum_d_square / steps) - std::pow(d_mean, 2)));
  }

  std::array<double, 4> stats{v_mean, v_RMS, d_mean, d_RMS};
  return stats;

  /*std::ofstream outputfile;
  outputfile.open("databoids.txt",
                  std::ios::app);  // stampa i dati al termine del file txt
  outputfile << species_ << ": " << v_mean << " , " << v_RMS << " , " << d_mean
             << " , " << d_RMS << '\n';
  outputfile.close();

  std::cout << species_ << ": Mean velocity= " << v_mean << ", with standard
  deviation= " << v_RMS << '\n'; std::cout << species_ << ": Mean distance= " <<
  d_mean << ", with standard deviation= " << d_RMS << '\n' << '\n';
*/
}

std::vector<Boids> const& Flock::state() const {
  return flock_;
}
