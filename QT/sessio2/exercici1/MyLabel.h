#include <QLabel>
class MyLabel: public QLabel

{
    Q_OBJECT
public:
    MyLabel(QWidget *parent);
    
    QString mytext;
    
public slots:
    void truncarText(int);
    void guardarText(QString);
};
