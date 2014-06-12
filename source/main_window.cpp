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
#include <QColorDialog>
#include <sstream>
#include <cstdio>
#include <iostream>
#include "main_window.h"
#include "moc_main_window.cpp"

/**
 * Constructor
 */
MainWindow::MainWindow(QMainWindow* parent)
: QMainWindow(parent)
{
    this->ui_.setupUi(this);
    x1Clip_ = y1Clip_ = z1Clip_ = 0;
    x2Clip_ = y2Clip_ = z2Clip_ = 99;
    xClipLinkVal_ = yClipLinkVal_ = zClipLinkVal_ = 99;
}

MainWindow::~MainWindow() {
}

void MainWindow::destroyWindow(QObject* obj){
 //  if(obj)
   //   this->ui_.mdiArea->removeSubWindow((QMdiSubWindow*)obj);
}

void MainWindow::setPropColorText() {
   std::string ss(this->ui_.colorPropText->text().toStdString());
   int r,g,b;
   if (3 != sscanf(ss.c_str(),"%d, %d, %d",&r,&g,&b)) return;
   r = std::min(std::max(r,0),255);
   g = std::min(std::max(g,0),255);
   b = std::min(std::max(b,0),255);
   QString qss = QString("background-color: rgb(%1,%2,%3);").arg(r).arg(g).arg(b);
   this->ui_.colorPropButton->setStyleSheet(qss);
}

void MainWindow::setPropColor() {
   QColor c = QColorDialog::getColor(
         this->ui_.colorPropButton->palette().color(QPalette::Window));
   QString qss = QString("background-color: %1").arg(c.name());
   this->ui_.colorPropButton->setStyleSheet(qss);
   QString txt = QString("%1, %2, %3").arg(c.red()).arg(c.green()).arg(c.blue());
   this->ui_.colorPropText->setText(txt);
}

void MainWindow::setBackgroundColor() {
    QColor c = QColorDialog::getColor(
                                      this->ui_.backgroundColorButton->palette().color(QPalette::Window));
    QString qss = QString("background-color: %1").arg(c.name());
    this->ui_.backgroundColorButton->setStyleSheet(qss);
}

void MainWindow::keyPressEvent(QKeyEvent *k) {
   FLGLWindow *window = new FLGLWindow();
   QMdiSubWindow *sw = nullptr;
   switch(k->key()) {
   case Qt::Key_Shift:
      this->ui_.highlightButton->setChecked(true);
      break;
   case Qt::Key_Z:
      this->ui_.diffuseHighlightButton->setChecked(true);
      break;
   case Qt::Key_X:
      this->ui_.resetHighlightedButton->setChecked(true);
      break;
   case Qt::Key_P:
      sw = new QMdiSubWindow;
      if (sw && window) {
         sw->setWidget(window);
         sw->setAttribute(Qt::WA_DeleteOnClose);
         this->ui_.mdiArea->addSubWindow(sw);
          sw->show();
          this->ui_.mdiArea->tileSubWindows ();
         //glWindows_.push_back(sw);
         //QObject::connect(sw,SIGNAL(destroyed(QObject*)),this,SLOT(destroyWindow(QObject*)));
      }
      break;
   default:
      QMainWindow::keyPressEvent(k);
      break;
   }
}

void MainWindow::keyReleaseEvent(QKeyEvent *k) {
   switch(k->key()) {
   case Qt::Key_Shift:
      this->ui_.highlightButton->setChecked(false);
      break;
   case Qt::Key_Z:
      this->ui_.diffuseHighlightButton->setChecked(false);
      break;
   case Qt::Key_X:
      this->ui_.resetHighlightedButton->setChecked(false);
      break;
   default:
      QMainWindow::keyPressEvent(k);
      break;
   }
}

void MainWindow::resetRotations() {
    this->ui_.xRotationSlider->setValue(0);
    this->ui_.yRotationSlider->setValue(0);
    this->ui_.zRotationSlider->setValue(0);
}

void MainWindow::x1ClipSlider(int n) {
    if (this->ui_.xClipLink->isChecked()) {
        if (this->ui_.x2ClipSlider->value() + n - x1Clip_ >= this->ui_.x2ClipSlider->maximum()) {
            x1Clip_ = this->ui_.x1ClipSlider->maximum()-xClipLinkVal_;
            x2Clip_ = this->ui_.x2ClipSlider->maximum();
            this->ui_.x1ClipSlider->setValue(x1Clip_);
            this->ui_.x2ClipSlider->setValue(x2Clip_);
            return;
        }
        else {
            this->ui_.x2ClipSlider->setValue(this->ui_.x2ClipSlider->value()+
                                             n - x1Clip_);
        }
    }
    else if (this->ui_.x1ClipSlider->value() > this->ui_.x2ClipSlider->value())
        this->ui_.x2ClipSlider->setValue(n);
    x1Clip_ = n;
}

void MainWindow::y1ClipSlider(int n) {
    if (this->ui_.yClipLink->isChecked()){
        if (this->ui_.y2ClipSlider->value() + n - y1Clip_ >= this->ui_.y2ClipSlider->maximum()) {
            y1Clip_ = this->ui_.y1ClipSlider->maximum()-yClipLinkVal_;
            y2Clip_ = this->ui_.y2ClipSlider->maximum();
            this->ui_.y1ClipSlider->setValue(y1Clip_);
            this->ui_.y2ClipSlider->setValue(y2Clip_);
            return;
        }
        else {
            this->ui_.y2ClipSlider->setValue(this->ui_.y2ClipSlider->value()+
                                             n - y1Clip_);
        }
    }
    else if (this->ui_.y1ClipSlider->value() > this->ui_.y2ClipSlider->value())
        this->ui_.y2ClipSlider->setValue(n);
    y1Clip_ = n;
}

void MainWindow::z1ClipSlider(int n) {
    if (this->ui_.zClipLink->isChecked()){
        if (this->ui_.z2ClipSlider->value() + n - z1Clip_ >= this->ui_.z2ClipSlider->maximum()) {
            z1Clip_ = this->ui_.z1ClipSlider->maximum()-zClipLinkVal_;
            z2Clip_ = this->ui_.z2ClipSlider->maximum();
            this->ui_.z1ClipSlider->setValue(z1Clip_);
            this->ui_.z2ClipSlider->setValue(z2Clip_);
            return;
        }
        else {
            this->ui_.z2ClipSlider->setValue(this->ui_.z2ClipSlider->value()+
                                             n - z1Clip_);
        }
    }
    else if (this->ui_.z1ClipSlider->value() > this->ui_.z2ClipSlider->value())
        this->ui_.z2ClipSlider->setValue(n);
    z1Clip_ = n;
}

void MainWindow::x2ClipSlider(int n) {
    if (this->ui_.xClipLink->isChecked()) {
        if (this->ui_.x1ClipSlider->value() + n - x2Clip_ <= this->ui_.x2ClipSlider->minimum()) {
            x2Clip_ = this->ui_.x2ClipSlider->minimum()+xClipLinkVal_;
            x1Clip_ = this->ui_.x1ClipSlider->minimum();
            this->ui_.x1ClipSlider->setValue(x1Clip_);
            this->ui_.x2ClipSlider->setValue(x2Clip_);
            return;
        }
        else {
            this->ui_.x1ClipSlider->setValue(this->ui_.x1ClipSlider->value()+
                                             n - x2Clip_);
        }
    }
    else if (this->ui_.x1ClipSlider->value() > this->ui_.x2ClipSlider->value())
        this->ui_.x1ClipSlider->setValue(n);
    x2Clip_ = n;
}

void MainWindow::y2ClipSlider(int n) {
    if (this->ui_.yClipLink->isChecked()){
        if (this->ui_.y1ClipSlider->value() + n - y2Clip_ <= this->ui_.y2ClipSlider->minimum()) {
            y2Clip_ = this->ui_.y2ClipSlider->minimum()+yClipLinkVal_;
            y1Clip_ = this->ui_.y1ClipSlider->minimum();
            this->ui_.y1ClipSlider->setValue(y1Clip_);
            this->ui_.y2ClipSlider->setValue(y2Clip_);
            return;
        }
        else {
            this->ui_.y1ClipSlider->setValue(this->ui_.y1ClipSlider->value()+
                                             n - y2Clip_);
        }
    }
    else if (this->ui_.y1ClipSlider->value() > this->ui_.y2ClipSlider->value())
        this->ui_.y1ClipSlider->setValue(n);
    y2Clip_ = n;
}

void MainWindow::z2ClipSlider(int n) {
    if (this->ui_.zClipLink->isChecked()){
        if (this->ui_.z1ClipSlider->value() + n - z2Clip_ <= this->ui_.z2ClipSlider->minimum()) {
            z2Clip_ = this->ui_.z2ClipSlider->minimum()+zClipLinkVal_;
            z1Clip_ = this->ui_.z1ClipSlider->minimum();
            this->ui_.z1ClipSlider->setValue(z1Clip_);
            this->ui_.z2ClipSlider->setValue(z2Clip_);
            return;
        }
        else {
            this->ui_.z1ClipSlider->setValue(this->ui_.z1ClipSlider->value()+
                                             n - z2Clip_);
        }
    }
    else if (this->ui_.z1ClipSlider->value() > this->ui_.z2ClipSlider->value())
        this->ui_.z1ClipSlider->setValue(n);
    z2Clip_ = n;
}

void MainWindow::xClipLinkToggle(bool b) { if (b) xClipLinkVal_ = x2Clip_ - x1Clip_; }

void MainWindow::yClipLinkToggle(bool b) { if (b) yClipLinkVal_ = y2Clip_ - y1Clip_; }

void MainWindow::zClipLinkToggle(bool b) { if (b) zClipLinkVal_ = z2Clip_ - z1Clip_; }

void MainWindow::resetClipping() {
    x1Clip_ = this->ui_.x1ClipSlider->minimum();
    y1Clip_ = this->ui_.y1ClipSlider->minimum();
    z1Clip_ = this->ui_.z1ClipSlider->minimum();
    x2Clip_ = this->ui_.x2ClipSlider->maximum();
    y2Clip_ = this->ui_.y2ClipSlider->maximum();
    z2Clip_ = this->ui_.z2ClipSlider->maximum();
    this->ui_.x1ClipSlider->setValue(x1Clip_);
    this->ui_.y1ClipSlider->setValue(y1Clip_);
    this->ui_.z1ClipSlider->setValue(z1Clip_);
    this->ui_.x2ClipSlider->setValue(x2Clip_);
    this->ui_.y2ClipSlider->setValue(y2Clip_);
    this->ui_.z2ClipSlider->setValue(z2Clip_);
    
    this->ui_.xClipLink->setChecked(false);
    this->ui_.yClipLink->setChecked(false);
    this->ui_.zClipLink->setChecked(false);
}


void MainWindow::resetClippingRotation() {
    this->ui_.xClipRotateSlider->setValue(0);
    this->ui_.yClipRotateSlider->setValue(0);
    this->ui_.zClipRotateSlider->setValue(0);
}

void MainWindow::setClippingXY() {
    int zSlice = (this->ui_.z1ClipSlider->minimum() + this->ui_.z1ClipSlider->maximum()) / 2;
    int zWidth = (this->ui_.z1ClipSlider->maximum() - this->ui_.z1ClipSlider->minimum()) / 20;
    z1Clip_ = zSlice;
    z2Clip_ = zSlice+zWidth;
    this->ui_.z1ClipSlider->setValue(zSlice);
    this->ui_.z2ClipSlider->setValue(zSlice+zWidth);
    
    this->ui_.xClipLink->setChecked(false);
    this->ui_.yClipLink->setChecked(false);
    this->ui_.zClipLink->setChecked(true);
}

void MainWindow::setClippingXZ(){
    int ySlice = (this->ui_.y1ClipSlider->minimum() + this->ui_.y1ClipSlider->maximum()) / 2;
    int yWidth = (this->ui_.y1ClipSlider->maximum() - this->ui_.y1ClipSlider->minimum()) / 20;
    y1Clip_ = ySlice;
    y2Clip_ = ySlice+yWidth;
    this->ui_.y1ClipSlider->setValue(ySlice);
    this->ui_.y2ClipSlider->setValue(ySlice+yWidth);
    
    this->ui_.xClipLink->setChecked(false);
    this->ui_.yClipLink->setChecked(true);
    this->ui_.zClipLink->setChecked(false);
}

void MainWindow::setClippingYZ() {
    int xSlice = (this->ui_.x1ClipSlider->minimum() + this->ui_.x1ClipSlider->maximum()) / 2;
    int xWidth = (this->ui_.x1ClipSlider->maximum() - this->ui_.x1ClipSlider->minimum()) / 20;
    x1Clip_ = xSlice;
    x2Clip_ = xSlice+xWidth;
    this->ui_.x1ClipSlider->setValue(xSlice);
    this->ui_.x2ClipSlider->setValue(xSlice+xWidth);
    
    this->ui_.xClipLink->setChecked(true);
    this->ui_.yClipLink->setChecked(false);
    this->ui_.zClipLink->setChecked(false);
}


void MainWindow::redSliders() {
    if (this->ui_.redLink->isChecked())  {
        if (this->ui_.blueLink->isChecked())  {
            this->ui_.blueGammaSlider->setValue(this->ui_.redGammaSlider->value());
            this->ui_.blueLumSlider->setValue(this->ui_.redLumSlider->value());
            this->ui_.blueEqSlider->setValue(this->ui_.redEqSlider->value());
        }
        if (this->ui_.greenLink->isChecked())  {
            this->ui_.greenLumSlider->setValue(this->ui_.redLumSlider->value());
            this->ui_.greenGammaSlider->setValue(this->ui_.redGammaSlider->value());
            this->ui_.greenEqSlider->setValue(this->ui_.redEqSlider->value());
        }
    }
}
void MainWindow::greenSliders(){
    if (this->ui_.greenLink->isChecked())  {
        if (this->ui_.redLink->isChecked())  {
            this->ui_.redGammaSlider->setValue(this->ui_.greenGammaSlider->value());
            this->ui_.redLumSlider->setValue(this->ui_.greenLumSlider->value());
            this->ui_.redEqSlider->setValue(this->ui_.greenEqSlider->value());
        }
        if (this->ui_.blueLink->isChecked())  {
            this->ui_.blueEqSlider->setValue(this->ui_.greenEqSlider->value());
            this->ui_.blueGammaSlider->setValue(this->ui_.greenGammaSlider->value());
            this->ui_.blueLumSlider->setValue(this->ui_.greenLumSlider->value());
        }
    }
}

void MainWindow::blueSliders() {
    if (this->ui_.blueLink->isChecked())  {
        if (this->ui_.redLink->isChecked())  {
            this->ui_.redGammaSlider->setValue(this->ui_.blueGammaSlider->value());
            this->ui_.redLumSlider->setValue(this->ui_.blueLumSlider->value());
            this->ui_.redEqSlider->setValue(this->ui_.blueEqSlider->value());
        }
        if (this->ui_.greenLink->isChecked())  {
            this->ui_.greenGammaSlider->setValue(this->ui_.blueGammaSlider->value());
            this->ui_.greenEqSlider->setValue(this->ui_.blueEqSlider->value());
            this->ui_.greenLumSlider->setValue(this->ui_.blueLumSlider->value());
        }
    }
}

void MainWindow::rotations45() {
    if (!this->ui_.inc45Check->isChecked())
        return;
    int x = this->ui_.xRotationSlider->value();
    int y = this->ui_.yRotationSlider->value();
    int z = this->ui_.zRotationSlider->value();
    x /= 45; y /= 45; z /= 45;
    this->ui_.xRotationSlider->setValue(x*45);
    this->ui_.yRotationSlider->setValue(y*45);
    this->ui_.zRotationSlider->setValue(z*45);
    
}