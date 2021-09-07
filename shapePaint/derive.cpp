#include "derive.h"

QVector<QPoint> Line::draw(){
    assert(m_args.size() == 2);
    double x0 = m_args[0].x(),y0 = m_args[0].y();
    double x1 = m_args[1].x(),y1 = m_args[1].y();
    QVector<QPoint>points_tmp;
    points_tmp.append(QPoint(x0,y0));
    double dx = 0,dy = 0;
    double px = qAbs(x0-x1),py = qAbs(y0-y1);
    dx = px>=py?(x0==x1?0:(x0<x1?1:-1)):(x1-x0)/py;
    dy = px>py?(y1-y0)/px:(y0==y1?0:(y0<y1?1:-1));
    while( (px>=py &&x0 != x1) || (px<py && y0 != y1)){
        x0+=dx;
        y0+=dy;
        points_tmp.append(QPoint(x0,y0));
    }
    return  points_tmp;
}
Shape * Line::clone(){
    Shape *t = new Line(m_args,m_pen);
    t->setArgs(m_args);
    t->translate(QPoint(10,10));
    return t;
}
QVector<QPoint> ConnectLine::draw(){
    int sz = m_args.size();
    if (sz<2)
        return {};
    Line* tmp_line = new Line({m_args[0],m_args[1]},m_pen);
    QVector<QPoint>points_tmp;
    for(int i=1;i<sz;++i){
        tmp_line->setArgs({m_args[i-1],m_args[i]});
        points_tmp.append(tmp_line->points());
    }
    delete tmp_line;
    return points_tmp;
}

Shape * ConnectLine::clone(){
    Shape *t = new ConnectLine(m_args,m_pen);
    t->setArgs(m_args);
    t->translate(QPoint(10,10));
    return t;
}

QVector<QPoint> Triangle::draw(){
    assert(m_args.size()==2);
    int x0 = m_args[0].x();
    int x1 = m_args[1].x(),y1=m_args[1].y();
    int x2=(x0<<1)-x1,y2=y1;
    QVector<QPoint>args_tmp = m_args;
    args_tmp.append(QPoint(x2,y2));
    Polygon* pol_tmp = new Polygon(args_tmp,m_pen);
    QVector<QPoint>points_tmp;
    points_tmp = pol_tmp->draw();
    delete pol_tmp;
    return points_tmp;
}

Shape * Triangle::clone(){
    Shape *t = new Triangle(m_args,m_pen);
    t->setArgs(m_args);
    t->translate(QPoint(10,10));
    return t;
}
QVector<QPoint> Rectangle::draw(){
    assert(m_args.size()==2);
    int x0 = m_args[0].x(),y0=m_args[0].y();
    int x1 = m_args[1].x(),y1=m_args[1].y();
    QVector<QPoint>args_tmp = m_args;
    args_tmp.insert(1,QPoint(x1,y0));
    args_tmp.append(QPoint(x0,y1));
    Polygon* pol_tmp = new Polygon(args_tmp,m_pen);
    QVector<QPoint>points_tmp;
    points_tmp = pol_tmp->draw();
    delete pol_tmp;
    return points_tmp;
}

Shape * Rectangle::clone(){
    Shape *t = new Rectangle(m_args,m_pen);
    t->setArgs(m_args);
    t->translate(QPoint(10,10));
    return t;
}

QVector<QPoint> Polygon::draw(){
    int sz = m_args.size();
    if (sz<2)
        return {};
    Line* tmp_line = new Line({m_args[0],m_args[1]},m_pen);
    QVector<QPoint>points_tmp;
    for(int i=1;i<sz;++i){
        tmp_line->setArgs({m_args[i-1],m_args[i]});
        points_tmp.append(tmp_line->points());
    }
    tmp_line->setArgs({m_args[sz-1],m_args[0]});
    points_tmp.append(tmp_line->points());
    delete tmp_line;
    return points_tmp;
}

Shape *Polygon::clone(){
    Shape *t = new Polygon(m_args,m_pen);
    t->setArgs(m_args);
    t->translate(QPoint(10,10));
    return t;
}

QVector<QPoint> Circle::draw(){
    assert(m_args.size()==2);
    int cx = m_args[0].x(),cy=m_args[0].y();
    int x1 = m_args[1].x(),y1=m_args[1].y();
    double r = qSqrt(qPow((x1-cx),2) + qPow((y1-cy),2));
    double x=0,y=r,d=5/4-r;
    QVector<QPoint>points_tmp;
    while(x<=y){
        if(d>=0){
            ++x;
            --y;
            d=d+2*(x-y)+5;
        }
        else{
            ++x;
            d=d+2*x+3;
        }
        points_tmp.append(QPoint(cx+x,cy+y));
        points_tmp.append(QPoint(cx+x,cy-y));
        points_tmp.append(QPoint(cx-x,cy+y));
        points_tmp.append(QPoint(cx-x,cy-y));
        points_tmp.append(QPoint(cx+y,cy+x));
        points_tmp.append(QPoint(cx+y,cy-x));
        points_tmp.append(QPoint(cx-y,cy+x));
        points_tmp.append(QPoint(cx-y,cy-x));
    }
    return points_tmp;
}

Shape * Circle::clone(){
    Shape *t = new Circle(m_args,m_pen);
    t->setArgs(m_args);
    t->translate(QPoint(10,10));
    return t;
}

QVector<QPoint> Ellipse::draw(){
    assert(m_args.size()==2);
    int cx = (m_args[0].x()+m_args[1].x())>>1,cy = (m_args[0].y()+m_args[1].y())>>1;
    double ra=qAbs(m_args[0].x()-cx),rb=qAbs(m_args[0].y()-cy);
    double raSquare = qPow(ra,2),rbSquare = qPow(rb,2);
    double x=0,y=rb;
    double pk = rbSquare - raSquare * rb + raSquare / 4;
    QVector<QPoint>points_tmp;
    while (rbSquare * (x + 1) < raSquare * (y - 0.5)){
        if (pk < 0){
            pk += rbSquare * (2 * x + 3);
            ++x;
        }
        else{
            pk += rbSquare * (2 * x + 3) + raSquare * (2 - 2 * y);
            ++x;
            --y;
        }
        points_tmp.append(QPoint(cx+x,cy+y));
        points_tmp.append(QPoint(cx+x,cy-y));
        points_tmp.append(QPoint(cx-x,cy+y));
        points_tmp.append(QPoint(cx-x,cy-y));
    }
    pk = rb * (x + 0.5) * 2 + rb * 2 * (y - 1) - ra * 2 * rb;
    while (y > 0){
        if (pk < 0){
            ++x;
            --y;
            pk += 2 * rbSquare * x - 2 * raSquare * y + raSquare;
        }
        else{
            --y;
            pk += raSquare - 2 * raSquare * y;
        }
        points_tmp.append(QPoint(cx+x,cy+y));
        points_tmp.append(QPoint(cx+x,cy-y));
        points_tmp.append(QPoint(cx-x,cy+y));
        points_tmp.append(QPoint(cx-x,cy-y));
    }
    return points_tmp;
}

Shape * Ellipse::clone(){
    Shape *t = new Ellipse(m_args,m_pen);
    t->setArgs(m_args);
    t->translate(QPoint(10,10));
    return t;
}

void Composite::add(Shape *i_shape) {
    m_group.append(i_shape);
}
void Composite::remove(Shape *i_shape) {
    m_group.removeAll(i_shape);
}

QVector<QPoint> Composite::draw(){
    for(auto&t:m_group){
        if (t->type()==LEAF){
            t->setPoints(t->draw());
        }
        else{
            t->draw();
        }
    }
    return {};
}

Shape * Composite::clone(){
    Shape *t = new Composite(m_pen);
    for(auto&item:m_group){
        if (item->type()==LEAF){
            t->add(item->clone());
        }
        else{
           item->clone();
        }
    }
    return t;
}
