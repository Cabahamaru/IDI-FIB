TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /usr/include/glm

INCLUDEPATH += ./Model

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h

SOURCES += ./Model/model.cpp

HEADERS += ./Model/model.h

SOURCES += main.cpp MyForm.cpp \
        MyGLWidget.cpp 
