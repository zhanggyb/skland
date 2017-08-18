//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/gui/application.hpp>
#include <skland/gui/window.hpp>
#include <skland/gui/egl-widget.hpp>


#include <GLES2/gl2.h>
#include <sys/time.h>

using namespace skland;
using namespace skland::gui;

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

class Triangle: public EGLWidget {

 public:

  Triangle()
      : EGLWidget() {
  }

  virtual ~Triangle() {
    glDeleteProgram(program_);
  }

 protected:

  virtual void OnInitialize() override;

  virtual void OnResize(int width, int height) override;

  virtual void OnRender() override;

 private:

  GLuint program_;

  GLuint rotation_uniform_;
  GLuint pos_;
  GLuint col_;

};


void Triangle::OnInitialize() {
  MakeCurrent();

  GLuint frag, vert;
  GLint status;

  frag = create_shader(frag_shader_text, GL_FRAGMENT_SHADER);
  vert = create_shader(vert_shader_text, GL_VERTEX_SHADER);

  program_ = glCreateProgram();
  glAttachShader(program_, frag);
  glAttachShader(program_, vert);
  glLinkProgram(program_);

  glGetProgramiv(program_, GL_LINK_STATUS, &status);
  if (!status) {
    char log[1000];
    GLsizei len;
    glGetProgramInfoLog(program_, 1000, &len, log);
    fprintf(stderr, "Error: linking:\n%*s\n", len, log);
    exit(1);
  }

  glUseProgram(program_);

  pos_ = 0;
  col_ = 1;

  glBindAttribLocation(program_, pos_, "pos");
  glBindAttribLocation(program_, col_, "color");
  glLinkProgram(program_);

  rotation_uniform_ =
      glGetUniformLocation(program_, "rotation");

  SwapBuffers();
}

void Triangle::OnResize(int width, int height) {

}

void Triangle::OnRender() {
  MakeCurrent();

  static const GLfloat verts[3][2] = {
      {-0.5, -0.5},
      {0.5, -0.5},
      {0, 0.5}
  };
  static const GLfloat colors[3][3] = {
      {1, 0, 0},
      {0, 1, 0},
      {0, 0, 1}
  };
  GLfloat angle;
  GLfloat rotation[4][4] = {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
  };
  static const uint32_t speed_div = 5;
  struct timeval tv;

  gettimeofday(&tv, NULL);
  uint32_t time = tv.tv_sec * 1000 + tv.tv_usec / 1000;

  angle = (time / speed_div) % 360 * M_PI / 180.0;
  rotation[0][0] = cos(angle);
  rotation[0][2] = sin(angle);
  rotation[2][0] = -sin(angle);
  rotation[2][2] = cos(angle);

  glViewport(0, 0, GetWidth(), GetHeight());

  glUniformMatrix4fv(rotation_uniform_, 1, GL_FALSE,
                     (GLfloat *) rotation);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glVertexAttribPointer(pos_, 2, GL_FLOAT, GL_FALSE, 0, verts);
  glVertexAttribPointer(col_, 3, GL_FLOAT, GL_FALSE, 0, colors);
  glEnableVertexAttribArray(pos_);
  glEnableVertexAttribArray(col_);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(pos_);
  glDisableVertexAttribArray(col_);

  SwapBuffers();
}

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

/*
 *
 */
TEST_F(Test, slider_1) {
  int argc = 1;
  char argv1[] = "gui-slider";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  Window *win = new Window(480, 360, "Test EGL Widget");
  win->SetAppId("EGLWidget");

  Triangle *widget = new Triangle;
  win->SetContentView(widget);

  win->Show();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}
