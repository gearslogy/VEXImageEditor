#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWidget>
class ImageView : public QGraphicsView {
public:

    ImageView(QWidget* parent = nullptr) :QGraphicsView(parent) {
        setTransformationAnchor(QGraphicsView::AnchorViewCenter);
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        setRenderHints(QPainter::Antialiasing);

        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        centerOn(size().width(), size().height());
        setDragMode(QGraphicsView::RubberBandDrag);
        setMouseTracking(true);
        setWindowFlags(Qt::WindowStaysOnTopHint);

        
    }

    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* e) override;
    void drawBackground(QPainter* p, const QRectF& rct);

private:
    float zoom = 0;
    QPointF dPos;
    QPointF trackPos;

};


class ImageScene : public QGraphicsScene {
public:
    ImageScene(QObject* parent = nullptr) :QGraphicsScene(parent) {
        setSceneRect(-90000, -90000, 180000, 180000);

    }
};
    