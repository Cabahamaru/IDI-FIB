#include "MyGLWidget.h"
#include "glm/gtc/matrix_transform.hpp"
#include <QKeyEvent>
#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
}

void MyGLWidget::paintGL ()
{
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Activem l'Array a pintar 
  glBindVertexArray(VAO1);
 
  modelTransform();
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 3);
  
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  glViewport (0, 0, w, h);
}

void MyGLWidget::createBuffers ()
{
  glm::vec3 Vertices[3];  // Tres vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-1.0, -1.0, 0.0);
  Vertices[1] = glm::vec3(1.0, -1.0, 0.0);
  Vertices[2] = glm::vec3(0.0, 1.0, 0.0);
  
  glm::vec3 color[3];
  color[0] = glm::vec3(1.,0.,0.);
  color[1] = glm::vec3(0.,1.,0.);
  color[2] = glm::vec3(0.,0.,1.);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  GLuint VBO2;
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
  
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

float scl1 = 0.0;
float scl2 = 0.0;
float angle = 0.0;

void MyGLWidget::modelTransform() {
    glm::mat4 TG(1.0);
    TG = glm::rotate (TG,angle,glm::vec3(0.,0.,1.));
    TG = glm::translate (TG,glm::vec3(scl1,scl2,0.0));
    glUniformMatrix4fv(transLoc,1,GL_FALSE,&TG[0][0]);
}

void MyGLWidget::keyPressEvent (QKeyEvent *e) {
    makeCurrent();
    switch (e->key()) {
        case Qt::Key_Left :
            scl1 -= 0.5;
            angle += (float)M_PI/4;
            modelTransform();
        break;
        
        case Qt::Key_Right :
            scl1 += 0.5;
            angle += (float)M_PI/4;
            modelTransform();
        break;
        
        case Qt::Key_Up :
            scl2 += 0.5;
            angle += (float)M_PI/4;
            modelTransform();
        break;
        
        case Qt::Key_Down :
            scl2 -= 0.5;
            angle += (float)M_PI/4;
            modelTransform();
        break;
        default: e->ignore();
    }
    update();
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/fragshad.frag");
  vs.compileSourceFile("shaders/vertshad.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  transLoc = glGetUniformLocation (program->programId(), "TG");
  colorLoc=glGetAttribLocation(program->programId(),"color");
}
