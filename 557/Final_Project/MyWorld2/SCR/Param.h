#pragma once

#include <iostream>
#include <cmath>
#include<map>
#include<vector>
#include<string>
#include <windows.h>
#include <gl/GLUT.h>
#include "Point.h"
#include "Robot.h"
#include "Man.h"
#include "Cube.h"
#include "Digitalv.h"
#include "TexCube.h"


using namespace std;

#define DELAY 12
#define NUM_OF_LINE 32
#define BLOCK_SIZE 1
#define VIEW_SCALE 2
#define PI 3.1415926535898
#define MAX_CHAR 128



//const double INCREMENT = 0.05;

// World
Point center = Point(0, 0, 0);
Point lookAt = Point(5, 5, 5);
Point cameraAt = Point(5, 5, 5);
Point godAt = Point(8, 11, -18);

float last_mm_x = 0;
float last_mm_y = 0;

bool reset_god = false;
int view_person = 0;  // 0 Aerial view£¬1 First-person perspective£¬2 Third-person perspective
int last_view = 0;
enum { GOD, FIRST_PERSON, THIRD_PERSON, BALL };
enum { HEAD, HAIR, BODY, THIGH_L, THIGH_R, ARM_L, ARMR, EYE_L, EYE_R, MOUTH };

float near_sight = 0.1f;
float far_sight = 100;
int scr_w;
int scr_h;

// 
bool trackingMouse = false;
bool redrawContinue = false;
bool trackballMove = false;

// angle change
float angle = 0.0, axis[3], trans[3];
float lastPos[3] = { 0.0F, 0.0F, 0.0F };
float look_dy = 0;
int curx, cury;
int startX, startY;

//plane 
static int xRot = 0.;
static int yRot = 0;
static int zRot = 0;
static double x_air, y_air, z_air;
static double fly_distance = 20;

static float fly_speed = 0.15;
bool flying = false;
bool spining = false;

// man
Man man;
vector<Robot> crawler_arr;
vector<Point> torch_arr;

// jumping
bool jumping = false;
bool falling = false;
float jump_speed = 0.3;
float fall_speed = 0.3;
static float max_height = 3.0;
static float ground_height = 1.00;


//relative loc
bool left_forward = true;
bool right_forward = false;
bool look_from_left = true;



//sun
GLfloat s_angle = 0.0f;
GLfloat LightPosition[] = { 0.0f,0.0f,0.0f,1.0f };		// LightPosition

GLfloat ambient = 0.2f;
GLfloat LightAmbient[] = { ambient,ambient,ambient,1.0f };		//LightAmbient
GLfloat diffuse = 0.2f;
GLfloat LightDiffuse[] = { diffuse,0,0,1.0f };		//LightDiffuse
GLfloat specular = 0.2f;
GLfloat LightSpecular[] = { specular,0.6f,0.6f,1.0f };	//LightSpecular

														
//matEmission
GLfloat matEmissionR, matEmissionG, matEmissionB;
GLfloat	MatEmission[] = { matEmissionR, matEmissionG, matEmissionB, 1.0f };
GLfloat matAmbientR, matAmbientG, matAmbientB;
GLfloat	MatAmbient[] = { matAmbientR, matAmbientG, matAmbientB, 1.0f };
GLfloat matDiffuseR, matDiffuseG, matDiffuseB;
GLfloat MatDiffuse[] = { matDiffuseR, matDiffuseG, matDiffuseB, 1.0f };
GLfloat matSpecularR, matSpecularG, matSpecularB;
GLfloat MatSpecular[] = { matSpecularR, matSpecularG, matSpecularB, 1.0f };
GLfloat MatShininess;

//window size
//todo
int WindowWidth = 600;
int WindowHeight = 600;
#include"Lable.h"

