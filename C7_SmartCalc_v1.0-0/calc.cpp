#include "calc.h"
#include "credit.h"

#include "ui_calc.h"

Calc::Calc(QWidget *parent) : QMainWindow(parent), ui(new Ui::Calc) {
  setStyleSheet(
      "QLineEdit#dataX {color: black; font: bold 13px 'Pixeloid Sans';}");
  initCalcUi();
  connectAll();
  ui->textEdit->setFocus();
}

Calc::~Calc() {
  delete groupSimpleSymbols;
  delete groupFunctions;
  delete ui;
}

void Calc::initCalcUi() {
  ui->setupUi(this);
  ui->textEdit->setAlignment(Qt::AlignRight);
  ui->dataX->setAlignment(Qt::AlignRight);
  setStyleSheet("QTextEdit#textEdit {padding-top:" +
                QString::number(getPaddingTextEdit()) +
                "px; color:#000;background-color:rgb(180, 179, 161);}");
  createGroupSimpleSymbols();
  createGroupFunctions();
}

int Calc::getPaddingTextEdit() {
  return PADDING_TEXT_EDIT - ui->textEdit->document()->size().height();
}

void Calc::connectAll() {
  connect(ui->textEdit, &QTextEdit::textChanged, this, &Calc::textEditChanged);
  for (auto i : groupSimpleSymbols->buttons()) {
    connect(i, &QPushButton::clicked, this, &Calc::numSimpleSym_clicked);
  }
  for (auto i : groupFunctions->buttons()) {
    connect(i, &QPushButton::clicked, this, &Calc::numFunctions_clicked);
  }
  connect(ui->numExp, &QPushButton::clicked, this, &Calc::numExp_clicked);
  connect(ui->numDel, &QPushButton::clicked, this, &Calc::numDel_clicked);
  connect(ui->numAC, &QPushButton::clicked, this, &Calc::numAC_clicked);
  connect(ui->numEqual, &QPushButton::clicked, this, &Calc::numEqual_clicked);
  connect(ui->showX, &QPushButton::clicked, this, &Calc::showX_clicked);
  connect(ui->showSwitch, &QSlider::valueChanged, this,
          &Calc::showSwitch_changed);
  connect(ui->showGraph, &QPushButton::clicked, this, &Calc::showGraph_clicked);
  connect(ui->dataX, &QLineEdit::textChanged, this, &Calc::dataX_changed);
  connect(ui->credit, &QPushButton::clicked, this, &Calc::credit_clicked);
  connect(ui->deposit, &QPushButton::clicked, this, &Calc::deposit_clicked);
}

void Calc::credit_clicked() {
  credit_win = new Credit();
  credit_win->exec();
}

void Calc::deposit_clicked() {
  deposit_win = new Deposit();
  deposit_win->exec();
}

void Calc::dataX_changed() {
  if (ui->dataX->text() == "")
    ui->dataX->setText("0");
}

void Calc::showSwitch_changed(int value) {
  if (value == 1) {
    win = new Graph(ui->textEdit);
    win->exec();
    ui->showSwitch->setValue(0);
  }
}

void Calc::showGraph_clicked() {
  win = new Graph(ui->textEdit);
  win->exec();
  ui->showSwitch->setValue(0);
}

void Calc::showX_clicked() { ui->showSwitch->setSliderPosition(0); }

void Calc::textEditChanged() {
  int paddingTextEdit = getPaddingTextEdit();
  if (paddingTextEdit >= 0) {
    setStyleSheet("QTextEdit#textEdit {padding-top:" +
                  QString::number(paddingTextEdit) + "px;}");
  }
}

void Calc::checkTextEditOnError() {
  if (ui->textEdit->toPlainText() == "Error!" ||
      ui->textEdit->toPlainText() == "inf" ||
      ui->textEdit->toPlainText() == "-inf" ||
      ui->textEdit->toPlainText() == "nan") {
    numAC_clicked();
  }
}

void Calc::numSimpleSym_clicked() {
  checkTextEditOnError();
  ui->textEdit->insertPlainText(((QPushButton *)sender())->text());
}

void Calc::numFunctions_clicked() {
  checkTextEditOnError();
  ui->textEdit->insertPlainText(((QPushButton *)sender())->text() + "(");
}

void Calc::numExp_clicked() {
  checkTextEditOnError();
  ui->textEdit->insertPlainText("^");
}

void Calc::numDel_clicked() {
  checkTextEditOnError();
  ui->textEdit->textCursor().deletePreviousChar();
}

void Calc::numAC_clicked() {
  ui->textEdit->clear();
  ui->textEdit->setAlignment(Qt::AlignRight);
}

void Calc::numEqual_clicked() {
  QString my_str = ui->textEdit->toPlainText();
  if (my_str != "") {
    StackOut *stack_output = nullptr;
    int error = 0;
    double y = 0;
    if (my_str.length() <= 255) {
      my_str.replace(",", ".");
      double x = ui->dataX->text().toDouble();
      error = calculateRPN(my_str.toLocal8Bit().data(), &stack_output, x, &y);
      numAC_clicked();
    } else {
      error = 1;
    }
    if (error == 0) {
      ui->textEdit->insertPlainText(QString::number(y, 'g', 16));
    } else {
      ui->textEdit->insertPlainText("Error!");
    }
    removeStackOut(stack_output);
  }
}

void Calc::createGroupSimpleSymbols() {
  groupSimpleSymbols = new QButtonGroup(this);
  groupSimpleSymbols->addButton(ui->numZero);
  groupSimpleSymbols->addButton(ui->numOne);
  groupSimpleSymbols->addButton(ui->numTwo);
  groupSimpleSymbols->addButton(ui->numThree);
  groupSimpleSymbols->addButton(ui->numFour);
  groupSimpleSymbols->addButton(ui->numFive);
  groupSimpleSymbols->addButton(ui->numSix);
  groupSimpleSymbols->addButton(ui->numSeven);
  groupSimpleSymbols->addButton(ui->numEight);
  groupSimpleSymbols->addButton(ui->numNine);
  groupSimpleSymbols->addButton(ui->numDot);
  groupSimpleSymbols->addButton(ui->numE);
  groupSimpleSymbols->addButton(ui->numX);
  groupSimpleSymbols->addButton(ui->numLBracket);
  groupSimpleSymbols->addButton(ui->numRBracket);
  groupSimpleSymbols->addButton(ui->numPlus);
  groupSimpleSymbols->addButton(ui->numMinus);
  groupSimpleSymbols->addButton(ui->numMul);
  groupSimpleSymbols->addButton(ui->numDiv);
  groupSimpleSymbols->addButton(ui->numMod);
}

void Calc::createGroupFunctions() {
  groupFunctions = new QButtonGroup(this);
  groupFunctions->addButton(ui->numSin);
  groupFunctions->addButton(ui->numCos);
  groupFunctions->addButton(ui->numTan);
  groupFunctions->addButton(ui->numAsin);
  groupFunctions->addButton(ui->numAcos);
  groupFunctions->addButton(ui->numAtan);
  groupFunctions->addButton(ui->numLog);
  groupFunctions->addButton(ui->numLn);
  groupFunctions->addButton(ui->numSqrt);
}
