#ifndef MODETAB_H
#define MODETAB_H

#include <QWidget>
#include <QDebug>

#include <QLabel>
#include <QComboBox>
#include <QGridLayout>

class ModeTab : public QWidget
{
    Q_OBJECT
public:
    explicit ModeTab(QWidget *parent = nullptr);

    QComboBox *moveKeyCombo;
    QComboBox *moveAuxCombo;
    QComboBox *deleteKeyCombo;
    QComboBox *deleteAuxCombo;
    QComboBox *drawKeyCombo;
    QComboBox *drawAuxCombo;
    QComboBox *saveKeyCombo;
    QComboBox *saveAuxCombo;

    QComboBox *previousKeyCombo;
    QComboBox *previousAuxCombo;
    QComboBox *nextKeyCombo;
    QComboBox *nextAuxCombo;

    void combine(QString mode, QComboBox& cb_k, QComboBox& cb_a);

signals:
    void passModeKey(QString, QString);

public slots:
    void passDrawKeyText(const QString&);
    void passDrawAuxText(const QString&);
    void passMoveKeyText(const QString&);
    void passMoveAuxText(const QString&);
    void passSaveKeyText(const QString&);
    void passSaveAuxText(const QString&);
    void passDeleteKeyText(const QString&);
    void passDeleteAuxText(const QString&);
    void passPreviousKeyText(const QString&);
    void passPreviousAuxText(const QString&);
    void passNextKeyText(const QString&);
    void passNextAuxText(const QString&);

    void initMode(QMap<QString, Qt::Key> keymap);
};

#endif // MODETAB_H
