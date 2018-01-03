#include "dialog.h"
#include "ui_dialog.h"
#include <QPushButton>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QWidget::setWindowTitle("Image Viewer");
    QWidget::setWindowFlags(Qt::Window | Qt::WindowTitleHint);

    QString root = QDir::rootPath();
    dirModel = new QFileSystemModel(this);
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    dirModel->setRootPath(root);

    ui->dirView->setModel(dirModel);

    for (int i = 1; i < dirModel->columnCount(); i++)
        ui->dirView->hideColumn(i);

    QModelIndex index = dirModel->index(root);
    ui->dirView->expand(index);
    ui->dirView->setCurrentIndex(index);

    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    fileModel->setNameFilters(QStringList() << "*.jpg" << "*.gif" << "*.png");
    fileModel->setRootPath(root);

    ui->fileView->setModel(fileModel);

    connect(fileModel, SIGNAL(directoryLoaded(QString)), this, SLOT(loadImagePaths(QString)));

    ui->splitter->setSizes(QList<int>() << 800 << 200);
    ui->splitter_2->setSizes(QList<int>() << 700 << 150);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setImage(QString filePath)
{
     int index = filePath.lastIndexOf("/");
     currentFile = filePath.mid(index + 1);

     QPixmap pic(filePath);
     currentImage = pic;

     int w = ui->imgLabel->width();
     int h = ui->imgLabel->height();
     ui->imgLabel->setAlignment(Qt::AlignCenter);
     ui->imgLabel->setPixmap(pic.scaled(w, h, Qt::KeepAspectRatio));
     QWidget::setWindowTitle(currentFile);
}

void Dialog::generateThumbnails(QString path)
{
    for (QString image : filePaths)
    {
        QPixmap pic(path + QDir::separator() + image);
        ButtonLabel *label = new ButtonLabel(this);
        connect(label, SIGNAL(clicked()), this, SLOT(on_thumbnail_clicked()));
        label->setFrameStyle(QFrame::WinPanel | QFrame::Raised);
        label->setLineWidth(3);
        label->setStyleSheet("QLabel {background-color:black}");
        label->setPixmap(pic.scaled(WIDTH, HEIGHT, Qt::KeepAspectRatio));
        label->setAccessibleName(path + QDir::separator() + image);
        ui->widgetLayout->addWidget(label);
        thumbnails.append(label);
    }
}


void Dialog::removeThumbnails()
{
    if (thumbnails.empty())
        return;
    for (ButtonLabel *thumbnail : thumbnails)
    {
        ui->widgetLayout->removeWidget(thumbnail);
        delete thumbnail;
    }
    thumbnails.clear();
}

void Dialog::on_thumbnail_clicked()
{
    ButtonLabel *label = (ButtonLabel*)sender();
    int width = ui->imageArea->width();
    int height = ui->imageArea->height();
    ui->imgLabel->setFixedWidth(width);
    ui->imgLabel->setFixedHeight(height);
    setImage(label->accessibleName());
}

void Dialog::enableToolButtons(bool enabled)
{
    ui->leftButton->setEnabled(enabled);
    ui->rightButton->setEnabled(enabled);
    ui->increaseButton->setEnabled(enabled);
    ui->decreaseButton->setEnabled(enabled);
    ui->fitToScreenButton->setEnabled(enabled);
    ui->trueSizeButton->setEnabled(enabled);
}

void Dialog::loadImagePaths(const QString & path)
{
    currentDirectory = path;
    filePaths.clear();
    QModelIndex parentIndex = fileModel->index(path);
    for (int row = 0; row < fileModel->rowCount(parentIndex); row++)
    {
        QModelIndex index = fileModel->index(row, 0, parentIndex);
        QString filepath = fileModel->data(index).toString();
        if (filepath.endsWith(".jpg") || filepath.endsWith(".png") || filepath.endsWith(".gif"))
            filePaths.append(filepath);
    }
    if (!filePaths.empty())
    {
        enableToolButtons(true);
        setImage(path + QDir::separator() + filePaths.at(0));
        currentFile = filePaths.at(0);
    }
    else
    {
        enableToolButtons(false);
        ui->imgLabel->setPixmap(QPixmap());
        currentFile = "";
        QWidget::setWindowTitle("Image Viewer");
    }
    removeThumbnails();
    generateThumbnails(path + QDir::separator());
}

void Dialog::on_dirView_clicked(const QModelIndex &index)
{
    QString mPath = dirModel->fileInfo(index).absoluteFilePath();
    ui->fileView->setRootIndex(fileModel->setRootPath(mPath));
}

void Dialog::on_fileView_clicked(const QModelIndex &index)
{
    QString file = fileModel->data(index).toString();
    if (!filePaths.contains(file))
        return;
    int width = ui->imageArea->width();
    int height = ui->imageArea->height();
    ui->imgLabel->setFixedWidth(width);
    ui->imgLabel->setFixedHeight(height);
    setImage(currentDirectory + QDir::separator() + file);
}

void Dialog::on_leftButton_clicked()
{
    if (filePaths.empty())
        return;
    QModelIndex parentIndex = fileModel->index(currentDirectory);
    QModelIndex index = fileModel->index(currentDirectory + QDir::separator() + currentFile);
    int row = index.row() - 1;
    if (row < 0)
        row = fileModel->rowCount(parentIndex) - 1;
    QModelIndex nextIndex = fileModel->index(row, 0, parentIndex);
    QString filepath = fileModel->data(nextIndex).toString();
    while (!filePaths.contains(filepath))
    {
        row--;
        if (row < 0)
            row = fileModel->rowCount(parentIndex) - 1;
        nextIndex = fileModel->index(row, 0, parentIndex);
        filepath = fileModel->data(nextIndex).toString();
    }
    int width = ui->imageArea->width();
    int height = ui->imageArea->height();
    ui->imgLabel->setFixedWidth(width);
    ui->imgLabel->setFixedHeight(height);
    setImage(currentDirectory + QDir::separator() + filepath);
}

void Dialog::on_rightButton_clicked()
{
    if (filePaths.empty())
        return;
    QModelIndex parentIndex = fileModel->index(currentDirectory);
    QModelIndex index = fileModel->index(currentDirectory + QDir::separator() + currentFile);
    int row = index.row() + 1;
    if (row > fileModel->rowCount(parentIndex) - 1)
        row = 0;
    QModelIndex nextIndex = fileModel->index(row, 0, parentIndex);
    QString filepath = fileModel->data(nextIndex).toString();
    while (!filePaths.contains(filepath))
    {
        row++;
        if (row > fileModel->rowCount(parentIndex) - 1)
            row = 0;
        nextIndex = fileModel->index(row, 0, parentIndex);
        filepath = fileModel->data(nextIndex).toString();
    }
    int width = ui->imageArea->width();
    int height = ui->imageArea->height();
    ui->imgLabel->setFixedWidth(width);
    ui->imgLabel->setFixedHeight(height);
    setImage(currentDirectory + QDir::separator() + filepath);
}

void Dialog::on_increaseButton_clicked()
{
    int width = ui->imgLabel->width() * 1.15;
    int height = ui->imgLabel->height() * 1.15;
    if (width > MAX_IMAGE_WIDTH)
        width = MAX_IMAGE_WIDTH;
    if (height > MAX_IMAGE_HEIGHT)
        height = MAX_IMAGE_HEIGHT;
    ui->imgLabel->setFixedWidth(width);
    ui->imgLabel->setFixedHeight(height);
    setImage(currentDirectory + QDir::separator() + currentFile);
}

void Dialog::on_decreaseButton_clicked()
{
    int width = ui->imgLabel->width() / 1.15;
    int height = ui->imgLabel->height() / 1.15;
    if (width < MIN_IMAGE_WIDTH)
        width = MIN_IMAGE_WIDTH;
    if (height < MIN_IMAGE_HEIGHT)
        height = MIN_IMAGE_HEIGHT;
    ui->imgLabel->setFixedWidth(width);
    ui->imgLabel->setFixedHeight(height);
    setImage(currentDirectory + QDir::separator() + currentFile);
}

void Dialog::on_fitToScreenButton_clicked()
{
    int width = ui->imageArea->width();
    int height = ui->imageArea->height();
    ui->imgLabel->setFixedWidth(width);
    ui->imgLabel->setFixedHeight(height);
    setImage(currentDirectory + QDir::separator() + currentFile);
}

void Dialog::on_trueSizeButton_clicked()
{
    int width = currentImage.width();
    int height = currentImage.height();
    ui->imgLabel->setFixedWidth(width);
    ui->imgLabel->setFixedHeight(height);
    setImage(currentDirectory + QDir::separator() + currentFile);
}
