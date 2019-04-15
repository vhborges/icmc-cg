#include <catavento.hpp>
#include <cg_window.hpp>

int main(int argc, char const *argv[]) {
  cgicmc::Window window;
  Catavento catavento;
  window.createWindow();
  window.loadShaders();
  window.prepare();
  window.run(catavento);
}