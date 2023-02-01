#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QAction>
#include <QBasicTimer>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmap>
#include <QTimer>
#include <QtOpenGL>

#include "colorform.h"
#include "qgifimage.h"
#include "settingform.h"
#include "widget.h"

extern "C" {
#include "parser.h"
}

QT_BEGIN_NAMESPACE namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  Widget *myGLWidget;

 private slots:
  void xMoveSliderChanged(int value);
  void yMoveSliderChanged(int value);
  void zMoveSliderChanged(int value);
  void zoomSliderChanged(int value);

  void actionOpenFile_triggered();
  void actionAboutQt_triggered();
  void actionAboutApp_triggered();
  void actionExit_triggered();
  void actionSettings_triggered();
  void actionBmp_triggered();
  void actionJpeg_triggered();
  void actionRecordGIF_triggered();

 public slots:
  void applySettings(settingDraw set_draw);

 protected:
  void timerEvent(QTimerEvent *e) override;

 private:
  const int count_frame_in_5_sec = 50;
  Ui::MainWindow *ui;
  SettingForm *settingForm;
  void debugPrintModel(model_3d *model, int error);
  QString m_path_objects;
  void connectMove();
  void connectRotate();
  void connectZoom();
  void connectVisualEffects();
  void connectElementsMenu();
  void connectOther();

  void disconnectMove();
  void disconnectRotate();
  void disconnectZoom();
  void disconnectVisualEffects();

  void initGraphicElements();

  void takeFrame();

  QGifImage *my_gif;
  QBasicTimer timer;

  QString s21_err_messages[6] = {"NO ERROR", "NOT OPEN FILE", "WRONG OBJ",
                                 "MALLOC",   "WRONG MATRIX",  "REALLOC"};
};
#endif  // MAINWINDOW_H_
