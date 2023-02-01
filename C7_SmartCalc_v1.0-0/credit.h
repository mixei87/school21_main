#ifndef SRC_CREDIT_H
#define SRC_CREDIT_H

#include <math.h>

#include <QDialog>
#include <QPushButton>
#include <QStandardItemModel>
#include <QString>
#include <QTextEdit>
#include <QValidator>

namespace Ui {
class Credit;
}

class Credit : public QDialog {
  Q_OBJECT

 public:
  explicit Credit(QWidget *parent = nullptr);
  ~Credit();

 private slots:
  void payment_schedule_clicked();

 private:
  const int COLUMN_TABLE = 5;

  Ui::Credit *ui;
  QValidator *m_validator_sum_credit = nullptr;
  QValidator *m_validator_credit_term = nullptr;
  QValidator *m_validator_percent = nullptr;
  QStandardItemModel *m_model_table = nullptr;
  QVector<QString> m_info_warning;
  QVector<QString> m_header_items;
  QString m_month_payment;
  QString m_over_payment;
  QString m_total_payment;

  void writeTextEditor(QTextEdit *text_edit, QString str);

  void setDataToModel(QStandardItemModel *model, int row, int col,
                      QVariant value);
  void setAllDataToPaymentTable(QStandardItemModel *model, int row, int col,
                                QVector<QString> data);
  void addDataToTable(QStandardItemModel *model, int row, int col, double sum,
                      double pay_percents, double pay_credit, double payment);
  void initTable();
  void initModel();
  void setAllValidators();
  void printDataTable();
  void printPaymentInfo();
  void initInfoWarning();
  int checkInputData();
  QString double_to_str(double x);
};

#endif  // SRC_CREDIT_H
