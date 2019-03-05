#include <QLabel>
//#include <stdlib.h>
class MyLabel: public QLabel

{
    Q_OBJECT
public:
    MyLabel(QWidget *parent);
    
    int hour = 0;
    int minute = 0;
    bool alarm = false;
public slots:
    void setHour(int);
    void setMinute(int);
    void alarma_off();
    void alarma_on();
    void tractaReset();
signals:
    void reset_all(int);
};
