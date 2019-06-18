// QLabelMouseTracker
//  - a modified QLabel
//  - displays (and rescales without interpolation) an image,
//  - tracks the mouse location in terms of the pixels of the original image
//  - provides the colour of the last pixel that was clicked on
//  - emits a signal when the user clicks the mouse

#pragma once

#include <QLabel>
#include <QMouseEvent>

class QLabelMouseTracker : public QLabel {
  Q_OBJECT
 public:
  QLabelMouseTracker(QWidget *parent);
  void setImage(const QImage &img);
  const QImage &getImage() const;
  const QRgb &getColour() const;

 signals:
  void mouseClicked();

 protected:
  void mouseMoveEvent(QMouseEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

 private:
  // (x,y) location of current pixel
  QPoint currentPixel;
  // colour of pixel at last mouse click position
  QRgb colour;
  // QImage used for pixel location and colour
  QImage image;
  // Pixmap used to display scaled version of image
  QPixmap pixmap = QPixmap(1, 1);
};
