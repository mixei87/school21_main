#include <QApplication>
#include <QLabel>

#include "calc.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QFontDatabase::addApplicationFont(
      ":/fonts/resources/fonts/EurostileBold.ttf");
  QFontDatabase::addApplicationFont(
      ":/fonts/resources/fonts/EurostileExtended-Roman.ttf");
  QFontDatabase::addApplicationFont(":/fonts/resources/fonts/PixeloidMono.ttf");
  QFontDatabase::addApplicationFont(":/fonts/resources/fonts/PixeloidSans.ttf");
  QFontDatabase::addApplicationFont(
      ":/fonts/resources/fonts/PixeloidSansBold.ttf");
  Calc w;
  w.show();
  return a.exec();
}
