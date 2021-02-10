#pragma once


#include <QWidget>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QSplitter>
#include "ImageView.h"
#include <iostream>

class CodeEditor :public QTextEdit {
    Q_OBJECT
public:
    CodeEditor(QWidget* parent = nullptr) :QTextEdit(parent) {
    }

    void focusInEvent(QFocusEvent* e) {
        std::cout << "focus in\n";
    }
    void focusOutEvent(QFocusEvent* e) {
        std::cout << "focus out\n";
        emit focusSignal();
    }
    inline void setBuffer(const QString& buf) {
        this->buffer = buf;
    }
    inline bool isSameBuffer() {
        return buffer == this->toPlainText();
    }
    bool firstCode = true;
signals:
    void focusSignal();
private:
    QString buffer;
    
};




class MainWidget : public QWidget {
    Q_OBJECT
public:
    MainWidget(QWidget* p = nullptr);

public slots:
    void compileAndRunning();
private:
    QHBoxLayout* mainLayout;
    ImageView* view;
    ImageScene* scene;
    CodeEditor* editor;
    QSplitter* mainSpt;

    // append prescript for our ui
    void prescript();

    void outscript(const QString& filePath, const QString &text);
   
};
