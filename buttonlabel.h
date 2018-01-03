#ifndef BUTTONLABEL_H
#define BUTTONLABEL_H
#include <QLabel>

class ButtonLabel : public QLabel
{
    Q_OBJECT
public:
    ButtonLabel(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *ev);

signals:
    void clicked();

};

#endif // BUTTONLABEL_H
