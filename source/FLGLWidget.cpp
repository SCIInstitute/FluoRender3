/**
 * @file FLGLWidget.cpp
 * @author Brig Bagley
 * @version 15 May 2014
 */
#include <QtGui>
#include <QtOpenGL>
#include <cmath>
#include <cstdio>
#include <iostream>
#include "FLGLWidget.h"
#include "qtlogo.h"

FLGLWidget::FLGLWidget(const QGLFormat& format, QWidget * parent) :
    QGLWidget(format, parent),
    m_vertexBuffer( QGLBuffer::VertexBuffer )
{
    QGLFormat glFormat = QGLWidget::format();
    qDebug() << QString( "OpenGL Version = %1, %2" )
    .arg( glFormat.majorVersion() )
    .arg( glFormat.minorVersion() );
    
    if ( !glFormat.sampleBuffers() )
        qWarning() << "Could not enable sample buffers";
    
    qDebug() << "OpenGL context valid =" << context()->isValid();

    logo = 0;
   xRot = 0;
   yRot = 0;
   zRot = 0;
}

FLGLWidget::~FLGLWidget() {}

QSize FLGLWidget::minimumSizeHint() const
{
   return QSize(50, 50);
}

QSize FLGLWidget::sizeHint() const
{
   return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
   while (angle < 0)
      angle += 360 * 16;
   while (angle > 360 * 16)
      angle -= 360 * 16;
}

void FLGLWidget::setXRotation(int angle)
{
   qNormalizeAngle(angle);
   if (angle != xRot) {
      xRot = angle;
      emit xRotationChanged(angle);
      updateGL();
   }
}

void FLGLWidget::setYRotation(int angle)
{
   qNormalizeAngle(angle);
   if (angle != yRot) {
      yRot = angle;
      emit yRotationChanged(angle);
      updateGL();
   }
}

void FLGLWidget::setZRotation(int angle)
{
   qNormalizeAngle(angle);
   if (angle != zRot) {
      zRot = angle;
      emit zRotationChanged(angle);
      updateGL();
   }
}

void FLGLWidget::initializeGL() {
//    QGLFormat glFormat = QGLWidget::format();
//    if ( !glFormat.sampleBuffers() )
//        qWarning() << "Could not enable sample buffers";
    
    // Set the clear color to black
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glEnable(GL_DEPTH_TEST);
    if (!m_shader.addShaderFromSourceFile(QGLShader::Vertex,
                                    "simple.vert"))
        qWarning() << m_shader.log();
    if(!m_shader.addShaderFromSourceFile(QGLShader::Fragment,
                                     "simple.frag"))
        qWarning() << m_shader.log();
    
    points_[0] = pts_[0] = QVector4D(-0.5f, -0.5f, 0.0f, 1.0f);
    points_[1] = pts_[1] = QVector4D(+0.5f, -0.5f, 0.0f, 1.0f);
    points_[2] = pts_[2] = QVector4D(+0.0f, +0.5f, 0.0f, 1.0f);

    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern( QGLBuffer::DynamicDraw );
    m_vertexBuffer.bind();
    
    m_vertexBuffer.allocate( pts_, 3 * 4 * sizeof( float ) );
    if(!m_shader.link())
        qWarning() << m_shader.log();
    
    uint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    m_shader.setAttributeBuffer( "vertex", GL_FLOAT, 0, 4 );
    m_shader.enableAttributeArray( "vertex" );
//    m_shader.setAttributeBuffer( "modelMatrix", GL_FLOAT, 0, 16 );
//    m_shader.enableAttributeArray( "modelMatrix" );
//    m_shader.setAttributeBuffer( "projectionMatrix", GL_FLOAT, 0, 16 );
//    m_shader.enableAttributeArray( "projectionMatrix" );
//    m_shader.setAttributeBuffer( "viewMatrix", GL_FLOAT, 0, 16 );
//    m_shader.enableAttributeArray( "viewMatrix" );
    
}


void FLGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(!m_shader.bind())
        qWarning() << m_shader.log();
    
//    m_shader.enableAttributeArray("vertex");
//    m_shader.setAttributeArray("vertex", points);
//    m_shader.setUniformValue("projectionMatrix", xMat);
//    m_shader.setUniformValue("viewMatrix", yMat);
//    m_shader.setUniformValue("modelMatrix", QMatrix4x4());
    m_shader.setUniformValue("color", QColor(0,1,0));
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
//    m_shader.disableAttributeArray("vertex");
//    m_shader.release();
}

void FLGLWidget::resizeGL(int width, int height)
{
   height = qMax(1, height);
    w_ = width; h_ = height;
    glViewport(0, 0, width, height);
    QMatrix4x4 a, b;
    a.perspective(60.f, 1., .1f, 10.f);
    b.rotate(zRot / 16., QVector3D(0,0,1));
    b.rotate(yRot / 16., QVector3D(0,1,0));
    b.rotate(xRot / 16., QVector3D(1,0,0));
    for(size_t v = 0; v < 3; v++)
        pts_[v] = a * b * points_[v];
    m_vertexBuffer.allocate( pts_, 3 * 4 * sizeof( float ) );
    
}

void FLGLWidget::mousePressEvent(QMouseEvent *event)
{
   lastPos = event->pos();
}

void FLGLWidget::mouseMoveEvent(QMouseEvent *event)
{
   int dx = event->x() - lastPos.x();
   int dy = event->y() - lastPos.y();

   if (event->buttons() & Qt::LeftButton) {
      setXRotation(xRot + 8 * dy);
      setYRotation(yRot + 8 * dx);
   } else if (event->buttons() & Qt::RightButton) {
      setXRotation(xRot + 8 * dy);
      setZRotation(zRot + 8 * dx);
   }
   lastPos = event->pos();
    
    QMatrix4x4 a,b;
    a.perspective(60.f, 1., .1f, 10.f);
    b.rotate(zRot / 16., QVector3D(0,0,1));
    b.rotate(yRot / 16., QVector3D(0,1,0));
    b.rotate(xRot / 16., QVector3D(1,0,0));
    for(size_t v = 0; v < 3; v++)
        pts_[v] = a * b * points_[v];
    m_vertexBuffer.allocate( pts_, 3 * 4 * sizeof( float ) );
}
