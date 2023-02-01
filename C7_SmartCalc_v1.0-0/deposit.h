#ifndef SRC_DEPOSIT_H
#define SRC_DEPOSIT_H

#include <math.h>

#include <QComboBox>
#include <QDate>
#include <QDialog>
#include <QFontDatabase>
#include <QPushButton>
#include <QStandardItemModel>
#include <QString>
#include <QTableView>
#include <QTextEdit>
#include <QValidator>

namespace Ui {
class Deposit;
}

class Deposit : public QDialog {
  Q_OBJECT

 public:
  explicit Deposit(QWidget *parent = nullptr);
  double myRound(double x, int precision);
  ~Deposit();

 private slots:
  void calculate_deposit_clicked();
  void add_to_deposit_clicked();
  void pay_to_account_clicked();
  void refill_deposit_currentIndexChanged(int index);

 private:
  typedef enum Items_Combobox {
    IN_THE_END_TERM,
    NOT_DEFINE,
    EVERY_DAY,
    EVERY_WEEK,
    EVERY_1_MONTH,
    EVERY_2_MONTH,
    EVERY_3_MONTH,
    EVERY_4_MONTH,
    EVERY_6_MONTH,
    EVERY_12_MONTH,
  } Items_Combobox;

  const int COLUMN_DEPOSIT = 6;
  const int COLUMN_ADDITIONAL_TABLE = 2;

  Ui::Deposit *ui;

  QValidator *m_validator_sum_deposit = nullptr;
  QValidator *m_validator_term_deposit = nullptr;
  QValidator *m_validator_deposit_rate = nullptr;
  QValidator *m_validator_tax_rate = nullptr;
  QValidator *m_validator_key_rate = nullptr;
  QValidator *m_validator_sum_refill = nullptr;

  QStandardItemModel *m_model_table_deposit = nullptr;

  QVector<QString> m_info_period_date = {
      "в конце срока", "не предусмотрено", "ежедневно",     "еженедельно",
      "раз в месяц",   "раз в 2 месяца",   "раз в квартал", "раз в 4 месяца",
      "раз в полгода", "раз в год"};

  QVector<QString> m_info_warning = {
      "Введите сумму депозита: от 1 до 100'000'000",
      "Введите срок депозита: от 1 до 60",
      "Введите процентную ставку вклада: от 0.01% до 100%",
      "Введите налоговую ставку: от 0% до 100%",
      "Введите ключевую ставку ЦБ: от 0% до 100%",
      "Введите доп.сумму: от 1 до 100'000'000"};
  QVector<QString> m_headers_table_deposit = {
      "Дата\nрасчета",      "Начислено\nпроцентов",
      "Уплаченный\nналог",  "",
      "Пополнение\nвклада", "Сумма вклада\nна конец срока"};
  QVector<QString> m_headers_additional_table = {"Месяц", "Сумма"};

  QVector<QString> m_name_title_for_info_message = {
      "Сумма вклада:",
      "Сумма довложений:",
      "Начисленные проценты:",
      "Облагается налогом:",
      "Удержано налогов:",
      "Доход за вычетом налогов:",
      "Cумма на вкладе к концу срока:"};

  double m_sum_start_deposit = 0;
  double m_sum_refill = 0;
  double m_sum_percent = 0;
  double m_sum_tax_base = 0;
  double m_sum_tax = 0;
  double m_profit_after_tax = 0;
  double m_sum_finish_deposit = 0;

  QVector<double *> m_title_for_info_message = {
      &m_sum_start_deposit, &m_sum_refill, &m_sum_percent,
      &m_sum_tax_base,      &m_sum_tax,    &m_profit_after_tax,
      &m_sum_finish_deposit};

  void initAll();
  void initStyle();
  void allConnect();
  void writeTextEditor(QTextEdit *text_edit, QString str);
  QString double_to_str(double x);
  void addDataToTable(int row, QDate date, double percent, double tax,
                      double capitalization, double refill, double sum);
  void initItemsCombobox();
  void initDepositTable();
  void setAllValidators();
  void printDataTable();
  void initTitleForInfoMessage();
  void printPaymentInfo();
  int checkInputData();
  int getDataCombobox(QComboBox *combobox);
  int matchTermRefill();
  QDate matchTermCapitalization(QDate start_date);
  QDate matchTermPayment(QDate start_date, QDate finish_date);
  void setNextDatePay(bool flag_capitalization, QDate *next_date_pay,
                      QDate start_date, QDate finish_date);
};

#endif  // SRC_DEPOSIT_H
