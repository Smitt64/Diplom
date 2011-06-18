#include "dockpanel.h"

DockPanel::DockPanel(QWidget *parent, Qt::WindowFlags flags) :
    QDockWidget(parent)
{
}

void DockPanel::setPreferedWidth(const int iWidth)
{
    m_iPreferedWidth = iWidth;

}
int DockPanel::preferedWidth(void)
{
    return m_iPreferedWidth;

}
void DockPanel::setPreferedHeight(const int iHieght)
{
    m_PreferedHeight = iHieght;

}
int DockPanel::preferedHeight(void)
{
    return m_PreferedHeight;
}
QSize DockPanel::sizeHint(void)
{
    QSize sz  = QDockWidget::sizeHint();

        sz.setHeight(m_PreferedHeight);
    sz.setWidth(m_iPreferedWidth);

    return sz;
}
