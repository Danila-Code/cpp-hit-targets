#include "mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QString>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), p_quad_tree_(new QuadTree(Rectangle({0, 0}, {99, 99}))) {
    ui->setupUi(this);

    setFixedSize(800, 650);

    ui->le_source_file->setText("F:\\Dev\\interview\\kotlin\\coords.txt");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pb_select_dir_clicked() {
    QString file_path = QFileDialog::getOpenFileName(this,
                                                     "Выбрать текстовый файл",
                                                     QDir::homePath(),
                                                     "Text files (*.txt);;All files (*.*)");

    if (file_path.isEmpty()) {
        return;
    }

    ui->le_source_file->setText(file_path);
}

void MainWindow::on_pb_build_quad_tree_clicked() {
    QString file_path = ui->le_source_file->text();

    if (file_path.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Файл не выбран");
        return;
    }

    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", QString("Не удалось открыть файл:\n%1").arg(file.errorString()));
        return;
    }

    QTextStream in(&file);

    auto points = ReadPoints(in);
    file.close();

    if (!points.has_value()) {
        QMessageBox::critical(this, "Ошибка", QString("Неверный формат входного файла"));
        return;
    }

    qDebug() << "Прочитано точек " << points.value().size();

    SetPoints(points.value());
    update();
}

void MainWindow::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    if (!p_quad_tree_) {
        return;
    }

    auto root = p_quad_tree_->getRoot();
    if (!root) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Область рисования (с отступами от панели управления)
    QRect drawArea(10, 10, 700, 500);
    painter.translate(drawArea.left(), drawArea.top());

    /*painter.setBrush(Qt::red);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(0, 0), 1, 1);*/

    // Масштабирование: 100x100 -> 700x500
    double scaleX = static_cast<double>(drawArea.width()) / 100.0;
    double scaleY = static_cast<double>(drawArea.height()) / 100.0;
    painter.scale(scaleX, scaleY);

    /*painter.setBrush(Qt::red);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(0, 0), 1, 1);*/

    // Рисуем границы квадрантов
    drawQuadTree(painter, root);

    // Рисуем точки
    drawPoints(painter);
}

std::optional<QVector<Point>> MainWindow::ReadPoints(QTextStream& in) {
    QVector<Point> points;
    for (QString line = in.readLine(); !line.isEmpty(); line = in.readLine()) {
        auto point = GetPoint(line);

        if (!point.has_value()) {
            return std::nullopt;
        }

        points.push_back(point.value());
    }
    return points;
}

void MainWindow::SetPoints(const QVector<Point>& points) {
    for (const auto& point : points) {
        p_quad_tree_->insert(point);
    }
}

std::optional<Point> MainWindow::GetPoint(const QString& str) {
    const QChar delim = ',';
    qsizetype delim_pos = str.indexOf(delim);

    if (delim_pos == -1) {
        return std::nullopt;
    }

    bool ok_x = false, ok_y = false;
    int x = str.left(delim_pos).toInt(&ok_x);
    int y = str.mid(delim_pos + 1).toInt(&ok_y);

    if (!ok_x || !ok_y) {
        return std::nullopt;
    }

    return Point(x, y);
}

void MainWindow::drawQuadTree(QPainter& painter, const QuadTree::QuadNode* const node) {
    if (!node) {
        return;
    }

    Rectangle rect = node->getRect();

    painter.setPen(QPen(Qt::gray, 0.2));
    painter.drawRect(rect.left_down.x, 99 - rect.right_up.y, rect.getWidth(), rect.getHeight());

    qDebug() << "x0 = " << rect.left_down.x << " y0 = " << rect.right_up.y << " width = " << rect.getWidth()
             << " height = " << rect.getHeight();

    auto subnodes = node->getSubnodes();
    if (!subnodes) {
        return;
    }

    for (int i = 0; i < NODE_COUNT; ++i) {
        drawQuadTree(painter, &subnodes[i]);
    }
}

void MainWindow::drawPoints(QPainter& painter) {
    painter.setBrush(Qt::red);
    painter.setPen(Qt::NoPen);

    std::vector<Point> points = p_quad_tree_->getPoints();

    for (auto& point : points) {
        painter.drawEllipse(QPointF(point.x, 99 - point.y), 0.3, 0.3);
    }
}
