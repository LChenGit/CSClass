//
//  main_spotlight.cpp
//  HCI 557 Spotlight example
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//

// stl include
#include <iostream>
#include <string>
#include <algorithm>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "Sphere3D.h"
#include "GLAppearance.h"




using namespace std;


// The handle to the window object
GLFWwindow*         window;

// Define some of the global variables we're using for this sample
GLuint program;

/* A trackball to move and rotate the camera view */
extern Trackball trackball;



int main(int argc, const char * argv[])
{


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Init glfw, create a window, and init glew

	// Init the GLFW Window
	window = initWindow();


	// Init the glew api
	initGlew();



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Create some models

	// coordinate system
	CoordSystem* cs = new CoordSystem(40.0);


	// create 4 apperance objects.
	GLAppearance* apperance_red = new GLAppearance("../../data/shaders/multi_vertex_lights.vs", "../../data/shaders/multi_vertex_lights.fs");
	GLAppearance* apperance_blue = new GLAppearance("../../data/shaders/multi_vertex_lights.vs", "../../data/shaders/multi_vertex_lights.fs");
	GLAppearance* apperance_green = new GLAppearance("../../data/shaders/multi_vertex_lights.vs", "../../data/shaders/multi_vertex_lights.fs");
	GLAppearance* apperance_gold = new GLAppearance("../../data/shaders/multi_vertex_lights.vs", "../../data/shaders/multi_vertex_lights.fs");


	//create lights
	//direct light for red ball
	GLDirectLightSource  light_source_red;
	light_source_red._lightPos = glm::vec4(0.0, 2.0, 7.0, 1.0);
	light_source_red._ambient_intensity = 0.3;
	light_source_red._specular_intensity = 6.0;
	light_source_red._diffuse_intensity = 6.0;
	light_source_red._attenuation_coeff = 0.02;
	//light_source1._cone_angle = 12.0; // in degree
	//light_source1._cone_direction = glm::vec3(-1.0, -1.0, 0.0); // this must be aligned with the object and light position.

	//direct light for blue ball
	GLDirectLightSource  light_source_blue;
	light_source_blue._lightPos = glm::vec4(2.0, 0.0, 7.0, 1.0);
	light_source_blue._ambient_intensity = 0.1;
	light_source_blue._specular_intensity = 5.5;
	light_source_blue._diffuse_intensity = 1.0;
	light_source_blue._attenuation_coeff = 0.02;

	//spot light for green ball
	GLSpotLightSource  light_source_greenspot;
	light_source_greenspot._lightPos = glm::vec4(10.0, 3.0, 10.0, 1.0);
	light_source_greenspot._ambient_intensity = 0.0;
	light_source_greenspot._specular_intensity = 1.5;
	light_source_greenspot._diffuse_intensity = 1.0;
	light_source_greenspot._attenuation_coeff = 0.02;
	light_source_greenspot._cone_angle = 20.0; // in degree
	light_source_greenspot._cone_direction = glm::vec3(-1.0, 0.0, -2.0); // this must be aligned with the object and light position.

    //direct light for gold ball
	GLDirectLightSource  light_source_gold;
	light_source_gold._lightPos = glm::vec4(25.0, 5.0, 15.0, 1.0);
	light_source_gold._ambient_intensity = 0.2;
	light_source_gold._specular_intensity = 5.0;
	light_source_gold._diffuse_intensity = 1.0;
	light_source_gold._attenuation_coeff = 0.02;

	//spot light for gold ball
	GLSpotLightSource  light_source_goldspot;
	light_source_goldspot._lightPos = glm::vec4(20.0, 10.0, 7.0, 1.0);
	light_source_goldspot._ambient_intensity = 0.1;
	light_source_goldspot._specular_intensity = 5.0;
	light_source_goldspot._diffuse_intensity = 1.0;
	light_source_goldspot._attenuation_coeff = 0.02;
	light_source_goldspot._cone_angle = 9.0; // in degree
	light_source_goldspot._cone_direction = glm::vec3(-1.0, -1.5, -1.0); // this must be aligned with the object and light position.


	// add the spot light to this apperance object
	apperance_red->addLightSource(light_source_red);
	apperance_blue->addLightSource(light_source_blue);
	apperance_green->addLightSource(light_source_greenspot);
	apperance_gold->addLightSource(light_source_gold);
	apperance_gold->addLightSource(light_source_goldspot);

	// Create a material object
	GLMaterial material_red;
	material_red._diffuse_material = glm::vec3(1.5, 0.0, 0.0);
	material_red._ambient_material = glm::vec3(0.2, 0.0, 0.0);
	material_red._specular_material = glm::vec3(1.0, 1.0, 1.0);
	material_red._shininess = 50.0;

	GLMaterial material_blue;
	material_blue._diffuse_material = glm::vec3(0.0, 0.0, 1.5);
	material_blue._ambient_material = glm::vec3(0.0, 0.0, 0.2);
	material_blue._specular_material = glm::vec3(0.0, 0.0, 0.0);
	material_blue._shininess = 12.0;

	GLMaterial material_green;
	material_green._diffuse_material = glm::vec3(0.0, 0.8, 0.2);
	material_green._ambient_material = glm::vec3(0.0, 0.0, 0.0);
	material_green._specular_material = glm::vec3(1.0, 1.0, 1.0);
	material_green._shininess = 50.0;

	GLMaterial material_gold;
	material_gold._diffuse_material = glm::vec3(0.8, 0.8, 0.0);
	material_gold._ambient_material = glm::vec3(0.8, 0.8, 0.0);
	material_gold._specular_material = glm::vec3(1.0, 1.0, 1.0);
	material_gold._shininess = 30.0;

	// Add the material to the apperance object
	apperance_red->setMaterial(material_red);
	apperance_red->finalize();

	apperance_blue->setMaterial(material_blue);
	apperance_blue->finalize();

	apperance_green->setMaterial(material_green);
	apperance_green->finalize();

	apperance_gold->setMaterial(material_gold);
	apperance_gold->finalize();

	// create the sphere geometry
	GLSphere3D* sphere_red = new GLSphere3D(-15, 0.0, 0.0, 3.0, 90, 50);
	GLSphere3D* sphere_blue = new GLSphere3D(-5, 0.0, 0.0, 3.0, 90, 50);
	GLSphere3D* sphere_green = new GLSphere3D(5, 0.0, 0.0, 3.0, 180, 180);
	GLSphere3D* sphere_gold = new GLSphere3D(15, 0.0, 0.0, 3.0, 90, 50);
	sphere_red->setApperance(*apperance_red);
	sphere_red->init();

	sphere_blue->setApperance(*apperance_blue);
	sphere_blue->init();

	sphere_green->setApperance(*apperance_green);
	sphere_green->init();

	sphere_gold->setApperance(*apperance_gold);
	sphere_gold->init();


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Main render loop

	// Set up our green background color
	static const GLfloat clear_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// This sets the camera to a new location
	// the first parameter is the eye position, the second the center location, and the third the up vector. 
	SetViewAsLookAt(glm::vec3(12.0f, 12.0f, 35.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


	// Enable depth test
	// ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	// sphere->enableNormalVectorRenderer();

	// This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
	while (!glfwWindowShouldClose(window))
	{

		// Clear the entire buffer with our green color (sets the background to be green).
		glClearBufferfv(GL_COLOR, 0, clear_color);
		glClearBufferfv(GL_DEPTH, 0, clear_depth);


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// This renders the objects

		// Set the trackball locatiom
		SetTrackballLocation(trackball.getRotationMatrix());

		// draw the objects
		cs->draw();

		sphere_red->draw();
		sphere_blue->draw();
		sphere_green->draw();
		sphere_gold->draw();

		//// This renders the objects
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		// Swap the buffers so that what we drew will appear on the screen.
		glfwSwapBuffers(window);
		glfwPollEvents();

	}


	delete cs;


}

