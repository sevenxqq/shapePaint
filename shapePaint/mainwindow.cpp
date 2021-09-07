#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_undo={{}};
    m_redo={{}};
}

MainWindow::~MainWindow()
{
    for(Shape*p:m_shapes)
        delete p;
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    m_image.fill(Qt::white);
    m_painter.begin(&m_image);
    if (!file.isEmpty())
    {
        QImage temp(file);
        m_painter.drawImage(QPoint((m_image.width() - temp.width()) / 2, (m_image.height() - temp.height()) / 2),temp);
    }
    for(Shape* p:m_shapes){
        p->paint(m_painter);
    }
    m_painter.end();
    ui->label->setPixmap(QPixmap::fromImage(m_image));
}

void MainWindow::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    ui->label->resize(ui->centralwidget->size());
    m_image = QImage(ui->label->size(), QImage::Format_RGB32);
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    QPoint pos = ui->label->mapFromGlobal(event->globalPos());
    switch(m_state){
    case LINE:m_cursor_points={pos,pos}; m_pshape = new Line(m_cursor_points,m_pen); m_shapes.append(m_pshape);m_undo.append(m_shapes);  break;
    case CONNECTLINE:break;
    case TRIANGLE:m_cursor_points={pos,pos}; m_pshape = new Triangle(m_cursor_points,m_pen); m_shapes.append(m_pshape); m_undo.append(m_shapes); break;
    case RECTANGLE:m_cursor_points={pos,pos}; m_pshape = new Rectangle(m_cursor_points,m_pen); m_shapes.append(m_pshape); m_undo.append(m_shapes); break;
    case POLYGON: break;
    case CIRCLE:m_cursor_points={pos,pos}; m_pshape = new Circle(m_cursor_points,m_pen); m_shapes.append(m_pshape);m_undo.append(m_shapes);  break;
    case ELLIPSE:m_cursor_points={pos,pos}; m_pshape = new Ellipse(m_cursor_points,m_pen); m_shapes.append(m_pshape); m_undo.append(m_shapes); break;
    case TRANSLATE:
    case ROTATE:
    case CLIP:
    case COPY:
    case TRASH:
        m_pshape = nullptr;
        m_cursor_points={pos};
        for(auto&p:m_shapes){
            if (p->contain(pos)){
                m_pshape = p;
                break;
            }
        }
        break;
    case COMBINE:break;

    default:break;
    }
    update();

}
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    QPoint pos = ui->label->mapFromGlobal(event->globalPos());
    switch(m_state){
    case LINE:m_cursor_points[1] = pos; m_pshape->setArgs(m_cursor_points);  break;
    case CONNECTLINE:break;
    case TRIANGLE:m_cursor_points[1] = pos; m_pshape->setArgs(m_cursor_points);  break;
    case RECTANGLE:m_cursor_points[1] = pos; m_pshape->setArgs(m_cursor_points);  break;
    case POLYGON:break;
    case CIRCLE:m_cursor_points[1] = pos; m_pshape->setArgs(m_cursor_points);  break;
    case ELLIPSE:m_cursor_points[1] = pos; m_pshape->setArgs(m_cursor_points);  break;
    case TRANSLATE:if(m_pshape==nullptr) break;  m_pshape->translate(pos-m_cursor_points[0]); m_cursor_points[0]=pos; break;
    case ROTATE:if (m_pshape==nullptr)break; m_pshape->rotate(m_cursor_points[0],pos);break;
    case COMBINE:break;
    default:break;
    }
    update();

}
void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    QPoint pos = ui->label->mapFromGlobal(event->globalPos());
    switch(m_state){
    case LINE:m_cursor_points[1] = pos; m_pshape->setArgs(m_cursor_points);  break;
    case CONNECTLINE:
        m_cursor_points.append(pos);
        if (m_cursor_points.size()==1){
            m_pshape = new ConnectLine(m_cursor_points,m_pen);
            m_shapes.append(m_pshape);
            m_undo.append(m_shapes);
        }
        m_pshape->setArgs(m_cursor_points);
        break;
    case TRIANGLE:m_cursor_points[1] = pos; m_pshape->setArgs(m_cursor_points);  break;
    case RECTANGLE:m_cursor_points[1] = pos; m_pshape->setArgs(m_cursor_points);  break;
    case POLYGON:
        m_cursor_points.append(pos);
        if (m_cursor_points.size()==1){
            m_pshape = new Polygon(m_cursor_points,m_pen);
            m_shapes.append(m_pshape);
            m_undo.append(m_shapes);
        }
        m_pshape->setArgs(m_cursor_points);
        break;
    case CIRCLE:m_cursor_points[1] = pos; m_pshape->setArgs(m_cursor_points);  break;
    case ELLIPSE:m_cursor_points[1] = pos; m_pshape->setArgs(m_cursor_points);  break;
    case TRANSLATE:if(m_pshape==nullptr) {m_cursor_points.clear(); break;} m_pshape->translate(pos-m_cursor_points[0]); m_cursor_points.clear(); m_undo.append(m_shapes);break;
    case ROTATE:if (m_pshape==nullptr){m_cursor_points.clear(); break;} m_pshape->rotate(m_cursor_points[0],pos);m_cursor_points.clear();m_undo.append(m_shapes);break;
    case CLIP:break;
    case COPY:if (m_pshape==nullptr){m_cursor_points.clear(); break;}m_pshape = m_pshape->clone();m_shapes.append(m_pshape); m_cursor_points.clear(); m_undo.append(m_shapes);break;
    case TRASH:m_shapes.removeAll(m_pshape);m_undo.append(m_shapes);break;
    case COMBINE:
        if (m_pshape == nullptr){
            m_pshape = new Composite(m_pen);
            m_shapes.append(m_pshape);
            m_undo.append(m_shapes);
        }
        for(int i=m_shapes.size()-1;i>=0;--i){
            Shape* p = m_shapes[i];
            if (p->contain(pos)){
                Shape* p_tmp = p->clone();
                m_pshape->add(p_tmp);
                break;
            }
        }
        break;
    default:break;
    }
    update();
}


void MainWindow::on_action_open_triggered()
{
    file = (QFileDialog::getOpenFileName(this, QString(), QString(), "Image Files(*.bmp *.jpg *.png)"));
}

void MainWindow::on_action_save_triggered()
{
    m_image.save(QFileDialog::getSaveFileName(this, QString(), QString(), "Image Files(*.bmp *.jpg *.png)"));
}

void MainWindow::on_action_line_triggered()
{
      m_state = LINE;
      m_cursor_points.clear();
}

void MainWindow::on_action_connectLine_triggered(){
    m_state = CONNECTLINE;
    m_cursor_points.clear();
}

void MainWindow::on_action_triangle_triggered()
{
    m_state = TRIANGLE;
    m_cursor_points.clear();
}

void MainWindow::on_action_rectangle_triggered()
{
    m_state = RECTANGLE;
    m_cursor_points.clear();
}

void MainWindow::on_action_polygon_triggered()
{
    m_state = POLYGON;
    m_cursor_points.clear();
}

void MainWindow::on_action_circle_triggered()
{
    m_state = CIRCLE;
    m_cursor_points.clear();
}

void MainWindow::on_action_ellipse_triggered()
{
    m_state = ELLIPSE;
    m_cursor_points.clear();
}



void MainWindow::on_action_translate_triggered()
{
    m_state = TRANSLATE;
    m_cursor_points.clear();
}

void MainWindow::on_action_rotate_triggered()
{
    m_state = ROTATE;
    m_cursor_points.clear();
}

void MainWindow::on_action_clip_triggered()
{
    m_state = CLIP;
    m_cursor_points.clear();
}

void  MainWindow::on_action_undo_triggered(){
    if(m_undo.size()>1){
        m_redo.push_back(m_undo.back());
        m_undo.pop_back();
    }
    m_shapes = m_undo.back();
    update();
}
void  MainWindow::on_action_redo_triggered(){
    if(m_redo.size()>1){
        m_undo.push_back(m_redo.back());
        m_redo.pop_back();
    }
    m_shapes = m_redo.back();
    update();
}

void MainWindow::on_action_pallet_triggered()
{
    m_pen.setColor(QColorDialog::getColor(m_pen.color(), this));
}

void MainWindow::on_action_penUp_triggered(){
     m_pen.setWidth(m_pen.width() + 1);
}
void MainWindow::on_action_penDown_triggered(){
    if (m_pen.width() > 1)
        m_pen.setWidth(m_pen.width() - 1);
}

void MainWindow::on_action_copy_triggered(){
    m_state = COPY;
    m_cursor_points.clear();
}

void MainWindow::on_action_trash_triggered()
{
    m_state = TRASH;
    m_cursor_points.clear();
}

void MainWindow::on_action_combine_triggered(){
   m_state = COMBINE;
   m_cursor_points.clear();
   m_pshape = nullptr;
}

void MainWindow::on_action_help_triggered()
{

}

