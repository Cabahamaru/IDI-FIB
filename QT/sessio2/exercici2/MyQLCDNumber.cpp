 #include "MyQLCDNumber.h"

MyQLCDNumber::MyQLCDNumber(QWidget *parent=0): QLCDNumber(parent) {}

void MyQLCDNumber::display_colors(int x) {
    if (x == 0) setStyleSheet("color:rgb(0, 255, 0)");
    else if (x%2) setStyleSheet("color:rgb(255, 0, 0)");
    else setStyleSheet("color:rgb(0, 0, 255)");
    display(x);
}

void MyQLCDNumber::setZero() {
    setStyleSheet("color:rgb(0, 255, 0)");
    display(0);
}

//setStyleSheet("color:rgb(0, 255, 0)");
