#include "graph.h"

#include "ui_graph.h"
#include <limits.h>

Graph::Graph(QTextEdit *textEdit, QWidget *parent)
    : QDialog(parent), ui(new Ui::Graph) {
  ui->setupUi(this);
  initStartValues(textEdit);
  connectAll();
  createGraph_clicked();
}

Graph::~Graph() { delete ui; }

void Graph::mySetText(QLineEdit *line_edit, double value) {
  line_edit->setText(QString::number(value, 'g', 4));
}

void Graph::mySetText(QLineEdit *line_edit, unsigned long value) {
  line_edit->setText(QString::number(value, 'g', 4));
}

double Graph::getDoubleFromText(QLineEdit *line_edit) {
  return line_edit->text().toDouble();
}

void Graph::to_count_step_clicked() {
  double stepX = getDoubleFromText(ui->stepX);
  if (stepX <= 1E-16) {
    mySetText(ui->stepX, 0.0);
    mySetText(ui->countStepX, 1.0);
  } else {
    double tmp =
        (getDoubleFromText(ui->endX) - getDoubleFromText(ui->startX)) / stepX;
    if (tmp < 1 + (double)ULONG_MAX) {
      unsigned long count_step = 1 + (unsigned long)tmp;
      mySetText(ui->countStepX, count_step);
    } else {
      mySetText(ui->stepX, 0.0);
      mySetText(ui->countStepX, 1.0);
    }
  }
}
void Graph::to_stepX_clicked() {
  unsigned long count_step = (unsigned long)getDoubleFromText(ui->countStepX);
  if (count_step <= 1) {
    mySetText(ui->stepX,
              getDoubleFromText(ui->endX) - getDoubleFromText(ui->startX));
    mySetText(ui->countStepX, 1.0);
  } else {
    double stepX =
        (getDoubleFromText(ui->endX) - getDoubleFromText(ui->startX)) /
        (count_step - 1);
    mySetText(ui->stepX, stepX);
    mySetText(ui->countStepX, count_step);
  }
}

void Graph::initStartValues(QTextEdit *textEdit) {
  ui->exprGraph->setText(textEdit->toPlainText());
  double startX = -20;
  double endX = 20;
  double startY = -20;
  double endY = 20;
  unsigned long countStepX = 100001;
  mySetText(ui->startX, startX);
  mySetText(ui->endX, endX);
  mySetText(ui->startY, startY);
  mySetText(ui->endY, endY);
  mySetText(ui->countStepX, countStepX);
  mySetText(ui->stepX, (endX - startX) / (countStepX - 1));
}

void Graph::connectAll() {
  connect(ui->to_count_step, &QPushButton::clicked, this,
          &Graph::to_count_step_clicked);
  connect(ui->to_stepX, &QPushButton::clicked, this, &Graph::to_stepX_clicked);

  connect(ui->createGraph, &QPushButton::clicked, this,
          &Graph::createGraph_clicked);
}

void Graph::createGraph_clicked() {
  double endX = getDoubleFromText(ui->endX);
  double startX = getDoubleFromText(ui->startX);
  double endY = getDoubleFromText(ui->endY);
  double startY = getDoubleFromText(ui->startY);
  double countStepX = getDoubleFromText(ui->countStepX);
  double stepX = getDoubleFromText(ui->stepX);

  if (endX > startX && endY > startY) {
    to_stepX_clicked();
    if (countStepX >= 1 && (endX - startX) / stepX < 1 + (double)ULONG_MAX) {
      ui->graph->clearGraphs();
      QVector<double> x_axis, y_axis;
      QString my_str = ui->exprGraph->text();
      my_str.replace(",", ".");
      createCoordinatesForGraph(my_str.toLocal8Bit().data(), &x_axis, &y_axis);
      initSettingsGraph(x_axis, y_axis);
      ui->graph->replot();
    } else {
      ui->graph->clearGraphs();
      ui->graph->replot();
      ui->exprGraph->setText("Incorrect ranges!");
    }
  } else {
    ui->graph->clearGraphs();
    ui->graph->replot();
    ui->exprGraph->setText("Incorrect ranges!");
  }
}

void Graph::createCoordinatesForGraph(char *str_expression,
                                      QVector<double> *x_axis,
                                      QVector<double> *y_axis) {
  StackOut *stack_output = nullptr;
  int error = 0;
  double x = ui->startX->text().toDouble();
  double y = 0;
  double stepX = ui->stepX->text().toDouble();
  unsigned long countStepX = (unsigned long)getDoubleFromText(ui->countStepX);

  for (unsigned long i = 0; error == 0 && i < countStepX; i++, x += stepX) {
    error = calculateRPN(str_expression, &stack_output, x, &y);
    if (error != 0) {
      ui->exprGraph->setText("Error!");
      break;
    }
    x_axis->push_back(x);
    y_axis->push_back(y);
  }
  removeStackOut(stack_output);
}

void Graph::initSettingsGraph(QVector<double> x_axis, QVector<double> y_axis) {
  ui->graph->xAxis->setRange(getDoubleFromText(ui->startX),
                             getDoubleFromText(ui->endX));
  ui->graph->yAxis->setRange(getDoubleFromText(ui->startY),
                             getDoubleFromText(ui->endY));
  ui->graph->addGraph();
  ui->graph->graph(0)->setLineStyle(QCPGraph::lsNone);
  ui->graph->graph(0)->setScatterStyle(
      QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
  ui->graph->graph(0)->addData(x_axis, y_axis);
  ui->graph->setInteraction(QCP::iRangeDrag, true);
  ui->graph->setInteraction(QCP::iRangeZoom, true);
}
