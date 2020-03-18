#include "Window.h"

/* 
 * Declare your variables below. Unnamed namespace is used here to avoid 
 * declaring global or static variables.
 */
#define DAY 1
#define NIGHT 0
#define SUN 1
#define MOON 0
namespace
{
    int waveNum = 4;
    int clouds_on = 1;
    int bloom = 1;
    int paused = 0;
    int mesh_on = 0;
    float exposure = 0.1f;
    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    unsigned int hdrFBO;
    unsigned int colorBuffers[2];
    unsigned int rboDepth;
    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];

	int width, height;
	std::string windowTitle("GLFW Starter Project");
    
    int nightShift = NIGHT;
    
    glm::vec2 mouse_point;
    glm::vec3 lastPoint;
    const float m_ROTSCALE = 100.0f;
    int rightbtn = 0;

    WaveCalculator* wave;
    OceanMesh* plane;
    Skydome* skybox;
    Clouds* clouds;
    PointLight* sunPtLight;
    PointLight* moonPtLight;
    Object* sun;
    Object* moon;

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
    GLuint oceanNSLoc;

    // -------- material -------
    GLuint diffuseLoc;
    GLuint specularLoc;
    GLuint ambientLoc;
    GLuint shininessLoc;
    // -------- pointLight ---------
    GLuint sunLightColorLoc;
    GLuint sunLightPosLoc;
    GLuint moonLightColorLoc;
    GLuint moonLightPosLoc;
    // -------- light rep(sun/moon) ------
    GLuint objectProgram; // The shader program id.
    GLuint objectProjectionLoc; // Location of projection in shader.
    GLuint objectViewLoc; // Location of view in shader.
    GLuint objectModelLoc; // Location of model in shader.
    GLuint objectColorLoc;

    GLuint skyboxProgram;
    GLuint skyboxProjLoc;
    GLuint skyboxViewLoc;
    GLuint skyboxModelLoc;

    GLuint cloudsProgram;
    GLuint cloudsProjectionLoc;
    GLuint cloudsModelLoc;
    GLuint cloudsViewLoc;
    GLuint cloudsdLoc;
    GLuint cloudsNSLoc;
GLuint cloudsRotLoc;


    // ---------- bloom -----------
    GLuint blurProgram;
    GLuint blurHoriLoc;

    GLuint blendProgram;
    GLuint blendBloomLoc;
    GLuint blendExpLoc;

    // clock
    std::clock_t start;
    double duration;
};

bool Window::initializeProgram()
{
	// Create a shader program with a vertex shader and a fragment shader.
	oceanProgram = LoadShaders("shaders/oceanShader.vert", "shaders/oceanShader.frag");
    skyboxProgram = LoadShaders("shaders/skydomeShader.vert", "shaders/skydomeShader.frag");
    cloudsProgram = LoadShaders("shaders/cloudsShader.vert", "shaders/cloudsShader.frag");
    objectProgram = LoadShaders("shaders/objectShader.vert", "shaders/objectShader.frag");
    blurProgram = LoadShaders("shaders/blurShader.vert", "shaders/blurShader.frag");
    blendProgram = LoadShaders("shaders/blendShader.vert", "shaders/blendShader.frag");

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
    if (!objectProgram)
    {
        std::cerr << "Failed to initialize object shader program" << std::endl;
        return false;
    }
    if (!blurProgram)
    {
        std::cerr << "Failed to initialize blur shader program" << std::endl;
        return false;
    }
    if (!blendProgram)
    {
        std::cerr << "Failed to initialize blend shader program" << std::endl;
        return false;
    }

	return true;
}

bool Window::initializeObjects()
{

    skybox = new Skydome();
    clouds = new Clouds();
    wave = new WaveCalculator(waveNum);
    plane = new OceanMesh(300.0f, 0.0f, 300, wave);
    start = std::clock();
    sunPtLight = new PointLight(glm::vec3(1.0f), glm::vec3(0, 30, -100));
    sunPtLight -> rotate(32.0f, glm::vec3(1.0f,0.0f,0.0f));
    sun = new Object(glm::vec3(252.0f/255.0f, 212.0f/255.0f, 64.0f/255.0f), glm::vec3(0, 30, -600), glm::vec3(40.0f));
    sun -> rotate(32.0f, glm::vec3(1.0f,0.0f,0.0f));
    moonPtLight = new PointLight(glm::vec3(1.0f), glm::vec3(0, 30, -100));
    moonPtLight -> rotate(-90.0f, glm::vec3(1.0f,0.0f,0.0f));
    moon = new Object(glm::vec3(254.0f/255.0f, 252.0f/255.0f, 215.0f/255.0f), glm::vec3(0, 30, -600), glm::vec3(30.0f));
    moon -> rotate(-90.0f, glm::vec3(1.0f,0.0f,0.0f));

    enableBloom();

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete plane;
    delete skybox;
    delete clouds;
    delete sun;
    delete moon;
	// Delete the shader program.
    glDeleteProgram(skyboxProgram);
    glDeleteProgram(objectProgram);
	glDeleteProgram(oceanProgram);
    glDeleteProgram(cloudsProgram);
    glDeleteProgram(blurProgram);
    glDeleteProgram(blendProgram);
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
    enableBloom();
	// Set the projection matrix.
	projection = glm::perspective(glm::radians(fovy),
		(float)width / (float)height, near, far);
}

void Window::idleCallback()
{
    
	// Perform any updates as necessary.
    if (!paused) {
        skybox -> update();
        sunPtLight -> update(SUN);
        sun -> update(SUN);
        moonPtLight -> update(MOON);
        moon -> update(MOON);
    }
    clouds -> update(cloudsdLoc);
    duration = (std::clock() - start)/(double)CLOCKS_PER_SEC;
    plane -> update((float)duration);
}

void Window::displayCallback(GLFWwindow* window)
{
	// Clear the color and depth buffers.
    // glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    
    // ----------------------- rendering ----------------------
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
    skybox->draw();
    // if (nightShift == DAY) skybox->draw(DAY);
    // else skybox->draw(NIGHT);
    // glDepthMask(GL_TRUE);
    // sun/moon
    glUseProgram(objectProgram);
    objectProjectionLoc = glGetUniformLocation(objectProgram, "projection");
    objectViewLoc = glGetUniformLocation(objectProgram, "view");
    objectModelLoc = glGetUniformLocation(objectProgram, "model");
    objectColorLoc = glGetUniformLocation(objectProgram, "color");
    
    // if (nightShift){
    glm::mat4 objectModel = sun->getModel();
    glm::vec3 objectColor = sun->getColor();
    glUniformMatrix4fv(objectProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(objectViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(objectModelLoc, 1, GL_FALSE, glm::value_ptr(objectModel));
    glUniform3fv(objectColorLoc, 1, glm::value_ptr(objectColor));
    sun->draw();
    // }
    // else {
    objectModel = moon->getModel();
    objectColor = moon->getColor();
    glUniformMatrix4fv(objectProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(objectViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(objectModelLoc, 1, GL_FALSE, glm::value_ptr(objectModel));
    glUniform3fv(objectColorLoc, 1, glm::value_ptr(objectColor));
    moon->draw();
   // }
    float xRot = skybox->getXRot();
    // for clouds
    if (clouds_on) {
    glUseProgram(cloudsProgram);
    // Get the locations of uniform variables.
    cloudsProjectionLoc = glGetUniformLocation(cloudsProgram, "projection");
    cloudsViewLoc = glGetUniformLocation(cloudsProgram, "view");
    cloudsModelLoc = glGetUniformLocation(cloudsProgram, "model");
    cloudsdLoc = glGetUniformLocation(cloudsProgram, "d");
    cloudsNSLoc = glGetUniformLocation(cloudsProgram, "nightShift");
    cloudsRotLoc = glGetUniformLocation(cloudsProgram, "rotAmt");
    glm::mat4 cloudsModel = clouds->getModel();
    glUniformMatrix4fv(cloudsProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(cloudsViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(cloudsModelLoc, 1, GL_FALSE, glm::value_ptr(cloudsModel));
    glUniform1f(cloudsdLoc, 0.01f);
    glUniform1i(cloudsNSLoc, 1);
    glUniform1f(cloudsRotLoc, xRot);
    
    clouds->draw();
    }
    // Activate the shader program.
    glUseProgram(oceanProgram);
    // Get the locations of uniform variables.
    oceanProjectionLoc = glGetUniformLocation(oceanProgram, "projection");
    oceanViewLoc = glGetUniformLocation(oceanProgram, "view");
    oceanModelLoc = glGetUniformLocation(oceanProgram, "model");
    oceanCamLoc = glGetUniformLocation(oceanProgram, "cameraPos");
    oceanXRotLoc = glGetUniformLocation(oceanProgram, "xRot");
    oceanNSLoc = glGetUniformLocation(oceanProgram, "nightShift");

    // materials
    diffuseLoc = glGetUniformLocation(oceanProgram, "material.diffuse");
    specularLoc = glGetUniformLocation(oceanProgram, "material.specular");
    ambientLoc = glGetUniformLocation(oceanProgram, "material.ambient");
    shininessLoc = glGetUniformLocation(oceanProgram, "material.shininess");
    glUniform3fv(diffuseLoc, 1, glm::value_ptr(glm::vec3(0.1f)));
    glUniform3fv(specularLoc, 1, glm::value_ptr(glm::vec3(1.0f)));
    glUniform3fv(ambientLoc, 1, glm::value_ptr(glm::vec3(1.0f)));
    glUniform1f(shininessLoc, 30.0f);
    // point light
    sunLightColorLoc = glGetUniformLocation(oceanProgram, "sunPtLight.color");
    sunLightPosLoc = glGetUniformLocation(oceanProgram, "sunPtLight.position");
    glm::mat4 sun2World = sunPtLight->getToWorld();
    glm::vec3 sunLightPosition = view * sun2World * glm::vec4(sunPtLight->getPosition(), 1.0f);
    glUniform3fv(sunLightColorLoc, 1, glm::value_ptr(sunPtLight->getColor()));
    glUniform3fv(sunLightPosLoc, 1, glm::value_ptr(sunLightPosition));
    moonLightColorLoc = glGetUniformLocation(oceanProgram, "moonPtLight.color");
    moonLightPosLoc = glGetUniformLocation(oceanProgram, "moonPtLight.position");
    glm::mat4 moon2World = moonPtLight->getToWorld();
    glm::vec3 moonLightPosition = view * moon2World * glm::vec4(moonPtLight->getPosition(), 1.0f);
    glUniform3fv(moonLightColorLoc, 1, glm::value_ptr(moonPtLight->getColor()));
    glUniform3fv(moonLightPosLoc, 1, glm::value_ptr(moonLightPosition));
    
	glm::mat4 model = glm::mat4(1.0f);
    // float xRot = 0.0f;
	glUniformMatrix4fv(oceanProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(oceanViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(oceanModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(oceanCamLoc, 1, GL_FALSE, glm::value_ptr(eye));
    glUniform1f(oceanXRotLoc, xRot);
    glUniform1i(oceanNSLoc, 1);

	// Render the object.
    // GLuint skyTex = skybox -> getTextureID();
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, skyTex);
    if (mesh_on)
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    plane->draw();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ----------------------------- blurring ------------------------------
    
    bool horizontal = true, first_iteration = true;
    unsigned int amount = 50;
    glUseProgram(blurProgram);
    for (unsigned int i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
        blurHoriLoc = glGetUniformLocation(blurProgram, "horizontal");
        glUniform1i(blurHoriLoc, horizontal);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
        renderQuad();
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // -------------------------- blooming ---------------------------

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(blendProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
    // shaderBloomFinal.setInt("bloom", bloom);
    // shaderBloomFinal.setFloat("exposure", exposure);
    blendBloomLoc = glGetUniformLocation(blendProgram, "bloom");
    blendExpLoc = glGetUniformLocation(blendProgram, "exposure");
    glUniform1i(blendBloomLoc, bloom);
    glUniform1f(blendExpLoc, exposure);
    glDisable(GL_CULL_FACE);
    renderQuad();
    std::cout << "bloom: " << (bloom ? "on" : "off") << "| exposure: " << exposure << std::endl;
    

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
            nightShift = !nightShift;
            break;
        case GLFW_KEY_B:
            bloom = !bloom;
            break;
        case GLFW_KEY_R:
            delete(clouds);
            clouds = new Clouds();
            break;
        case GLFW_KEY_C:
            clouds_on = !clouds_on;
            break;
        case GLFW_KEY_M:
            mesh_on = !mesh_on;
            break;
        case GLFW_KEY_P:
            paused = !paused;
            break;
        case GLFW_KEY_LEFT_BRACKET:
            if (waveNum > 0) wave->setWaveNum(--waveNum);
            break;
        case GLFW_KEY_RIGHT_BRACKET:
            if (waveNum < 6) wave->setWaveNum(++waveNum);
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

void Window::enableBloom() {
    
    // unsigned int hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    // create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)
    // unsigned int colorBuffers[2];
    glGenTextures(2, colorBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
    }
    // create and attach depth buffer (renderbuffer)
    // unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ping-pong-framebuffer for blurring
    // unsigned int pingpongFBO[2];
    // unsigned int pingpongColorbuffers[2];
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorbuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        // !!!!!!!!!!!!!!!!!!!!1
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
        // also check if framebuffers are complete (no need for depth buffer)
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
    }
    glUseProgram(blurProgram);
    glUniform1i(glGetUniformLocation(blurProgram, "image"), 0);

    glUseProgram(blendProgram);
    glUniform1i(glGetUniformLocation(blendProgram, "scene"), 0);
    glUniform1i(glGetUniformLocation(blendProgram, "bloomBlur"), 1);
}

void Window::renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
