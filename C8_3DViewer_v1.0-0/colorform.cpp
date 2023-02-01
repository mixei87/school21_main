#include "colorform.h"

#include "ui_colorform.h"

ColorForm::ColorForm(QWidget *parent) : QDialog(parent), ui(new Ui::ColorForm) {
  ui->setupUi(this);
  connect(ui->pBut1_white, &QPushButton::clicked, this,
          &ColorForm::pBut1_white_clicked);
  connect(ui->pBut2_black, &QPushButton::clicked, this,
          &ColorForm::pBut2_black_clicked);
  connect(ui->pBut3_gray, &QPushButton::clicked, this,
          &ColorForm::pBut3_gray_clicked);
  connect(ui->pBut4_red, &QPushButton::clicked, this,
          &ColorForm::pBut4_red_clicked);
  connect(ui->pBut5_green, &QPushButton::clicked, this,
          &ColorForm::pBut5_green_clicked);
  connect(ui->pBut6_blue, &QPushButton::clicked, this,
          &ColorForm::pBut6_blue_clicked);
  connect(ui->pBut7_cyan, &QPushButton::clicked, this,
          &ColorForm::pBut7_cyan_clicked);
  connect(ui->pBut8_magenta, &QPushButton::clicked, this,
          &ColorForm::pBut8_magenta_clicked);
  connect(ui->pBut9_yellow, &QPushButton::clicked, this,
          &ColorForm::pBut9_yellow_clicked);
}

ColorForm::~ColorForm() { delete ui; }

void ColorForm::pBut1_white_clicked() {
  emit getColor("white");
  close();
}
void ColorForm::pBut2_black_clicked() {
  emit getColor("black");
  close();
}
void ColorForm::pBut3_gray_clicked() {
  emit getColor("darkGray");
  close();
}
void ColorForm::pBut4_red_clicked() {
  emit getColor("darkRed");
  close();
}
void ColorForm::pBut5_green_clicked() {
  emit getColor("darkGreen");
  close();
}
void ColorForm::pBut6_blue_clicked() {
  emit getColor("darkBlue");
  close();
}
void ColorForm::pBut7_cyan_clicked() {
  emit getColor("darkCyan");
  close();
}
void ColorForm::pBut8_magenta_clicked() {
  emit getColor("darkMagenta");
  close();
}
void ColorForm::pBut9_yellow_clicked() {
  emit getColor("yellow");
  close();
}
