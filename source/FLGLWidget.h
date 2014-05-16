/**
 * @file FLGLWidget.h
 * @author Brig Bagley
 * @version 15 May 2014
 */
#ifndef FLGLWIDGET_H
#define FLGLWIDGET_H

#include <QGLWidget>

class QtLogo;

/**
 * This class does all of the OpenGL drawing within FL in a Widget Window.
 */
class FLGLWidget : public QGLWidget {

   Q_OBJECT

   public:
      FLGLWidget(QWidget * parent = 0);
      virtual ~FLGLWidget();
      QSize minimumSizeHint() const;
      QSize sizeHint() const;
      public slots:
         void setXRotation(int angle);
      void setYRotation(int angle);
      void setZRotation(int angle);

signals:
      void xRotationChanged(int angle);
      void yRotationChanged(int angle);
      void zRotationChanged(int angle);
   protected:
      void initializeGL();
      void paintGL();
      void resizeGL(int width, int height);
      void mousePressEvent(QMouseEvent *event);
      void mouseMoveEvent(QMouseEvent *event);
   private:
      QtLogo *logo;
      int xRot;
      int yRot;
      int zRot;
      QPoint lastPos;
      QColor qtGreen;
      QColor qtPurple;
};
#endif
