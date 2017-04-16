#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "canvas.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = 0);

 protected:
 private:
  Canvas* canvas;
};

#endif  // MAINWINDOW_H
