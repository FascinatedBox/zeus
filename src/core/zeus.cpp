#include "controller.h"
#include <QApplication>

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  ZeusController zc = ZeusController();

  zc.start();
  return app.exec();
}
