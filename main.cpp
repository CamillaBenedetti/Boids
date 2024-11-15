// opzioni di compilazione: g++ -Wall -Wextra main.cpp boids.cpp flock.cpp -lsfml-graphics -lsfml-window -lsfml-system -fsanitize=address

#include "Flock.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>
#include <string>

auto main_evolve(Flock& flock, int steps_per_evolution, sf::Time delta_t) {
  double const dt{delta_t.asSeconds()};  // converte in secondi il tempo

  for (int i{0}; i != steps_per_evolution; ++i) {
    flock.evolve(dt);
  }
  return flock.state();  // ritorna il flock aggiornato
}

int ask_par(rule_parameters& par) {  // funzione per settare da terminale i parametri e
                             // il numero di boids da generare
std::cout
      << "Do you want default parameters? Y/n:  ";
  char option;
  std::cin >> option;
  option = std::tolower(option);
  if (option != 'y') {
  std::cout
      << "Insert parameters s, a, c (note: accepts values between 0 and 1): ";
  std::cin >> par.s >> par.a >> par.c;

  if (!(par.s > 0. && par.s < 1.) || !(par.a > 0. && par.a < 1.) ||
      !(par.c > 0. && par.c < 1.)) {
    throw std::runtime_error{"The values should be between 0 and 1"};
  }
  assert(par.s > 0. && par.s < 1. && par.a > 0. && par.a < 1. && par.c > 0. &&
         par.c < 1.);
  }

  int n_of_boids;
  std::cout
      << "Insert the number of boids (note: accepts values bigger than 0): ";
  std::cin >> n_of_boids;
  if (n_of_boids < 0) {
    throw std::runtime_error{"The number must be equal or bigger than 0."};
  }
  return n_of_boids;
}

int main() {
  auto const delta_t{
      sf::milliseconds(1)};  // costanti per il tempo e l'evoluzione
  int const fps = 60;
  int const steps_per_evolution{1000 / fps};

  double const display_width =
      .9 * sf::VideoMode::getDesktopMode()
               .width;  // dimensioni della finestra desktop
  double const display_height = .9 * sf::VideoMode::getDesktopMode().height;

  rule_parameters par{};
  auto n_of_boids = ask_par(par);
  Flock flock(par, {display_width, display_height});  // primo flock
  flock.generate(n_of_boids);

  rule_parameters new_par{};
  Flock new_flock(new_par, {display_width, display_height});  // secondo flock
  new_flock.change_species('B');

  int n{0};  // numero della finestra per testare key-F
  int m{0};  // number di boids aggiunti per testare key-A

  sf::RenderWindow window(
      sf::VideoMode(display_width, display_height),  // apertura finestra
      "Testing Boids");
  window.setFramerateLimit(fps);

  sf::CircleShape triangle{10, 3};  // boid comune (triangolo verde)
  triangle.setFillColor(sf::Color::Green);
  triangle.setOrigin(5., 5.);  // origine figura spostato da TopLeft a Centro
  triangle.setScale(1., 1.5);

  sf::CircleShape token_A{6, 30};  // segnalino rosso per key-A
  token_A.setOrigin(3, 3);  // origine figura spostato da TopLeft a Centro

  sf::CircleShape triangle_F{
      10, 3};  // boid dello stormo prodotto con F-key (triangle_Fo blu)
  triangle_F.setFillColor(sf::Color::Blue);
  triangle_F.setOrigin(5, 5);  // origine figura spostato da TopLeft a Centro
  triangle_F.setScale(1, 1.5);

  while (window.isOpen()) {  // loop continuo per comandi finestra grafica
    sf::Event event;         // evento generico
    while (window.pollEvent(event)) {
      if (event.type ==
          sf::Event::Closed) {  // evento = chiusura con X della finestra
        window.close();
      }

      // aggiunge un boid random ogni volta che A-key è premuto
      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::A) {  // evento tasto A premuto
        flock.generate(1);  // genera un boid e lo mette nel flock
        ++m;                // aumenta il conto dei boids aggiunti
        std::string title{"Test " + std::to_string(m) +
                          " of random ADD. Current flock size is " +
                          std::to_string(flock.size())};
        window.setTitle(title);

        token_A.setFillColor(sf::Color::Red);
        token_A.setPosition(flock.back().p().x, flock.back().p().y);
      }

      // rende trasparente il segnalino rosso attivato da key-A
      if (event.type == sf::Event::KeyReleased &&
          event.key.code == sf::Keyboard::A) {  // evento tasto A rilasciato
        token_A.setFillColor(sf::Color::Transparent);
      }

      // aggiunge boids e parametri al secondo flock
      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::F) {
        ++n;
        ;
        auto new_n_of_boids = ask_par(new_par);
        new_flock.set_par(new_par);
        new_flock.generate(new_n_of_boids);

        std::string title{"Test " + std::to_string(n) +
                          " of NEW FLOCK. Current new flock size is " +
                          std::to_string(new_flock.size())};
        window.setTitle(title);
      }

      // codice per chiusura alternativa
      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::Q) {  // evento tasto Q premuto
        window.close();
      }
    }
    window.clear(sf::Color::White);  // ad ogni loop cancella tutta la tavola e
                                     // ridisegna flock aggiornato da capo

    if (flock.size() != 0) {
      auto const state = main_evolve(flock, steps_per_evolution, delta_t);

      for (auto& boid : state) {
        triangle.setPosition(boid.p().x, boid.p().y);
        triangle.setRotation(boid.direction());
        window.draw(triangle);
        window.draw(token_A);
      }
    }

    if (new_flock.size() != 0) {  // se flock di boids blu esiste, esegue queste
                                  // azioni, altrimenti non necessarie
      auto const new_state =
          main_evolve(new_flock, steps_per_evolution, delta_t);
      for (auto& new_boid : new_state) {
        triangle_F.setPosition(new_boid.p().x, new_boid.p().y);
        triangle_F.setRotation(new_boid.direction());
        window.draw(triangle_F);
      }
    }

    window.display();  // mostra finestra nello stato finale al termine di ogni
                       // loop
  }
}