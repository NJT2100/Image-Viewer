#include "buttonlabel.h"
#include <QDebug>
#include <QApplication>

ButtonLabel::ButtonLabel(QWidget *parent)
    :QLabel(parent)
{

}

void ButtonLabel::mousePressEvent(QMouseEvent *ev)
{
    emit clicked();
}
