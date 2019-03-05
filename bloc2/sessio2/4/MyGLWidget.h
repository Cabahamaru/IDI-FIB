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
    void createBuffers ();
    void carregaShaders ();
    void projectTransform();
    void viewTransform();
    void modelTransform();
    void modelTransform_terra();
    void initCamera();
    void calculaCapsaModel();
    Model m;
    
    GLuint transLoc;
    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint projLoc, viewLoc;
    // VAO i VBO names
    GLuint VAO_patricio;
    GLuint VAO_Terra;
    // Program
    QOpenGLShaderProgram *program;
    // Internal vars
    glm::vec3 Pmin = glm::vec3(-2.5,0,-2.5);
    glm::vec3 Pmax = glm::vec3(2.5,4,2.5);
    float radi = (glm::distance(Pmin,Pmax))/2.0f;
    glm::vec3 VRP, OBS, UP;
    float ra, zNear, zFar, FOV;
    float angleinit;
    glm::vec3 pos;
    glm::vec3 centreBasePatr;
    float escala;
};

