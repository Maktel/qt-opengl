#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include "mainwindow.h"

int main(int argc, char* argv[]) {
  srand((unsigned int)time(NULL));
  // Exactly one application is needed for each program, no matter how many
  // windows are displayed. QApplication is used with all GUI apps.
  QApplication a(argc, argv);

  QRect r = QApplication::desktop()->availableGeometry();

  int width = r.width() / 2 - 100;
  int height = r.height() - 100;

  if (bool fullscreen = false) {
    width = r.width();
    height = r.height();
  }

  qDebug() << "screen dimensions: " << width << " x " << height;

  // just pass max dimensions
  MainWindow w;

  // without .show() program will run with no interface, doing nothing
  w.show();

  return a.exec();
}
