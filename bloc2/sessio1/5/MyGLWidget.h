#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "./Model/model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);
    
  private:
    void initCamera ();
    void createBuffers ();
    void carregaShaders ();
    void projectTransform();
    void viewTransform();
    void modelTransform();
    void modelTransform_terra();
    
    Model m;
    
    GLuint transLoc;
    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint projLoc, viewLoc;
    // VAO i VBO names
    GLuint VAO_Homer;
    GLuint VAO_Terra;
    // Program
    QOpenGLShaderProgram *program;
    // Internal vars
    float scale = 1.0;
    float angle = 0.0;
    
    float d = 5.;
    float radi;
    glm::vec3 Pmin, Pmax;
    
    glm::vec3 VRP;
    glm::vec3 OBS;
    glm::vec3 up;
    
    glm::vec3 pos;
};
