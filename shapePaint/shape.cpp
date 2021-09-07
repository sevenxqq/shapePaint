#include "shape.h"

Shape::Shape(QVector<QPoint>i_args,QPen i_pen){
    m_args = i_args;
    m_pen = i_pen;
}

QPen Shape::pen(){
    return m_pen;
}
QVector<QPoint> Shape::draw(){
    return {};

}

Shape* Shape::clone(){
    Shape *t = new Shape(m_args,m_pen);
    t->setArgs(m_args);
    t->translate(QPoint(10,10));
    return t;
}

void Shape::paint(QPainter& i_painter){
    if (m_type==LEAF){
        i_painter.setPen(m_pen);
        i_painter.drawPoints(m_points);
    }
    else{
        for(auto&t:m_group){
            t->paint(i_painter);
        }
    }
}

void Shape::add(Shape* i_shape){}
void Shape::remove(Shape* i_shape){}

void Shape::setArgs(QVector<QPoint>i_args){
     m_args = i_args;
     m_points = draw();
}
void Shape::afterOp(QVector<QPoint> i_args){
    m_undo.append(m_args);
}

void Shape::setArgs(QPoint i_pos){
    for(auto&p:m_args)
        p+=i_pos;
}
QVector<QPoint> Shape::points(){
    return m_points;
}

bool Shape::contain(QPoint i_point){
    if (m_type==LEAF){
        for(auto& point:m_points){
            QPoint diff = i_point - point;
            if (qPow(diff.x(),2) + qPow(diff.y(),2) <= 20)
                return true;
        }
    }
    else{
        for(auto&p:m_group)
            if (p->contain(i_point))
                return true;
    }
    return false;
}
QVector<QPoint> Shape::translate(QPoint i_pos){
    if (m_type==LEAF){
        for(auto&p:m_args)
            p+=i_pos;
        for(auto&p:m_points)
            p+=i_pos;
    }
    else{
        for(auto&p:m_group)
            p->translate(i_pos);
    }
    return m_points;
}

QVector<QPoint>Shape:: rotate(QPoint p1,QPoint p2){
    qreal r= qAtan2( qreal(p2.y()-p1.y()) , qreal(p2.x()-p1.x()));
    QVector<QPoint> args_tmp;
    qreal cosr = qCos(r), sinr = qSin(r);
    qreal dx=1.0, dy=1.0;
//    for (auto& arg : m_args)
//    {
//        dx = arg.x() - p1.x();
//        dy = arg.y() - p1.y();
//        qreal x1 = dx * cosr - dy * sinr + p1.x();
//        qreal y1 = dx * sinr + dy * cosr + p1.y();
//        args_tmp.append(QPoint(x1,y1));
//    }
//    m_args = args_tmp;
//    m_points = draw();
    for(auto&point:m_points){
        dx = qreal(point.x() - p1.x());
        dy = qreal(point.y() - p1.y());
        point.rx() = dx * cosr - dy * sinr+qreal(p1.x());
        point.ry() = dx * sinr + dy * cosr+qreal(p1.y());
    }
      return m_points;
}

QVector<QPoint> Shape::scale(){
      return m_points;
}

QVector<QPoint> Shape::clip(QPoint lt, QPoint rb){
      return m_points;
}




