#ifndef SHAPE_H
#define SHAPE_H

#include <QVector>
#include <QPoint>
#include <QPen>
#include <QtAlgorithms>
#include <QtMath>
#include <QPainter>
class Shape{  
public:
    enum Type {LEAF,COMPOSITE} stype;
    Shape(){}
    Shape(QVector<QPoint>i_args,QPen i_pen);
    virtual ~Shape(){}
    void setArgs(QVector<QPoint>i_args);
    void setArgs(QPoint i_pos);
    void setPoints(QVector<QPoint>i_points){m_points=i_points;}
    void afterOp(QVector<QPoint>i_args);
    QPen pen();
    QVector<QPoint> points();
    Type type(){return m_type;}
    virtual QVector<QPoint> draw();
    void paint(QPainter& i_painter);
    virtual Shape* clone();
    virtual void add(Shape* i_shape);
    virtual void remove(Shape* i_shape);
    bool contain(QPoint i_point);
    QVector<QPoint> translate(QPoint pos);
    QVector<QPoint> rotate(QPoint p1,QPoint p2);
    QVector<QPoint> scale();
    QVector<QPoint> clip(QPoint lt, QPoint rb);
protected:

    Type m_type;
    QList<Shape*> m_group;
    QVector<QPoint>m_args;          //参数
    QVector<QPoint>m_points;        //图元点集
    QPoint m_center;                //中心点
    QPen m_pen;                     //笔刷
    QList<QVector<QPoint>>m_undo;   //保留过往参数
};



#endif // SHAPE_H
