#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCoreApplication>
#include <QDebug>

#include <QMainWindow>
#include <QListWidget>
#include <QTabWidget>
#include <QWidget>

#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDir>
#include <QMap>

#include <QKeySequence>
#include <QKeyEvent>

#include <mygraphicsview.h>
#include <labeltab.h>
#include <modetab.h>

#include <iostream>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void keyPressEvent(QKeyEvent* event);

private:

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    MyGraphicsView *mGraphicsView;

    QTabWidget *tabWidget;
    LabelTab *labelTab;
    ModeTab *modeTab;

    QPushButton *openDirBtn;
    QPushButton *saveDirBtn;
    QLineEdit *openDirEdit;
    QLineEdit *saveDirEdit;

    QPushButton *moveBtn;
    QLineEdit *moveEdit;

    QFileInfoList list;
    QLabel *msgLabel;

    QString openDir, saveDir;

    int mode = 0;
    int bg_width = 0;
    int bg_height = 0;
    int bg_width_final = 0;
    int bg_height_final = 0;

    int imageIndex;

    Qt::Key toKey(QString const & str);

    void openNextImg();
    void openPreImg();

    void writeXml();

private slots:
    void receivePos(QPoint start, QPoint end);
    void setOpenDir();
    void setSaveDir();
    void refreshSize();
    void goIndex();

};

#endif // MAINWINDOW_H
