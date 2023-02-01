#include "settingform.h"

#include "ui_settingform.h"

SettingForm::SettingForm(QWidget *parent)
    : QDialog(parent), ui(new Ui::SettingForm) {
  ui->setupUi(this);
  m_settingColor = new ColorForm;
  connectAll();
  initSettingsFromFile();
  initValuesToForm();
}

SettingForm::~SettingForm() { delete ui; }

settingDraw SettingForm::getSettings() { return m_set_draw; }

void SettingForm::initValuesToForm() {
  ui->pBut1_color_line->setStyleSheet("background-color: " +
                                      m_set_draw.color_line);
  ui->pBut2_color_vertex->setStyleSheet("background-color: " +
                                        m_set_draw.color_vertex);
  ui->pBut3_color_background->setStyleSheet("background-color: " +
                                            m_set_draw.color_background);
  ui->dSpinBox1_width_line->setValue(m_set_draw.width_line);
  ui->dSpinBox2_size_vertex->setValue(m_set_draw.size_vertex);
  ui->radBut1_central_projection->setChecked(m_set_draw.proj_type_center);
  ui->radBut2_parallel_projection->setChecked(m_set_draw.proj_type_parallel);
  ui->radBut3_solid_line->setChecked(m_set_draw.line_type_solid);
  ui->radBut4_dotted_line->setChecked(m_set_draw.line_type_dotted);
  ui->radBut5_no_vertex->setChecked(m_set_draw.no_vertex);
  ui->radBut6_circle_vertex->setChecked(m_set_draw.circle_vertex);
  ui->radBut7_square_vertex->setChecked(m_set_draw.square_vertex);
}

void SettingForm::connectAll() {
  connect(m_settingColor, &ColorForm::getColor, this, &SettingForm::setColor);
  connect(ui->pBut1_color_line, &QPushButton::clicked, this,
          &SettingForm::colorLine_clicked);
  connect(ui->pBut2_color_vertex, &QPushButton::clicked, this,
          &SettingForm::colorVertex_clicked);
  connect(ui->pBut3_color_background, &QPushButton::clicked, this,
          &SettingForm::colorBackground_clicked);
  connect(ui->pBut4_save, &QPushButton::clicked, this,
          &SettingForm::saveForm_clicked);

  connect(ui->radBut1_central_projection, &QRadioButton::clicked, this,
          &SettingForm::projTypeCenter_clicked);
  connect(ui->radBut2_parallel_projection, &QRadioButton::clicked, this,
          &SettingForm::projTypeParallel_clicked);
  connect(ui->radBut3_solid_line, &QRadioButton::clicked, this,
          &SettingForm::lineTypeSolid_clicked);
  connect(ui->radBut4_dotted_line, &QRadioButton::clicked, this,
          &SettingForm::lineTypeDotted_clicked);
  connect(ui->radBut5_no_vertex, &QRadioButton::clicked, this,
          &SettingForm::noVertex_clicked);
  connect(ui->radBut6_circle_vertex, &QRadioButton::clicked, this,
          &SettingForm::circleVertex_clicked);
  connect(ui->radBut7_square_vertex, &QRadioButton::clicked, this,
          &SettingForm::squareVertex_clicked);

  connect(ui->dSpinBox1_width_line, &QDoubleSpinBox::valueChanged, this,
          &SettingForm::widthLine_clicked);
  connect(ui->dSpinBox2_size_vertex, &QDoubleSpinBox::valueChanged, this,
          &SettingForm::sizeVertex_clicked);
}

void SettingForm::initSettingsFromFile() {
  m_startSetting = new QSettings("settings.conf", QSettings::NativeFormat);
  m_set_draw.proj_type_center =
      m_startSetting->value("Settings/proj_type_center", true).toBool();
  m_set_draw.proj_type_parallel =
      m_startSetting->value("Settings/proj_type_parallel", false).toBool();
  m_set_draw.line_type_solid =
      m_startSetting->value("Settings/line_type_solid", true).toBool();
  m_set_draw.line_type_dotted =
      m_startSetting->value("Settings/line_type_dotted", false).toBool();
  m_set_draw.color_line =
      m_startSetting->value("Settings/color_line", "white").toString();
  m_set_draw.width_line =
      m_startSetting->value("Settings/width_line", 1.0f).toFloat();
  m_set_draw.no_vertex =
      m_startSetting->value("Settings/no_vertex", true).toBool();
  m_set_draw.circle_vertex =
      m_startSetting->value("Settings/circle_vertex", false).toBool();
  m_set_draw.square_vertex =
      m_startSetting->value("Settings/square_vertex", false).toBool();
  m_set_draw.color_vertex =
      m_startSetting->value("Settings/color_vertex", "white").toString();
  m_set_draw.size_vertex =
      m_startSetting->value("Settings/size_vertex", 1.0f).toFloat();
  m_set_draw.color_background =
      m_startSetting->value("Settings/color_background", "black").toString();
}

void SettingForm::saveForm_clicked() {
  QSettings *settings = new QSettings("settings.conf", QSettings::NativeFormat);
  settings->beginGroup("Settings");
  settings->setValue("color_line", m_set_draw.color_line);
  settings->setValue("color_vertex", m_set_draw.color_vertex);
  settings->setValue("color_background", m_set_draw.color_background);
  settings->setValue("width_line", m_set_draw.width_line);
  settings->setValue("size_vertex", m_set_draw.size_vertex);

  if (ui->radBut1_central_projection->isChecked()) {
    settings->setValue("proj_type_center", true);
    settings->setValue("proj_type_parallel", false);
  } else if (ui->radBut2_parallel_projection->isChecked()) {
    settings->setValue("proj_type_center", false);
    settings->setValue("proj_type_parallel", true);
  }

  if (ui->radBut3_solid_line->isChecked()) {
    settings->setValue("line_type_solid", true);
    settings->setValue("line_type_dotted", false);
  } else if (ui->radBut4_dotted_line->isChecked()) {
    settings->setValue("line_type_solid", false);
    settings->setValue("line_type_dotted", true);
  }

  if (ui->radBut5_no_vertex->isChecked()) {
    settings->setValue("no_vertex", true);
    settings->setValue("circle_vertex", false);
    settings->setValue("square_vertex", false);
  } else if (ui->radBut6_circle_vertex->isChecked()) {
    settings->setValue("no_vertex", false);
    settings->setValue("circle_vertex", true);
    settings->setValue("square_vertex", false);
  } else if (ui->radBut7_square_vertex->isChecked()) {
    settings->setValue("no_vertex", false);
    settings->setValue("circle_vertex", false);
    settings->setValue("square_vertex", true);
  }

  settings->endGroup();
  settings->sync();
  emit backSetting(m_set_draw);
  close();
}

void SettingForm::setColor(QString a) {
  if (m_flag_color_line) {
    m_set_draw.color_line = a;
    ui->pBut1_color_line->setStyleSheet("background-color: " +
                                        m_set_draw.color_line);
    m_flag_color_line = false;
  } else if (m_flag_color_vertex) {
    m_set_draw.color_vertex = a;
    ui->pBut2_color_vertex->setStyleSheet("background-color: " +
                                          m_set_draw.color_vertex);
    m_flag_color_vertex = false;
  } else if (m_flag_color_background) {
    m_set_draw.color_background = a;
    ui->pBut3_color_background->setStyleSheet("background-color: " +
                                              m_set_draw.color_background);
    m_flag_color_background = false;
  }
}

void SettingForm::projTypeCenter_clicked() {
  m_set_draw.proj_type_center = true;
  m_set_draw.proj_type_parallel = false;
}
void SettingForm::projTypeParallel_clicked() {
  m_set_draw.proj_type_center = false;
  m_set_draw.proj_type_parallel = true;
}
void SettingForm::lineTypeSolid_clicked() {
  m_set_draw.line_type_solid = true;
  m_set_draw.line_type_dotted = false;
}
void SettingForm::lineTypeDotted_clicked() {
  m_set_draw.line_type_solid = false;
  m_set_draw.line_type_dotted = true;
}
void SettingForm::colorLine_clicked() {
  m_flag_color_line = true;
  m_settingColor->setModal(true);
  m_settingColor->exec();
}
void SettingForm::widthLine_clicked(double value) {
  m_set_draw.width_line = (float)value;
}
void SettingForm::noVertex_clicked() {
  m_set_draw.no_vertex = true;
  m_set_draw.circle_vertex = false;
  m_set_draw.square_vertex = false;
}
void SettingForm::circleVertex_clicked() {
  m_set_draw.no_vertex = false;
  m_set_draw.circle_vertex = true;
  m_set_draw.square_vertex = false;
}
void SettingForm::squareVertex_clicked() {
  m_set_draw.no_vertex = false;
  m_set_draw.circle_vertex = false;
  m_set_draw.square_vertex = true;
}
void SettingForm::colorVertex_clicked() {
  m_flag_color_vertex = true;
  m_settingColor->setModal(true);
  m_settingColor->exec();
}
void SettingForm::sizeVertex_clicked(double value) {
  m_set_draw.size_vertex = (float)value;
}

void SettingForm::colorBackground_clicked() {
  m_flag_color_background = true;
  m_settingColor->setModal(true);
  m_settingColor->exec();
}
