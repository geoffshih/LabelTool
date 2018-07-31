#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    setMouseTracking(true);
    startPoint.setX(0);
    startPoint.setY(0);
    endPoint.setX(50);
    endPoint.setY(50);
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    QGraphicsView::mouseMoveEvent(e);
    this->scene()->update();
}

void MyGraphicsView::mousePressEvent(QMouseEvent *e)
{
    QGraphicsView::mousePressEvent(e);
    startPoint = e->pos();

    if(startPoint.x() < 0 )startPoint.setX(0);
    if(startPoint.y() < 0 )startPoint.setY(0);

    if(startPoint.x() > maxPoint.x())startPoint.setX(maxPoint.x()-1);
    if(startPoint.y() > maxPoint.y())startPoint.setY(maxPoint.y()-1);

    this->scene()->update();
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *e)
{
    QGraphicsView::mouseReleaseEvent(e);
    endPoint = e->pos();

    if(endPoint.x() < 0 )endPoint.setX(0);
    if(endPoint.y() < 0 )endPoint.setY(0);

    if(endPoint.x() > maxPoint.x())endPoint.setX(maxPoint.x()-1);
    if(endPoint.y() > maxPoint.y())endPoint.setY(maxPoint.y()-1);

    if(mode==0)
    {
        int x = qAbs(startPoint.x()-endPoint.x());
        int y = qAbs(startPoint.y()-endPoint.y());
        if(x>=20 && y>=20)
        {
            emit sendPos(startPoint, endPoint);
        }
    }
    this->scene()->update();
}

void MyGraphicsView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    this->scene()->setSceneRect(0, 0, event->size().width(), event->size().height());
    emit doRefresh();
}


void MyGraphicsView::setMode(int mode)
{
    this->mode = mode;
}

void MyGraphicsView::setMaxPoint(int x, int y)
{
    this->maxPoint = QPoint(x,y);
}
