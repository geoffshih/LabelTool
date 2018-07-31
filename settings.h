#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QDebug>

#include <QString>
#include <QColor>
#include <QList>
#include <QtXml>

class Settings: public QObject
{
     Q_OBJECT
public:
    Settings();

    struct LabelObj {
        QString name;
        QString quick_key;
        QColor color;
    };

    void createLabel(QString name, QString quick_key, QColor color);
    void addLabel(LabelObj label);
    void setLabel(QString old_name, QString name, QString quick_key, QColor color);
    void deleteLabel(QString name);

    QList<LabelObj> getLabels();
    LabelObj getLabel(QString name);

    QList<Qt::Key> getKeyValues();
    Qt::Key getKeyValue(QString str);
    QString getKeyString(Qt::Key key);

    Qt::Key toKey(QString const & str);
    QString colorToString();

    void readSetting();
    void writeSetting();

signals:
    void callInitLabel();
    void callInitMode(QMap<QString, Qt::Key>);

public slots:
    void setModeKey(QString, QString );

private:
    QList<LabelObj> labelList;
    QString inputImgPath, outImgPath;
    QMap<QString, Qt::Key> keyMap;

};

#endif // SETTINGS_H
