#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent=0): QLabel(parent) {}

void MyLabel::truncarText(int x)
{
    QString s = mytext;
    s.truncate(x);
    setText(s);
}

void MyLabel::guardarText(QString s) 
{
    mytext = s;
    setText(mytext);
}
