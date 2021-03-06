#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QDialog>
#include <qmainwindow.h>
#include <qtextformat.h>
#include <qtextdocument.h>
#include <qtextlist.h>
#include "texteditwidget.h"
#include "../questpanel/questpanel.h"

namespace Ui {
    class TextEditor;
}

class TextEditView : public QMainWindow
{
    Q_OBJECT
public:
    TextEditView(QuestPanel *pan, QWidget *parent = 0);
   // FSHANDLE

private slots:
    void updateColor();
    void cursorPositionChanged();
    void currentCharFormatChanged(QTextCharFormat format);

private:
    void mergeFormatOnWordOrSelection(QTextCharFormat format);
    void alignmentChanged(Qt::Alignment a);
    void fontChanged(QFont f);

    QuestPanel *panel;
    QComboBox *fontSizeBox;
    QFontComboBox *fontBox;
    QToolBar *formating;

    QAction *actionBold, *actionItalic;
    QAction *actionFontColor;

    TextEditWidget *textEdit;
};

class TextEditor : public QDialog {
    Q_OBJECT
public:
    TextEditor(QuestPanel *pan, QWidget *parent = 0);
    ~TextEditor();

    QString getPlainText();
    QString getHTML();
    void setHtml(QString html);
    QByteArray getHTMLData();

    void setDocument(QTextDocument *document);
    //void loadResources();

protected:
    QuestPanel *panel;
    //void mergeFormatOnWordOrSelection(QTextCharFormat format);
    void changeEvent(QEvent *e);

    //void alignmentChanged(Qt::Alignment a);
    //void fontChanged(QFont f);

    TextEditWidget *textEdit;

private:
    Ui::TextEditor *ui;

protected slots:
    void textBold();
    void textItalic();
    void textFamily(QString f);
    void textSize(QString p);
    void textColor();

    void textLeft();
    void textCenter();
    void textRight();

    void insertImage();

    //void cursorPositionChanged();
    //void currentCharFormatChanged(QTextCharFormat format);
    void textStyle(int styleIndex);

    //void fontChanged(const QFont &f);
};

#endif // TEXTEDITOR_H
