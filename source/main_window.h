/**
 * Copyright (c) 2009, Paul Gideon Dann
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtGui>
#include <QColor>
#include <QMdiSubWindow>
#include <vector>
#include "ui_main_window.h"
#include "FLGLWidget.h"
#include "FLGLWindow.h"

/**
 * Main window class for the application
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    MainWindow(QMainWindow* parent = 0);
    virtual ~MainWindow();
  public slots:
    void setBackgroundColor();
    void setPropColor();
    void setPropColorText();
    void destroyWindow(QObject* obj = 0);
    void resetRotations();
    void x1ClipSlider(int n);
    void x2ClipSlider(int n);
    void y1ClipSlider(int n);
    void y2ClipSlider(int n);
    void z1ClipSlider(int n);
    void z2ClipSlider(int n);
    void xClipLinkToggle(bool b);
    void yClipLinkToggle(bool b);
    void zClipLinkToggle(bool b);
    void resetClipping();
    void resetClippingRotation();
    void setClippingXY();
    void setClippingXZ();
    void setClippingYZ();
    void redSliders();
    void greenSliders();
    void blueSliders();
    void rotations45();


  private:
    void keyPressEvent(QKeyEvent *k) override;
    void keyReleaseEvent(QKeyEvent *k) override;
    
    int x1Clip_, x2Clip_, y1Clip_, y2Clip_, z1Clip_, z2Clip_,
        xClipLinkVal_, yClipLinkVal_, zClipLinkVal_;

    Ui::MainWindow ui_;
    std::vector<QMdiSubWindow*> glWindows_;
};

#endif
