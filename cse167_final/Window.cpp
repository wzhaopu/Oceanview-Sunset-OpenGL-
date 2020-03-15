#include "Window.h"

/* 
 * Declare your variables below. Unnamed namespace is used here to avoid 
 * declaring global or static variables.
 */
namespace
{
	int width, height;
	std::string windowTitle("GLFW Starter Project");

    WaveCalculator* wave;
    OceanMesh* plane;
    Skybox* skybox;

	glm::vec3 eye(0, 20, 40); // Camera position.
	glm::vec3 center(0, 15, 0); // The point we are looking at.
	glm::vec3 up(0, 1, 0); // The up direction of the camera.
	float fovy = 60;
	float near = 1;
	float far = 2000;
	glm::mat4 view = glm::lookAt(eye, center, up); // View matrix, defined by eye, center and up.
	glm::mat4 projection; // Projection matrix.

	GLuint oceanProgram; // The shader program id.
	GLuint oceanProjectionLoc; // Location of projection in shader.
	GLuint oceanViewLoc; // Location of view in shader.
	GLuint oceanModelLoc; // Location of model in shader.
    GLuint oceanCamLoc;

    GLuint skyboxProgram;
    GLuint skyboxProjLoc;
    GLuint skyboxViewLoc;


    std::clock_t start;
    double duration;
};

bool Window::initializeProgram()
{
	// Create a shader program with a vertex shader and a fragment shader.
	oceanProgram = LoadShaders("shaders/oceanShader.vert", "shaders/oceanShader.frag");
    skyboxProgram = LoadShaders("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
	// Check the shader program.
	if (!oceanProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
    if (!skyboxProgram)
    {
        std::cerr << "Failed to initialize skybox shader program" << std::endl;
        return false;
    }

    /*
	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	oceanProjectionLoc = glGetUniformLocation(oceanProgram, "projection");
	oceanViewLoc = glGetUniformLocation(oceanProgram, "view");
	oceanModelLoc = glGetUniformLocation(oceanProgram, "model");
	oceanColorLoc = glGetUniformLocation(oceanProgram, "color");
     */
	return true;
}

bool Window::initializeObjects()
{
	// Create a cube of size 5.
	// cube = new Cube(5.0f);
    skybox = new Skybox();
    wave = new WaveCalculator(6);
    plane = new OceanMesh(300.0f, 0, 300, wave);
    start = std::clock();
    // plane->print();
	// Set cube to be the first to display
	// currentObj = cube;

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete plane;

	// Delete the shader program.
    glDeleteProgram(skyboxProgram);
	glDeleteProgram(oceanProgram);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int w, int h)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height);
#endif
	// width = w;
	// height = h;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	projection = glm::perspective(glm::radians(fovy),
		(float)width / (float)height, near, far);
}

void Window::idleCallback()
{
    
	// Perform any updates as necessary.
    duration = (std::clock() - start)/(double)CLOCKS_PER_SEC;
    // std::cout << "time: " << duration << "\n";
	plane -> update((float)duration);
}

void Window::displayCallback(GLFWwindow* window)
{
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // for skybox
    glUseProgram(skyboxProgram);
    glDepthMask(GL_FALSE);
    skyboxProjLoc = glGetUniformLocation(skyboxProgram, "projection");
    skyboxViewLoc = glGetUniformLocation(skyboxProgram, "view");
    glUniformMatrix4fv(skyboxProjLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(skyboxViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    skybox->draw();
    glDepthMask(GL_TRUE);
	
    
    // Activate the shader program.
    glUseProgram(oceanProgram);
    // Get the locations of uniform variables.
    oceanProjectionLoc = glGetUniformLocation(oceanProgram, "projection");
    oceanViewLoc = glGetUniformLocation(oceanProgram, "view");
    oceanModelLoc = glGetUniformLocation(oceanProgram, "model");
    oceanCamLoc = glGetUniformLocation(oceanProgram, "cameraPos");

    // Specify the values of the uniform variables we are going to use.
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 color = glm::vec3(1.0f, 0.95f, 0.1f);;
	glUniformMatrix4fv(oceanProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(oceanViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(oceanModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(oceanCamLoc, 1, GL_FALSE, glm::value_ptr(eye));

	// Render the object.
    plane->draw();

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */

	 // Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		default:
			break;
		}
	}
}
