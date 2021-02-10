#include "ImageView.h"
#include <QWheelEvent>
#include <QDebug>

void ImageView::mousePressEvent(QMouseEvent* e)
{
    if (e->buttons() == Qt::MiddleButton) {
        dPos = mapToScene(e->pos());
        setCursor(Qt::SizeAllCursor);
    }
    return QGraphicsView::mousePressEvent(e);
}

void ImageView::mouseReleaseEvent(QMouseEvent* e)
{
    setCursor(Qt::ArrowCursor);
    return QGraphicsView::mouseReleaseEvent(e);
}

void ImageView::mouseMoveEvent(QMouseEvent* event)
{
    trackPos = mapToScene(event->pos());  // get cursor pos on scene
    if (event->buttons() == Qt::MiddleButton) {
        auto pt = mapToScene(event->pos());
        auto offset = pt - dPos;
        QPoint sceneCenter = QPoint(viewport()->width() / 2, viewport()->height() / 2);
        QPointF newCenter = mapToScene(sceneCenter);
        auto target = newCenter - offset;
        centerOn(target);
    }
    return QGraphicsView::mouseMoveEvent(event);
}

void ImageView::wheelEvent(QWheelEvent* e)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    float factor = 1.1;

    auto angleOffset = e->angleDelta();
    qDebug() << angleOffset;

    if (angleOffset.y()>0) {
        zoom += 1;
        scale(factor, factor);
    }

    else {
        zoom -= 1;
        scale(1.0 / factor, 1.0 / factor);
    }
    e->accept();
   
}

void ImageView::drawBackground(QPainter* p, const QRectF& rct)
{
    auto brush = QBrush();
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(50, 50, 50));
    p->setBrush(brush);
    p->drawRect(rct);
}

