#include "scene.hh"

Scene::Scene(int width, int height) : width_(width), height_(height)
{}

void check_error() {
    if (glGetError() != GL_NO_ERROR)
        std::cerr << "ERROR:" << __LINE__ << std::endl;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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


    glEnable(GL_DEPTH_TEST);check_error();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);check_error();
    glEnable(GL_CULL_FACE);check_error();
    glClearColor(0.4,0.4,0.4,1.0);check_error();
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    
    if (init_glew() == false) {
        std::cerr << "ERROR: failed to initialize glew" << std::endl;
        return false;
    }
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
      buffer += buffer + line + '\n';
    }
    buffer += '\0';
    return buffer;
}

bool
Scene::shader(std::string vertex_shader_src, std::string fragment_shader_src) {
    std::vector<GLuint> shader_list(2);
    glCreateShader(GL_VERTEX_SHADER);
    // shader_list.push_back(glCreateShader(GL_VERTEX_SHADER)); check_error();
    // std::cout << "test" << std::endl;
    // shader_list.push_back(glCreateShader(GL_FRAGMENT_SHADER)); check_error();

    glShaderSource(shader_list[0], 1, (const GLchar**)(load_file(vertex_shader_src).c_str()), 0);check_error();
    glShaderSource(shader_list[1], 1, (const GLchar**)(load_file(fragment_shader_src).c_str()), 0);check_error();

    for (auto shader : shader_list) {
        GLint compile_status = GL_TRUE;
        glCompileShader(shader);check_error();
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);check_error();
        if(compile_status != GL_TRUE) {
          std::cerr << "ERROR" << std::endl;
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
    if (link_status == false) {
        GLint log_size;
        char *program_log;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
        program_log = (char*)std::malloc(log_size+1); /* +1 pour le caractere de fin de chaine '\0' */
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

    glUseProgram(program);
    return true;
}
