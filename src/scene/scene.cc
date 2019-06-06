#include "scene.hh"

GLuint vao_cpy = 0;
GLuint program_cpy = 0;
GLuint program_compute_cpy = 0;
GLuint textid_cpy = 0;

static const std::vector<GLfloat> vertices_position = {
            -1.0f, -0.5f, -0.5f,
        };


#define TEST_OPENGL_ERROR()                                                             \
  do {									\
    GLenum err = glGetError();					                        \
    if (err != GL_NO_ERROR) std::cerr << "OpenGL ERROR!" << __LINE__ << std::endl;      \
  } while(0)

Scene::Scene(int width, int height) : width_(width), height_(height)
{}

void check_error() {
    if (glGetError() != GL_NO_ERROR)
        std::cerr << "ERROR:" << __LINE__ << std::endl;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vao_cpy);TEST_OPENGL_ERROR();
    glDrawArrays(GL_POINTS, 0, vertices_position.size());TEST_OPENGL_ERROR();
    glBindVertexArray(0);TEST_OPENGL_ERROR();
    glutSwapBuffers();
}

bool init_glew() {
    return (glewInit() == GLEW_OK);
}

bool
Scene::init(int argc, char *argv[]) {
    /* Init all of size*/
    glutInit(&argc, argv);
    glutInitContextVersion(4,5);
    glutInitContextProfile(GLUT_CORE_PROFILE|GLUT_DEBUG); TEST_OPENGL_ERROR();
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH); TEST_OPENGL_ERROR();
    glutInitWindowSize(width_, height_);TEST_OPENGL_ERROR();
    glutInitWindowPosition (100, 100);
    window_ = glutCreateWindow("Fire in the hole");
    glEnable(GL_DEPTH_TEST);TEST_OPENGL_ERROR();
    glEnable(GL_PROGRAM_POINT_SIZE);TEST_OPENGL_ERROR();
    glutDisplayFunc(display);

    if (init_glew() == false) {
        std::cerr << "ERROR: failed to initialize glew" << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);TEST_OPENGL_ERROR();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);TEST_OPENGL_ERROR();
    glEnable(GL_CULL_FACE);TEST_OPENGL_ERROR();
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glViewport(0, 0, 512, 512);

    return true;
}

std::string load_file(std::string path_file) {
    std::ifstream file;
    std::string buffer;
    file.open(path_file);
    if (file.is_open() == false) {
        std::cerr << "ERROR while opening file " << path_file << std::endl;
        return buffer;
    }
    std::string line;
    while(getline(file, line)) {
      buffer += line + '\n';
    }
    buffer += '\0';
    file.close();
    return buffer;
}

GLuint
Scene::shader(std::string vertex_shader_src, std::string fragment_shader_src, std::string compute_shader_src) {
    std::vector<GLuint> shader_list;
    std::string content;
    const char* content_char;

    auto vertex_shader = glCreateShader(GL_VERTEX_SHADER); TEST_OPENGL_ERROR();
    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER); TEST_OPENGL_ERROR();
    auto compute_shader = glCreateShader(GL_COMPUTE_SHADER); TEST_OPENGL_ERROR();

    content = load_file(vertex_shader_src);
    content_char = content.c_str();
    glShaderSource(vertex_shader, 1, &content_char, 0); TEST_OPENGL_ERROR();

    content = load_file(fragment_shader_src);
    content_char = content.c_str();
    glShaderSource(fragment_shader, 1, &content_char, 0);TEST_OPENGL_ERROR();

    content = load_file(compute_shader_src);
    content_char = content.c_str();
    glShaderSource(compute_shader, 1, &content_char, 0);TEST_OPENGL_ERROR();

    auto program = glCreateProgram();TEST_OPENGL_ERROR();

    GLint compile_status = GL_TRUE;
    glCompileShader(vertex_shader);TEST_OPENGL_ERROR();
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);TEST_OPENGL_ERROR();
    if(compile_status != GL_TRUE) {
      GLint log_size;
      char *shader_log;
      glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_size);
      shader_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
      if(shader_log == NULL) {
             return false;
      }
      glGetShaderInfoLog(vertex_shader, log_size, &log_size, shader_log);
      std::cerr << shader_log << std::endl;
      return false;
    }
    glAttachShader(program, vertex_shader);

    glCompileShader(fragment_shader);TEST_OPENGL_ERROR();
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);TEST_OPENGL_ERROR();
    if(compile_status != GL_TRUE) {
      GLint log_size;
      char *shader_log;
      glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_size);
      shader_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
      if(shader_log == NULL) {
             return false;
      }
      glGetShaderInfoLog(fragment_shader, log_size, &log_size, shader_log);
      std::cerr << shader_log << std::endl;
      return false;
}
    glAttachShader(program, fragment_shader);

    // glBindFragDataLocation(program, 0, "color");
    GLint link_status=GL_TRUE;
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (compile_status != GL_TRUE) {
        std::cerr << "ERROR: link" << std::endl;
        return -1;
    }

    auto program_compute = glCreateProgram(); TEST_OPENGL_ERROR();
    if (program_compute == 0)
        return -1;

    glCompileShader(compute_shader);TEST_OPENGL_ERROR();
    glGetShaderiv(compute_shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE) {
        std::cerr << "ERROR: compile" << std::endl;
        return -1;
    }
    glAttachShader(program_compute, compute_shader);TEST_OPENGL_ERROR();

    glLinkProgram(program_compute);TEST_OPENGL_ERROR();
    glGetProgramiv(program_compute, GL_LINK_STATUS, &link_status);TEST_OPENGL_ERROR();
    if (compile_status != GL_TRUE) {
        std::cerr << "ERROR: link" << std::endl;
        return -1;
    }

    program_ = program;
    program_cpy = program_;
    program_compute_ = program_compute;
    program_compute_cpy = program_compute_;




    glUseProgram(program);
    return program;
}


bool
Scene::init_object() {

    GLuint vao;
    glUseProgram(program_);

    glGenVertexArrays(1, &vao);TEST_OPENGL_ERROR();
    glBindVertexArray(vao);TEST_OPENGL_ERROR();
    vao_ = vao;
    vao_cpy = vao_;

    GLint vertex_location = glGetAttribLocation(program_,"position");TEST_OPENGL_ERROR();
    // GLint color_location = glGetAttribLocation(program_,"color");TEST_OPENGL_ERROR();
    std::cout << "size: " << vertices_position.size() << std::endl;
    for (auto vert : vertices_position) {
        std::cout << vert << std::endl;
    }
    if (vertex_location != -1) {
        GLuint vbo;
        glGenBuffers(1, &vbo); TEST_OPENGL_ERROR();
        glBindBuffer(GL_ARRAY_BUFFER, vbo); TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, vertices_position.size()*sizeof(float), vertices_position.data(), GL_STREAM_DRAW); TEST_OPENGL_ERROR();
        glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0); TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(vertex_location); TEST_OPENGL_ERROR();
    }

    int vertexColorLocation = glGetUniformLocation(program_, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);

    int translation = glGetUniformLocation(program_, "translate");
    glUniform1f(translation, 0.0f);
    glBindVertexArray(0);
    return true;
}


GLuint
Scene::init_texture() {
    GLuint texHandle;
	glGenTextures(1, &texHandle); TEST_OPENGL_ERROR();

	glActiveTexture(GL_TEXTURE0); TEST_OPENGL_ERROR();
	glBindTexture(GL_TEXTURE_2D, texHandle); TEST_OPENGL_ERROR();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); TEST_OPENGL_ERROR();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); TEST_OPENGL_ERROR();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, NULL);TEST_OPENGL_ERROR();


	glBindImageTexture(0, texHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);TEST_OPENGL_ERROR();

    textid_ = texHandle;
    textid_cpy = textid_;
	return textid_;
}

void
Scene::update() {

}


GLuint
Scene::program_compute_get() const {
    return program_compute_;
}

GLuint
Scene::program_get() const {
    return program_;
}

GLuint
Scene::vao_get() const {
    return vao_;
}
