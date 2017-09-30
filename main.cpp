#pragma once
// Math constants
#define _USE_MATH_DEFINES
#include <cmath>  
#include <random>

// Std. Includes
#include <string>
#include <time.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include "glm/ext.hpp"


// Other Libs
#include "SOIL2/SOIL2.h"

// project includes
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include "Body.h"
#include "Particle.h"


// time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



// main function
int main()
{
	// create application
	Application app = Application::Application();
	app.initRender();
	Application::camera.setCameraPosition(glm::vec3(0.0f, 5.0f, 20.0f));
			
	// create ground plane
	Mesh plane = Mesh::Mesh();
	Mesh plane2 = Mesh::Mesh();

	plane.scale(glm::vec3(10.0f, 10.0f, 10.0f));
	plane.setShader(Shader("resources/shaders/core.vert", "resources/shaders/core.frag"));
	plane2.translate(glm::vec3(0.0f, 5.0f, -5.0f));
	plane2.rotate(1.57f, glm::vec3(1.0f, 0.0f, 0.0f));
	plane2.scale(glm::vec3(10.0f, 10.0f, 10.0f));
	plane2.setShader(Shader("resources/shaders/core.vert", "resources/shaders/core.frag"));
	// create particles
	Particle balls[3];
	glm::vec3 velocity[sizeof(balls) / sizeof(balls[0])];
	for (int i = 0; i < sizeof(balls) / sizeof(balls[0]); i++)
	{
		velocity[i] = glm::vec3(3.0f, 2.0f, 1.0f);
		balls[i] = Particle::Particle();
		balls[i].setPos(glm::vec3(0.0f+1.0f*i, 7.5f, 0.0f));
		balls[i].getMesh().setShader(Shader("resources/shaders/core.vert", "resources/shaders/core_blue.frag"));
	}
	// time
	const double deltaTime = 0.00005;
	GLfloat currentTime = (GLfloat)glfwGetTime();
	double accumulator = 0.0;
	
	//Newton
	glm::vec3 allForces;
	glm::vec3 airDragForce;
	glm::vec3 gravityForce;
	glm::vec3 windForce;
	glm::vec3 g = glm::vec3(0.0f, -9.8f, 0.0f);
	float mass = 1.0f;
	
	glm::vec3 acceleration;
	// Game loop
	while (!glfwWindowShouldClose(app.getWindow()))
	{
		// Set frame time
		GLfloat newTime = (GLfloat)glfwGetTime();
		// the animation can be sped up or slowed down by multiplying currentFrame by a factor.
		GLfloat frameTime = newTime - currentTime;
		currentTime = newTime;		
		accumulator += frameTime;

		/*
		**	INTERACTION
		*/
		// Manage interaction
		app.doMovement(deltaTime);

		while (accumulator >= deltaTime)
		{
			/*
			**	SIMULATION
			*/
			//windForce = glm::vec3(2.0f, 0.0f, 0.0f);

			gravityForce = mass * g;

			//airDragForce = (1.25f * .47f * velocity[1]*velocity[1]) / 2.0f;

			allForces = /*windForce + */ gravityForce;// +airDragForce;

			acceleration = allForces / mass;
			velocity[1] += deltaTime*acceleration;

			balls[2].setVel(velocity[2]);
			balls[2].translate(balls[2].getVel()*deltaTime); //Forward Euler

			velocity[2] += deltaTime*acceleration;
			for (int i = 0; i < sizeof(balls)/sizeof(balls[0]); i++)
			{		

				

				if (balls[i].getPos().y < 0.00f)
				{
					balls[i].setPos(1, 0.0f);
					velocity[i].y *= -0.80f;
					velocity[i].z *= 0.98f;
					velocity[i].x *= 0.98f;
				}else if (balls[i].getPos().y > 10.00f)
				{

					balls[i].setPos(1, 10.0f);
					velocity[i].y *= -1.0f;
				}
				if (balls[i].getPos().x < -5.0f)
				{

					balls[i].setPos(0, -5.0f);
					velocity[i].x *= -1.0f;
				}else if (balls[i].getPos().x > 5.0f)
				{

					balls[i].setPos(0, 5.0f);
					velocity[i].x *= -1.0f;
				}
				if (balls[i].getPos().z > 5.0f)
				{

					balls[i].setPos(2, 5.0f);
					velocity[i].z *= -1.0f;
				}else if (balls[i].getPos().z < -5.0f)
				{

					balls[i].setPos(2, -5.0f);
					velocity[i].z *= -1.0f;
				}
			
				//balls[i]balls[i].getVel()*deltaTime);
				
			}
			balls[1].setVel(velocity[1]);
		
			balls[1].translate(balls[1].getVel()*deltaTime); //Semi-Implicit Euler

			accumulator -= deltaTime;
		}
		/*
		**	RENDER 
		*/		
		// clear buffer
		app.clear();
		// draw groud plane
		app.draw(plane);
		app.draw(plane2);
		// draw particles
		for (int i = 0; i < sizeof(balls) / sizeof(balls[0]); i++)
		app.draw(balls[i].getMesh());
		
		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

