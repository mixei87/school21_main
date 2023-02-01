#ifndef COLORFORM_H_
#define COLORFORM_H_
#include <QDialog>
namespace Ui {
class ColorForm;
}
class ColorForm : public QDialog {
  Q_OBJECT

 public:
  explicit ColorForm(QWidget *parent = nullptr);
  ~ColorForm();

 signals:
  void getColor(QString a);

 private slots:
  void pBut1_white_clicked();
  void pBut2_black_clicked();
  void pBut3_gray_clicked();
  void pBut4_red_clicked();
  void pBut5_green_clicked();
  void pBut6_blue_clicked();
  void pBut7_cyan_clicked();
  void pBut8_magenta_clicked();
  void pBut9_yellow_clicked();

 private:
  Ui::ColorForm *ui;
};
#endif  // COLORFORM_H_
