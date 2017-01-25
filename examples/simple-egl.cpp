/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <skland/gui/application.hpp>
#include <skland/gui/egl-window.hpp>

#include <GLES2/gl2.h>
#include <sys/time.h>
#include <assert.h>

using namespace skland;

class SimpleWindow : public EGLWindow {
 public:

  SimpleWindow()
      : EGLWindow(250, 250) {}

  virtual  ~SimpleWindow() {
    glDeleteProgram(program);
  }

 protected:

  virtual void OnInitializeEGL() override;

  virtual void OnResizeEGL(int width, int height) override;

  virtual void OnRenderEGL() override;

 private:
  GLuint program;
  GLint attribute_coord2d;
};

void SimpleWindow::OnInitializeEGL() {
  GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);

  const char *vs_source =
#ifdef GL_ES_VERSION_2_0
      "#version 100\n"  // OpenGL ES 2.0
#else
          "#version 120\n"  // OpenGL 2.1
#endif
          "attribute vec2 coord2d;                  "
          "void main(void) {                        "
          "  gl_Position = vec4(coord2d, 0.0, 1.0); "
          "}";
  glShaderSource(vs, 1, &vs_source, NULL);
  glCompileShader(vs);
  glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
  if (!compile_ok) {
    fprintf(stderr, "Error in vertex shader\n");
    exit(1);
  }

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fs_source =
#ifdef GL_ES_VERSION_2_0
      "#version 100\n"  // OpenGL ES 2.0
#else
          "#version 120\n"  // OpenGL 2.1
#endif
          "void main(void) {        "
          "  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); "
          "}";
  glShaderSource(fs, 1, &fs_source, NULL);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
  if (!compile_ok) {
    fprintf(stderr, "Error in fragment shader\n");
    exit(1);
  }
  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    fprintf(stderr, "glLinkProgram:");
    exit(1);
  }
  const char *attribute_name = "coord2d";
  attribute_coord2d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord2d == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    exit(1);
  }
}

void SimpleWindow::OnResizeEGL(int width, int height) {

}

void SimpleWindow::OnRenderEGL() {
  glClearColor(0.0, 0.0, 0.0, 0.5);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(program);
  glEnableVertexAttribArray(attribute_coord2d);
  GLfloat triangle_vertices[] = {
      0.0f, 0.5f,
      -0.6f, -0.5f,
      0.6f, -0.5f
  };

  /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
  glVertexAttribPointer(
      attribute_coord2d, // attribute
      2,                 // number of elements per vertex, here (x,y)
      GL_FLOAT,          // the type of each element
      GL_FALSE,          // take our values as-is
      0,                 // no extra data between each position
      triangle_vertices  // pointer to the C array
  );

  /* Push each element in buffer_vertices to the vertex shader */
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray(attribute_coord2d);
}

int main(int argc, char *argv[]) {
  Application app(argc, argv);

//  Window *win = new Window(480, 360, "Simple EGL");
//  win->SetAppId("Simple-EGL");
//
//  EGLWidget *widget = new EGLWidget;
//  win->SetMainWidget(widget);

  SimpleWindow *win = new SimpleWindow();
  win->Show();

  return app.Run();
}
