#ifndef SETTINGFORM_H_
#define SETTINGFORM_H_
#include <QDialog>
#include <QSettings>

#include "colorform.h"
namespace Ui {
class SettingForm;
}

typedef struct settingDraw {
  bool proj_type_center;
  bool proj_type_parallel;
  bool line_type_solid;
  bool line_type_dotted;
  QString color_line;
  float width_line;
  bool no_vertex;
  bool circle_vertex;
  bool square_vertex;
  QString color_vertex;
  float size_vertex;
  QString color_background;
} settingDraw;

class SettingForm : public QDialog {
  Q_OBJECT

 public:
  explicit SettingForm(QWidget *parent = nullptr);
  ~SettingForm();
  void initSettingsFromFile();
  settingDraw getSettings();

 signals:
  void backSetting(settingDraw m_set_draw);

 public slots:
  void setColor(QString a);

 private slots:
  void saveForm_clicked();
  void projTypeCenter_clicked();
  void projTypeParallel_clicked();
  void lineTypeSolid_clicked();
  void lineTypeDotted_clicked();
  void colorLine_clicked();
  void widthLine_clicked(double value);
  void noVertex_clicked();
  void circleVertex_clicked();
  void squareVertex_clicked();
  void colorVertex_clicked();
  void sizeVertex_clicked(double value);
  void colorBackground_clicked();

 private:
  Ui::SettingForm *ui;
  ColorForm *m_settingColor;
  settingDraw m_set_draw;
  QSettings *m_startSetting;

  bool m_flag_color_line = false;
  bool m_flag_color_vertex = false;
  bool m_flag_color_background = false;

  void connectAll();
  void initValuesToForm();
};
#endif  // SETTINGFORM_H_
