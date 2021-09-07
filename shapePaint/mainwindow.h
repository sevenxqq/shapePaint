#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QMainWindow>
#include "shape.h"
#include "derive.h"
#include <QPen>
#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>
#include <QFileDialog>
#include <iostream>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    enum State {NONE,LINE,CONNECTLINE, TRIANGLE, RECTANGLE, POLYGON, CIRCLE, ELLIPSE,
                TRANSLATE, ROTATE, CLIP, COPY,TRASH,COMBINE} state;
    QVector<QPoint> m_cursor_points;			// 记录鼠标位置
    QList<Shape*> m_shapes;            //已经绘制的图形
    QList<QList<Shape*>> m_undo;           //撤回
    QList<QList<Shape*>> m_redo;           //恢复
    Shape *m_pshape;                //当前操纵图元
    State m_state;                  //当前绘制状态
    QImage m_image;					// 画布
    QPainter m_painter;				// 画笔
    QPen m_pen;						// 笔刷
    QString file;                   //加载的文件名

private slots:
    void on_action_open_triggered();
    void on_action_save_triggered();
    void on_action_line_triggered();
    void on_action_connectLine_triggered();
    void on_action_triangle_triggered();
    void on_action_rectangle_triggered();
    void on_action_polygon_triggered();
    void on_action_circle_triggered();
    void on_action_ellipse_triggered();
    void on_action_translate_triggered();
    void on_action_rotate_triggered();
    void on_action_clip_triggered();
    void on_action_undo_triggered();
    void on_action_redo_triggered();
    void on_action_pallet_triggered();
    void on_action_copy_triggered();
    void on_action_trash_triggered();
    void on_action_help_triggered();
    void on_action_combine_triggered();
    void on_action_penUp_triggered();
    void on_action_penDown_triggered();
};
#endif // MAINWINDOW_H
