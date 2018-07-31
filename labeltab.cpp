#include "labeltab.h"

LabelTab::LabelTab(QWidget *parent) : QWidget(parent)
{
    input = new QLineEdit(this);
    labelListWidget = new QListWidget(this);
    inputBtn = new QPushButton("Add",this);
    editBtn = new QPushButton("Edit",this);
    deleteBtn = new QPushButton("Del",this);

    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(input);
    inputLayout->addWidget(inputBtn);
    inputLayout->setStretchFactor(input, 3);
    inputLayout->setStretchFactor(inputBtn, 1);

    QWidget *inputWidget = new QWidget();
    inputWidget->setLayout(inputLayout);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(editBtn);
    btnLayout->addWidget(deleteBtn);

    QWidget *btnWidget = new QWidget();
    btnWidget->setLayout(btnLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(inputWidget);
    mainLayout->addWidget(labelListWidget);
    mainLayout->addWidget(btnWidget);

    setLayout(mainLayout);

    settings = new Settings();

    connect(labelListWidget, SIGNAL (currentTextChanged(const QString &)), this, SLOT (currentLabelChange(const QString &)));

    connect(inputBtn, SIGNAL (clicked()), this, SLOT (addLabel()));
    connect(editBtn, SIGNAL (clicked()), this, SLOT (editLabel()));
    connect(deleteBtn, SIGNAL (clicked()), this, SLOT (deleteLabel()));

}

void LabelTab::initLabel()
{
    for(int i=0; i<settings->getLabels().count(); i++)
    {
        QListWidgetItem* listItem =new QListWidgetItem(settings->getLabels()[i].name, labelListWidget);

        QPixmap pixmap(16,16);
        pixmap.fill(settings->getLabels()[i].color);
        QIcon icon(pixmap);
        listItem->setIcon(icon);
        listItem->setToolTip(settings->getLabels()[i].quick_key);
        labelListWidget->addItem(listItem);
    }
}

void LabelTab::addLabel()
{
    if((!input->text().isEmpty())&&useColorDialog()&&useInputKey())
    {
        QListWidgetItem* listItem =new QListWidgetItem(input->text(), labelListWidget);

        QPixmap pixmap(16,16);
        pixmap.fill(currentColor);
        QIcon icon(pixmap);
        listItem->setIcon(icon);
        listItem->setSelected(true);
        listItem->setToolTip(tmpKey);
        labelListWidget->addItem(listItem);
        settings->createLabel(input->text(),tmpKey,currentColor);
    }
    else
    {
        qDebug() << "no input or no select color";        
        input->clear();
        tmpKey="";
    }
    input->clear();
    tmpKey="";
}

void LabelTab::editLabel()
{
    if(!labelListWidget->selectedItems().isEmpty())
    {
        // set label name
        bool ok;
        QString oldtext = labelListWidget->selectedItems().at(0)->text();
        QString text = QInputDialog::getText(this, tr("Edit label name"),
                                             tr("Label name:"), QLineEdit::Normal,labelListWidget->selectedItems().at(0)->text(), &ok);
        if(ok && !text.isEmpty())
                labelListWidget->selectedItems().at(0)->setText(text);

        // set label color
        QColor oldcolor;

        QColor color = QColorDialog::getColor(settings->getLabel(oldtext).color,
                                              this, tr("Select LabelColor"), QColorDialog::DontUseNativeDialog);
        if(color.isValid())
        {
            QPixmap pixmap(16,16);
            pixmap.fill(color);
            QIcon icon(pixmap);
            labelListWidget->selectedItems().at(0)->setIcon(icon);
        }
        if(editInputKey(oldtext))
        {
            labelListWidget->selectedItems().at(0)->setToolTip(tmpKey);
        }

        settings->setLabel(oldtext, text, tmpKey, color);
        tmpKey="";
    }
}

void LabelTab::deleteLabel()
{
    QList<QListWidgetItem*> items = labelListWidget->selectedItems();
    foreach(QListWidgetItem *item, items)
    {
        settings->deleteLabel(item->text());
        delete labelListWidget->takeItem(labelListWidget->row(item));
    }
}

bool LabelTab::setSelectLabel(QString labelName)
{
    for(int i = 0; i < labelListWidget->count(); ++i)
    {
        QListWidgetItem* item = labelListWidget->item(i);
        item->setSelected(false);
        if(item->text().compare(labelName) == 0)
        {
            item->setSelected(true);
            currentColor = settings->getLabel(item->text()).color;
        }
    }
    return 0;
}

void LabelTab::currentLabelChange(const QString & labelname)
{
    currentColor = settings->getLabel(labelname).color;
}

void LabelTab::setCurrentColor(QColor color)
{
    currentColor = color;
}
QColor LabelTab::getCurrentColor()
{
    return currentColor;
}


bool LabelTab::useColorDialog()
{
    QColor color = QColorDialog::getColor(Qt::red, this, tr("Select LabelColor"), QColorDialog::DontUseNativeDialog);
    if(color.isValid())
    {
        currentColor = color;
        return true;
    }
    return false;
}

bool LabelTab::useInputKey()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Edit label key"),
                                         tr("Label_Key:"), QLineEdit::Normal,"", &ok);
    if(ok && !text.isEmpty())
    {
        tmpKey = text;
        return true;
    }
    return false;

}
bool LabelTab::editInputKey(QString labelname)
{

    bool ok;
    QString text = QInputDialog::getText(this, tr("Edit label key"), tr("Label_Key:"),
                                         QLineEdit::Normal,settings->getLabel(labelname).quick_key, &ok);
    if(ok && !text.isEmpty())
    {
        tmpKey = text;
        return true;
    }
    return false;

}
QString LabelTab::findLabelByColor(QColor color)
{
    color.setAlpha(255);
    QList<Settings::LabelObj> labels = settings->getLabels();
    for(int i = 0; i < labels.count(); ++i)
    {
        if(labels[i].color == color)
        {
            return labels[i].name;
        }
    }
    return "Unknown";
}
