#include "scene.hh"

GLuint vao_cpy = 0;
GLuint textid_cpy = 0;

std::vector<GLfloat> list_positons_;
std::vector<GLfloat> list_color_;

int wind_select = 0;


#define TEST_OPENGL_ERROR()                                                             \
  do {									\
    GLenum err = glGetError();					                        \
    if (err != GL_NO_ERROR) std::cerr << "OpenGL ERROR!" << __LINE__ << std::endl;      \
  } while(0)

/* This is the callback to get a key trigger */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_O && action == GLFW_PRESS)
        wind_select ++;
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        wind_select --;
}

/* Default Constructor */
Scene::Scene() : width_(0), height_(0), motor_(Motor(0))
{}


/* Constructor */
Scene::Scene(int width, int height, unsigned int max_part) : width_(width), height_(height), motor_(Motor(0)), max_particules_(max_part)
{}

/* Display shader on the screen */
void
Scene::display() {
    glUseProgram(program_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);TEST_OPENGL_ERROR();
    glBindVertexArray(vao_);TEST_OPENGL_ERROR();
    glDrawArrays(GL_POINTS, 0, list_positons_.size() / 3);TEST_OPENGL_ERROR();
    glBindVertexArray(0);TEST_OPENGL_ERROR();
    glfwSwapBuffers(window_);
}

bool init_glew() {
    return (glewInit() == GLEW_OK);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/* Init the scene with glew and glwd */
bool
Scene::init() {
    /* Init glwf window */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Window creation */
    auto window = glfwCreateWindow(width_, height_, "fire in the hole", NULL, NULL);
    window_ = window;
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    /* GL setting */
    glEnable(GL_DEPTH_TEST);TEST_OPENGL_ERROR();
    glEnable(GL_PROGRAM_POINT_SIZE);TEST_OPENGL_ERROR();
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

/* Open a file and get the content in a string */
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

/* Init vertex and fragment shader */
GLint
Scene::shader(std::string vertex_shader_src, std::string fragment_shader_src) {
    std::vector<GLuint> shader_list;
    std::string content;
    const char* content_char;
    GLint link_status=GL_TRUE;
    GLint compile_status = GL_TRUE;

    /* Create shaders */
    auto vertex_shader = glCreateShader(GL_VERTEX_SHADER); TEST_OPENGL_ERROR();
    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER); TEST_OPENGL_ERROR();

    /* add shader source for vertex shader */
    content = load_file(vertex_shader_src);
    content_char = content.c_str();
    glShaderSource(vertex_shader, 1, &content_char, 0); TEST_OPENGL_ERROR();

    /* And to fragment source */
    content = load_file(fragment_shader_src);
    content_char = content.c_str();
    glShaderSource(fragment_shader, 1, &content_char, 0);TEST_OPENGL_ERROR();

    /* Creation of the program */
    auto program = glCreateProgram();TEST_OPENGL_ERROR();


    /* Compile the vertex shader */
    glCompileShader(vertex_shader);TEST_OPENGL_ERROR();
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);TEST_OPENGL_ERROR();
    if(compile_status != GL_TRUE) {
      GLint log_size;
      char *shader_log;
      glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_size);
      shader_log = (char*)std::malloc(log_size+1);
      if(shader_log == NULL) {
             return -1;
      }
      glGetShaderInfoLog(vertex_shader, log_size, &log_size, shader_log);
      std::cerr << shader_log << std::endl;
      return -1;
    }
    glAttachShader(program, vertex_shader);

    /* Compile the fragment shader */
    glCompileShader(fragment_shader);TEST_OPENGL_ERROR();
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);TEST_OPENGL_ERROR();
    if(compile_status != GL_TRUE) {
      GLint log_size;
      char *shader_log;
      glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_size);
      shader_log = (char*)std::malloc(log_size+1);

      if(shader_log == NULL) {
             return -1;
      }

      glGetShaderInfoLog(fragment_shader, log_size, &log_size, shader_log);
      std::cerr << shader_log << std::endl;
      return -1;
    }
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (compile_status != GL_TRUE) {
        std::cerr << "ERROR: link" << std::endl;
        return -1;
    }

    program_ = program;

    glUseProgram(program_);

    return program;
}


bool
Scene::init_object() {
    /* Create the VAO */
    GLuint vao;
    glGenVertexArrays(1, &vao);TEST_OPENGL_ERROR();
    glBindVertexArray(vao);TEST_OPENGL_ERROR();
    vao_ = vao;
    GLint vertex_location = glGetAttribLocation(program_,"position");TEST_OPENGL_ERROR();
    GLint color_location = glGetAttribLocation(program_,"color");TEST_OPENGL_ERROR();
    if (vertex_location != -1) {
        /* Init vertex buffer */
        GLuint vbo;
        glGenBuffers(1, &vbo); TEST_OPENGL_ERROR();
        glBindBuffer(GL_ARRAY_BUFFER, vbo); TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, list_positons_.size() * sizeof(float), list_positons_.data(),  GL_DYNAMIC_DRAW); TEST_OPENGL_ERROR();
        glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0); TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(vertex_location); TEST_OPENGL_ERROR();
        vbo_vertices_ = vbo;
    }

    if (color_location != -1) {
        /* Init color buffer */
        GLuint vbo;
        glGenBuffers(1, &vbo); TEST_OPENGL_ERROR();
        glBindBuffer(GL_ARRAY_BUFFER, vbo); TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, list_color_.size() * sizeof(float), list_color_.data(),  GL_DYNAMIC_DRAW); TEST_OPENGL_ERROR();
        glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 0, 0); TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(color_location); TEST_OPENGL_ERROR();
        vbo_color_ = vbo;
    }
    glBindVertexArray(0);
    return true;
}

Motor
Scene::init_motor(unsigned int nb_particles) {
    /* Init the motor of particles */
    motor_ = Motor(nb_particles);

    /* Create particles */
    auto list_pos = motor_.create(0);

    /* add positions and color to VBO */
    list_positons_ = std::get<0>(list_pos);
    list_color_ = std::get<1>(list_pos);
    return motor_;
}

void
Scene::update() {
    /* Create new particle at bottom */
    auto new_list = motor_.create(300);

    /* Update position */
    auto list_pos = motor_.update(wind_select);

    list_color_ = std::get<1>(new_list);
    list_positons_ = std::get<0>(new_list);

    if (std::get<1>(list_pos).size() >= max_particules_) {
        /* If we reach max particles, delete 300 particles */
        /* Update VBO */
        list_positons_.insert(list_positons_.end(), std::get<0>(list_pos).end() - max_particules_, std::get<0>(list_pos).end());TEST_OPENGL_ERROR();
        list_color_.insert(list_color_.end(), std::get<1>(list_pos).end() - max_particules_, std::get<1>(list_pos).end());
        motor_.kill(300);
    }
    else {
        /* Update VBO */
        list_positons_.insert(list_positons_.end(), std::get<0>(list_pos).begin(), std::get<0>(list_pos).end());TEST_OPENGL_ERROR();
        list_color_.insert(list_color_.end(), std::get<1>(list_pos).begin(), std::get<1>(list_pos).end());
    }
}

GLuint
Scene::program_get() const {
    return program_;
}

GLuint
Scene::vao_get() const {
    return vao_;
}

GLFWwindow*
Scene::window_get() const {
    return window_;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    window = window;
    glViewport(0, 0, width, height);
}
