#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"

#include <iostream>
#include <math.h>
#include <Windows.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
float ballY(float x);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// animation
bool coll = false;
float xTrack = 1.0f;
float yTrack = -1.25f;
float rr = 0.0f;
int idx = 0, flip = 0;

bool b = true;
float temp = 0.0;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "computer graphics project", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// build and compile shaders
	// -------------------------
	Shader ourShader("shaders/vertexShader.txt", "shaders/fragmentShader.txt");
	Shader ourShader2("shaders/vertexShader2.txt", "shaders/fragmentShader2.txt");


	// load models
	// -----------

	//Model toru("toru-oikawa/oioikawakawa.obj", true);
	Model ball("ball/ball.obj", false);
	Model football_goal("football goal/football-goal.obj", false);
	Model ground("ground/wood.obj", false);
	Model skyBox("skyBox/skybox.obj", false);
	Model table("table/table.obj", false);

	//Model player("player/player.obj",true);

	Model player[] = {
		Model("player/0.obj",true), Model("player/3.obj",true), Model("player/6.obj",true), Model("player/9.obj",true) ,
		Model("player/12.obj",true), Model("player/15.obj",true), Model("player/18.obj",true), Model("player/21.obj",true),
		Model("player/24.obj",true), Model("player/27.obj",true), Model("player/30.obj",true), };
	//----------------------------------------------------------------------------------------
	float color1[] = { 1.0f, 1.0f, 1.0f, };
	float color2[] = { 0.7f, 0.7f, 0.0f, };

	float vertices[] = {
		// blue glass
		0.0f, 0.0f, -2.0f,		0.0f, 0.0f,1.0f,
		0.0f, 0.0f, +2.0f,		0.0f, 0.0f,1.0f,
		0.0f, 2.0f, +2.0f,		0.0f, 0.0f,1.0f,
		0.0f, 0.0f, -2.0f,		0.0f, 0.0f,1.0f,
		0.0f, 2.0f, -2.0f,		0.0f, 0.0f,1.0f,
		0.0f, 2.0f, +2.0f,		0.0f, 0.0f,1.0f,
		// light 1
		-0.5f, -0.5f, -0.5f,	color1[0],color1[1],color1[2],
		0.5f, -0.5f, -0.5f,		color1[0],color1[1],color1[2],
		0.5f,  0.5f, -0.5f,		color1[0],color1[1],color1[2],
		0.5f,  0.5f, -0.5f,		color1[0],color1[1],color1[2],
		-0.5f,  0.5f, -0.5f,	color1[0],color1[1],color1[2],
		-0.5f, -0.5f, -0.5f,	color1[0],color1[1],color1[2],

		-0.5f, -0.5f,  0.5f,	color1[0],color1[1],color1[2],
		0.5f, -0.5f,  0.5f,		color1[0],color1[1],color1[2],
		0.5f,  0.5f,  0.5f,		color1[0],color1[1],color1[2],
		0.5f,  0.5f,  0.5f,		color1[0],color1[1],color1[2],
		-0.5f,  0.5f,  0.5f,	color1[0],color1[1],color1[2],
		-0.5f, -0.5f,  0.5f,	color1[0],color1[1],color1[2],

		-0.5f,  0.5f,  0.5f,	color1[0],color1[1],color1[2],
		-0.5f,  0.5f, -0.5f,	color1[0],color1[1],color1[2],
		-0.5f, -0.5f, -0.5f,	color1[0],color1[1],color1[2],
		-0.5f, -0.5f, -0.5f,	color1[0],color1[1],color1[2],
		-0.5f, -0.5f,  0.5f,	color1[0],color1[1],color1[2],
		-0.5f,  0.5f,  0.5f,	color1[0],color1[1],color1[2],

		0.5f,  0.5f,  0.5f,		color1[0],color1[1],color1[2],
		0.5f,  0.5f, -0.5f,		color1[0],color1[1],color1[2],
		0.5f, -0.5f, -0.5f,		color1[0],color1[1],color1[2],
		0.5f, -0.5f, -0.5f,		color1[0],color1[1],color1[2],
		0.5f, -0.5f,  0.5f,		color1[0],color1[1],color1[2],
		0.5f,  0.5f,  0.5f,		color1[0],color1[1],color1[2],

		-0.5f, -0.5f, -0.5f,	color1[0],color1[1],color1[2],
		0.5f, -0.5f, -0.5f,		color1[0],color1[1],color1[2],
		0.5f, -0.5f,  0.5f,		color1[0],color1[1],color1[2],
		0.5f, -0.5f,  0.5f,		color1[0],color1[1],color1[2],
		-0.5f, -0.5f,  0.5f,	color1[0],color1[1],color1[2],
		-0.5f, -0.5f, -0.5f,	color1[0],color1[1],color1[2],

		-0.5f,  0.5f, -0.5f,	color1[0],color1[1],color1[2],
		0.5f,  0.5f, -0.5f,		color1[0],color1[1],color1[2],
		0.5f,  0.5f,  0.5f,		color1[0],color1[1],color1[2],
		0.5f,  0.5f,  0.5f,		color1[0],color1[1],color1[2],
		-0.5f,  0.5f,  0.5f,	color1[0],color1[1],color1[2],
		-0.5f,  0.5f, -0.5f,	color1[0],color1[1],color1[2],

		// light 2
		-0.5f, -0.5f, -0.5f,	color2[0],color2[1],color2[2],
		0.5f, -0.5f, -0.5f,		color2[0],color2[1],color2[2],
		0.5f,  0.5f, -0.5f,		color2[0],color2[1],color2[2],
		0.5f,  0.5f, -0.5f,		color2[0],color2[1],color2[2],
		-0.5f,  0.5f, -0.5f,	color2[0],color2[1],color2[2],
		-0.5f, -0.5f, -0.5f,	color2[0],color2[1],color2[2],

		-0.5f, -0.5f,  0.5f,	color2[0],color2[1],color2[2],
		0.5f, -0.5f,  0.5f,		color2[0],color2[1],color2[2],
		0.5f,  0.5f,  0.5f,		color2[0],color2[1],color2[2],
		0.5f,  0.5f,  0.5f,		color2[0],color2[1],color2[2],
		-0.5f,  0.5f,  0.5f,	color2[0],color2[1],color2[2],
		-0.5f, -0.5f,  0.5f,	color2[0],color2[1],color2[2],

		-0.5f,  0.5f,  0.5f,	color2[0],color2[1],color2[2],
		-0.5f,  0.5f, -0.5f,	color2[0],color2[1],color2[2],
		-0.5f, -0.5f, -0.5f,	color2[0],color2[1],color2[2],
		-0.5f, -0.5f, -0.5f,	color2[0],color2[1],color2[2],
		-0.5f, -0.5f,  0.5f,	color2[0],color2[1],color2[2],
		-0.5f,  0.5f,  0.5f,	color2[0],color2[1],color2[2],

		0.5f,  0.5f,  0.5f,		color2[0],color2[1],color2[2],
		0.5f,  0.5f, -0.5f,		color2[0],color2[1],color2[2],
		0.5f, -0.5f, -0.5f,		color2[0],color2[1],color2[2],
		0.5f, -0.5f, -0.5f,		color2[0],color2[1],color2[2],
		0.5f, -0.5f,  0.5f,		color2[0],color2[1],color2[2],
		0.5f,  0.5f,  0.5f,		color2[0],color2[1],color2[2],

		-0.5f, -0.5f, -0.5f,	color2[0],color2[1],color2[2],
		0.5f, -0.5f, -0.5f,		color2[0],color2[1],color2[2],
		0.5f, -0.5f,  0.5f,		color2[0],color2[1],color2[2],
		0.5f, -0.5f,  0.5f,		color2[0],color2[1],color2[2],
		-0.5f, -0.5f,  0.5f,	color2[0],color2[1],color2[2],
		-0.5f, -0.5f, -0.5f,	color2[0],color2[1],color2[2],

		-0.5f,  0.5f, -0.5f,	color2[0],color2[1],color2[2],
		0.5f,  0.5f, -0.5f,		color2[0],color2[1],color2[2],
		0.5f,  0.5f,  0.5f,		color2[0],color2[1],color2[2],
		0.5f,  0.5f,  0.5f,		color2[0],color2[1],color2[2],
		-0.5f,  0.5f,  0.5f,	color2[0],color2[1],color2[2],
		-0.5f,  0.5f, -0.5f,	color2[0],color2[1],color2[2],
	};



	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// vertex color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//--------------------------------------
	glm::vec3 lightPos1(10.0, 3.0, 5.0);
	glm::vec3 lightPos2(10.0f, 3.0f, -8.0f);
	//--------------------------------------



	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------

	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			glfwPollEvents();
			continue;
		}

		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		ourShader.use();

		//------------- SET LIGHTS COLOR --------------------------
		ourShader.setVec3("lightColor1", glm::vec3(color1[0], color1[1], color1[2]));
		ourShader.setVec3("lightColor2", glm::vec3(color2[0], color2[1], color2[2]));

		ourShader.setVec3("lightPos1", lightPos1);
		ourShader.setVec3("lightPos2", lightPos2);

		ourShader.setVec3("viewPos", camera.Position);



		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);



		// render the loaded models
		//-------------------------------------------
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 10.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(40.0f, 40.0f, 40.0f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);

		skyBox.Draw(ourShader);

		//-------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.6f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		ourShader.setMat4("model", model);


		player[idx].Draw(ourShader);
		//toru.Draw(ourShader);

		flip++;
		flip = flip % 8;
		if (flip == 7) {
			idx++;
			flip = 0;
		}
		if (idx > 10) idx = 10;

		//-------------------------------------------

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(xTrack, yTrack, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.000007f, 0.000007f, 0.000007f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);

		ball.Draw(ourShader);

		float rightLimit = 21.0f;
		if (!coll && xTrack < rightLimit && idx >= 10) {
			xTrack += 0.3;
			yTrack = ballY(xTrack);
		}
		else if (xTrack >= rightLimit) {
			coll = true;
		}

		float leftLimit = 1.0f;
		float mid = 15.0f;
		if (coll && xTrack > leftLimit) {
			xTrack -= 0.1;

			if (xTrack > mid)
				yTrack = (((xTrack - 15)*(xTrack - 15)) / 10.0) - 1.25;
			else
				yTrack = -1.25f;
			/*

			*/

		}
		else if (xTrack < leftLimit) {
			coll = false;
			idx = 0;
		}
		///--------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scen
		model = glm::scale(model, glm::vec3(0.05f, 0.0f, 0.05f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f)); ///
		ourShader.setMat4("model", model);

		ground.Draw(ourShader);
		//----------------------------------------------------------------


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, -1.75f, -5.0f)); // translate it down so it's at the center of the scen
		model = glm::scale(model, glm::vec3(0.00005f, 0.00005f, 0.00005f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, 4.71f, glm::vec3(1.0f, 0.0f, 0.0f)); ///
		ourShader.setMat4("model", model);

		table.Draw(ourShader);
		//--------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(rightLimit - 1, -2.0f, 0.0f)); // translate it down so it's at the center of the scen
		model = glm::scale(model, glm::vec3(0.000002f, 0.000003f, 0.000002f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, -190.065f, glm::vec3(1.0f, 0.0f, 0.0f)); ///
		ourShader.setMat4("model", model);

		football_goal.Draw(ourShader);

		//----------------------------------------------------------------
		ourShader2.use();
		ourShader2.setMat4("projection", projection);
		ourShader2.setMat4("view", view);
		glBindVertexArray(VAO);

		//--------- move light source ------------------------------
		if (b) temp += 0.1;
		else temp -= 0.1;

		if (temp >= 10 || temp <= -10) b = !b;

		lightPos2 = glm::vec3(temp, lightPos2.y, lightPos2.z);
		//----------------------------------------------------------

		// draw light 1
		ourShader2.setFloat("opacity", 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, lightPos1);
		ourShader2.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 6, 42);

		// draw light 2
		ourShader2.setFloat("opacity", 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, lightPos2);
		ourShader2.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 42, 78);

		// draw the glass
		ourShader2.setFloat("opacity", 0.5f);
		rr += 0.01f;
		if (rr > 360.0f) rr = 0.0;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 2.2f, -5.0f));
		model = glm::rotate(model, rr, glm::vec3(0.0f, 1.0f, 0.0f));
		ourShader2.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//----------------------------------------------------------------

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
// calculate y pos of ball
float ballY(float x) {
	float a = -0.08625f;
	float b = 1.96f;
	float c = -3.12375f;
	float x2 = x*x;

	return (a*x2) + (b*x) + c;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		coll = false;
		xTrack = 1.0f;
		yTrack = -1.25f;
		idx = 0, flip = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

