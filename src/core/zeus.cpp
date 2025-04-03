#include "controller.h"
#include <QApplication>

void initRegexes(void);

int main(int argc, char **argv) {
  initRegexes();

  QApplication app(argc, argv);
  ZeusController zc = ZeusController();

  zc.start();
  return app.exec();
}
