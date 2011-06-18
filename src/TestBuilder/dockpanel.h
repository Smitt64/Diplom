#ifndef DOCKPANEL_H
#define DOCKPANEL_H

#include <QDockWidget>

class DockPanel : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockPanel(QWidget *parent = 0, Qt::WindowFlags flags = 0);

    void setPreferedWidth(int width);
    int preferedWidth(void);
    void setPreferedHeight(int hieght);
    int preferedHeight(void);
    QSize sizeHint(void);
signals:

public slots:

private:
    int m_iPreferedWidth; //Желаемая ширина
    int m_PreferedHeight; //Желаемая высота
};

#endif // DOCKPANEL_H
