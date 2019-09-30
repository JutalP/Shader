#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Windows.h>
#include "Material.h"
#include "Mesh.h"
#include "Transform.h"
#include "Texture.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "Screen.h"

bool ShouldQuit = false;
bool KeyState[GLFW_KEY_LAST] = { false };
Camera TheCamera;

double MouseX = 0.0;
double MouseY = 0.0;

bool CameraMovementActive = false;

Mesh TriangleMesh;
Mesh QuadMesh;
Mesh CubeMesh;
Material DefaultMaterial;
Texture CherylTexture;

const float CubeVertexData[] = {
	// FRONT FACE
	-1.f, -1.f, +1.f,		0.f, 0.f, 1.f,		0.f, 0.f,	// 0
	+1.f, -1.f, +1.f,		0.f, 0.f, 1.f,		1.f, 0.f,	// 1
	+1.f, +1.f, +1.f,		0.f, 0.f, 1.f,		1.f, 1.f,	// 2
	-1.f, +1.f, +1.f,		0.f, 0.f, 1.f,		0.f, 1.f,	// 3

	// BACK FACE
	-1.f, -1.f, -1.f,		0.f, 0.f, -1.f,		0.f, 0.f,	// 4
	+1.f, -1.f, -1.f,		0.f, 0.f, -1.f,		1.f, 0.f,	// 5
	+1.f, +1.f, -1.f,		0.f, 0.f, -1.f,		1.f, 1.f,	// 6
	-1.f, +1.f, -1.f,		0.f, 0.f, -1.f,		0.f, 1.f,	// 7

	// RIGHT FACE
	+1.f, -1.f, +1.f,		1.f, 0.f, 0.f,		0.f, 0.f,	// 8
	+1.f, -1.f, -1.f,		1.f, 0.f, 0.f,		1.f, 0.f,	// 9
	+1.f, +1.f, -1.f,		1.f, 0.f, 0.f,		1.f, 1.f,	// 10
	+1.f, +1.f, +1.f,		1.f, 0.f, 0.f,		0.f, 1.f,	// 11

	// LEFT FACE
	-1.f, -1.f, +1.f,		-1.f, 0.f, 0.f,		0.f, 0.f,	// 12
	-1.f, -1.f, -1.f,		-1.f, 0.f, 0.f,		1.f, 0.f,	// 13
	-1.f, +1.f, -1.f,		-1.f, 0.f, 0.f,		1.f, 1.f,	// 14
	-1.f, +1.f, +1.f,		-1.f, 0.f, 0.f,		0.f, 1.f,	// 15

	// TOP FACE
	-1.0, +1.f, +1.f,		0.f, 1.f, 0.f,		0.f, 0.f,	// 16
	+1.f, +1.f, +1.f,		0.f, 1.f, 0.f,		1.f, 0.f,	// 17
	+1.f, +1.f, -1.f,		0.f, 1.f, 0.f,		1.f, 1.f,	// 18
	-1.f, +1.f, -1.f,		0.f, 1.f, 0.f,		0.f, 1.f,	// 19

	// BOTTOM FACE
	-1.0, -1.f, +1.f,		0.f, -1.f, 0.f,		0.f, 0.f,	// 20
	+1.f, -1.f, +1.f,		0.f, -1.f, 0.f,		1.f, 0.f,	// 21
	+1.f, -1.f, -1.f,		0.f, -1.f, 0.f,		1.f, 1.f,	// 22
	-1.f, -1.f, -1.f,		0.f, -1.f, 0.f,		0.f, 1.f,	// 23
};

const unsigned int CubeIndexData[] = {
	// FRONT FACE
	0, 1, 2,	0, 2, 3,
	// BACK FACE
	4, 5, 6,	4, 6, 7,
	// RIGHT FACE
	8, 9, 10,	8, 10, 11,
	// LEFT FACE
	12, 13, 14,	12, 14, 15,
	// TOP FACE
	16, 17, 18, 16, 18, 19,
	// BOTTOM FACE
	20, 21, 22, 20, 22, 23,
};


bool IsKeyPressed(int Key)
{
	return KeyState[Key];
}

void OnKeyEvent(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
{
	if (Action == GLFW_REPEAT)
		return;

	KeyState[Key] = (Action == GLFW_PRESS); 

	if (Action == GLFW_PRESS)
	{
		printf("OnKeyEvent( %d )\n", Key);

		if (Key == GLFW_KEY_ESCAPE)
			ShouldQuit = true;
	}
}

void OnMouseButton(GLFWwindow* Window, int Button, int Action, int Mods)
{
	if (Button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		CameraMovementActive = (Action == GLFW_PRESS);
		glfwSetInputMode(Window, GLFW_CURSOR, CameraMovementActive ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}
}

void OnCursorMove(GLFWwindow* Window, double X, double Y)
{
	MouseX = X;
	MouseY = Y;
}

void RenderQuad(const Transform& Transform)
{
	QuadMesh.Bind();
	DefaultMaterial.Set("u_World", Transform.GetMatrix());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void RenderTriangle(const Transform& Transform)
{
	TriangleMesh.Bind();
	DefaultMaterial.Set("u_World", Transform.GetMatrix());
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void RenderCube(const Transform& Transform)
{
	CubeMesh.Bind();
	DefaultMaterial.Set("u_World", Transform.GetMatrix());
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void RenderFullscreenQuad()
{
	glBindVertexArray(0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void RenderScene(bool RenderPlayer)
{
	CherylTexture.Bind();

	float Time = glfwGetTime();

	// Render floor
	RenderCube(Transform(glm::vec3(0.f, -2.f, 0.f), quat_identity, glm::vec3(50.f, 0.4f, 50.f)));

	// Child and baby cubes
	{
		Transform BaseTransform;
		BaseTransform.Position = glm::vec3(0.f, 0.f, glm::sin(Time) * 5.f);
		BaseTransform.Rotation = glm::angleAxis(Time, glm::vec3(0.f, 1.f, 0.f));

		Transform ChildTransform;
		ChildTransform.Position = glm::vec3(2.f + glm::sin(Time), 0.f, 0.f);
		ChildTransform.Scale = glm::vec3(0.5f);
		ChildTransform.Rotation = glm::angleAxis(Time, glm::vec3(1.f, 0.f, 0.f));

		Transform BabyTransform;
		BabyTransform.Position = glm::vec3(0.f, 2.5f, 0.f);
		BabyTransform.Scale = glm::vec3(0.5f);

		RenderCube(BaseTransform);
		RenderCube(BaseTransform * ChildTransform);
		RenderCube(BaseTransform * ChildTransform * BabyTransform);
	}

	// Weird saw thing MonkaS
	{
		Transform BaseTransform;
		BaseTransform.Position = glm::vec3(3.f, 2.f, 1.f);
		BaseTransform.Rotation = glm::angleAxis(Time, glm::normalize(glm::vec3(0.5f, -0.4f, 1.f)));

		RenderCube(BaseTransform);

		int NumTriangles = 30;
		for (int i = 0; i < NumTriangles; ++i)
		{
			float AngleInc = (glm::pi<float>() * 2.f) / NumTriangles;
			float Angle = Time + AngleInc * i;
			float SawAngle = Time * 5.f + AngleInc * i;

			Transform ChildTransform;
			ChildTransform.Position = glm::vec3(2.5f, sin(Time * 0.6f + AngleInc * i * 2.f) * 1.f, 0.f);
			ChildTransform.Scale = glm::vec3(0.5f);

			Transform SpinTransform;
			SpinTransform.Rotation = glm::angleAxis(Angle, glm::vec3(0.f, 1.f, 0.f));

			Transform SawTransform;
			SawTransform.Rotation = glm::angleAxis(SawAngle, glm::vec3(0.f, 0.f, 1.f));

			RenderCube(BaseTransform * SpinTransform * ChildTransform * SawTransform);
		}
	}

	{
		Transform CurrentTransform;
		CurrentTransform.Position = glm::vec3(-4.f, 3.f, -2.f);
		RenderCube(CurrentTransform);

		int Iterations = 40;
		for (int i = 0; i < Iterations; ++i)
		{
			Transform Delta;
			Delta.Position = glm::vec3(-1.5f, 0.f, 0.f);
			Delta.Scale = glm::vec3(0.9f);
			Delta.Rotation = glm::angleAxis(sin(-Time + i * 0.04f) * 0.2f, glm::vec3(0.f, 0.f, 1.f));

			Transform TwirlDelta;
			TwirlDelta.Rotation = glm::angleAxis(sin(-Time + i * 0.25f) * 0.15f, glm::vec3(0.f, 1.f, 0.f));

			Transform TwistDelta;
			TwistDelta.Rotation = glm::angleAxis(sin(-Time * 0.4f + i * 0.21f) * 0.12f, glm::vec3(1.f, 0.f, 0.f));

			CurrentTransform = CurrentTransform * TwirlDelta * Delta * TwistDelta;
			RenderCube(CurrentTransform);
		}
	}

	// Render the player
	if (RenderPlayer)
		RenderCube(Transform(TheCamera.Position));
}

int main()
{
	/* WINDOW STUFF */
	// Initialize GLFW, create a window and context
	glfwInit();

	GLFWwindow* Window;
	Window = glfwCreateWindow(ScreenWidth, ScreenHeight, "This is OpenGL my dude", nullptr, nullptr);

	glfwSetKeyCallback(Window, OnKeyEvent);
	glfwSetCursorPosCallback(Window, OnCursorMove);
	glfwSetMouseButtonCallback(Window, OnMouseButton);
	glfwMakeContextCurrent(Window);
	glfwSwapInterval(0);

	// After the context is bound, we can import the OpenGL extensions, through the extension wrangler
	glewInit();

	/* VERTEX STUFF */

	// Triangle, bejbii
	float TriData[] = {
		-0.5f, -0.5f, 0.0f,		0.f, 0.f, 1.f,		0.0f, 0.0f,
		+0.5f, -0.5f, 0.0f,		0.f, 0.f, 1.f,		1.0f, 0.0f,
		+0.0f, +0.5f, 0.0f,		0.f, 0.f, 1.f,		0.5f, 1.0f,
	};

	TriangleMesh.LoadVerts(TriData, sizeof(TriData), nullptr, 0);

	float QuadData[] = {
		-0.5f, -0.5f, 0.f,		0.f, 0.f, 1.f,		0.f, 0.f,
		+0.5f, -0.5f, 0.f,		0.f, 0.f, 1.f,		1.f, 0.f,
		-0.5f, +0.5f, 0.f,		0.f, 0.f, 1.f,		0.f, 1.f,
		+0.5f, +0.5f, 0.f,		0.f, 0.f, 1.f,		1.f, 1.f,
	};

	QuadMesh.LoadVerts(QuadData, sizeof(QuadData), nullptr, 0);

	CubeMesh.LoadVerts(CubeVertexData, sizeof(CubeVertexData), CubeIndexData, sizeof(CubeIndexData));

	DefaultMaterial.LoadFiles("Res/Shader/default.vert", "Res/Shader/default.frag");
	DefaultMaterial.Use();

	Material PostProcessMaterial;
	PostProcessMaterial.LoadFiles("Res/Shader/post_process.vert", "Res/Shader/post_process.frag");
	PostProcessMaterial.Set("u_FrameColor", 0);
	PostProcessMaterial.Set("u_FrameDepth", 1);

	CherylTexture.LoadFile("Res/brick.jpg");
	CherylTexture.Bind();

	float Ratio = (float)ScreenWidth / ScreenHeight;
	float OrthoSize = 2.f;

	TheCamera.Position = glm::vec3(0.f, 0.f, 5.f);

	DefaultMaterial.Set("u_LightDirection", glm::normalize(glm::vec3(-1.f, -1.f, -1.f)));

	glm::mat4 Projection;
	Projection = glm::perspective(glm::radians(60.f), Ratio, 0.2f, 100.f);
	DefaultMaterial.Set("u_Projection", Projection);

	glEnable(GL_DEPTH_TEST);
	float LastFrameTime = 0.f;

	double LastMouseX = 0.0;
	double LastMouseY = 0.0;

	FrameBuffer FB;
	FB.Create(2000, 2000);

	// Main loop of the program
	while (!glfwWindowShouldClose(Window) && !ShouldQuit)
	{
		DefaultMaterial.Use();

		float CurrentTime = glfwGetTime();
		float DeltaTime = CurrentTime - LastFrameTime;
		LastFrameTime = CurrentTime;

		double MouseDeltaX = MouseX - LastMouseX;
		double MouseDeltaY = MouseY - LastMouseY;
		LastMouseX = MouseX;
		LastMouseY = MouseY;

		if (CameraMovementActive)
		{
			TheCamera.AddYaw(-MouseDeltaX * 0.05f);
			TheCamera.AddPitch(-MouseDeltaY * 0.05f);

			glm::vec3 CameraMovement = glm::vec3(0.f);
			if (IsKeyPressed(GLFW_KEY_W))
				CameraMovement += TheCamera.GetForwardVector();
			if (IsKeyPressed(GLFW_KEY_S))
				CameraMovement += -TheCamera.GetForwardVector();
			if (IsKeyPressed(GLFW_KEY_A))
				CameraMovement += -TheCamera.GetRightVector();
			if (IsKeyPressed(GLFW_KEY_D))
				CameraMovement += TheCamera.GetRightVector();

			if (IsKeyPressed(GLFW_KEY_E))
				CameraMovement += glm::vec3(0.f, 1.f, 0.f);
			if (IsKeyPressed(GLFW_KEY_Q))
				CameraMovement -= glm::vec3(0.f, 1.f, 0.f);

			TheCamera.Position += CameraMovement * 5.f * DeltaTime;
		}

		DefaultMaterial.Set("u_View", TheCamera.GetViewMatrix());
		DefaultMaterial.Set("u_EyePosition", TheCamera.Position);

		// Clear the screen!
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);

		// Render to Frame Buffer
		FB.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderScene(false);

		FB.Unbind();

		// Render to screen!
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//FB.ColorTexture.Bind();
		//RenderQuad(Transform(glm::vec3(0.f, 20.f, -20.f), quat_identity, glm::vec3(40.f)));

		PostProcessMaterial.Use();
		FB.ColorTexture.Bind(0);
		FB.DepthTexture.Bind(1);
		RenderFullscreenQuad();


		// Swap the back-buffer to the front and poll and handle window events
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	return 0;
}
