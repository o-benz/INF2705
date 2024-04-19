#ifndef VERTICES_DATA_H
#define VERTICES_DATA_H

// Merge avec le fichier du tp1

const GLfloat cubeVertices[] = {
    // +z
    -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
     // -z
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
     // +x
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
     // -x
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
     // +y
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
     // -y
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
};

const GLuint cubeIndexes[] = {
     0,  1,  2,
     1,  3,  2,

     4,  5,  6,
     5,  7,  6,

     8,  9, 10,
     9, 11, 10,

    12, 13, 14,
    13, 15, 14,

    16, 17, 18,
    17, 19, 18,

    20, 21, 22,
    21, 23, 22,
};

const GLfloat groundVertices[] = {
	-30.0f, -1.0f, -30.0f, 0.0f, 0.0f,
	-30.0f, -1.0f,  30.0f, 0.0f, 20.0f,
	 30.0f, -1.0f, -30.0f, 20.0f, 0.0f,
	 30.0f, -1.0f,  30.0f, 20.0f, 20.0f,
};

const GLuint groundIndexes[] = {
	0, 1, 2,
	1, 3, 2,
};

const GLfloat riverVertices[] = {
	-30.0f, -1.0f, -50.0f, 0.0f, 0.0f,
	-30.0f, -1.0f, -30.0f, 0.0f, 2.0f,
	 30.0f, -1.0f, -50.0f, 5.0f, 0.0f,
	 30.0f, -1.0f, -30.0f, 5.0f, 2.0f,
};

const GLuint riverIndexes[] = {
	0, 1, 2,
	1, 3, 2,
};

const GLfloat hudVertices[] = {
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
};

const GLuint hudIndexes[] = {
	0, 1, 2,
	1, 3, 2,
};

const GLfloat skyboxVertices[] = {
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

const char* skyboxPathsindexes[] = {
	"../textures/skybox/Daylight Box_Right.bmp",
	"../textures/skybox/Daylight Box_Left.bmp",
	"../textures/skybox/Daylight Box_Top.bmp",
	"../textures/skybox/Daylight Box_Bottom.bmp",
	"../textures/skybox/Daylight Box_Front.bmp",
	"../textures/skybox/Daylight Box_Back.bmp",
};

#endif // VERTICES_DATA_H
