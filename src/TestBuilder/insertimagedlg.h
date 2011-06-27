#ifndef INSERTIMAGEDLG_H
#define INSERTIMAGEDLG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include "../filesystem/filesystem.h"

class InsertImageDlg : public QDialog
{
    Q_OBJECT
public:
    explicit InsertImageDlg(FSHANDLE *handle, QWidget *parent = 0);
    QString resourceName();

signals:

private slots:
    void addImage();
    void onListDoubleClick(QListWidgetItem */*item*/);

private:
    QDialogButtonBox *buttons;
    QBoxLayout *layout;
    QListWidget *list;
    QPushButton *addButton;
    FSHANDLE *handle;

};

#endif // INSERTIMAGEDLG_H
