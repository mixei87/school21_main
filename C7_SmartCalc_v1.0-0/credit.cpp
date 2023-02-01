#include "credit.h"

#include "ui_credit.h"

Credit::Credit(QWidget *parent) : QDialog(parent), ui(new Ui::Credit) {
  ui->setupUi(this);
  initTable();
  initInfoWarning();
  setAllValidators();
  connect(ui->payment_schedule, &QPushButton::clicked, this,
          &Credit::payment_schedule_clicked);
}

Credit::~Credit() { delete ui; }

void Credit::initTable() {
  ui->table_pay_shedule->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
  ui->table_pay_shedule->horizontalHeader()->setDefaultAlignment(
      Qt::AlignLeft | Qt::AlignVCenter);
  m_header_items = {" Месяц", " Остаток", " Проценты", " Погашено", " Платёж"};
}

void Credit::initInfoWarning() {
  m_info_warning = {"Введите сумму кредита: от 1 до 100'000'000",
                    "Введите срок кредита: от 1 до 360",
                    "Введите процентную ставку: от 0.01% до 100%"};
}

void Credit::setAllValidators() {
  m_validator_sum_credit = new QIntValidator(1, 100'000'000, this);
  ui->sum_credit->setValidator(m_validator_sum_credit);

  m_validator_credit_term = new QIntValidator(1, 360, this);
  ui->term->setValidator(m_validator_credit_term);

  m_validator_percent = new QDoubleValidator(0.01, 100, 2, this);
  ui->percent->setValidator(m_validator_percent);
}

void Credit::payment_schedule_clicked() {
  ui->info_message->clear();
  if (m_model_table) m_model_table->clear();
  if (!checkInputData()) {
    initModel();
    printDataTable();
    printPaymentInfo();
  }
}

void Credit::printPaymentInfo() {
  writeTextEditor(ui->info_message, "Ежемесячный платёж:\t " + m_month_payment);
  writeTextEditor(ui->info_message, "Переплата:\t\t " + m_over_payment);
  writeTextEditor(ui->info_message, "Общая сумма выплат:\t " + m_total_payment);
}

int Credit::checkInputData() {
  int error = 0;
  int flag_accept_input = (!ui->percent->hasAcceptableInput() << 2) +
                          (!ui->term->hasAcceptableInput() << 1) +
                          (!ui->sum_credit->hasAcceptableInput());
  for (int i = 0; i < 3; i++, flag_accept_input >>= 1) {
    if (flag_accept_input % 2) {
      writeTextEditor(ui->info_message, m_info_warning.at(i));
      error = 1;
    }
  }
  return error;
}

void Credit::initModel() {
  ui->table_pay_shedule->setStyleSheet(
      "QTableView#table_pay_shedule {gridline-color: #b0b0b0;} "
      "QHeaderView::section {color: black; background-color: white; "
      "font-size: 14pt; border-style: none; border-bottom: 1px solid "
      "#b0b0b0; border-right: 1px solid #b0b0b0;} "
      "QHeaderView::section:horizontal {border-top: 1px solid #b0b0b0;} "
      "QHeaderView::section:vertical {border-left : 1px solid #b0b0b0;} "
      "QTableView::item {background-color: #606060; color: white;}");
  m_model_table =
      new QStandardItemModel(ui->term->text().toInt(), COLUMN_TABLE, this);
  for (int i = 0; i < COLUMN_TABLE; i++) {
    m_model_table->setHorizontalHeaderItem(
        i, new QStandardItem(m_header_items.at(i)));
  }
  ui->table_pay_shedule->setModel(m_model_table);
}

void Credit::printDataTable() {
  double sum = ui->sum_credit->text().toDouble();
  int term = ui->term->text().toInt();
  double percent = ui->percent->text().toDouble() / (100 * 12);
  double payment = 0;
  double pay_credit;
  double pay_percents;
  double sum_payment = 0;

  if (ui->annuity->isChecked()) {
    payment = sum * (percent + percent / (pow(1 + percent, term) - 1));
    m_month_payment = double_to_str(payment);
    sum_payment = payment * term;
    for (int row = 0; row < term; row++) {
      pay_percents = sum * percent;
      pay_credit = payment - pay_percents;
      sum -= pay_credit;
      addDataToTable(m_model_table, row, COLUMN_TABLE, sum, pay_percents,
                     pay_credit, payment);
    }
  } else if (ui->differential->isChecked()) {
    pay_credit = sum / term;
    for (int row = 0; row < term; row++) {
      pay_percents = sum * percent;
      payment = pay_credit + pay_percents;
      sum_payment += payment;
      if (row == 0) m_month_payment = double_to_str(payment);
      sum -= pay_credit;
      addDataToTable(m_model_table, row, COLUMN_TABLE, sum, pay_percents,
                     pay_credit, payment);
    }
    m_month_payment += " - " + double_to_str(payment);
  }
  m_total_payment = double_to_str(sum_payment);
  m_over_payment =
      double_to_str(sum_payment - ui->sum_credit->text().toDouble());
}

void Credit::addDataToTable(QStandardItemModel *model, int row, int col,
                            double sum, double pay_percents, double pay_credit,
                            double payment) {
  QVector<QString> data_to_payment = {
      QString::number(row + 1), double_to_str(sum), double_to_str(pay_percents),
      double_to_str(pay_credit), double_to_str(payment)};
  setAllDataToPaymentTable(model, row, col, data_to_payment);
}

void Credit::setAllDataToPaymentTable(QStandardItemModel *model, int row,
                                      int col, QVector<QString> data) {
  for (int i = 0; i < col; i++) {
    setDataToModel(model, row, i, data.at(i));
  }
}

void Credit::setDataToModel(QStandardItemModel *model, int row, int col,
                            QVariant value) {
  model->setData(model->index(row, col, QModelIndex()), value);
}

QString Credit::double_to_str(double x) { return QString::number(x, 'f', 2); }

void Credit::writeTextEditor(QTextEdit *text_edit, QString str) {
  if (text_edit->toPlainText().isEmpty()) {
    text_edit->insertPlainText(str);
  } else {
    text_edit->moveCursor(QTextCursor::End);
    text_edit->insertPlainText("\n" + str);
  }
}
