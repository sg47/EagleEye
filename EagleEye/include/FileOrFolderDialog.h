#pragma once

#include <QFileDialog>
#include <qtreeview.h>
class FileDialog : public QFileDialog
{
    Q_OBJECT
public:
    explicit FileDialog(QWidget *parent = Q_NULLPTR, QString title = QString(), QString dir = QString());

    QStringList selected() const;

    public slots:
    void openClicked();

private:
    QTreeView *treeView;
    QPushButton *openButton;
    QStringList selectedFilePaths;
};