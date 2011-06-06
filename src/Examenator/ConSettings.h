#ifndef ConSettings_H
#define ConSettings_H

#include <QDialog>

namespace Ui {
    class ConSettings;
}

class ConSettings : public QDialog
{
    Q_OBJECT

public:
    explicit ConSettings(QWidget *parent = 0);
    ~ConSettings();

    QString getServer();
    int getPort();

private:
    Ui::ConSettings *ui;
};

#endif // ConSettings_H
