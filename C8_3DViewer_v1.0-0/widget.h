#ifndef WIDGET_H_
#define WIDGET_H_
#define GL_SILENCE_DEPRECATION
#include <QFileInfo>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QSettings>
#include <QVector2D>

#include "settingform.h"

extern "C" {
#include "parser.h"
}

struct VertexData {
  VertexData() {}
  explicit VertexData(QVector3D p) : position(p) {}
  //  VertexData(QVector3D p, QVector2D t, QVector3D n)
  //      : position(p), texCoord(t), normal(n) {}
  QVector3D position;
  //  QVector2D texCoord;
  //  QVector3D normal;
};

class Widget : public QOpenGLWidget {
  Q_OBJECT

 public:
  explicit Widget(QWidget *parent = nullptr);
  ~Widget();
  model_3d *model = nullptr;
  void initObject(model_3d *model);
  void setSettings(settingDraw set_draw);

 public slots:
  void xMoveSliderChanged(int value);
  void yMoveSliderChanged(int value);
  void zMoveSliderChanged(int value);

  void xRotateSliderChanged(int value);
  void yRotateSliderChanged(int value);
  void zRotateSliderChanged(int value);

  void zoomSliderChanged(int value);

 signals:
  void xMoveToSpinBox(int angle);
  void yMoveToSpinBox(int angle);
  void zMoveToSpinBox(int angle);

  void xMoveToSlider(int angle);
  void yMoveToSlider(int angle);
  void zMoveToSlider(int angle);

  void xRotateToSpinBox(int angle);
  void yRotateToSpinBox(int angle);
  void zRotateToSpinBox(int angle);

  void xRotateToSlider(int angle);
  void yRotateToSlider(int angle);
  void zRotateToSlider(int angle);

  void zoomToSlider(int angle);
  void zoomToSpinBox(int angle);

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void initShaders();
  void mousePressEvent(QMouseEvent *e) override;
  void mouseMoveEvent(QMouseEvent *e) override;
  void wheelEvent(QWheelEvent *e) override;

 private:
  QMatrix4x4 m_projectionMatrix;
  QMatrix4x4 modelViewMatrix;
  QOpenGLShaderProgram m_program;
  QOpenGLTexture *m_texture = nullptr;
  QOpenGLBuffer m_arrayBuffer;
  QOpenGLBuffer m_indexBuffer;
  QVector2D m_mousePressPosition;
  settingDraw m_set_draw;

  const float M_ZROT_START = 0;
  float m_xMove = 0.0f;
  float m_yMove = 0.0f;
  float m_zMove = 0.0f;
  float m_xRot = 0.0f;
  float m_yRot = 0.0f;
  float m_zRot = 0.0f;
  float m_scale = 1.0f;

  float m_aspect;

  void setXRotation(int angle);
  void setYRotation(int angle);
  void setZRotation(int angle);
  void qNormalizeAngle(int *angle);

  matrix4x4 my_matrix;
  void QMatrix4x4ToMatrix4x4();
  void Matrix4x4ToQMatrix4x4();

  void myMoveGL();
  void myRotateGL();
  void myScale();
  void initCoordinates();
  void changeProjectionMatrix();
};

#endif  // WIDGET_H_
