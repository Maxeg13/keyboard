#ifndef Arrows_H
#define Arrows_H

#include <QWidget>

class Arrows : public QWidget
{
    Q_OBJECT
public:
    QPoint centre[4];
    bool checked[4]={0,0,0,0};
    explicit Arrows(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *e);
signals:

public slots:

};

#endif // Arrows_H
