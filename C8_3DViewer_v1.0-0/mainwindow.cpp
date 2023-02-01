#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  QString temp_path = QDir::currentPath();
#ifdef __APPLE__
  m_path_objects = temp_path + "/../../../../src/objects/";
#elif __linux__
  m_path_objects = temp_path + "/../src/objects/";
#endif
  ui->setupUi(this);

  settingForm = new SettingForm;
  ui->myGLWidget->setSettings(settingForm->getSettings());
  ui->statusbar->showMessage("Считываем настройки из файла!", 3000);
  connectVisualEffects();
  connectElementsMenu();
  connectOther();
}

MainWindow::~MainWindow() {
  if (ui->myGLWidget->model) {
    free_memory(ui->myGLWidget->model);
    ui->myGLWidget->model = nullptr;
  }
  delete ui;
}

// Слайдеры перемещения
void MainWindow::xMoveSliderChanged(int value) {
  ui->xMoveSpinbox->setValue(value);
}

void MainWindow::yMoveSliderChanged(int value) {
  ui->yMoveSpinbox->setValue(value);
}

void MainWindow::zMoveSliderChanged(int value) {
  ui->zMoveSpinbox->setValue(value);
}

// Слайдер масштабирования
void MainWindow::zoomSliderChanged(int value) {
  ui->zoomSpinbox->setValue(value);
}

void MainWindow::initGraphicElements() {
  ui->xMoveSlider->setValue(0.0f);
  ui->yMoveSlider->setValue(0.0f);
  ui->zMoveSlider->setValue(0.0f);
  ui->xMoveSpinbox->setValue(0.0f);
  ui->yMoveSpinbox->setValue(0.0f);
  ui->zMoveSpinbox->setValue(0.0f);

  ui->xRotateSlider->setValue(0.0f);
  ui->yRotateSlider->setValue(0.0f);
  ui->zRotateSlider->setValue(0.0f);
  ui->xRotateSpinbox->setValue(0.0f);
  ui->yRotateSpinbox->setValue(0.0f);
  ui->zRotateSpinbox->setValue(0.0f);

  ui->zoomSlider->setValue(100.0f);
  ui->zoomSpinbox->setValue(100.0f);
}

void MainWindow::actionOpenFile_triggered() {
  QString file =
      QFileDialog::getOpenFileName(this, tr("Open File"), m_path_objects);
  if (file != "") {
    disconnectVisualEffects();
    initGraphicElements();
    connectVisualEffects();

    ui->statusbar->clearMessage();
    if (ui->myGLWidget->model) {
      free_memory(ui->myGLWidget->model);
      ui->myGLWidget->model = nullptr;
    }

    int error = NO_ERROR;

    ui->myGLWidget->model = new model_3d;
    error = init_model(ui->myGLWidget->model);
    if (error == NO_ERROR) {
      error = parser_data(ui->myGLWidget->model, file.toLocal8Bit().data());
    } else {
      QMessageBox::warning(this, "Warning: Open file", "Can not open file!");
    }

    if (error == NO_ERROR) {
      QFileInfo fileInfo(file);
      ui->field_filename->setText(fileInfo.fileName());
      ui->myGLWidget->initObject(ui->myGLWidget->model);

      QString info_text_file =
          "Number of facet: " +
          QString::number(ui->myGLWidget->model->num_poligon) +
          ". Number of vertex: " +
          QString::number(ui->myGLWidget->model->vertex_coord.vertex);
      ui->statusbar->showMessage(info_text_file);
    } else {
      free_memory(ui->myGLWidget->model);
      ui->myGLWidget->model = nullptr;
      QString error_message = "Error: " + s21_err_messages[error];
      ui->statusbar->showMessage(error_message, 5000);
    }
  }
}

void MainWindow::actionSettings_triggered() {
  settingForm->setModal(true);
  settingForm->exec();
}
void MainWindow::actionExit_triggered() { QApplication::quit(); }

void MainWindow::actionBmp_triggered() {
  const QStringList validSuffixes({"bmp", ".bmp"});
  QString imagePath = QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                                   tr("BMP (*.bmp)"));
  QImage imageSave = ui->myGLWidget->grabFramebuffer();
  if (!imageSave.isNull() && !imagePath.isEmpty()) {
    QFileInfo info(imagePath);
    if (!validSuffixes.contains(info.suffix())) {
      imagePath = info.baseName() + ".bmp";
    }
    imageSave.save(imagePath);
  }
  ui->statusbar->showMessage("Файл сохранен!");
}

void MainWindow::actionJpeg_triggered() {
  const QStringList validSuffixes({"jpeg", ".jpeg"});
  QString imagePath = QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                                   tr("JPEG (*.jpeg)"));
  QImage imageSave = ui->myGLWidget->grabFramebuffer();
  if (!imageSave.isNull() && !imagePath.isEmpty()) {
    QFileInfo info(imagePath);
    if (!validSuffixes.contains(info.suffix())) {
      imagePath = info.baseName() + ".jpeg";
    }
    imageSave.save(imagePath);
  }
  ui->statusbar->showMessage("Файл сохранен!");
}

void MainWindow::actionRecordGIF_triggered() {
  if (!ui->field_filename->text().isEmpty() &&
      ui->actionRecordGIF->text() == "Record GIF") {
    ui->actionRecordGIF->setText("Recording");
    ui->statusbar->showMessage("GIF is Recording!!!");
    my_gif = new QGifImage;
    my_gif->setDefaultDelay(100);
    timer.start(100, this);
  }
}

void MainWindow::actionAboutApp_triggered() {
  QMessageBox::information(
      this, "About Me", "This app was developed by: rdexter, dpaleo and pjian");
}

void MainWindow::actionAboutQt_triggered() { QApplication::aboutQt(); }

void MainWindow::timerEvent(QTimerEvent *) {
  if (my_gif->frameCount() < count_frame_in_5_sec) {
    QImage frame = ui->myGLWidget->grabFramebuffer();
    my_gif->addFrame(frame.scaled(640, 480));
  } else {
    timer.stop();
    ui->actionRecordGIF->setText("Record GIF");
    ui->statusbar->showMessage("GIF Complete!!!");
    const QStringList validSuffixes({"gif", ".gif"});
    QString imagePath = QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                                     tr("GIF (*.gif)"));
    QFileInfo info(imagePath);
    if (!validSuffixes.contains(info.suffix())) {
      imagePath = info.baseName() + ".gif";
    }
    my_gif->save(imagePath);
  }
}

void MainWindow::applySettings(settingDraw set_draw) {
  ui->myGLWidget->setSettings(set_draw);
  ui->statusbar->showMessage("Настройки обновлены!");
}

void MainWindow::debugPrintModel(model_3d *model, int error) {
  QDebug dbg(QtDebugMsg);
  dbg << "error: " << error << Qt::endl;
  dbg << "vertex: " << model->vertex_coord.vertex << Qt::endl;
  dbg << "vertex_array:" << Qt::endl;
  for (size_t i = 0; i < model->vertex_coord.vertex; i++) {
    for (int j = 0; j < model->vertex_coord.columns; j++) {
      dbg << model->vertex_coord.matrix[i][j];
    }
    dbg << Qt::endl;
  }
  dbg << "facet:" << model->num_poligon << Qt::endl;

  for (size_t i = 0; i < model->num_poligon; i++) {
    for (size_t j = 0; j < model->poligon[i].num_poligon_vertexes; j++) {
      dbg << model->poligon[i].vertex_array[j];
    }
    dbg << Qt::endl;
  }
}

void MainWindow::connectVisualEffects() {
  connectMove();
  connectRotate();
  connectZoom();
}

void MainWindow::connectElementsMenu() {
  connect(ui->actionOpenFile, &QAction::triggered, this,
          &MainWindow::actionOpenFile_triggered);
  connect(ui->actionSettings, &QAction::triggered, this,
          &MainWindow::actionSettings_triggered);
  connect(ui->actionExit, &QAction::triggered, this,
          &MainWindow::actionExit_triggered);
  connect(ui->actionBmp, &QAction::triggered, this,
          &MainWindow::actionBmp_triggered);
  connect(ui->actionJpeg, &QAction::triggered, this,
          &MainWindow::actionJpeg_triggered);
  connect(ui->actionRecordGIF, &QAction::triggered, this,
          &MainWindow::actionRecordGIF_triggered);
  connect(ui->actionAboutApp, &QAction::triggered, this,
          &MainWindow::actionAboutApp_triggered);
  connect(ui->actionAboutQt, &QAction::triggered, this,
          &MainWindow::actionAboutQt_triggered);
}

void MainWindow::connectOther() {
  connect(ui->pushButton_OpenFile, &QPushButton::clicked, this,
          &MainWindow::actionOpenFile_triggered);
  connect(settingForm, &SettingForm::backSetting, this,
          &MainWindow::applySettings);
}

void MainWindow::connectMove() {
  connect(ui->xMoveSlider, &QSlider::valueChanged, ui->myGLWidget,
          &Widget::xMoveSliderChanged);
  connect(ui->yMoveSlider, &QSlider::valueChanged, ui->myGLWidget,
          &Widget::yMoveSliderChanged);
  connect(ui->zMoveSlider, &QSlider::valueChanged, ui->myGLWidget,
          &Widget::zMoveSliderChanged);
  connect(ui->xMoveSpinbox, &QSpinBox::valueChanged, ui->xMoveSlider,
          &QSlider::setValue);
  connect(ui->yMoveSpinbox, &QSpinBox::valueChanged, ui->yMoveSlider,
          &QSlider::setValue);
  connect(ui->zMoveSpinbox, &QSpinBox::valueChanged, ui->zMoveSlider,
          &QSlider::setValue);
  connect(ui->myGLWidget, &Widget::xMoveToSpinBox, ui->xMoveSpinbox,
          &QSpinBox::setValue);
  connect(ui->myGLWidget, &Widget::yMoveToSpinBox, ui->yMoveSpinbox,
          &QSpinBox::setValue);
  connect(ui->myGLWidget, &Widget::zMoveToSpinBox, ui->zMoveSpinbox,
          &QSpinBox::setValue);
  connect(ui->myGLWidget, &Widget::xMoveToSlider, ui->xMoveSlider,
          &QSlider::setValue);
  connect(ui->myGLWidget, &Widget::yMoveToSlider, ui->yMoveSlider,
          &QSlider::setValue);
  connect(ui->myGLWidget, &Widget::zMoveToSlider, ui->zMoveSlider,
          &QSlider::setValue);
}

void MainWindow::connectRotate() {
  connect(ui->xRotateSlider, &QSlider::valueChanged, ui->myGLWidget,
          &Widget::xRotateSliderChanged);
  connect(ui->yRotateSlider, &QSlider::valueChanged, ui->myGLWidget,
          &Widget::yRotateSliderChanged);
  connect(ui->zRotateSlider, &QSlider::valueChanged, ui->myGLWidget,
          &Widget::zRotateSliderChanged);
  connect(ui->xRotateSpinbox, &QSpinBox::valueChanged, ui->xRotateSlider,
          &QSlider::setValue);
  connect(ui->yRotateSpinbox, &QSpinBox::valueChanged, ui->yRotateSlider,
          &QSlider::setValue);
  connect(ui->zRotateSpinbox, &QSpinBox::valueChanged, ui->zRotateSlider,
          &QSlider::setValue);
  connect(ui->myGLWidget, &Widget::xRotateToSpinBox, ui->xRotateSpinbox,
          &QSpinBox::setValue);
  connect(ui->myGLWidget, &Widget::yRotateToSpinBox, ui->yRotateSpinbox,
          &QSpinBox::setValue);
  connect(ui->myGLWidget, &Widget::zRotateToSpinBox, ui->zRotateSpinbox,
          &QSpinBox::setValue);
  connect(ui->myGLWidget, &Widget::xRotateToSlider, ui->xRotateSlider,
          &QSlider::setValue);
  connect(ui->myGLWidget, &Widget::yRotateToSlider, ui->yRotateSlider,
          &QSlider::setValue);
  connect(ui->myGLWidget, &Widget::zRotateToSlider, ui->zRotateSlider,
          &QSlider::setValue);
}

void MainWindow::connectZoom() {
  connect(ui->zoomSlider, &QSlider::valueChanged, ui->myGLWidget,
          &Widget::zoomSliderChanged);
  connect(ui->zoomSpinbox, &QSpinBox::valueChanged, ui->zoomSlider,
          &QSlider::setValue);
  connect(ui->myGLWidget, &Widget::zoomToSpinBox, ui->zoomSpinbox,
          &QSpinBox::setValue);
  connect(ui->myGLWidget, &Widget::zoomToSlider, ui->zoomSlider,
          &QSlider::setValue);
}

void MainWindow::disconnectVisualEffects() {
  disconnectMove();
  disconnectRotate();
  disconnectZoom();
}

void MainWindow::disconnectMove() {
  disconnect(ui->xMoveSlider, &QSlider::valueChanged, ui->myGLWidget,
             &Widget::xMoveSliderChanged);
  disconnect(ui->yMoveSlider, &QSlider::valueChanged, ui->myGLWidget,
             &Widget::yMoveSliderChanged);
  disconnect(ui->zMoveSlider, &QSlider::valueChanged, ui->myGLWidget,
             &Widget::zMoveSliderChanged);
  disconnect(ui->xMoveSpinbox, &QSpinBox::valueChanged, ui->xMoveSlider,
             &QSlider::setValue);
  disconnect(ui->yMoveSpinbox, &QSpinBox::valueChanged, ui->yMoveSlider,
             &QSlider::setValue);
  disconnect(ui->zMoveSpinbox, &QSpinBox::valueChanged, ui->zMoveSlider,
             &QSlider::setValue);
  disconnect(ui->myGLWidget, &Widget::xMoveToSpinBox, ui->xMoveSpinbox,
             &QSpinBox::setValue);
  disconnect(ui->myGLWidget, &Widget::yMoveToSpinBox, ui->yMoveSpinbox,
             &QSpinBox::setValue);
  disconnect(ui->myGLWidget, &Widget::zMoveToSpinBox, ui->zMoveSpinbox,
             &QSpinBox::setValue);
  disconnect(ui->myGLWidget, &Widget::xMoveToSlider, ui->xMoveSlider,
             &QSlider::setValue);
  disconnect(ui->myGLWidget, &Widget::yMoveToSlider, ui->yMoveSlider,
             &QSlider::setValue);
  disconnect(ui->myGLWidget, &Widget::zMoveToSlider, ui->zMoveSlider,
             &QSlider::setValue);
}

void MainWindow::disconnectRotate() {
  disconnect(ui->xRotateSlider, &QSlider::valueChanged, ui->myGLWidget,
             &Widget::xRotateSliderChanged);
  disconnect(ui->yRotateSlider, &QSlider::valueChanged, ui->myGLWidget,
             &Widget::yRotateSliderChanged);
  disconnect(ui->zRotateSlider, &QSlider::valueChanged, ui->myGLWidget,
             &Widget::zRotateSliderChanged);
  disconnect(ui->xRotateSpinbox, &QSpinBox::valueChanged, ui->xRotateSlider,
             &QSlider::setValue);
  disconnect(ui->yRotateSpinbox, &QSpinBox::valueChanged, ui->yRotateSlider,
             &QSlider::setValue);
  disconnect(ui->zRotateSpinbox, &QSpinBox::valueChanged, ui->zRotateSlider,
             &QSlider::setValue);
  disconnect(ui->myGLWidget, &Widget::xRotateToSpinBox, ui->xRotateSpinbox,
             &QSpinBox::setValue);
  disconnect(ui->myGLWidget, &Widget::yRotateToSpinBox, ui->yRotateSpinbox,
             &QSpinBox::setValue);
  disconnect(ui->myGLWidget, &Widget::zRotateToSpinBox, ui->zRotateSpinbox,
             &QSpinBox::setValue);
  disconnect(ui->myGLWidget, &Widget::xRotateToSlider, ui->xRotateSlider,
             &QSlider::setValue);
  disconnect(ui->myGLWidget, &Widget::yRotateToSlider, ui->yRotateSlider,
             &QSlider::setValue);
  disconnect(ui->myGLWidget, &Widget::zRotateToSlider, ui->zRotateSlider,
             &QSlider::setValue);
}

void MainWindow::disconnectZoom() {
  disconnect(ui->zoomSlider, &QSlider::valueChanged, ui->myGLWidget,
             &Widget::zoomSliderChanged);
  disconnect(ui->zoomSpinbox, &QSpinBox::valueChanged, ui->zoomSlider,
             &QSlider::setValue);
  disconnect(ui->myGLWidget, &Widget::zoomToSpinBox, ui->zoomSpinbox,
             &QSpinBox::setValue);
  disconnect(ui->myGLWidget, &Widget::zoomToSlider, ui->zoomSlider,
             &QSlider::setValue);
}
