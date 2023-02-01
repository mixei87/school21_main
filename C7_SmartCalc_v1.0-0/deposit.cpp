#include "deposit.h"

#include "credit.h"
#include "ui_deposit.h"

Deposit::Deposit(QWidget *parent) : QDialog(parent), ui(new Ui::Deposit) {
  ui->setupUi(this);
  setAllValidators();
  initAll();
  allConnect();
}

Deposit::~Deposit() { delete ui; }

void Deposit::initAll() {
  initItemsCombobox();
  initStyle();
}

void Deposit::initItemsCombobox() {
  for (int i = 0; i < 10; i++) {
    if (i != IN_THE_END_TERM && i != EVERY_DAY && i != EVERY_WEEK)
      ui->frequency_refill->insertItem(i, m_info_period_date.at(i), i);
  }
  for (int i = 0; i < 10; i++) {
    if (i != IN_THE_END_TERM && i != NOT_DEFINE)
      ui->frequency_capitalization->insertItem(i, m_info_period_date.at(i), i);
  }
  for (int i = 0; i < 10; i++) {
    if (i != NOT_DEFINE)
      ui->frequency_payment->insertItem(i, m_info_period_date.at(i), i);
  }
}

void Deposit::initStyle() {
  ui->label_6_frequency_payment->setVisible(false);
  ui->frequency_payment->setVisible(false);
  ui->sum_refill->setVisible(false);
  ui->date_start_deposit->setDate(QDate::currentDate());

  ui->table_deposit->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
  ui->table_deposit->horizontalHeader()->setFixedHeight(45);
  ui->table_deposit->verticalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);

  QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  fixedFont.setPointSize(16);
  ui->info_message->setCurrentFont(fixedFont);
}

void Deposit::initDepositTable() {
  m_model_table_deposit = new QStandardItemModel(0, COLUMN_DEPOSIT, this);
  for (int col = 0; col < COLUMN_DEPOSIT; col++) {
    QStandardItem *item;
    if (col != 3)
      item = new QStandardItem(m_headers_table_deposit.at(col));
    else if (ui->add_to_deposit->isChecked())
      item = new QStandardItem("Добавлено\nко вкладу");
    else
      item = new QStandardItem("Выплачено\nпроцентов");
    m_model_table_deposit->setHorizontalHeaderItem(col, item);
  }
  ui->table_deposit->setModel(m_model_table_deposit);
}

void Deposit::setAllValidators() {
  m_validator_sum_deposit = new QIntValidator(1, 100'000'000, this);
  ui->sum_deposit->setValidator(m_validator_sum_deposit);

  m_validator_term_deposit = new QIntValidator(1, 60, this);
  ui->term_deposit->setValidator(m_validator_term_deposit);

  m_validator_deposit_rate = new QDoubleValidator(0.01, 100, 2, this);
  ui->deposit_rate->setValidator(m_validator_deposit_rate);

  m_validator_tax_rate = new QDoubleValidator(0, 100, 2, this);
  ui->tax_rate->setValidator(m_validator_tax_rate);

  m_validator_key_rate = new QDoubleValidator(0, 100, 2, this);
  ui->key_rate->setValidator(m_validator_key_rate);

  m_validator_sum_refill = new QIntValidator(1, 100'000'000, this);
  ui->sum_refill->setValidator(m_validator_sum_refill);
}

void Deposit::allConnect() {
  connect(ui->calculate_deposit, &QPushButton::clicked, this,
          &Deposit::calculate_deposit_clicked);
  connect(ui->add_to_deposit, &QRadioButton::clicked, this,
          &Deposit::add_to_deposit_clicked);
  connect(ui->pay_to_account, &QRadioButton::clicked, this,
          &Deposit::pay_to_account_clicked);
  connect(ui->frequency_refill, &QComboBox::currentIndexChanged, this,
          &Deposit::refill_deposit_currentIndexChanged);
}

void Deposit::refill_deposit_currentIndexChanged(int index) {
  if (ui->frequency_refill->itemData(index).toInt() == NOT_DEFINE)
    ui->sum_refill->setVisible(false);
  else
    ui->sum_refill->setVisible(true);
}

void Deposit::add_to_deposit_clicked() {
  ui->label_6_frequency_payment->setVisible(false);
  ui->frequency_payment->setVisible(false);
  ui->label_7_capitalization->setVisible(true);
  ui->frequency_capitalization->setVisible(true);
}
void Deposit::pay_to_account_clicked() {
  ui->label_7_capitalization->setVisible(false);
  ui->frequency_capitalization->setVisible(false);
  ui->label_6_frequency_payment->setVisible(true);
  ui->frequency_payment->setVisible(true);
}

void Deposit::calculate_deposit_clicked() {
  ui->info_message->clear();
  if (m_model_table_deposit) m_model_table_deposit->clear();
  if (!checkInputData()) {
    initDepositTable();
    printDataTable();
    printPaymentInfo();
  }
}

int Deposit::checkInputData() {
  int error = 0;
  int flag_accept_input = 0;
  if (ui->sum_refill->isVisible())
    flag_accept_input += (!ui->sum_refill->hasAcceptableInput() << 5);
  flag_accept_input += (!ui->key_rate->hasAcceptableInput() << 4) +
                       (!ui->tax_rate->hasAcceptableInput() << 3) +
                       (!ui->deposit_rate->hasAcceptableInput() << 2) +
                       (!ui->term_deposit->hasAcceptableInput() << 1) +
                       (!ui->sum_deposit->hasAcceptableInput());
  for (int i = 0; i < 6; i++, flag_accept_input >>= 1) {
    if (flag_accept_input % 2) {
      writeTextEditor(ui->info_message, m_info_warning.at(i));
      error = 1;
    }
  }
  return error;
}

void Deposit::printPaymentInfo() {
  auto name_title = m_name_title_for_info_message.begin();
  auto title = m_title_for_info_message.begin();
  for (; name_title < m_name_title_for_info_message.end() &&
         title < m_title_for_info_message.end();
       name_title++, title++) {
    writeTextEditor(
        ui->info_message,
        QString("%1%2").arg(*name_title, -30).arg(double_to_str(**title), 16));
  }
}

void Deposit::initTitleForInfoMessage() {
  for (auto title = m_title_for_info_message.begin() + 1;
       title < m_title_for_info_message.end() - 1; title++) {
    **title = 0;
  }
}

void Deposit::printDataTable() {
  bool flag_capitalization = ui->add_to_deposit->isChecked();
  double sum = ui->sum_deposit->text().toDouble();
  double percent_leap_year = 1.0 / (100 * 366);
  double percent_not_leap_year = 1.0 / (100 * 365);
  double deposit_rate = ui->deposit_rate->text().toDouble();
  double tax_rate = ui->tax_rate->text().toDouble() / 100;
  double key_rate_for_tax = ui->key_rate->text().toDouble() + 5;
  double sum_percent = 0;
  double tax_base = 0;
  double sum_refill;
  int refill_term = matchTermRefill();
  if (refill_term != 0)
    sum_refill = ui->sum_refill->text().toDouble();
  else
    sum_refill = 0;

  int months = ui->term_deposit->text().toInt();
  QDate start_date = ui->date_start_deposit->date();
  QDate finish_date = start_date.addMonths(months);
  int count_days = start_date.daysTo(finish_date);

  QDate date = start_date;
  QDate next_date_refill = start_date.addMonths(refill_term);
  QDate next_date_pay;
  setNextDatePay(flag_capitalization, &next_date_pay, start_date, finish_date);

  initTitleForInfoMessage();
  m_sum_start_deposit = sum;

  for (int i = 1; i <= count_days; i++) {
    double for_percent;
    double sum_percent_today;
    double profit_after_tax = 0;
    double tax = 0;
    double sum_refill_today = 0;
    double sum_percent_for_pay_today = 0;

    if (QDate::isLeapYear(date.year())) {
      for_percent = percent_leap_year;
    } else {
      for_percent = percent_not_leap_year;
    }

    date = date.addDays(1);
    sum_percent_today = myRound(sum * deposit_rate * for_percent, 2);
    sum_percent += sum_percent_today;
    if (deposit_rate > key_rate_for_tax) {
      tax_base +=
          sum_percent_today - myRound(sum * key_rate_for_tax * for_percent, 2);
    }

    if (date == next_date_refill && date != finish_date) {
      sum += sum_refill;
      sum_refill_today = sum_refill;
      m_sum_refill += sum_refill_today;
    }

    if (date == next_date_pay || date == finish_date) {
      tax = myRound(tax_base * tax_rate, 0);
      profit_after_tax = sum_percent - tax;
      if (flag_capitalization) sum += profit_after_tax;
      m_sum_tax_base += tax_base;
      m_sum_tax += tax;
      m_sum_percent += sum_percent;
      m_profit_after_tax += profit_after_tax;
      sum_percent_for_pay_today = sum_percent;
      sum_percent = 0;
      tax_base = 0;
    }

    if (date == next_date_pay || date == next_date_refill ||
        date == finish_date)
      addDataToTable(i - 1, date, sum_percent_for_pay_today, tax,
                     profit_after_tax, sum_refill_today, sum);
    if (date == next_date_pay) {
      setNextDatePay(flag_capitalization, &next_date_pay, next_date_pay,
                     finish_date);
    }
    if (date == next_date_refill && date != finish_date)
      next_date_refill = next_date_refill.addMonths(refill_term);
  }
  m_sum_finish_deposit = sum;
}

void Deposit::setNextDatePay(bool flag_capitalization, QDate *next_date_pay,
                             QDate start_date, QDate finish_date) {
  if (flag_capitalization)
    *next_date_pay = matchTermCapitalization(start_date);
  else
    *next_date_pay = matchTermPayment(start_date, finish_date);
}

QDate Deposit::matchTermCapitalization(QDate start_date) {
  QDate next_date = start_date;
  int term_capitalization_enum = getDataCombobox(ui->frequency_capitalization);
  if (term_capitalization_enum == EVERY_DAY)
    next_date = start_date.addDays(1);
  else if (term_capitalization_enum == EVERY_WEEK)
    next_date = start_date.addDays(7);
  else if (term_capitalization_enum == EVERY_1_MONTH)
    next_date = start_date.addMonths(1);
  else if (term_capitalization_enum == EVERY_2_MONTH)
    next_date = start_date.addMonths(2);
  else if (term_capitalization_enum == EVERY_3_MONTH)
    next_date = start_date.addMonths(3);
  else if (term_capitalization_enum == EVERY_4_MONTH)
    next_date = start_date.addMonths(4);
  else if (term_capitalization_enum == EVERY_6_MONTH)
    next_date = start_date.addMonths(6);
  else if (term_capitalization_enum == EVERY_12_MONTH)
    next_date = start_date.addMonths(12);
  return next_date;
}

QDate Deposit::matchTermPayment(QDate start_date, QDate finish_date) {
  QDate next_date = start_date;
  int term_payment_enum = getDataCombobox(ui->frequency_payment);
  if (term_payment_enum == IN_THE_END_TERM)
    next_date = finish_date;
  else if (term_payment_enum == EVERY_DAY)
    next_date = start_date.addDays(1);
  else if (term_payment_enum == EVERY_WEEK)
    next_date = start_date.addDays(7);
  else if (term_payment_enum == EVERY_1_MONTH)
    next_date = start_date.addMonths(1);
  else if (term_payment_enum == EVERY_2_MONTH)
    next_date = start_date.addMonths(2);
  else if (term_payment_enum == EVERY_3_MONTH)
    next_date = start_date.addMonths(3);
  else if (term_payment_enum == EVERY_4_MONTH)
    next_date = start_date.addMonths(4);
  else if (term_payment_enum == EVERY_6_MONTH)
    next_date = start_date.addMonths(6);
  else if (term_payment_enum == EVERY_12_MONTH)
    next_date = start_date.addMonths(12);
  return next_date;
}

int Deposit::getDataCombobox(QComboBox *combobox) {
  return combobox->itemData(combobox->currentIndex()).toInt();
}

int Deposit::matchTermRefill() {
  int term_refill = 0;
  int term_refill_enum = getDataCombobox(ui->frequency_refill);
  if (term_refill_enum == NOT_DEFINE)
    term_refill = 0;
  else if (term_refill_enum == EVERY_1_MONTH)
    term_refill = 1;
  else if (term_refill_enum == EVERY_2_MONTH)
    term_refill = 2;
  else if (term_refill_enum == EVERY_3_MONTH)
    term_refill = 3;
  else if (term_refill_enum == EVERY_4_MONTH)
    term_refill = 4;
  else if (term_refill_enum == EVERY_6_MONTH)
    term_refill = 6;
  else if (term_refill_enum == EVERY_12_MONTH)
    term_refill = 12;
  return term_refill;
}

void Deposit::addDataToTable(int row, QDate date, double percent, double tax,
                             double capitalization, double refill, double sum) {
  QList<QStandardItem *> items;
  items.append(new QStandardItem(QString::number(row + 1) + " день\n" +
                                 date.toString("dd.MM.yyyy")));
  items.append(new QStandardItem(double_to_str(percent)));
  items.append(new QStandardItem(double_to_str(tax)));
  items.append(new QStandardItem(double_to_str(capitalization)));
  items.append(new QStandardItem(double_to_str(refill)));
  items.append(new QStandardItem(double_to_str(sum)));
  m_model_table_deposit->appendRow(items);
}

double Deposit::myRound(double x, int precision) {
  double num = pow(10, precision);
  return round(x * num) / num;
}

QString Deposit::double_to_str(double x) { return QString::number(x, 'f', 2); }

void Deposit::writeTextEditor(QTextEdit *text_edit, QString str) {
  if (text_edit->toPlainText().isEmpty()) {
    text_edit->insertPlainText(str);
  } else {
    text_edit->moveCursor(QTextCursor::End);
    text_edit->insertPlainText("\n" + str);
  }
}
