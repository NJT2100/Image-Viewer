#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>
#include "buttonlabel.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void enableToolButtons(bool enabled);
    void generateThumbnails(QString path);
    void removeThumbnails();
    void setImage(QString filePath);

    const int HEIGHT = 115;
    const int WIDTH = 125;
    const int MAX_IMAGE_WIDTH = 3000;
    const int MAX_IMAGE_HEIGHT = 3000;
    const int MIN_IMAGE_WIDTH = 30;
    const int MIN_IMAGE_HEIGHT = 30;

public slots:
        void on_thumbnail_clicked();

private slots:
    void on_dirView_clicked(const QModelIndex &index);
    void loadImagePaths(const QString & path);
    void on_fileView_clicked(const QModelIndex &index);

    void on_leftButton_clicked();

    void on_rightButton_clicked();

    void on_increaseButton_clicked();

    void on_decreaseButton_clicked();

    void on_fitToScreenButton_clicked();

    void on_trueSizeButton_clicked();

private:
    Ui::Dialog *ui;
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;
    QList<ButtonLabel*> thumbnails;
    QStringList filePaths;
    QString currentDirectory;
    QString currentFile;
    QPixmap currentImage;
};

#endif // DIALOG_H
