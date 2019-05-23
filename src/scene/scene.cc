#include "scene.hh"

GLuint vao_cpy = 0;

std::vector<GLfloat> vertices_position = {
            0.0, 0.0, 0.0,
            0.5, 0.0, 0.0,
            0.5, 0.5, 0.0,

            0.0, 0.0, 0.0,
            0.0, 0.5, 0.0,
            -0.5, 0.5, 0.0,

            0.0, 0.0, 0.0,
            -0.5, 0.0, 0.0,
            -0.5, -0.5, 0.0,

            0.0, 0.0, 0.0,
            0.0, -0.5, 0.0,
            0.5, -0.5, 0.0
        };

std::vector<GLfloat> color_buffer = {
    1.0, 1.0, 1.0
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
    std::cout << vao_cpy << std::endl;
    glBindVertexArray(vao_cpy);check_error();
    glDrawArrays(GL_TRIANGLES, 0, vertices_position.size());check_error();
    glBindVertexArray(0);check_error();
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
    glutInitContextProfile(GLUT_CORE_PROFILE|GLUT_DEBUG); check_error();
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH); check_error();
    glutInitWindowSize(width_, height_);check_error();
    glutInitWindowPosition (100, 100);
    glutCreateWindow("Fire in the hole");
    glEnable(GL_DEPTH_TEST);check_error();
    glutDisplayFunc(display);

    if (init_glew() == false) {
        std::cerr << "ERROR: failed to initialize glew" << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);check_error();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);check_error();
    glEnable(GL_CULL_FACE);check_error();
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glPixelStorei(GL_PACK_ALIGNMENT,1);

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

bool
Scene::shader(std::string vertex_shader_src, std::string fragment_shader_src) {
    std::vector<GLuint> shader_list;
    shader_list.push_back(glCreateShader(GL_VERTEX_SHADER)); check_error();
    shader_list.push_back(glCreateShader(GL_FRAGMENT_SHADER)); check_error();

    std::string content = load_file(vertex_shader_src);
    auto content_char = content.c_str();
    glShaderSource(shader_list[0], 1, &content_char, 0); check_error();

    content = load_file(fragment_shader_src);
    content_char = content.c_str();
    glShaderSource(shader_list[1], 1, &content_char, 0);check_error();

    for (auto shader : shader_list) {
        GLint compile_status = GL_TRUE;
        glCompileShader(shader);check_error();
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);check_error();
        if(compile_status != GL_TRUE) {
          GLint log_size;
          char *shader_log;
          glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
          shader_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
          if(shader_log == NULL) {
    	         return false;
          }
          glGetShaderInfoLog(shader, log_size, &log_size, shader_log);
          std::cerr << shader_log << std::endl;
          return false;
        }
    }
    std::cout << "create program" << std::endl;
    auto program = glCreateProgram();check_error();
    if (program == 0)
        return false;
    for (auto shader: shader_list) {
        glAttachShader(program, shader); check_error();
    }
    GLint link_status=GL_TRUE;
    glLinkProgram(program);check_error();
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE) {
        GLint log_size;
        char *program_log;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
        program_log = (char*)std::malloc(log_size+1);
        if(program_log == NULL) {
          glDeleteProgram(program);
          program=0;
          return false;
        }
        glGetProgramInfoLog(program, log_size, &log_size, program_log);
        glDeleteProgram(program);
        std::cerr << "ERROR: " << program_log << std::endl;
        program=0;
        return false;
    }
    program_ = program;
    glUseProgram(program);
    return true;
}


bool
Scene::init_object() {

    GLuint vao;
    int nb_obj = 0;
    std::vector<GLuint> list_obj;
    glGenVertexArrays(1, &vao);check_error();
    glBindVertexArray(vao);check_error();
    vao_ = vao;
    vao_cpy = vao_;

    GLint vertex_location = glGetAttribLocation(program_,"position");check_error();
    // GLint color_location = glGetAttribLocation(program_,"color");check_error();
    if (vertex_location != -1) {
        GLuint vbo;
        glGenBuffers(1, &vbo); check_error();
        std::cout << vbo << std::endl;
        glBindBuffer(GL_ARRAY_BUFFER, vbo); check_error();
        glBufferData(GL_ARRAY_BUFFER, vertices_position.size()*sizeof(float), vertices_position.data(), GL_STATIC_DRAW); check_error();
        glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0); check_error();
        // std::cout << "test1" << std::endl;
        glEnableVertexAttribArray(vertex_location); check_error();
    }
    // if (color_location != -1) {
    //     GLuint buff;
    //     glGenBuffers(1, &buff); check_error();
    //     std::cout << buff << std::endl;
    //     glBindBuffer(GL_ARRAY_BUFFER, buff); check_error();
    //     glBufferData(GL_ARRAY_BUFFER, color_buffer.size()*sizeof(float), color_buffer.data(), GL_STATIC_DRAW); check_error();
    //     glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, 0, 0); check_error();
    //     glEnableVertexAttribArray(color_location); check_error();
    // }

    list_obj.push_back(vertex_location);
    // list_obj.push_back(color_location);

    glBindVertexArray(0);
    return true;
}
