#ifndef LABELTAB_H
#define LABELTAB_H

#include <QWidget>
#include <QDebug>

#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QtXml>

#include <QColorDialog>
#include <QInputDialog>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <settings.h>

class LabelTab : public QWidget
{
    Q_OBJECT
public:
    explicit LabelTab(QWidget *parent = nullptr);

    QLineEdit *input;

    QListWidget *labelListWidget;
    QPushButton *inputBtn;
    QPushButton *editBtn;
    QPushButton *deleteBtn;

    QColor currentColor;
    QString tmpKey;

    Settings *settings;

    bool useColorDialog();
    bool useInputKey();
    bool editInputKey(QString labelname);

    bool setSelectLabel(QString labelName);
    void setCurrentColor(QColor color);

    QString findLabelByColor(QColor color);
    QColor getCurrentColor();

signals:

public slots:
    void addLabel();
    void editLabel();
    void deleteLabel();
    void initLabel();
    void currentLabelChange(const QString & labelname);
};

#endif // LABELTAB_H
