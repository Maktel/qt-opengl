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

  createActions();
  createMenus();

  QString message = tr("Test of status bar message");
  statusBar()->showMessage(message);

  setWindowTitle(tr("Marek Łuszczki OpenGL"));
  setMinimumSize(200, 200);
  resize(600, 600);
}

void MainWindow::createActions() {
  // & in name is to mark an entry shortcut

  load_file_act = new QAction(tr("&Load"), this);
  load_file_act->setShortcut(QKeySequence::Open);
  load_file_act->setStatusTip(tr("Load a file to canvas"));
  connect(load_file_act, &QAction::triggered, this, &MainWindow::loadFile);

  save_file_act = new QAction(tr("&Save"), this);
  save_file_act->setShortcut(QKeySequence::Save);
  save_file_act->setStatusTip(tr("Save image to a file"));
  connect(save_file_act, &QAction::triggered, this, &MainWindow::saveFile);

  quit_act = new QAction(tr("&Quit"), this);
  quit_act->setShortcut(QKeySequence::Quit);
  quit_act->setStatusTip(tr("Remember to save your work before quitting"));
  connect(quit_act, &QAction::triggered, this, &MainWindow::quit);
}

void MainWindow::createMenus() {
  file_menu = menuBar()->addMenu(tr("&File"));
  file_menu->addAction(load_file_act);
  file_menu->addAction(save_file_act);
  file_menu->addAction(quit_act);
}


/*  ------------------------------------------------------------------------  */
/*  SLOTS  */

void MainWindow::quit() { QApplication::quit(); }

void MainWindow::loadFile() {
  QString file_name = QFileDialog::getOpenFileName(
      this, tr("Choose image to open"), "..", tr("Images (*.png *.bmp *.jpg)"));
  if (!file_name.isEmpty())
    canvas->loadFile(file_name);
  else
    qWarning() << tr("File has not been chosen");
}

void MainWindow::saveFile() {
  // create a dialog in parent
  QFileDialog dialog(this);
  // mark as a save dialog
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  // default suffix in case of user giving none
  dialog.setDefaultSuffix("png");
  // also accept non-existing files
  dialog.setFileMode(QFileDialog::AnyFile);
  // list of accepted file formats
  dialog.setNameFilter(tr("All files (*)"));
  // detailed list view
  dialog.setViewMode(QFileDialog::Detail);
  // entry point for explorer
  dialog.setDirectory("..");

  QString file_name;
  if (dialog.exec()) {
    file_name = dialog.selectedFiles().at(0);
  }

  if (!file_name.isEmpty())
    canvas->saveFile(file_name);
  else
    qWarning() << tr("Save location has not been chosen correctly");
}
