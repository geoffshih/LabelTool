#include "modetab.h"

ModeTab::ModeTab(QWidget *parent) : QWidget(parent)
{
    moveKeyCombo = new QComboBox(this);
    moveAuxCombo = new QComboBox(this);
    drawKeyCombo = new QComboBox(this);
    drawAuxCombo = new QComboBox(this);
    saveKeyCombo = new QComboBox(this);
    saveAuxCombo = new QComboBox(this);
    deleteKeyCombo = new QComboBox(this);
    deleteAuxCombo = new QComboBox(this);

    previousKeyCombo = new QComboBox(this);
    previousAuxCombo = new QComboBox(this);
    nextKeyCombo = new QComboBox(this);
    nextAuxCombo = new QComboBox(this);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(new QLabel("Draw"),0,0);
    mainLayout->addWidget(drawKeyCombo,0,1);
    mainLayout->addWidget(drawAuxCombo,0,2);

    mainLayout->addWidget(new QLabel("Move"),1,0);
    mainLayout->addWidget(moveKeyCombo,1,1);
    mainLayout->addWidget(moveAuxCombo,1,2);

    mainLayout->addWidget(new QLabel("Delete"),2,0);
    mainLayout->addWidget(deleteKeyCombo,2,1);
    mainLayout->addWidget(deleteAuxCombo,2,2);

    mainLayout->addWidget(new QLabel("Save"),3,0);
    mainLayout->addWidget(saveKeyCombo,3,1);
    mainLayout->addWidget(saveAuxCombo,3,2);

    mainLayout->addWidget(new QLabel("Previous"),4,0);
    mainLayout->addWidget(previousKeyCombo,4,1);
    mainLayout->addWidget(previousAuxCombo,4,2);

    mainLayout->addWidget(new QLabel("Next"),5,0);
    mainLayout->addWidget(nextKeyCombo,5,1);
    mainLayout->addWidget(nextAuxCombo,5,2);

    setLayout(mainLayout);

    QStringList keyList, aukList;

    for(int i=0; i<26; i++)
    {
        keyList << QString(char(65+i));
    }

    aukList << "Null" << "Shift" << "Ctrl" << "Alt";
    moveAuxCombo->addItems(aukList);
    drawAuxCombo->addItems(aukList);
    saveAuxCombo->addItems(aukList);
    deleteAuxCombo->addItems(aukList);
    previousAuxCombo->addItems(aukList);
    nextAuxCombo->addItems(aukList);

    drawKeyCombo->addItems(keyList);
    drawKeyCombo->setMaxVisibleItems(10);
    drawKeyCombo->setEditable(true);

    moveKeyCombo->addItems(keyList);
    moveKeyCombo->setMaxVisibleItems(10);
    moveKeyCombo->setEditable(true);

    deleteKeyCombo->addItems(keyList);
    deleteKeyCombo->setMaxVisibleItems(10);
    deleteKeyCombo->setEditable(true);

    saveKeyCombo->addItems(keyList);
    saveKeyCombo->setMaxVisibleItems(10);
    saveKeyCombo->setEditable(true);

    previousKeyCombo->addItems(keyList);
    previousKeyCombo->setMaxVisibleItems(10);
    previousKeyCombo->setEditable(true);

    nextKeyCombo->addItems(keyList);
    nextKeyCombo->setMaxVisibleItems(10);
    nextKeyCombo->setEditable(true);

    connect(drawKeyCombo,SIGNAL(currentTextChanged(const QString&)),
            this,SLOT(passDrawKeyText(const QString&)));
    connect(moveKeyCombo,SIGNAL(currentTextChanged(const QString&)),
            this,SLOT(passMoveKeyText(const QString&)));
    connect(saveKeyCombo,SIGNAL(currentTextChanged(const QString&)),
            this,SLOT(passSaveKeyText(const QString&)));
    connect(deleteKeyCombo,SIGNAL(currentTextChanged(const QString&)),
            this,SLOT(passDeleteKeyText(const QString&)));
    connect(previousKeyCombo,SIGNAL(currentTextChanged(const QString&)),
            this,SLOT(passPreviousKeyText(const QString&)));
    connect(nextKeyCombo,SIGNAL(currentTextChanged(const QString&)),
            this,SLOT(passNextKeyText(const QString&)));

    connect(drawAuxCombo,SIGNAL(currentTextChanged(const QString&)),
            this,SLOT(passDrawAuxText(const QString&)));
    connect(moveAuxCombo,SIGNAL(currentTextChanged(const QString&)),
            this,SLOT(passMoveAuxText(const QString&)));
    connect(saveAuxCombo,SIGNAL(currentTextChanged(const QString&)),
            this,SLOT(passSaveAuxText(const QString&)));
    connect(deleteAuxCombo,SIGNAL(currentTextChanged(const QString&)),
            this,SLOT(passDeleteAuxText(const QString&)));
    connect(previousAuxCombo,SIGNAL(currentTextChanged(const QString&)),
            this,SLOT(passPreviousAuxText(const QString&)));
    connect(nextAuxCombo,SIGNAL(currentTextChanged(const QString&)),
            this,SLOT(passNextAuxText(const QString&)));

}

void ModeTab::initMode(QMap<QString, Qt::Key> keymap)
{
    QString drawStr, moveStr, deleteStr, saveStr, previousStr, nextStr;

    drawStr = QKeySequence(keymap["Draw"]).toString();
    moveStr = QKeySequence(keymap["Move"]).toString();
    saveStr = QKeySequence(keymap["Save"]).toString();
    deleteStr = QKeySequence(keymap["Delete"]).toString();
    previousStr = QKeySequence(keymap["Previous"]).toString();
    nextStr = QKeySequence(keymap["Next"]).toString();

    if(drawStr.contains("+"))
    {
        drawAuxCombo->setCurrentText(drawStr.split("+").at(0));
        drawKeyCombo->setCurrentText(drawStr.split("+").at(1));
    }
    else
    {
        drawAuxCombo->setCurrentText("Null");
        drawKeyCombo->setCurrentText(drawStr);
    }

    if(moveStr.contains("+"))
    {
        moveAuxCombo->setCurrentText(moveStr.split("+").at(0));
        moveKeyCombo->setCurrentText(moveStr.split("+").at(1));
    }
    else
    {
        moveAuxCombo->setCurrentText("Null");
        moveKeyCombo->setCurrentText(moveStr);
    }

    if(saveStr.contains("+"))
    {
        saveAuxCombo->setCurrentText(saveStr.split("+").at(0));
        saveKeyCombo->setCurrentText(saveStr.split("+").at(1));
    }
    else
    {
        saveAuxCombo->setCurrentText("Null");
        saveKeyCombo->setCurrentText(saveStr);
    }

    if(deleteStr.contains("+"))
    {
        deleteAuxCombo->setCurrentText(deleteStr.split("+").at(0));
        deleteKeyCombo->setCurrentText(deleteStr.split("+").at(1));
    }
    else
    {
        deleteAuxCombo->setCurrentText("Null");
        deleteKeyCombo->setCurrentText(deleteStr);
    }

    if(previousStr.contains("+"))
    {
        previousAuxCombo->setCurrentText(previousStr.split("+").at(0));
        previousKeyCombo->setCurrentText(previousStr.split("+").at(1));
    }
    else
    {
        previousAuxCombo->setCurrentText("Null");
        previousKeyCombo->setCurrentText(previousStr);
    }

    if(nextStr.contains("+"))
    {
        nextAuxCombo->setCurrentText(nextStr.split("+").at(0));
        nextKeyCombo->setCurrentText(nextStr.split("+").at(1));
    }
    else
    {
        nextAuxCombo->setCurrentText("Null");
        nextKeyCombo->setCurrentText(nextStr);
    }


}

void ModeTab::passDrawKeyText(const QString& ss)
{
    combine("Draw", *drawKeyCombo, *drawAuxCombo);
}
void ModeTab::passMoveKeyText(const QString& ss)
{
    combine("Move", *moveKeyCombo, *moveAuxCombo);
}
void ModeTab::passSaveKeyText(const QString& ss)
{
    combine("Save", *saveKeyCombo, *saveAuxCombo);
}
void ModeTab::passDeleteKeyText(const QString& ss)
{
    combine("Delete", *deleteKeyCombo, *deleteAuxCombo);
}
void ModeTab::passPreviousKeyText(const QString& ss)
{
    combine("Previous", *previousKeyCombo, *previousAuxCombo);
}
void ModeTab::passNextKeyText(const QString& ss)
{
    combine("Next", *nextKeyCombo, *nextAuxCombo);
}

void ModeTab::passDrawAuxText(const QString& ss)
{
    combine("Draw", *drawKeyCombo, *drawAuxCombo);
}
void ModeTab::passMoveAuxText(const QString& ss)
{
    combine("Move", *moveKeyCombo, *moveAuxCombo);
}
void ModeTab::passSaveAuxText(const QString& ss)
{
    combine("Save", *saveKeyCombo, *saveAuxCombo);
}
void ModeTab::passDeleteAuxText(const QString& ss)
{
    combine("Delete", *deleteKeyCombo, *deleteAuxCombo);
}
void ModeTab::passPreviousAuxText(const QString& ss)
{
    combine("Previous", *previousKeyCombo, *previousAuxCombo);
}
void ModeTab::passNextAuxText(const QString& ss)
{
    combine("Next", *nextKeyCombo, *nextAuxCombo);
}


void ModeTab::combine(QString mode, QComboBox& cb_k, QComboBox& cb_a)
{
    if(cb_a.currentText().compare("Null")==0)
    {
        emit passModeKey(mode,cb_k.currentText());
    }
    else
    {
        emit passModeKey(mode,cb_a.currentText()+ "+" +cb_k.currentText());
    }

}
