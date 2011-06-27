#include "insertimagedlg.h"
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>

InsertImageDlg::InsertImageDlg(FSHANDLE *arch, QWidget *parent) :
    QDialog(parent),
    handle(arch)
{
    layout = new QVBoxLayout;
    setLayout(layout);

    buttons = new QDialogButtonBox;

    buttons->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    addButton = buttons->addButton("Добавить", QDialogButtonBox::ActionRole);

    list = new QListWidget;
    //list->setViewMode(QListView::IconMode);
    list->setIconSize(QSize(64, 64));

    layout->addWidget(list);
    layout->addWidget(buttons);

    setWindowTitle("Выбор изображения...");

    setProperty("size", QSize(240, 320));

    //Составление списка изображений
    FileSystem::getInst()->fsOpen(handle);
    QStringList files = FileSystem::getInst()->fsGetListFiles("resources\\", handle);
    for(int i = 0; i < files.count(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(files[i]);
        QPixmap map;
        map.loadFromData(FileSystem::getInst()->fsGetFile("resources\\" + files[i], handle));
        item->setIcon(map);
        list->addItem(item);
    }
    FileSystem::getInst()->fsClose(handle);

    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addImage()));
    connect(list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onListDoubleClick(QListWidgetItem*)));
}

QString InsertImageDlg::resourceName()
{
    QListWidgetItem *item = NULL;
    item = list->currentItem();

    if(item)
    {
        return QString("resources\\" + list->currentItem()->text());
    }
}

void InsertImageDlg::addImage()
{
    QStringList file = QFileDialog::getOpenFileNames(this, "Открыть изображение...",
                                                 "/", "Images (*.png *.xpm *.jpg)");
    FileSystem::getInst()->fsOpen(handle);
    for(int i = 0; i < file.count(); i++)
    {
        QString str = file.at(i);
        if(FileSystem::getInst()->fsAddTempFile(str,
                                            "resources\\"  + QFileInfo(str).fileName(),
                                            handle))
        {
            QListWidgetItem *item = new QListWidgetItem;
            item->setText(QFileInfo(str).fileName());
            QPixmap map;
            map.loadFromData(FileSystem::getInst()->fsGetFile("resources\\" + item->text(), handle));
            item->setIcon(map);
            list->addItem(item);
            list->setCurrentItem(item);
        }
    }
    FileSystem::getInst()->fsClose(handle);
}

void InsertImageDlg::onListDoubleClick(QListWidgetItem */*item*/)
{
    accept();
}
