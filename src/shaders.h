//LIC

#ifndef SHADERS_H
#define SHADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <GL/glew.h>
#include <GL/glu.h>
#include <iostream>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "vertex.h"
#include "map_list.h"

//#define GL_GLEXT_PROTOTYPES

//HEA

class _shaders
{
public:
  GLuint load_shaders(QString File_vertex_shader, QString File_fragment_shader);
  GLuint load_shaders(const GLchar **Vertex_shader_source,const GLchar **Fragment_shader_source);

protected:
  bool read_file(QString File_name,QString &Code);
  void show_error(GLint Shader);
};
#endif
