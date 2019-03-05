#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent=0): QLabel(parent) {}


void MyLabel::alarma_off() {
    alarm = false;
    setText("Alarma desactivada");
    setStyleSheet("background-color: rgb(255, 0, 0); color: rgb(255, 255, 255);");
}

void MyLabel::alarma_on() {
    alarm = true;
    char buffer[100];
    QString str;
    setStyleSheet("background-color: rgb(0, 255, 0); color: rgb(255, 255, 255);");
    if (hour < 9 and minute < 9) sprintf(buffer,"Alarm -- 0%d:0%d",hour,minute);
    else if (hour < 9) sprintf(buffer,"Alarm -- 0%d:%d",hour,minute);
    else if (minute < 9) sprintf(buffer,"Alarm -- %d:0%d",hour,minute);
    else sprintf(buffer,"Alarm -- %d:%d",hour,minute);
    str = buffer;
    setText(str);
}

void MyLabel::setHour(int h) {
    hour = h;
    if (alarm) alarma_on();
}

void MyLabel::setMinute(int m) {
    minute = m;
    if (alarm) alarma_on();
}

void MyLabel::tractaReset() {
    hour = minute = 0;
    emit reset_all(0);
}
