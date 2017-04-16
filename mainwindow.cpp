#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  // create base for the layout
  QWidget* main_widget = new QWidget;
  setCentralWidget(main_widget);

  // create main part of the window
  canvas = new Canvas(this);
  canvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  QVBoxLayout* layout = new QVBoxLayout;
  layout->setMargin(5);
  layout->addWidget(canvas);
  main_widget->setLayout(layout);

  QString message = tr("Test of status bar message");
  statusBar()->showMessage(message);

  setWindowTitle(tr("Marek Łuszczki"));
  setMinimumSize(200, 200);
  resize(600, 600);
}
