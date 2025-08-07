#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QGraphicsView* view;
    QGraphicsScene* scene;
};

#endif // MAINWINDOW_H
