#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    mGraphicsView = new MyGraphicsView(this);
    setWindowTitle("LabelTool");

    mGraphicsView->setScene(scene);
    mGraphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    mGraphicsView->setAlignment(Qt::AlignCenter|Qt::AlignCenter);
    mGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mGraphicsView->setFrameStyle(0);

    connect(mGraphicsView, SIGNAL(sendPos(QPoint,QPoint)),
                         this, SLOT(receivePos(QPoint,QPoint)));
    connect(mGraphicsView, SIGNAL(doRefresh()),
                         this, SLOT(refreshSize()));


    QVBoxLayout *settinglayout = new QVBoxLayout;

    tabWidget = new QTabWidget;
    labelTab = new LabelTab;
    modeTab = new ModeTab;
    tabWidget->addTab(labelTab, tr("Label"));
    tabWidget->addTab(modeTab, tr("Mode"));
    tabWidget->setFixedSize(200,320);

    connect(labelTab->settings, SIGNAL (callInitLabel()), labelTab, SLOT (initLabel()));
    connect(labelTab->settings, SIGNAL (callInitMode(QMap<QString, Qt::Key>)), modeTab, SLOT (initMode(QMap<QString, Qt::Key>)));

    connect(modeTab, SIGNAL(passModeKey(QString,QString)),
                         labelTab->settings, SLOT(setModeKey(QString,QString)));

    openDirBtn = new QPushButton("open path");
    saveDirBtn = new QPushButton("save path");
    moveBtn = new QPushButton("index go");

    openDirEdit = new QLineEdit();
    saveDirEdit = new QLineEdit();
    moveEdit = new QLineEdit();

    openDirEdit->setReadOnly(true);
    saveDirEdit->setReadOnly(true);

    connect(openDirBtn, SIGNAL(clicked()), this, SLOT(setOpenDir()));
    connect(saveDirBtn, SIGNAL(clicked()), this, SLOT(setSaveDir()));
    connect(moveBtn, SIGNAL(clicked()), this, SLOT(goIndex()));

    QGridLayout *dirLayout = new QGridLayout;
    dirLayout->addWidget(openDirBtn,0,0,1,1);
    dirLayout->addWidget(openDirEdit,1,0,1,3);
    dirLayout->addWidget(saveDirBtn,2,0,1,1);
    dirLayout->addWidget(saveDirEdit,3,0,1,3);
    dirLayout->addWidget(moveBtn,4,1,1,2);
    dirLayout->addWidget(moveEdit,4,0,1,1);
    dirLayout->setAlignment(Qt::AlignLeft);
    dirLayout->setAlignment(Qt::AlignTop);

    QWidget *dirW = new QWidget();
    dirW->setLayout(dirLayout);
    dirW->setFixedWidth(200);
    settinglayout->addWidget(dirW);
    settinglayout->addWidget(tabWidget);

    QWidget *settingwindow = new QWidget();
    settingwindow->setLayout(settinglayout);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(mGraphicsView);
    layout->addWidget(settingwindow);
    layout->setStretch(0,1);
    layout->setStretch(1,0);

    QWidget *window = new QWidget();
    window->setLayout(layout);
    setCentralWidget(window);

    msgLabel = new QLabel;
    msgLabel->setAlignment(Qt::AlignHCenter);
    statusBar()->addWidget(msgLabel);

    resize(800,600);

    labelTab->settings->readSetting();
}

MainWindow::~MainWindow()
{
    labelTab->settings->writeSetting();
    delete ui;
}

void MainWindow::receivePos(QPoint start, QPoint end)
{
    qDebug() << "receivePos : " << start << end;
    QGraphicsRectItem *rectangle;
    QColor color = labelTab->getCurrentColor();
    color.setAlpha(128);

    rectangle = scene->addRect(QRectF(start, end));
    rectangle->setBrush(QBrush(color, Qt::Dense4Pattern));
}
void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if(event->modifiers()+event->key() == labelTab->settings->getKeyValue("Move"))
    {
        qDebug() << "set move mode.";
        mGraphicsView->setDragMode(QGraphicsView::RubberBandDrag);
        mode = 1;
        mGraphicsView->setMode(mode);

        foreach (QGraphicsItem *item, scene->items())
        {
            if(item->type() != QGraphicsPixmapItem::Type)
            {
                item->setFlag(QGraphicsItem::ItemIsMovable,true);
                item->setFlag(QGraphicsItem::ItemIsSelectable,true);
            }
        }

    }else if(event->modifiers()+event->key() == labelTab->settings->getKeyValue("Draw"))
    {
        qDebug() << "set draw mode.";
        mGraphicsView->setDragMode(QGraphicsView::RubberBandDrag);
        mode = 0;
        mGraphicsView->setMode(mode);

        foreach (QGraphicsItem *item, scene->items())
        {
            item->setFlag(QGraphicsItem::ItemIsMovable,false);
            item->setFlag(QGraphicsItem::ItemIsSelectable,false);
        }

    }else if(event->modifiers()+event->key() == labelTab->settings->getKeyValue("Delete"))
    {
        foreach (QGraphicsItem *item, scene->selectedItems())
        {
            scene->removeItem(item);
            delete item;
        }

    }else if(event->modifiers()+event->key() == labelTab->settings->getKeyValue("Save"))
    {
        writeXml();
        openNextImg();

    }else if(event->modifiers()+event->key() == labelTab->settings->getKeyValue("Previous"))
    {
        openPreImg();

    }else if(event->modifiers()+event->key() == labelTab->settings->getKeyValue("Next"))
    {
        openNextImg();

    }else if(labelTab->settings->getKeyValues().contains(Qt::Key(event->key())))
    {
        QString str = QString::fromStdString(labelTab->settings->getKeyString(Qt::Key(event->key())).toStdString());
        labelTab->setSelectLabel(str);
    }

}

Qt::Key MainWindow::toKey(QString const & str) {
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

void MainWindow::setOpenDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    openDirEdit->setText(dir);
    openDir = dir;

    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp" << "*.jpeg";

    QDir imgDir(openDir);
    imgDir.setFilter(QDir::Files);
    imgDir.setSorting(QDir::Name);
    list = imgDir.entryInfoList(filters);

    imageIndex = -1;
    openNextImg();
}

void MainWindow::setSaveDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    saveDirEdit->setText(dir);
    saveDir = dir;
}

void MainWindow::openNextImg()
{
    scene->clear();
    imageIndex++;

    if(imageIndex>=list.count()){imageIndex=list.count()-1;}

    QPixmap pixmap(list.at(imageIndex).absoluteFilePath());
    bg_width = pixmap.width();
    bg_height = pixmap.height();

    QString str = " [ " + QString::number(imageIndex) + " ] : ";
    moveEdit->setText(QString::number(imageIndex));
    msgLabel->setText(str + list.at(imageIndex).absoluteFilePath());

    QPixmap pixmap2 = pixmap.scaled(mGraphicsView->width(),mGraphicsView->height(),Qt::KeepAspectRatio);
    scene->addPixmap(pixmap2);
    bg_width_final = pixmap2.width();
    bg_height_final = pixmap2.height();

    mGraphicsView->setMaxPoint(bg_width_final, bg_height_final);
}
void MainWindow::openPreImg()
{
    scene->clear();
    imageIndex--;

    if(imageIndex<=0){imageIndex=0;}

    QPixmap pixmap(list.at(imageIndex).absoluteFilePath());
    bg_width = pixmap.width();
    bg_height = pixmap.height();

    QString str = " [ " + QString::number(imageIndex) + " ] : ";
    moveEdit->setText(QString::number(imageIndex));
    msgLabel->setText(str + list.at(imageIndex).absoluteFilePath());

    QPixmap pixmap2 = pixmap.scaled(mGraphicsView->width(),mGraphicsView->height(),Qt::KeepAspectRatio);
    scene->addPixmap(pixmap2);
    bg_width_final = pixmap2.width();
    bg_height_final = pixmap2.height();

    mGraphicsView->setMaxPoint(bg_width_final, bg_height_final);
}

void MainWindow::refreshSize()
{
    int bg_w, bg_h;
    double scale = 1;

    foreach (QGraphicsItem *item, scene->items())
    {
        if(item->type() == QGraphicsPixmapItem::Type)
        {
            QGraphicsPixmapItem* pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(item);
            bg_w = pixmapItem->pixmap().width();
            bg_h = pixmapItem->pixmap().height();
            QPixmap pixmap(list.at(imageIndex).absoluteFilePath());
            QPixmap pixmap2 = pixmap.scaled(mGraphicsView->width(),mGraphicsView->height(),Qt::KeepAspectRatio);
            pixmapItem->setPixmap(pixmap2);
            bg_width_final = pixmap2.width();
            bg_height_final = pixmap2.height();

            mGraphicsView->setMaxPoint(bg_width_final, bg_height_final);
            scale = double(bg_width_final)/bg_w;
        }
    }
    foreach (QGraphicsItem *item, scene->items())
    {
        if(item->type() == QGraphicsRectItem::Type)
        {
            QGraphicsRectItem* rectItem = qgraphicsitem_cast<QGraphicsRectItem*>(item);
            rectItem->setRect(rectItem->rect().topLeft().x()*scale, rectItem->rect().topLeft().y()*scale,
                              rectItem->rect().width()*scale,rectItem->rect().height()*scale);
        }
    }
}

void MainWindow::writeXml()
{
    QFile file(saveDir+"/"+list.at(imageIndex).baseName()+".xml");
    if (!file.open(QFile::WriteOnly | QFile::Text | QIODevice::Truncate))
    {
       qDebug() << "Error: cannot open file";
       return;
    }

    QDomDocument doc;
    QDomText text;

    QDomElement annotation = doc.createElement("annotation");
    doc.appendChild(annotation);

    QDomElement folder = doc.createElement("folder");
    text=doc.createTextNode(list.at(imageIndex).absolutePath());
    folder.appendChild(text);
    annotation.appendChild(folder);

    QDomElement filename = doc.createElement("filename");
    text=doc.createTextNode(list.at(imageIndex).fileName());
    filename.appendChild(text);
    annotation.appendChild(filename);

    QDomElement size = doc.createElement("size");
    QDomElement width = doc.createElement("width");
    text=doc.createTextNode(QString::number(bg_width));
    width.appendChild(text);
    QDomElement height = doc.createElement("height");
    text=doc.createTextNode(QString::number(bg_height));
    height.appendChild(text);
    QDomElement depth = doc.createElement("depth");
    text=doc.createTextNode(QString::number(3));
    depth.appendChild(text);

    size.appendChild(width);
    size.appendChild(height);
    size.appendChild(depth);
    annotation.appendChild(size);

    QDomElement segmented = doc.createElement("segmented");
    text=doc.createTextNode(QString::number(0));
    segmented.appendChild(text);
    annotation.appendChild(segmented);

    foreach (QGraphicsItem *item, scene->items())
    {
        if(item->type() == QGraphicsRectItem::Type)
        {
            QDomElement object = doc.createElement("object");

            QDomElement name = doc.createElement("name");
            QGraphicsRectItem* rectItem = qgraphicsitem_cast<QGraphicsRectItem*>(item);
            QString str = labelTab->findLabelByColor(rectItem->brush().color());
            text=doc.createTextNode(str);
            name.appendChild(text);
            QDomElement pose = doc.createElement("pose");
            text=doc.createTextNode("Unspecified");
            pose.appendChild(text);
            QDomElement truncated = doc.createElement("truncated");
            text=doc.createTextNode(QString::number(0));
            truncated.appendChild(text);
            QDomElement difficult = doc.createElement("difficult");
            text=doc.createTextNode(QString::number(0));
            difficult.appendChild(text);

            object.appendChild(name);
            object.appendChild(pose);
            object.appendChild(truncated);
            object.appendChild(difficult);

            QDomElement bndbox = doc.createElement("bndbox");

            QDomElement xmin = doc.createElement("xmin");
            text=doc.createTextNode(QString::number(int(item->boundingRect().topLeft().x()*bg_width/bg_width_final)));
            xmin.appendChild(text);
            QDomElement ymin = doc.createElement("ymin");
            text=doc.createTextNode(QString::number(int(item->boundingRect().topLeft().y()*bg_width/bg_width_final)));
            ymin.appendChild(text);
            QDomElement xmax = doc.createElement("xmax");
            if(item->boundingRect().bottomRight().x()*bg_width/bg_width_final >= bg_width)
            {
                text = doc.createTextNode(QString::number(bg_width-1));
            }
            else text=doc.createTextNode(QString::number(item->boundingRect().bottomRight().x()*bg_width/bg_width_final));
            xmax.appendChild(text);
            QDomElement ymax = doc.createElement("ymax");
            if(item->boundingRect().bottomRight().y()*bg_width/bg_width_final >= bg_height)
            {
                text = doc.createTextNode(QString::number(bg_height-1));
            }
            else text=doc.createTextNode(QString::number(item->boundingRect().bottomRight().y()*bg_width/bg_width_final));
            ymax.appendChild(text);

            bndbox.appendChild(xmin);
            bndbox.appendChild(ymin);
            bndbox.appendChild(xmax);
            bndbox.appendChild(ymax);
            object.appendChild(bndbox);
            annotation.appendChild(object);
        }
    }

    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();

    qDebug() << "write finished!";
}

void MainWindow::goIndex()
{
    scene->clear();
    int index = moveEdit->text().toInt();

    if(index >= list.count()){index = list.count()-1;}
    if(index <= 0){index = 0;}
    imageIndex = index;
    moveEdit->setText(QString::number(imageIndex));

    QPixmap pixmap(list.at(imageIndex).absoluteFilePath());
    bg_width = pixmap.width();
    bg_height = pixmap.height();

    QString str = " [ " + QString::number(imageIndex) + " ] : ";
    moveEdit->setText(QString::number(imageIndex));
    msgLabel->setText(str + list.at(imageIndex).absoluteFilePath());

    QPixmap pixmap2 = pixmap.scaled(mGraphicsView->width(),mGraphicsView->height(),Qt::KeepAspectRatio);
    scene->addPixmap(pixmap2);
    bg_width_final = pixmap2.width();
    bg_height_final = pixmap2.height();

    mGraphicsView->setMaxPoint(bg_width_final, bg_height_final);
}
