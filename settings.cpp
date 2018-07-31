#include "settings.h"

Settings::Settings()
{

}

void Settings::createLabel(QString name, QString quick_key, QColor color)
{
    LabelObj label;
    label.name = name;
    label.quick_key = quick_key;
    label.color = color;
    keyMap.insert(name, toKey(quick_key));
    addLabel(label);
}

void Settings::addLabel(LabelObj label)
{
    labelList.append(label);
}

QList<Settings::LabelObj> Settings::getLabels()
{
    return labelList;
}

Settings::LabelObj Settings::getLabel(QString name)
{
    for(int i=0; i<labelList.count(); i++)
    {
        if(labelList[i].name == name)
        {
            return labelList[i];
        }
    }
}

void Settings::setLabel(QString old_name, QString name, QString quick_key, QColor color)
{
    for(int i=0; i<labelList.count(); i++)
    {
        if(labelList[i].name == old_name)
        {
            labelList[i].name = name;
            labelList[i].quick_key = quick_key;
            labelList[i].color = color;

            keyMap.remove(old_name);
            keyMap.insert(labelList[i].name, toKey(quick_key));
            qDebug() << "editLabel finished" << labelList[i].name <<labelList[i].quick_key<<labelList[i].color;
            break;
        }
    }
}
void Settings::deleteLabel(QString name)
{
    for(int i=0; i<labelList.count(); i++)
    {
        if(labelList[i].name == name)
        {
            labelList.removeAt(i);
            keyMap.remove(name);
            return;
        }
    }
}

Qt::Key Settings::toKey(QString const & str) {
    QKeySequence seq(str);
    uint keyCode;

    if(seq.count() == 1)
        keyCode = seq[0];
    else {
        assert(seq.count() == 0);

        seq = QKeySequence(str + "+A");
        assert(seq.count() == 1);
        assert(seq[0] > 65);
        keyCode = seq[0] - 65;
    }
    return Qt::Key(keyCode);
}

void Settings::readSetting()
{
    QFile file("./settings.xml");
    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << "Error: cannot open file";
        return;
    }

    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        qDebug() << "Error: setContent file";
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();
    QDomNodeList nodes=root.childNodes();
    for(int i=0; i<nodes.count(); i++)
    {
        if(nodes.at(i).nodeName().compare("labels")==0)
        {
            QDomNodeList labels = nodes.at(i).childNodes();
            for(int j=0; j<labels.count(); j++)
            {
                QDomElement e = labels.at(j).toElement();
                LabelObj label;
                label.name = e.attribute("name");
                label.quick_key = e.attribute("key");
                label.color = QColor(e.attribute("color"));
                labelList.append(label);

                keyMap.insert( e.attribute("name"), toKey(e.attribute("key")));
                qDebug()<<e.attribute("name")<<"-"<<e.attribute("key")<<"-"<<e.attribute("color");
            }
        }
        if(nodes.at(i).nodeName().compare("modes")==0)
        {
            QDomNodeList modes = nodes.at(i).childNodes();
            for(int j=0; j<modes.count(); j++)
            {
                QDomElement e = modes.at(j).toElement();
                if(e.attribute("key")=="")
                    keyMap.insert( e.attribute("name"), Qt::Key_A);
                else
                    keyMap.insert( e.attribute("name"), toKey(e.attribute("key")));
                qDebug()<<e.tagName()<<"-"<<e.attribute("name")<<"-"<<e.attribute("key")<<"-"<<e.attribute("color");
            }
        }
    }

    qDebug() << "read finished!";

    emit callInitLabel();
    emit callInitMode(keyMap);

}

void Settings::writeSetting()
{
    QFile file("./settings.xml");
    if (!file.open(QFile::WriteOnly | QFile::Text | QIODevice::Truncate))
    {
       qDebug() << "Error: cannot open file";
       return;
    }

    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement config = doc.createElement("config");
    doc.appendChild(config);

    QDomElement labels = doc.createElement("labels");
    config.appendChild(labels);

    for(int i=0; i<labelList.count(); i++)
    {
        QDomElement label = doc.createElement("label");
        label.setAttribute("name", labelList[i].name);
        label.setAttribute("key", labelList[i].quick_key);
        label.setAttribute("color", labelList[i].color.name());

        labels.appendChild(label);
    }

    QDomElement modes = doc.createElement("modes");
    config.appendChild(modes);

    QDomElement mode1 = doc.createElement("mode");
    mode1.setAttribute("name", "Draw");
    mode1.setAttribute("key", QKeySequence(keyMap["Draw"]).toString());
    modes.appendChild(mode1);

    QDomElement mode2 = doc.createElement("mode");
    mode2.setAttribute("name", "Move");
    mode2.setAttribute("key", QKeySequence(keyMap["Move"]).toString());
    modes.appendChild(mode2);

    QDomElement mode3 = doc.createElement("mode");
    mode3.setAttribute("name", "Delete");
    mode3.setAttribute("key", QKeySequence(keyMap["Delete"]).toString());
    modes.appendChild(mode3);

    QDomElement mode4 = doc.createElement("mode");
    mode4.setAttribute("name", "Save");
    mode4.setAttribute("key", QKeySequence(keyMap["Save"]).toString());
    modes.appendChild(mode4);

    QDomElement mode5 = doc.createElement("mode");
    mode5.setAttribute("name", "Previous");
    mode5.setAttribute("key", QKeySequence(keyMap["Previous"]).toString());
    modes.appendChild(mode5);

    QDomElement mode6 = doc.createElement("mode");
    mode6.setAttribute("name", "Next");
    mode6.setAttribute("key", QKeySequence(keyMap["Next"]).toString());
    modes.appendChild(mode6);

    QTextStream out_stream(&file);
    doc.save(out_stream,4);
    file.close();

    qDebug() << "write finished!";
}

void Settings::setModeKey(QString mode, QString key)
{
    keyMap[mode] = toKey(key);
}

QList<Qt::Key> Settings::getKeyValues()
{
    return keyMap.values();
}

Qt::Key Settings::getKeyValue(QString str)
{
    return keyMap[str];
}

QString Settings::getKeyString(Qt::Key key)
{
    return keyMap.key(key);
}
