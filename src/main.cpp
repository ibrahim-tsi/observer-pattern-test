#include <SFML/Graphics.hpp>
#include <mutex>
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <cctype>
#include <algorithm>
#include <iostream>

std::mutex window_ptrs_mutex{};

class Window {
  sf::RenderWindow _ren{};
  sf::Color colour{sf::Color::Black};

public:
  Window(int width, int height, std::string title) :
    _ren(sf::VideoMode(width, height), title.c_str())
  {
    
  }

  void update(sf::Color colour) {
    this->colour = colour;
  }

  void refresh() {
    if (_ren.isOpen()) {
      sf::Event event{};

      while (_ren.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
          _ren.close();
      }

      _ren.clear(colour);
      _ren.display();
    }
  }
};

void get_cli_input(std::vector<std::unique_ptr<Window>>& window_ptrs) {
  while (true) {
    std::string input{};

    std::getline(std::cin, input);

    sf::Color colour{};

    if (input == "red")
      colour = sf::Color::Red;

    else if (input == "green")
      colour = sf::Color::Green;

    else if (input == "blue")
      colour = sf::Color::Blue;

    else
      return;

    for (auto& window_ptr : window_ptrs)
      window_ptr->update(colour);
  }
}

int main() {
  std::vector<std::unique_ptr<Window>> window_ptrs{};
  const int window_count{3};

  for (int i{0}; i < window_count; i++)
    window_ptrs.push_back(
      std::make_unique<Window>(500, 500, std::to_string(i))
    );

  std::thread(get_cli_input, std::ref(window_ptrs)).detach();

  while (true) {
    for (auto& window_ptr : window_ptrs)
      window_ptr->refresh();
  }

  return 0;
}