#include <QApplication>
#include <QSurfaceFormat>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
  } else {
    format.setVersion(3, 0);
  }
  QSurfaceFormat::setDefaultFormat(format);

  QApplication a(argc, argv);
  QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
  MainWindow w;
  w.show();
  return a.exec();
}
