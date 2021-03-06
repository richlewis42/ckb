#include <cstdio>
#include <QPainter>
#include "gradientbutton.h"

GradientButton::GradientButton(QWidget* parent, bool allowAlpha) :
    QPushButton(parent), _alpha(allowAlpha)
{
    connect(this, SIGNAL(clicked()), this, SLOT(pickGradient()));
    fromString("");
    setText("");
}

void GradientButton::updateImage(){
    const int w = 98, h = 16;
    QImage image(w, h, QImage::Format_RGB888);
    QPainter painter(&image);
    painter.setPen(Qt::NoPen);
    painter.fillRect(0, 0, w, h, QColor(0, 0, 0));
    if(_alpha){
        for(int x = 1; x < w - 1; x += h){
            painter.fillRect(x, 1, h / 2, h / 2 - 1, QColor(255, 255, 255));
            painter.fillRect(x + h / 2, 1, h / 2, h / 2 - 1, QColor(192, 192, 192));
            painter.fillRect(x, h / 2, h / 2, h / 2 - 1, QColor(192, 192, 192));
            painter.fillRect(x + h / 2, h / 2, h / 2, h / 2 - 1, QColor(255, 255, 255));
        }
    }
    QLinearGradient gradient(0., 0., w - 2, 0.);
    gradient.setStops(_stops);
    painter.fillRect(1, 1, w - 2, h - 2, QBrush(gradient));
    setIconSize(QSize(w, h));
    setIcon(QIcon(QPixmap::fromImage(image)));
}

void GradientButton::fromString(const QString& string){
    // Clear existing stops
    _stops.clear();
    // Parse string with sscanf
    QByteArray cString = string.toLatin1();
    const char* data = cString.data();
    char pos = -1;
    uchar a, r, g, b;
    while(1){
        int scanned = 0;
        char newpos;
        if(sscanf(data, "%hhd:%2hhx%2hhx%2hhx%2hhx%n", &newpos, &a, &r, &g, &b, &scanned) != 5)
            break;
        data += scanned;
        // Don't allow stops out-of-order or past 100
        if(newpos <= pos || newpos > 100)
            break;
        pos = newpos;
        if(!_alpha)
            a = 255;
        _stops.append(QGradientStop(pos / 100., QColor(r, g, b, a)));
    }
    // If no stops were scanned, create them from white
    if(_stops.count() == 0){
        _stops.append(QGradientStop(0., QColor(255, 255, 255)));
        _stops.append(QGradientStop(1., QColor(255, 255, 255)));
    }
    // Make sure there are stops at 0 and at 100
    if(_stops[0].first != 0.)
        _stops.prepend(QGradientStop(0., _stops[0].second));
    if(_stops.last().first != 1.)
        _stops.prepend(QGradientStop(1., _stops.last().second));
    updateImage();
}

QString GradientButton::toString() const {

}

void GradientButton::pickGradient(){
}

