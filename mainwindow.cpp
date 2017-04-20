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


  this->setAttribute(Qt::WA_AlwaysShowToolTips, true);
  main_widget->setAttribute(Qt::WA_AlwaysShowToolTips, true);

  QString message = tr("Help messages will be displayed here");
  statusBar()->showMessage(message);
  // TODO Create custom QStatusBar and overload clearMessage() with help text
  //  QLabel* perm_label = new QLabel(tr("Press Space to draw"));
  //  statusBar()->addPermanentWidget(perm_label, 0);

  setWindowTitle(tr("Marek \u0141uszczki OpenGL"));
  setMinimumSize(200, 200);
  resize(600, 600);
}

void MainWindow::createActions() {
  // & in name is to mark an entry shortcut
  // most slots could be simplified with lambdas

  // FILE
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


  // GENERAL
  mode_click_act = new QAction(tr("&Click"), this);
  mode_click_act->setCheckable(true);
  mode_click_act->setShortcut(QKeySequence(tr("K")));
  mode_click_act->setStatusTip(tr("Default, simple click mode"));
  connect(mode_click_act, &QAction::triggered, this, &MainWindow::modeClick);

  switch_debug_act = new QAction(tr("&Debug"), this);
  switch_debug_act->setCheckable(true);
  switch_debug_act->setShortcut(QKeySequence(tr("D")));
  switch_debug_act->setStatusTip(
      tr("Enable additional information, mouse clicks marking and more"));
  connect(switch_debug_act, &QAction::triggered, this,
          &MainWindow::switchDebug);

  switch_grid_act = new QAction(tr("&Grid"), this);
  switch_grid_act->setCheckable(true);
  switch_grid_act->setShortcut(QKeySequence(tr("G")));
  switch_grid_act->setStatusTip(
      tr("Enable grid for alignment debugging purposes"));
  connect(switch_grid_act, &QAction::triggered, this, &MainWindow::switchGrid);

  change_scale_act = new QAction(tr("&Scale"));
  change_scale_act->setShortcut(QKeySequence(tr("Z")));
  change_scale_act->setStatusTip(
      tr("Change zoom display level (does not affect image)"));
  connect(change_scale_act, &QAction::triggered, this,
          &MainWindow::changeScale);

  settings_general_act = new QAction(tr("&Settings"));
  settings_general_act->setShortcut(QKeySequence(tr("Ctrl+g")));
  settings_general_act->setStatusTip(
      tr("Change debugging colors and other settings"));
  connect(settings_general_act, &QAction::triggered, this,
          &MainWindow::settingsGeneral);

  // PAINTING
  mode_line_act = new QAction(tr("&Line"));
  mode_line_act->setCheckable(true);
  mode_line_act->setShortcut(QKeySequence(tr("L")));
  mode_line_act->setStatusTip(
      tr("After picking starting and end point, line draws automatically"));
  connect(mode_line_act, &QAction::triggered, this, &MainWindow::modeLine);

  mode_polygon_act = new QAction(tr("&Polygon"));
  mode_polygon_act->setCheckable(true);
  mode_polygon_act->setShortcut(QKeySequence(tr("P")));
  mode_polygon_act->setStatusTip(
      tr("Pick multiple points to create a polygon"));
  connect(mode_polygon_act, &QAction::triggered, this,
          &MainWindow::modePolygon);

  mode_circle_act = new QAction(tr("&Circle"));
  mode_circle_act->setCheckable(true);
  mode_circle_act->setShortcut(QKeySequence(tr("C")));
  mode_circle_act->setStatusTip(tr(
      "After picking centre and any border point, circle draws automatically"));
  connect(mode_circle_act, &QAction::triggered, this, &MainWindow::modeCircle);

  mode_bezier_act = new QAction(tr("&Bezier curve"));
  mode_bezier_act->setCheckable(true);
  mode_bezier_act->setShortcut(QKeySequence(tr("B")));
  mode_bezier_act->setStatusTip(
      tr("Pick 4 points to automatically draw Bezier curve"));
  connect(mode_bezier_act, &QAction::triggered, this, &MainWindow::modeBezier);

  mode_spline_act = new QAction(tr("Basis spl&ine"));
  mode_spline_act->setCheckable(true);
  mode_spline_act->setShortcut(QKeySequence(tr("S")));
  mode_spline_act->setStatusTip(
      tr("Pick multiple points to create a basis spline"));
  connect(mode_spline_act, &QAction::triggered, this, &MainWindow::modeSpline);

  settings_draw_act = new QAction(tr("&Settings"));
  settings_draw_act->setShortcut(QKeySequence(tr("Ctrl+d")));
  settings_draw_act->setStatusTip(
      tr("Change line, circle drawing functions and more"));
  connect(settings_draw_act, &QAction::triggered, this,
          &MainWindow::settingsDraw);


  // DRAWING
  mode_fill_act = new QAction(tr("&Fill"));
  mode_fill_act->setCheckable(true);
  mode_fill_act->setShortcut(QKeySequence(tr("F")));
  mode_fill_act->setStatusTip(tr("Fill area with color"));
  connect(mode_fill_act, &QAction::triggered, this, &MainWindow::modeFill);

  mode_hgradient_act = new QAction(tr("&Horizontal gradient"));
  mode_hgradient_act->setCheckable(true);
  mode_hgradient_act->setShortcut(QKeySequence(tr("H")));
  mode_hgradient_act->setStatusTip(
      tr("Fill whole canvas with horizontal gradient"));
  connect(mode_hgradient_act, &QAction::triggered, this,
          &MainWindow::modeHGradient);

  mode_vgradient_act = new QAction(tr("&Vertical gradient"));
  mode_vgradient_act->setCheckable(true);
  mode_vgradient_act->setShortcut(QKeySequence(tr("V")));
  mode_vgradient_act->setStatusTip(
      tr("Fill whole canvas with vertical gradient"));
  connect(mode_vgradient_act, &QAction::triggered, this,
          &MainWindow::modeVGradient);

  settings_paint_act = new QAction(tr("&Settings"));
  settings_paint_act->setShortcut(QKeySequence(tr("Ctrl+p")));
  settings_paint_act->setStatusTip(
      tr("Change colors and options for gradients"));
  connect(settings_paint_act, &QAction::triggered, this,
          &MainWindow::settingsPaint);


  tr_shift_act = new QAction(tr("&Translate"));
  tr_shift_act->setShortcut(QKeySequence(tr("T")));
  tr_shift_act->setStatusTip(tr("Translate image by a vector"));
  connect(tr_shift_act, &QAction::triggered, this, &MainWindow::trShift);

  tr_rotate_act = new QAction(tr("&Rotate"));
  tr_rotate_act->setShortcut(QKeySequence(tr("R")));
  tr_rotate_act->setStatusTip(tr("Rotate image by an angle"));
  connect(tr_rotate_act, &QAction::triggered, this, &MainWindow::trRotate);

  tr_scale_act = new QAction(tr("Sc&ale"));
  tr_scale_act->setShortcut(QKeySequence(tr("A")));
  tr_scale_act->setStatusTip(tr("Scale image by a scale rate"));
  connect(tr_scale_act, &QAction::triggered, this, &MainWindow::trScale);

  tr_shear_act = new QAction(tr("Sh&ear"));
  tr_shear_act->setShortcut(QKeySequence(tr("E")));
  tr_shear_act->setStatusTip(tr("Shear image by coefficients"));
  connect(tr_shear_act, &QAction::triggered, this, &MainWindow::trShear);

  settings_tr_act = new QAction(tr("&Settings"));
  settings_tr_act->setShortcut(QKeySequence(tr("Ctrl+t")));
  settings_tr_act->setStatusTip(
      tr("Set coefficients, make transformations in-place and more"));
  connect(settings_tr_act, &QAction::triggered, this, &MainWindow::settingsTr);


  about_act = new QAction(tr("&About"));
  about_act->setStatusTip(tr("Display information about this application"));
  connect(about_act, &QAction::triggered, this, &MainWindow::about);

  about_qt_act = new QAction(tr("About &Qt"));
  about_qt_act->setStatusTip(tr("Display information about Qt"));
  connect(about_qt_act, &QAction::triggered, this, &QApplication::aboutQt);

  // OTHER
  mode_group = new QActionGroup(this);
  mode_group->addAction(mode_click_act);
  mode_group->addAction(mode_line_act);
  mode_group->addAction(mode_polygon_act);
  mode_group->addAction(mode_circle_act);
  mode_group->addAction(mode_bezier_act);
  mode_group->addAction(mode_spline_act);
  mode_group->addAction(mode_fill_act);
  mode_group->addAction(mode_hgradient_act);
  mode_group->addAction(mode_vgradient_act);
  mode_click_act->setChecked(true);
}

void MainWindow::createMenus() {
  file_menu = menuBar()->addMenu(tr("&File"));
  file_menu->menuAction()->setStatusTip(tr("File operations"));
  file_menu->addAction(load_file_act);
  file_menu->addAction(save_file_act);
  file_menu->addSeparator();
  file_menu->addAction(quit_act);

  general_menu = menuBar()->addMenu(tr("&General"));
  general_menu->menuAction()->setStatusTip(tr("Core functionalities"));
  general_menu->addAction(mode_click_act);
  general_menu->addAction(switch_debug_act);
  general_menu->addAction(switch_grid_act);
  general_menu->addAction(change_scale_act);
  general_menu->addAction(settings_general_act);

  draw_menu = menuBar()->addMenu(tr("&Draw"));
  draw_menu->menuAction()->setStatusTip(
      tr("Pick mode, mark points with mouse and press Space to draw"));
  draw_menu->addAction(mode_line_act);
  draw_menu->addAction(mode_polygon_act);
  draw_menu->addAction(mode_circle_act);
  draw_menu->addAction(mode_bezier_act);
  draw_menu->addAction(mode_spline_act);
  draw_menu->addSeparator();
  draw_menu->addAction(settings_draw_act);

  paint_menu = menuBar()->addMenu(tr("&Paint"));
  paint_menu->menuAction()->setStatusTip(
      tr("Fill with color and set up gradients"));
  paint_menu->addAction(mode_fill_act);
  paint_menu->addAction(mode_hgradient_act);
  paint_menu->addAction(mode_vgradient_act);
  paint_menu->addSeparator();
  paint_menu->addAction(settings_paint_act);

  tr_menu = menuBar()->addMenu(tr("&Transform"));
  tr_menu->menuAction()->setStatusTip(tr("Transform image in multiple ways"));
  tr_menu->addAction(tr_shift_act);
  tr_menu->addAction(tr_rotate_act);
  tr_menu->addAction(tr_scale_act);
  tr_menu->addAction(tr_shear_act);
  tr_menu->addSeparator();
  tr_menu->addAction(settings_tr_act);

  about_menu = menuBar()->addMenu(tr("&Help"));
  about_menu->menuAction()->setStatusTip(tr("Get help about this application"));
  about_menu->addAction(about_act);
  about_menu->addAction(about_qt_act);
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

void MainWindow::modeClick() { canvas->setMode(Mode::click); }

void MainWindow::switchDebug() {
  debug = !debug;
  canvas->setDebug(debug);
}

void MainWindow::switchGrid() { canvas->switchGrid(); }

void MainWindow::changeScale() {
  QDialog dialog(this);
  dialog.setWindowTitle(tr("Set display zoom"));
  QFormLayout form(&dialog);

  QString label_text = tr("Current zoom level: ");
  QLabel label("");
  form.addRow(&label);

  QSlider zoom_slider(Qt::Horizontal, &dialog);
  const int multiplier = 6;
  zoom_slider.setRange(1, 4 * multiplier);
  zoom_slider.setValue(multiplier * canvas->settings.scale);
  zoom_slider.setTickInterval(multiplier);
  zoom_slider.setTickPosition(QSlider::TicksBelow);
  form.addWidget(&zoom_slider);

  label.setText(
      label_text +
      QString::number(zoom_slider.value() * 1.0 / multiplier, 'g', 2));
  connect(&zoom_slider, &QSlider::valueChanged, this, [&]() {
    label.setText(
        label_text +
        QString::number(zoom_slider.value() * 1.0 / multiplier, 'g', 2));
  });

  QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                           Qt::Horizontal, &dialog);
  buttons.setCenterButtons(true);
  form.addRow(&buttons);

  connect(&buttons, SIGNAL(accepted()), &dialog, SLOT(accept()));
  connect(&buttons, SIGNAL(rejected()), &dialog, SLOT(reject()));

  if (dialog.exec() == QDialog::Accepted) {
    canvas->setScale(zoom_slider.value() * 1.0 / multiplier);
  }
}

void MainWindow::settingsGeneral() {
  QDialog dialog(this);
  dialog.setWindowTitle(tr("General settings"));
  QFormLayout form(&dialog);

  QWidget debug_widget;
  QVBoxLayout debug_layout(&debug_widget);
  QLabel debug_label(tr("Debugging color (grid, clicks, additional lines) "));
  debug_layout.addWidget(&debug_label);

  QColor debug_color = canvas->settings.debug_color;
  ColorPicker debug_picker(&debug_color, tr("Debug"));
  debug_layout.addWidget(&debug_picker);

  //  debug_widget.setLayout(&debug_layout);

  form.addWidget(&debug_widget);


  form.addWidget(new DialogStandardButtons(&dialog));

  if (dialog.exec() == QDialog::Accepted) {
    canvas->setDebugColor(debug_color);
  }
}

void MainWindow::modeLine() { canvas->setMode(Mode::line); }
void MainWindow::modePolygon() { canvas->setMode(Mode::polygon); }
void MainWindow::modeCircle() { canvas->setMode(Mode::circle); }
void MainWindow::modeBezier() { canvas->setMode(Mode::bezier); }
void MainWindow::modeSpline() { canvas->setMode(Mode::spline); }

void MainWindow::settingsDraw() {
  QDialog dialog(this);
  dialog.setWindowTitle(tr("Drawing settings"));
  QFormLayout form(&dialog);

  QWidget draw;
  QVBoxLayout draw_layout(&draw);

  QLabel draw_label(tr("Main drawing color: "));
  draw_layout.addWidget(&draw_label);

  QColor draw_color = canvas->settings.main_color;
  ColorPicker draw_color_picker(&draw_color, tr("Main"));
  draw_layout.addWidget(&draw_color_picker);

  draw.setLayout(&draw_layout);
  form.addWidget(&draw);


  ButtonBox<LineType, QHBoxLayout> line_buttons(tr("Line drawing function: "),
                                                canvas->settings.line_type);
  line_buttons.addButton(tr("Bresenham's"), LineType::bresenham);
  line_buttons.addButton(tr("Xiaolin Wu's (antialiased)"),
                         LineType::antialiased);
  line_buttons.initializeChecked();

  form.addWidget(&line_buttons);


  ButtonBox<CircleType, QHBoxLayout> circle_buttons(
      tr("Circle drawing function: "), canvas->settings.circle_type);
  circle_buttons.addButton(tr("Bresenham's"), CircleType::bresenham);
  circle_buttons.addButton(tr("Approximated (using lines)"),
                           CircleType::approximated);
  circle_buttons.initializeChecked();

  form.addWidget(&circle_buttons);


  // number of circle steps when approximating
  Inputs inputs("Number of steps in circle approximation");
  inputs.addLabel(tr("Steps: "));
  inputs.addIntInput(1, canvas->settings.circle_steps, 128);
  inputs.switchInputs(!circle_buttons.buttons[1]->isChecked());
  connect(
      circle_buttons.buttons[1], &TypeRadioButton<CircleType>::toggled, this,
      [&]() { inputs.switchInputs(!circle_buttons.buttons[1]->isChecked()); });
  form.addWidget(&inputs);


  form.addWidget(new DialogStandardButtons(&dialog));

  if (dialog.exec() == QDialog::Accepted) {
    canvas->setMainColor(draw_color);

    canvas->setLineMode(line_buttons.selectedType());

    CircleType circle_type = circle_buttons.selectedType();
    canvas->setCircleMode(circle_type);
    if (circle_type == CircleType::approximated)
      canvas->setCircleSteps(inputs.ints[0]->value());
  }
}

void MainWindow::modeFill() { canvas->setMode(Mode::fill); }
void MainWindow::modeHGradient() { canvas->setMode(Mode::hgradient); }
void MainWindow::modeVGradient() { canvas->setMode(Mode::vgradient); }

void MainWindow::settingsPaint() {
  QDialog dialog(this);
  dialog.setWindowTitle(tr("Painting settings"));
  QFormLayout form(&dialog);


  // FILLING
  QWidget fill;
  QVBoxLayout fill_layout(&fill);

  QLabel fill_label(tr("Pick filling color: "));
  fill_layout.addWidget(&fill_label);

  QColor fill_color = canvas->settings.fill_color;
  ColorPicker fill_picker(&fill_color, tr("Fill"));
  fill_layout.addWidget(&fill_picker);

  bool fill_random = canvas->settings.fill_random;
  QCheckBox fill_box(tr("Use random color every time"));
  fill_layout.addWidget(&fill_box);

  fill_picker.button->setDisabled(fill_random);
  fill_picker.label->setDisabled(fill_random);
  if (fill_random) fill_box.setChecked(true);
  connect(&fill_box, &QCheckBox::stateChanged, this, [&]() {
    fill_random = fill_box.isChecked();
    fill_picker.button->setDisabled(fill_random);
    fill_picker.label->setDisabled(fill_random);
  });

  fill.setLayout(&fill_layout);
  form.addWidget(&fill);

  // function select buttons
  ButtonBox<FillType> fill_buttons(tr("Filling function: "),
                                   canvas->settings.fill_type);
  fill_buttons.addButton(tr("Scanline"), FillType::scanline);
  fill_buttons.addButton(tr("Flood fill stack-based"), FillType::stack);
  fill_buttons.addButton(tr("Border recursive flood fill"),
                         FillType::recursive);
  fill_buttons.initializeChecked();

  fill_buttons.setLabel(
      tr("Recursive implementation can cause crashes for big areas. Border "
         "variant tries to find a border color that is equal to current line "
         "color."),
      true);

  form.addWidget(&fill_buttons);


  form.addWidget(new Separator());


  // GRADIENTS
  QWidget gradient;
  QVBoxLayout gradient_layout(&gradient);

  QLabel gradient_label(tr("Pick gradient colors: "));
  gradient_layout.addWidget(&gradient_label);

  QColor start_color = canvas->settings.start_color;
  ColorPicker start_picker(&start_color, tr("Start"));
  gradient_layout.addWidget(&start_picker);

  QColor end_color = canvas->settings.end_color;
  ColorPicker end_picker(&end_color, tr("End"));
  gradient_layout.addWidget(&end_picker);

  gradient.setLayout(&gradient_layout);
  form.addWidget(&gradient);


  form.addWidget(new DialogStandardButtons(&dialog));

  if (dialog.exec() == QDialog::Accepted) {
    canvas->setFillSettings(fill_random, fill_color,
                            fill_buttons.selectedType());

    canvas->setGradientSettings(start_color, end_color);
  }
}

void MainWindow::trShift() {}
void MainWindow::trRotate() {}
void MainWindow::trScale() {}
void MainWindow::trShear() {}

void MainWindow::settingsTr() {
  QDialog dialog(this);
  dialog.setWindowTitle(tr("Transformations settings"));
  QFormLayout form(&dialog);


  // TRANSLATE
  Inputs shift(tr("Vector to shift image by: "));
  shift.addLabel(tr("X: "));
  shift.addIntInput(-canvas->settings.width, canvas->settings.shift_x,
                    canvas->settings.width);
  shift.addLabel(tr("Y: "));
  shift.addIntInput(-canvas->settings.height, canvas->settings.shift_y,
                    canvas->settings.height);

  form.addWidget(&shift);
  form.addWidget(new Separator());

  // ROTATE
  Inputs rotate(tr("Angle to rotate image by: "));
  rotate.addLabel(tr("Angle (in degrees): "));
  rotate.addDoubleInput(-180.0, canvas->settings.rotate_angle, 180.0);
  rotate.addCheckbox(tr("Rotate in-place (relative to center of image)"),
                     canvas->settings.rotate_inplace);

  form.addWidget(&rotate);
  form.addWidget(new Separator());

  // SCALE
  Inputs scale(tr("Factors to scale image by: "));
  scale.addLabel(tr("S_X: "));
  scale.addDoubleInput(0, canvas->settings.scale_x, 6.0);
  scale.addLabel(tr("S_Y: "));
  scale.addDoubleInput(0, canvas->settings.scale_y, 6.0);
  scale.addCheckbox(tr("Scale relative to center of image"),
                    canvas->settings.scale_inplace);

  form.addWidget(&scale);
  form.addWidget(new Separator());

  // SHEAR
  Inputs shear(tr("Coefficients to shear image by: "));
  shear.addLabel(tr("A_X: "));
  shear.addDoubleInput(-1.0, canvas->settings.shear_x, 1.0);
  shear.addLabel(tr("A_Y: "));
  shear.addDoubleInput(-1.0, canvas->settings.shear_y, 1.0);
  shear.addCheckbox(tr("Shear relative to center of image"),
                    canvas->settings.shear_inplace);

  form.addWidget(&shear);


  form.addRow(new DialogStandardButtons(&dialog));

  if (dialog.exec() == QDialog::Accepted) {
    canvas->setShift(shift.ints[0]->value(), shift.ints[1]->value());
    canvas->setRotate(rotate.doubles[0]->value(),
                      rotate.checkboxes[0]->isChecked());
    canvas->setScale(scale.doubles[0]->value(), scale.doubles[1]->value(),
                     scale.checkboxes[0]->isChecked());
    canvas->setShear(shear.doubles[0]->value(), shear.doubles[1]->value(),
                     shear.checkboxes[0]->isChecked());
  }
}

void MainWindow::about() {
  QString message(
      tr("Application by Marek \u0141uszczki, developed in 2nd quarter of "
         "2017, during 4th semester of Computer Science studies at Maria Curie "
         "Sklodowska University in Lublin."));

  QMessageBox::about(this, tr("About"), message);
}
