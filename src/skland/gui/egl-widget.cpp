//
// Created by zhanggyb on 16-12-27.
//

#include <skland/gui/egl-widget.hpp>

#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>
#include <skland/gui/egl-surface.hpp>
#include <skland/gui/display.hpp>
#include "internal/redraw-task.hpp"

#include <stdlib.h>
#include <assert.h>

namespace skland {

static const char *vert_shader_text =
    "uniform mat4 rotation;\n"
        "attribute vec4 pos;\n"
        "attribute vec4 color;\n"
        "varying vec4 v_color;\n"
        "void main() {\n"
        "  gl_Position = rotation * pos;\n"
        "  v_color = color;\n"
        "}\n";

static const char *frag_shader_text =
    "precision mediump float;\n"
        "varying vec4 v_color;\n"
        "void main() {\n"
        "  gl_FragColor = v_color;\n"
        "}\n";

static GLuint
create_shader(const char *source, GLenum shader_type) {
  GLuint shader;
  GLint status;

  shader = glCreateShader(shader_type);
  assert(shader != 0);

  glShaderSource(shader, 1, (const char **) &source, NULL);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status) {
    char log[1000];
    GLsizei len;
    glGetShaderInfoLog(shader, 1000, &len, log);
    fprintf(stderr, "Error: compiling %s: %*s\n",
            shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment",
            len, log);
    exit(1);
  }

  return shader;
}

EGLWidget::EGLWidget()
    : EGLWidget(400, 300) {

}

EGLWidget::EGLWidget(int width, int height)
    : AbstractWidget(width, height) {
  EGLSurface *surf = new EGLSurface;
  SetSurface(surf);

  InitializeGL();
}

EGLWidget::~EGLWidget() {

}

void EGLWidget::OnShow() {
  if (!redraw_task()->IsLinked()) {
    AddRedrawTask(redraw_task().get());
    surface()->Commit();
  }
}

void EGLWidget::OnResize(int width, int height) {
  resize(width, height);
}

void EGLWidget::OnMouseEnter(MouseEvent *event) {
  fprintf(stderr, "mouse enter\n");
  event->Accept();
}

void EGLWidget::OnMouseLeave(MouseEvent *event) {
  fprintf(stderr, "mouse leave\n");
  event->Accept();
}

void EGLWidget::OnMouseMove(MouseEvent *event) {
  event->Accept();
}

void EGLWidget::OnMouseButton(MouseEvent *event) {
  event->Accept();
}

void EGLWidget::OnKeyboardKey(KeyEvent *event) {

}

void EGLWidget::OnDraw(Canvas *canvas) {
  fprintf(stderr, "on draw\n");
}

void EGLWidget::InitializeGL() {
  GLuint frag, vert;
  GLuint program;
  GLint status;

  frag = create_shader(frag_shader_text, GL_FRAGMENT_SHADER);
  vert = create_shader(vert_shader_text, GL_VERTEX_SHADER);

  program = glCreateProgram();
  glAttachShader(program, frag);
  glAttachShader(program, vert);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status) {
    char log[1000];
    GLsizei len;
    glGetProgramInfoLog(program, 1000, &len, log);
    fprintf(stderr, "Error: linking:\n%*s\n", len, log);
    exit(1);
  }

  glUseProgram(program);

  pos = 0;
  col = 1;

  glBindAttribLocation(program, pos, "pos");
  glBindAttribLocation(program, col, "color");
  glLinkProgram(program);

  rotation_uniform = glGetUniformLocation(program, "rotation");
}

}