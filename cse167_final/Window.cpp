#include "Window.h"

/* 
 * Declare your variables below. Unnamed namespace is used here to avoid 
 * declaring global or static variables.
 */
#define DAY 1
#define NIGHT 0
namespace
{
	int width, height;
	std::string windowTitle("GLFW Starter Project");
    
    int nightShift = DAY;
    
    glm::vec2 mouse_point;
    glm::vec3 lastPoint;
    const float m_ROTSCALE = 100.0f;
    int rightbtn = 0;

    WaveCalculator* wave;
    OceanMesh* plane;
    Skydome* skybox;
    Clouds* clouds;
	glm::vec3 eye(0, 20, 0); // Camera position.
	glm::vec3 center(0, 20, -40); // The point we are looking at.
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
    GLuint oceanXRotLoc;

    GLuint skyboxProgram;
    GLuint skyboxProjLoc;
    GLuint skyboxViewLoc;
    GLuint skyboxModelLoc;

    GLuint cloudsProgram;
    GLuint cloudsProjectionLoc;
    GLuint cloudsModelLoc;
    GLuint cloudsViewLoc;
    GLuint cloudsdLoc;

    std::clock_t start;
    double duration;
};

bool Window::initializeProgram()
{
	// Create a shader program with a vertex shader and a fragment shader.
	oceanProgram = LoadShaders("shaders/oceanShader.vert", "shaders/oceanShader.frag");
    skyboxProgram = LoadShaders("shaders/skydomeShader.vert", "shaders/skydomeShader.frag");
    cloudsProgram = LoadShaders("shaders/cloudsShader.vert", "shaders/cloudsShader.frag");
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
    if (!cloudsProgram)
    {
        std::cerr << "Failed to initialize clouds shader program" << std::endl;
        return false;
    }

	return true;
}

bool Window::initializeObjects()
{
	// Create a cube of size 5.
	// cube = new Cube(5.0f);
    skybox = new Skydome();
    clouds = new Clouds();
    wave = new WaveCalculator(4);
    plane = new OceanMesh(500.0f, 0.0f, 500, wave);
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
    delete skybox;
    delete clouds;
	// Delete the shader program.
    glDeleteProgram(skyboxProgram);
	glDeleteProgram(oceanProgram);
    glDeleteProgram(cloudsProgram);

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
    skybox->update();
    plane -> update((float)duration);
    clouds -> update(cloudsdLoc);
}

void Window::displayCallback(GLFWwindow* window)
{
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    // for skybox
    glUseProgram(skyboxProgram);
    // glDepthMask(GL_FALSE);
    skyboxProjLoc = glGetUniformLocation(skyboxProgram, "projection");
    skyboxViewLoc = glGetUniformLocation(skyboxProgram, "view");
    skyboxModelLoc = glGetUniformLocation(skyboxProgram, "model");
    glm::mat4 skydomeModel = skybox -> getModel();
    glUniformMatrix4fv(skyboxModelLoc, 1, GL_FALSE, glm::value_ptr(skydomeModel));
    glUniformMatrix4fv(skyboxProjLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(skyboxViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // if (nightShift == DAY) skybox->draw(DAY);
    skybox->draw();
    // glDepthMask(GL_TRUE);
	
    // for skydome
    glUseProgram(cloudsProgram);
    // Get the locations of uniform variables.
    cloudsProjectionLoc = glGetUniformLocation(cloudsProgram, "projection");
    cloudsViewLoc = glGetUniformLocation(cloudsProgram, "view");
    cloudsModelLoc = glGetUniformLocation(cloudsProgram, "model");
    cloudsdLoc = glGetUniformLocation(cloudsProgram, "d");
    
    glm::mat4 cloudsModel = clouds->getModel();
    glUniformMatrix4fv(cloudsProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(cloudsViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(cloudsModelLoc, 1, GL_FALSE, glm::value_ptr(cloudsModel));
    glUniform1f(cloudsdLoc, 0.01f);
    clouds->draw();
    
    // Activate the shader program.
    glUseProgram(oceanProgram);
    // Get the locations of uniform variables.
    oceanProjectionLoc = glGetUniformLocation(oceanProgram, "projection");
    oceanViewLoc = glGetUniformLocation(oceanProgram, "view");
    oceanModelLoc = glGetUniformLocation(oceanProgram, "model");
    oceanCamLoc = glGetUniformLocation(oceanProgram, "cameraPos");
    oceanXRotLoc = glGetUniformLocation(oceanProgram, "xRot");

    // Specify the values of the uniform variables we are going to use.
	glm::mat4 model = glm::mat4(1.0f);
    float xRot = skybox->getXRot();
	glUniformMatrix4fv(oceanProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(oceanViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(oceanModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(oceanCamLoc, 1, GL_FALSE, glm::value_ptr(eye));
    glUniform1f(oceanXRotLoc, xRot);

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
        case GLFW_KEY_N:
            if (nightShift == DAY) nightShift = NIGHT;
            else nightShift = DAY;
            break;
        case GLFW_KEY_R:
            delete(clouds);
            clouds = new Clouds();
            break;
		default:
			break;
		}
	}
}

void Window::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        glm::vec2 point = mouse_point;
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            // skybox
            rightbtn = 1;
            lastPoint = trackBallMapping(point);
        }
    }
    else if (action == GLFW_RELEASE)
    {
        rightbtn = 0;
    }
}

glm::vec3 Window::trackBallMapping(glm::vec2 point)
{
    glm::vec3 v;
    float d;
    v.x = (2.0f * point.x - width) / width;
    v.y = (height - 2.0f * point.y) / height;
    v.z = 0.0f;
    d = glm::length(v);
    d = (d<1.0) ? d : 1.0;
    v.z = sqrtf(1.001 - d*d);
    glm::normalize(v);
    return v;
}

void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    mouse_point = { xpos, ypos };
    float rotAngle;
    glm::vec3 curPoint = trackBallMapping(mouse_point);
    glm::vec3 direction = curPoint - lastPoint;
    float velocity = glm::length(direction);
    
    if (rightbtn) {
            if (velocity > 0.0001) // If little movement - do nothing.
            {
                // Rotate about the axis that is perpendicular to the great circle connecting the mouse movements.
                glm::vec3 rotAxis;
                rotAxis = glm::cross(lastPoint, curPoint);
                rotAngle = velocity * m_ROTSCALE;
                rotateCamera(glm::vec3(0.0f, 1.0f, 0.0f), direction.x * m_ROTSCALE);
                rotateCamera(glm::vec3(1.0f, 0.0f, 0.0f), direction.y * m_ROTSCALE);
                lastPoint = curPoint;
            }
    }

        // Save the location of the current point for the next movement.
}


void Window::rotateCamera(glm::vec3 rotAxis, float rotAngle) {
    view = glm::rotate(glm::mat4(1.0f), rotAngle / 180.0f * glm::pi<float>(), rotAxis) * view;
}
