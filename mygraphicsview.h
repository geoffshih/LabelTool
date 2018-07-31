#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QDebug>

#include <QGraphicsItem>
#include <QMouseEvent>
#include <QPoint>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyGraphicsView(QWidget *parent = nullptr);

    void setMode(int mode);
    void setMaxPoint(int x, int y);

signals:
    void sendPos(QPoint,QPoint);
    void doRefresh();

public slots:

public:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

private:
    QPoint startPoint, endPoint, maxPoint;
    int mode;

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // MYGRAPHICSVIEW_H
