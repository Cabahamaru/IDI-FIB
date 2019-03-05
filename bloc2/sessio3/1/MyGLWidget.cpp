#include "MyGLWidget.h"

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
  glBindVertexArray (VAO_patricio);
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
  glm::mat4 TG (1.0f);
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  TG = glm::translate(TG, -centreBasePatr);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransform_terra () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
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
    case Qt::Key_Z: { // zoom-in, decrementar angle FOV
        FOV -= 0.1;
        projectTransform();
      break;
    }
    case Qt::Key_X: { // zoom-out, incrementar angle FOV
        FOV += 0.1;
        projectTransform();
      break;
    }

    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::createBuffers () 
{
  // Creació del Vertex Array Object per pintar
  m.load("./Patricio.obj");
  calculaCapsaModel();
  glGenVertexArrays(1, &VAO_patricio);
  glBindVertexArray(VAO_patricio);
  
  GLuint VBO_patricio[2];
  glGenBuffers(2, VBO_patricio);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO_patricio[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_vertices(), GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO_patricio[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matdiff(), GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  glm::vec3 posterra[6] = {
	glm::vec3(-2.5, 0.0, 2.5),
	glm::vec3(2.5, 0.0, 2.5),
	glm::vec3(-2.5, 0.0, -2.5),
	glm::vec3(-2.5, 0.0, -2.5),
	glm::vec3(2.5, 0.0, 2.5),
	glm::vec3(2.5, 0.0, -2.5),
  }; 
  
  glm::vec3 color(0.8,0.0,0.8);
  
  glm::vec3 colterra[6] = {
	color, color, color, color, color, color
  };
  
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);
  
  GLuint VBO_Terra[2];
  glGenBuffers(2, VBO_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colterra), colterra, GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  glBindVertexArray (0);
}

void MyGLWidget::calculaCapsaModel ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = m.vertices()[0];
  miny = maxy = m.vertices()[1];
  minz = maxz = m.vertices()[2];
  for (unsigned int i = 3; i < m.vertices().size(); i+=3)
  {
    if (m.vertices()[i+0] < minx)
      minx = m.vertices()[i+0];
    if (m.vertices()[i+0] > maxx)
      maxx = m.vertices()[i+0];
    if (m.vertices()[i+1] < miny)
      miny = m.vertices()[i+1];
    if (m.vertices()[i+1] > maxy)
      maxy = m.vertices()[i+1];
    if (m.vertices()[i+2] < minz)
      minz = m.vertices()[i+2];
    if (m.vertices()[i+2] > maxz)
      maxz = m.vertices()[i+2];
  }
  escala = 4.0/(maxy-miny);
  centreBasePatr[0] = (minx+maxx)/2.0; centreBasePatr[1] = miny; centreBasePatr[2] = (minz+maxz)/2.0;
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
