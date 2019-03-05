#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  scale = 1.0f;
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
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
  initCamera();
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Carreguem la transformació de model
  modelTransform();
  glBindVertexArray (VAO_Homer);
  // Activem el VAO per a pintar la caseta 

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
  
  modelTransform_terra();
  glBindVertexArray (VAO_Terra);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray (0);
}

void MyGLWidget::resizeGL (int w, int h) 
{
  glViewport(0, 0, w, h);
  //redimensioanr sense deformar ni retallar
  float raV = float(w)/float(h);
  ra = raV;
  if (raV < 1.0) FOV = 2.0f*atan(tan(angleinit)/raV);
  projectTransform();
}


void MyGLWidget::initCamera() {
    ra = 1.0;
    float d = 2.0f*radi;
    VRP = (Pmin+Pmax)/2.0f;
    OBS = VRP + d*(glm::vec3(0,0,1));
    UP = glm::vec3(0,1,0);
    zNear = d-radi;
    zFar = d+radi;
    FOV = 2.0f*asin(radi/d);
    angleinit = asin(radi/d);
    projectTransform();
    viewTransform();
}

void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::rotate (transform,angle,glm::vec3(0.,1.,0.));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransform_terra () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::viewTransform()
{
    //glm::lookAt(OBS, VRP, up)
    //glm::mat4 View=glm::lookAt(glm::vec3(0,0,1),glm::vec3(0,0,0),glm::vec3(0,1,0));
    glm::mat4 View = glm::lookAt(OBS,VRP,UP);
    glUniformMatrix4fv(viewLoc,1,GL_FALSE,&View[0][0]);
}

void MyGLWidget::projectTransform() 
{
    //glm::perspective(FOV en radians, ra window, znear, zfar)
    //glm::mat4 Proj=glm::perspective((float)M_PI/2.0f, 1.0f, 0.4f, 3.0f);
    glm::mat4 Proj = glm::perspective(FOV,ra,zNear,zFar);
    glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      break;
    }
    case Qt::Key_R: {
      angle += (float)M_PI/6;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::createBuffers () 
{
  // Creació del Vertex Array Object per pintar
  m.load("./HomerProves.obj");
  
  glGenVertexArrays(1, &VAO_Homer);
  glBindVertexArray(VAO_Homer);
  
  GLuint VBO_Homer[2];
  glGenBuffers(2, VBO_Homer);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_vertices(), GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matdiff(), GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  glm::vec3 posicio[6] = {
      glm::vec3(-1.,-1.,-1.),
      glm::vec3(1.,-1.,-1),
      glm::vec3(-1.,-1.,1.),
      glm::vec3(-1.,-1.,1.),
      glm::vec3(1.,-1.,1.),
      glm::vec3(1.,-1.,-1)
  };
  
    glm::vec3 color[6] = {
      glm::vec3(1.,0.,0.),
      glm::vec3(1.,0.,0.),
      glm::vec3(1.,0.,0.),
      glm::vec3(1.,0.,0.),
      glm::vec3(1.,0.,0.),
      glm::vec3(1.,0.,0.)
  };
  
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);
  
  GLuint VBO_Terra[2];
  glGenBuffers(2, VBO_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posicio), posicio, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  glBindVertexArray (0);
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
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  
  transLoc = glGetUniformLocation(program->programId(), "TG");
  
  projLoc = glGetUniformLocation(program->programId(), "proj");
  
  viewLoc = glGetUniformLocation(program->programId(), "view");
}
