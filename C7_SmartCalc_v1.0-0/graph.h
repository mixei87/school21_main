#ifndef SRC_GRAPH_H
#define SRC_GRAPH_H

extern "C" {
#include "reverse_polish_notation.h"
}
#include "qcustomplot.h"
namespace Ui {
class Graph;
}

class Graph : public QDialog {
  Q_OBJECT

 public:
  explicit Graph(QTextEdit *textEdit, QWidget *parent = nullptr);
  ~Graph();

 private slots:
  void createGraph_clicked();
  void to_count_step_clicked();
  void to_stepX_clicked();

 private:
  Ui::Graph *ui;
  void createCoordinatesForGraph(char *str_expression, QVector<double> *x_axis,
                                 QVector<double> *y_axis);
  void initStartValues(QTextEdit *textEdit);
  void connectAll();
  void mySetText(QLineEdit *line_edit, double value);
  void mySetText(QLineEdit *line_edit, unsigned long value);
  double getDoubleFromText(QLineEdit *line_edit);
  void initSettingsGraph(QVector<double> x_axis, QVector<double> y_axis);
};

#endif  // SRC_GRAPH_H
