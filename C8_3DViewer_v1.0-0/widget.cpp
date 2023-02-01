#include "widget.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_texture(0),
      m_indexBuffer(QOpenGLBuffer::IndexBuffer) {}

Widget::~Widget() {
  m_arrayBuffer.destroy();
  m_indexBuffer.destroy();
}

void Widget::initializeGL() {
  QColor color_background;
  if (m_set_draw.color_background != "")
    color_background = QColor(m_set_draw.color_background);
  else
    color_background = Qt::black;
  glClearColor(color_background.redF(), color_background.greenF(),
               color_background.blueF(), 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  initShaders();
  m_arrayBuffer.create();
  m_indexBuffer.create();
}

void Widget::resizeGL(int w, int h) {
  m_aspect = w / float(h ? h : 1);
  changeProjectionMatrix();
}

void Widget::changeProjectionMatrix() {
  m_projectionMatrix.setToIdentity();
  if (m_set_draw.proj_type_parallel)
    m_projectionMatrix.ortho(-50, 50, -30, 30, -100, 10000);
  else if (m_set_draw.proj_type_center)
    m_projectionMatrix.perspective(50, m_aspect, 0.1f, 10000);
}

void Widget::QMatrix4x4ToMatrix4x4() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      my_matrix.m[i][j] = modelViewMatrix(i, j);
    }
  }
}

void Widget::setSettings(settingDraw set_draw) {
  m_set_draw = set_draw;
  changeProjectionMatrix();
  update();
}

void Widget::Matrix4x4ToQMatrix4x4() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      modelViewMatrix(i, j) = my_matrix.m[i][j];
    }
  }
}

void Widget::myMoveGL() {
  s21_move_model(&my_matrix, m_xMove, m_yMove, m_zMove);
}

void Widget::myRotateGL() {
  s21_rotate_x_axis(&my_matrix, m_xRot);
  s21_rotate_y_axis(&my_matrix, m_yRot);
  s21_rotate_z_axis(&my_matrix, m_zRot);
}

void Widget::myScale() { s21_scale_model(&my_matrix, m_scale); }

void Widget::paintGL() {
  if (model) {
    QColor color_background = QColor(m_set_draw.color_background);
    glClearColor(color_background.redF(), color_background.greenF(),
                 color_background.blueF(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    modelViewMatrix.setToIdentity();
    QMatrix4x4ToMatrix4x4();
    myMoveGL();
    myRotateGL();
    myScale();
    Matrix4x4ToQMatrix4x4();

    int index_texture = 0;
    m_texture->bind(index_texture);

    m_program.bind();
    m_program.setUniformValue("qt_ModelViewProjectionMatrix",
                              m_projectionMatrix * modelViewMatrix);
    m_program.setUniformValue("qt_Texture0", index_texture);

    int offset = 0;
    m_arrayBuffer.bind();
    int vertLoc = m_program.attributeLocation("qt_Vertex");
    m_program.enableAttributeArray(vertLoc);
    m_program.setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3,
                                 sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texLoc = m_program.attributeLocation("qt_MultiTexCoord0");
    m_program.enableAttributeArray(texLoc);
    m_program.setAttributeBuffer(texLoc, GL_FLOAT, offset, 2,
                                 sizeof(VertexData));

    glEnable(GL_LINE_STIPPLE);

    if (m_set_draw.line_type_solid) {
      glLineStipple(1, 0x0101);
    } else if (m_set_draw.line_type_dotted) {
      glLineStipple(1, 0x00F0);
    }
    glLineWidth(m_set_draw.width_line);

    if (!(m_set_draw.no_vertex)) {
      glPointSize(m_set_draw.size_vertex);
      if (m_set_draw.circle_vertex) {
        glEnable(GL_POINT_SMOOTH);
      } else if (m_set_draw.square_vertex) {
        glDisable(GL_POINT_SMOOTH);
      }
      QColor color_vertex = QColor(m_set_draw.color_vertex);
      m_program.setUniformValue(
          "result_color", QVector4D(color_vertex.redF(), color_vertex.greenF(),
                                    color_vertex.blueF(), 1));

      glDrawElements(GL_POINTS, m_indexBuffer.size(), GL_UNSIGNED_INT, nullptr);
    }
    QColor color_line = QColor(m_set_draw.color_line);
    m_program.setUniformValue("result_color",
                              QVector4D(color_line.redF(), color_line.greenF(),
                                        color_line.blueF(), 1));
    m_indexBuffer.bind();

    int type_lines = GL_LINES;
    glDrawElements(type_lines, m_indexBuffer.size(), GL_UNSIGNED_INT, nullptr);
  }
}

void Widget::initShaders() {
  if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                         ":/vshader.vsh"))
    close();
  if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                         ":/fshader.fsh"))
    close();
  if (!m_program.link()) close();
}

void Widget::initObject(model_3d *model) {
  initCoordinates();
  QVector<QVector3D> vertices;
  for (size_t i = 0; i < model->vertex_coord.vertex; i++) {
    float data[3];
    for (int j = 0; j < 3; j++) {
      data[j] = model->vertex_coord.matrix[i][j];
    }
    vertices.push_back(QVector3D(data[0], data[1], data[2]));
  }

  QVector<GLuint> indices;
  for (size_t i = 0; i < model->num_poligon; i++) {
    for (size_t j = 0; j < model->poligon[i].num_poligon_vertexes; j++) {
      indices.push_back(model->poligon[i].vertex_array[j]);
    }
  }

  m_arrayBuffer.bind();
  m_arrayBuffer.allocate(vertices.data(),
                         vertices.length() * sizeof(VertexData));
  m_arrayBuffer.release();

  m_indexBuffer.bind();
  m_indexBuffer.allocate(indices.data(), indices.length() * sizeof(GLuint));
  m_indexBuffer.release();

  m_texture = new QOpenGLTexture(QImage(":/texture2.jpeg").mirrored());
  m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
  m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
  m_texture->setWrapMode(QOpenGLTexture::Repeat);
  modelViewMatrix.setToIdentity();
  m_zMove = M_ZROT_START;
  myMoveGL();
  update();
  m_zMove = 0.0f;
}

void Widget::mousePressEvent(QMouseEvent *e) {
  if (e->buttons() == Qt::LeftButton || e->buttons() == Qt::RightButton) {
    m_mousePressPosition = QVector2D(e->position());
  }
}

void Widget::wheelEvent(QWheelEvent *e) {
  if (e->angleDelta().y() > 0) {
    m_zMove *= 0.9f;
  } else {
    m_zMove *= 1.1f;
  }
  update();
}

void Widget::mouseMoveEvent(QMouseEvent *e) {
  QVector2D diff;
  if (e->buttons() == Qt::LeftButton || e->buttons() == Qt::RightButton) {
    diff = QVector2D(e->position()) - m_mousePressPosition;
  }
  if (e->buttons() == Qt::LeftButton) {
    QVector2D diff = QVector2D(e->position()) - m_mousePressPosition;
    if (e->modifiers() == Qt::ControlModifier) {
      setYRotation(m_yRot + diff.x());
      setZRotation(m_zRot + diff.y());
    } else if (e->modifiers() == Qt::AltModifier) {
      setXRotation(m_xRot + diff.y());
      setZRotation(m_zRot + diff.x());
    } else {
      setXRotation(m_xRot + diff.y());
      setYRotation(m_yRot + diff.x());
    }
  } else if (e->buttons() == Qt::RightButton) {
    if (diff.x() != 0 || diff.y() != 0) {
      emit xMoveToSlider((e->position().x() - m_xMove) / 16.0f);

      emit yMoveToSlider((e->position().y() - m_yMove) / 16.0f);
      update();
    }
  }

  if (e->buttons() == Qt::LeftButton || e->buttons() == Qt::RightButton) {
    update();
    m_mousePressPosition = QVector2D(e->position());
  }
}

void Widget::initCoordinates() {
  m_xMove = 0.0f;
  m_yMove = 0.0f;
  m_zMove = 0.0f;
  m_xRot = 0.0f;
  m_yRot = 0.0f;
  m_zRot = 0.0f;
  m_scale = 1.0f;
}

void Widget::setXRotation(int angle) {
  qNormalizeAngle(&angle);
  if (angle != m_xRot) {
    emit xRotateToSlider(angle);
    m_xRot = angle;
    update();
  }
}

void Widget::setYRotation(int angle) {
  qNormalizeAngle(&angle);
  if (angle != m_yRot) {
    emit yRotateToSlider(angle);
    m_yRot = angle;
    update();
  }
}

void Widget::setZRotation(int angle) {
  qNormalizeAngle(&angle);
  if (angle != m_zRot) {
    emit zRotateToSlider(angle);
    m_zRot = angle;
    update();
  }
}

void Widget::qNormalizeAngle(int *angle) {
  while (*angle < 0) *angle += 360;
  while (*angle > 360) *angle -= 360;
}

// Слайдеры перемещения
void Widget::xMoveSliderChanged(int value) {
  emit xMoveToSpinBox(value);
  m_xMove = float(value) / 10;
  update();
}

void Widget::yMoveSliderChanged(int value) {
  emit yMoveToSpinBox(value);
  m_yMove = float(value) / 10;
  update();
}

void Widget::zMoveSliderChanged(int value) {
  emit zMoveToSpinBox(value);
  m_zMove = float(value);
  update();
}

// Слайдеры вращения
void Widget::xRotateSliderChanged(int value) {
  emit xRotateToSpinBox(value);
  m_xRot = (float)value;
  update();
}

void Widget::yRotateSliderChanged(int value) {
  emit yRotateToSpinBox(value);
  m_yRot = (float)value;
  update();
}

void Widget::zRotateSliderChanged(int value) {
  emit zRotateToSpinBox(value);
  m_zRot = (float)value;
  update();
}

void Widget::zoomSliderChanged(int value) {
  emit zoomToSpinBox(value);
  m_scale = (float)value / 100.0f;
  update();
}
