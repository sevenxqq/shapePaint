#ifndef DERIVE_H
#define DERIVE_H
#include "shape.h"

class Line:public Shape{
public:
    Line(QVector<QPoint>i_args,QPen i_pen):Shape(i_args,i_pen){m_type=LEAF;}
    virtual ~Line(){}
    virtual QVector<QPoint> draw();
    virtual Shape * clone();
};

class ConnectLine:public Shape{
public:
    ConnectLine(QVector<QPoint>i_args,QPen i_pen):Shape(i_args,i_pen){m_type=LEAF;}
    virtual ~ConnectLine(){}
    virtual QVector<QPoint> draw();
    virtual Shape * clone();
};

class Triangle:public Shape{
public:
    Triangle(QVector<QPoint>i_args,QPen i_pen):Shape(i_args,i_pen){m_type=LEAF;}
    virtual ~Triangle(){}
    virtual QVector<QPoint> draw();
    virtual Shape * clone();

};

class Rectangle:public Shape{
public:
    Rectangle(QVector<QPoint>i_args,QPen i_pen):Shape(i_args,i_pen){m_type=LEAF;}
    virtual ~Rectangle(){}
    virtual QVector<QPoint> draw();
    virtual Shape * clone();

};

class Polygon:public Shape{
public:
    Polygon(QVector<QPoint>i_args,QPen i_pen):Shape(i_args,i_pen){m_type=LEAF;}
    virtual ~Polygon(){}
    virtual QVector<QPoint> draw();
    virtual Shape * clone();

};

class Circle:public Shape{
public:
    Circle(QVector<QPoint>i_args,QPen i_pen):Shape(i_args,i_pen){m_type=LEAF;}
    virtual ~Circle(){}
    virtual QVector<QPoint> draw();
    virtual Shape * clone();

};

class Ellipse:public Shape{
public:
    Ellipse(QVector<QPoint>i_args,QPen i_pen):Shape(i_args,i_pen){m_type=LEAF;}
    virtual ~Ellipse(){}
    virtual QVector<QPoint> draw();
    virtual Shape * clone();

};

class Composite:public Shape{
public:
    Composite(QPen i_pen){m_pen=i_pen;  m_type=COMPOSITE;}
    virtual ~Composite(){}
    virtual void add(Shape *i_shape);
    virtual void remove(Shape *i_shape) ;
    virtual QVector<QPoint> draw();
    virtual Shape * clone();
//private:

};

#endif // DERIVE_H
