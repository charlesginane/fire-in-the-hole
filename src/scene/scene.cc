#include "scene.hh"

GLuint vao_cpy = 0;
GLuint program_cpy = 0;
GLuint program_compute_cpy = 0;
GLuint textid_cpy = 0;

std::vector<GLfloat> list_positons_;
std::vector<GLfloat> list_color_;


#define TEST_OPENGL_ERROR()                                                             \
  do {									\
    GLenum err = glGetError();					                        \
    if (err != GL_NO_ERROR) std::cerr << "OpenGL ERROR!" << __LINE__ << std::endl;      \
  } while(0)

Scene::Scene() : width_(0), height_(0), motor_(Motor(0))
{}

Scene::Scene(int width, int height) : width_(width), height_(height), motor_(Motor(0))
{}

void check_error() {
    if (glGetError() != GL_NO_ERROR)
        std::cerr << "ERROR:" << __LINE__ << std::endl;
}

void
Scene::display() {
    glUseProgram(program_cpy);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);TEST_OPENGL_ERROR();
    glBindVertexArray(vao_cpy);TEST_OPENGL_ERROR();
    std::cout << "size" << list_positons_.size() << std::endl;
    glDrawArrays(GL_POINTS, 0, list_positons_.size() / 3);TEST_OPENGL_ERROR();
    glBindVertexArray(0);TEST_OPENGL_ERROR();
    glfwSwapBuffers(window_);
}

bool init_glew() {
    return (glewInit() == GLEW_OK);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

bool
Scene::init(int argc, char *argv[]) {
    /* Init all of size*/
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    auto window = glfwCreateWindow(width_, height_, "fire in the hole", NULL, NULL);
    window_ = window;
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

    std::cout << "ss " << std::endl;

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

    int translation = glGetUniformLocation(program, "translate");
    glUniform1f(translation, 0.0f);
    int vertexColorLocation = glGetUniformLocation(program, "ourColor");
    glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
    int special = glGetUniformLocation(program, "special");TEST_OPENGL_ERROR();
    glUniform1i(special, 0);TEST_OPENGL_ERROR();
    return program;
}


bool
Scene::init_object() {

    GLuint vao;
    // glUseProgram(program_);

    glGenVertexArrays(1, &vao);TEST_OPENGL_ERROR();
    glBindVertexArray(vao);TEST_OPENGL_ERROR();
    vao_ = vao;
    vao_cpy = vao_;
    GLint vertex_location = glGetAttribLocation(program_,"position");TEST_OPENGL_ERROR();
    GLint color_location = glGetAttribLocation(program_,"color");TEST_OPENGL_ERROR();
    std::cout << vertex_location << std::endl;
    std::cout << color_location << std::endl;
    if (vertex_location != -1) {
        GLuint vbo;
        glGenBuffers(1, &vbo); TEST_OPENGL_ERROR();
        glBindBuffer(GL_ARRAY_BUFFER, vbo); TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, list_positons_.size() * sizeof(float), list_positons_.data(),  GL_DYNAMIC_DRAW); TEST_OPENGL_ERROR();
        glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0); TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(vertex_location); TEST_OPENGL_ERROR();
        vbo_vertices_ = vbo;
        std::cout << vbo_vertices_ << std::endl;
    }

    if (color_location != -1) {
        GLuint vbo;
        glGenBuffers(1, &vbo); TEST_OPENGL_ERROR();
        glBindBuffer(GL_ARRAY_BUFFER, vbo); TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, list_color_.size() * sizeof(float), list_color_.data(),  GL_DYNAMIC_DRAW); TEST_OPENGL_ERROR();
        glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 0, 0); TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(color_location); TEST_OPENGL_ERROR();
    }

    // int vertexColorLocation = glGetUniformLocation(program_, "ourColor");
    // glUniform4f(vertexColorLocation, 1.0f, 1.0f, 0.0f, 1.0f);

    // int translation = glGetUniformLocation(program_, "translate");
    // glUniform1f(translation, 0.0f);

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

Motor
Scene::init_motor(unsigned int nb_particles) {
    motor_ = Motor(nb_particles);
    auto list_pos = motor_.create(0);
    list_positons_ = std::get<0>(list_pos);
    list_color_ = std::get<1>(list_pos);
    std::cout << list_color_.size() << std::endl;
    return motor_;
}

void
Scene::update(int program) {
    auto new_list = motor_.create(300);
    auto list_pos = motor_.update(program);

    //glfwSwapBuffers(window_);

    glBindVertexArray(vao_);TEST_OPENGL_ERROR();
    list_color_ = std::get<1>(list_pos);
    list_color_.insert(list_color_.end(), std::get<1>(list_pos).begin(), std::get<1>(list_pos).end());
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(std::get<1>(list_pos)) * sizeof(float), &std::get<1>(list_pos));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glBufferData(GL_ARRAY_BUFFER, list_color_.size()*sizeof(float), list_color_.data(), GL_DYNAMIC_DRAW);TEST_OPENGL_ERROR();
    //std::cout << list_color_.size() << std::endl;
    glBindVertexArray(0);TEST_OPENGL_ERROR();

    glBindVertexArray(vao_);TEST_OPENGL_ERROR();
    list_positons_ = std::get<0>(new_list);
    list_positons_.insert(list_positons_.end(), std::get<0>(list_pos).begin(), std::get<0>(list_pos).end());
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(std::get<0>(list_pos)) * sizeof(float), &std::get<0>(list_pos));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glBufferData(GL_ARRAY_BUFFER, list_positons_.size()*sizeof(float), list_positons_.data(), GL_DYNAMIC_DRAW);TEST_OPENGL_ERROR();
    glBindVertexArray(0);TEST_OPENGL_ERROR();


    //std::cout << "before: " << list_positons_.size() << std::endl;

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

GLFWwindow*
Scene::window_get() const {
    return window_;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
