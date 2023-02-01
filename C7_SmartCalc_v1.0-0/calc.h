#ifndef SRC_CALC_H
#define SRC_CALC_H
extern "C" {
#include "reverse_polish_notation.h"
}
#include <QButtonGroup>
#include <QKeyEvent>
#include <QLineEdit>
#include <QList>
#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QTextCursor>
#include <QTextEdit>
#include <QWindow>

#include "credit.h"
#include "deposit.h"
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Calc;
}
QT_END_NAMESPACE

class Calc : public QMainWindow {
  Q_OBJECT

 public:
  explicit Calc(QWidget *parent = nullptr);
  ~Calc();
  Ui::Calc *ui;

 private slots:
  void textEditChanged();
  void numSimpleSym_clicked();
  void numFunctions_clicked();
  void numExp_clicked();
  void numDel_clicked();
  void numAC_clicked();
  void numEqual_clicked();
  void showX_clicked();
  void showSwitch_changed(int value);
  void showGraph_clicked();
  void dataX_changed();
  void credit_clicked();
  void deposit_clicked();

 private:
  const int PADDING_TEXT_EDIT = 219;
  void initCalcUi();
  void createGroupSimpleSymbols();
  void createGroupFunctions();
  void checkTextEditOnError();
  void connectAll();
  int getPaddingTextEdit();
  QButtonGroup *groupSimpleSymbols;
  QButtonGroup *groupFunctions;
  Graph *win;
  Credit *credit_win;
  Deposit *deposit_win;
};

#endif  // SRC_CALC_H
