#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>

#include "../../quad_tree_lib/include/quad_tree.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_select_dir_clicked();

    void on_pb_build_quad_tree_clicked();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    std::optional<QVector<Point>> ReadPoints(QTextStream& in);

    void SetPoints(const QVector<Point>& points);

    std::optional<Point> GetPoint(const QString& str);

    void drawQuadTree(QPainter& painter, const QuadTree::QuadNode* const);

    void drawPoints(QPainter& painter);

private:
    Ui::MainWindow* ui;

    QScopedPointer<QuadTree> p_quad_tree_;
};
#endif  // MAINWINDOW_H
