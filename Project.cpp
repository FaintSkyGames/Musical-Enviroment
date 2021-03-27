#include "GL/glew.h"
#include "GL/freeglut.h"

// GLFW Library
#include "GLFW/glfw3.h"

#include "FileReader.h"

// GLM Library
#include <glm/glm.hpp> //includes GLM
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <fstream>
#include<vector>

#include "Project.h"

// To avoid typing std over and over
using namespace std;
#include <cstdlib>
#include <map>
#include <string>
#include <array>

FileReader reader = FileReader();
vector<float> songVertexData;
int pointsStart = 0;
int numberOfFrequences = 0;
int numberOfVertices = 0;
int pointsToShow = 0;
bool keepIncreasing = true;
string songName = "test";
string selectedShader = "white";
GLuint texture1;

float normaliseValue(float value, float min, float max);

//----------------------------------------------------------------------------
//
// Shaders & Textures
//


const GLchar* whiteVertexSource = R"glsl(
    #version 150 core
    in vec2 position;

    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

const GLchar* whiteFragmentSource = R"glsl(
    #version 150 core
    out vec4 outColor;

    void main()
    {
        outColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
)glsl";

const GLchar* blackVertexSource = R"glsl(
	#version 150 core
    in vec2 position;
    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

const GLchar* blackFragmentSource = R"glsl(
	#version 150 core
    out vec4 outColor;
    void main()
    {
        outColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
)glsl";
const GLchar* greenVertexSource = R"glsl(
	#version 150 core
    in vec2 position;
    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

const GLchar* greenFragmentSource = R"glsl(
	#version 150 core
    out vec4 outColor;
    void main()
    {
        outColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
)glsl";
const GLchar* redVertexSource = R"glsl(
	#version 150 core
    in vec2 position;
    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

const GLchar* redFragmentSource = R"glsl(
	#version 150 core
    out vec4 outColor;
    void main()
    {
        outColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
)glsl";
const GLchar* blueVertexSource = R"glsl(
	#version 150 core
    in vec2 position;
    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

const GLchar* blueFragmentSource = R"glsl(
	#version 150 core
    out vec4 outColor;
    void main()
    {
        outColor = vec4(0.0, 0.0, 1.0, 1.0);
    }
)glsl";


static GLuint CompileShader(const string& source, GLuint type) {
	GLuint id = glCreateShader(type);
	const char* src = source.c_str(); // Points to start of string
	glShaderSource(id, 1, &src, nullptr); // (Shader, num of elements, pointer to the pointer, length)
	glCompileShader(id);
	return id;
}

static void CreateShader(const string& vertexShader, const string& fragmentShader ) {
	GLuint program = glCreateProgram();
	GLuint vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
	GLuint fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	// Deleting as now in program
	glDeleteShader(vs);
	glDeleteShader(fs);

	glUseProgram(program);
}

void whiteShader() {
	// Create vetex shader object and load data
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &whiteVertexSource, NULL); // (object, location, array of source code string lengths or NULL)
	glCompileShader(vertexShader);

	// Create fragment shader object and load data
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &whiteFragmentSource, NULL); // (object, location, array of source code string lengths or NULL)
	glCompileShader(fragmentShader);


	// Connect the 2 shader objects into a program
	GLuint whiteShaderProgram = glCreateProgram();
	glAttachShader(whiteShaderProgram, vertexShader);
	glAttachShader(whiteShaderProgram, fragmentShader);
	glLinkProgram(whiteShaderProgram); // Link the program so changes can be made to shaders
	glUseProgram(whiteShaderProgram); // To start using shaders
}

void blackShader() {
	// Create vetex shader object and load data
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &blackVertexSource, NULL); // (object, location, array of source code string lengths or NULL)
	glCompileShader(vertexShader);

	// Create fragment shader object and load data
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &blackFragmentSource, NULL); // (object, location, array of source code string lengths or NULL)
	glCompileShader(fragmentShader);

	// Connect the 2 shader objects into a program
	GLuint blackShaderProgram = glCreateProgram();
	glAttachShader(blackShaderProgram, vertexShader);
	glAttachShader(blackShaderProgram, fragmentShader);
	glLinkProgram(blackShaderProgram); // Link the program so changes can be made to shaders
	glUseProgram(blackShaderProgram); // To start using shaders
}

void greenShader() {
	// Create vetex shader object and load data
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &greenVertexSource, NULL); // (object, location, array of source code string lengths or NULL)
	glCompileShader(vertexShader);

	// Create fragment shader object and load data
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &greenFragmentSource, NULL); // (object, location, array of source code string lengths or NULL)
	glCompileShader(fragmentShader);

	// Connect the 2 shader objects into a program
	GLuint greenShaderProgram = glCreateProgram();
	glAttachShader(greenShaderProgram, vertexShader);
	glAttachShader(greenShaderProgram, fragmentShader);
	glLinkProgram(greenShaderProgram); // Link the program so changes can be made to shaders
	glUseProgram(greenShaderProgram); // To start using shaders
}

void redShader() {
	// Create vetex shader object and load data
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &redVertexSource, NULL); // (object, location, array of source code string lengths or NULL)
	glCompileShader(vertexShader);

	// Create fragment shader object and load data
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &redFragmentSource, NULL); // (object, location, array of source code string lengths or NULL)
	glCompileShader(fragmentShader);

	// Connect the 2 shader objects into a program
	GLuint redShaderProgram = glCreateProgram();
	glAttachShader(redShaderProgram, vertexShader);
	glAttachShader(redShaderProgram, fragmentShader);
	glLinkProgram(redShaderProgram); // Link the program so changes can be made to shaders
	glUseProgram(redShaderProgram); // To start using shaders
}

void blueShader() {
	// Create vetex shader object and load data
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &blueVertexSource, NULL); // (object, location, array of source code string lengths or NULL)
	glCompileShader(vertexShader);

	// Create fragment shader object and load data
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &blueFragmentSource, NULL); // (object, location, array of source code string lengths or NULL)
	glCompileShader(fragmentShader);

	// Connect the 2 shader objects into a program
	GLuint blueShaderProgram = glCreateProgram();
	glAttachShader(blueShaderProgram, vertexShader);
	glAttachShader(blueShaderProgram, fragmentShader);
	glLinkProgram(blueShaderProgram); // Link the program so changes can be made to shaders
	glUseProgram(blueShaderProgram); // To start using shaders
}

void useSelectedShader() {
	if (selectedShader == "white")
	{
		whiteShader();
	}
	else if (selectedShader == "black")
	{
		blackShader();
	}
	else if (selectedShader == "green")
	{
		greenShader();
	}
	else if (selectedShader == "red")
	{
		redShader();
	}
	else if (selectedShader == "blue")
	{
		blueShader();
	}

}
//----------------------------------------------------------------------------
//
// UI
//

void Song1Btn() {
	// Create Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Set square vertices (x, y)
	float squareVertices[] = {
		-0.9f, -0.93f,
		-1.0f, -0.93f,
		-1.0f, -1.0f,
		-0.9f, -1.0f,
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	// Load data to graphics card
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), squareVertices, GL_STATIC_DRAW); // (Buffer, size in bytes, data, usage)

	// Specify the layout of the vertex data
	//GLint posAttrib = glGetAttribLocation(squareShaderProgram, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // (index, num of numbers in coords , type of data, conversion?, stride/size of each vertex, offset)

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void Song2Btn() {
	// Create Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Set square vertices (x, y)
	float squareVertices[] = {
		-0.9f, -0.83f,
		-1.0f, -0.83f,
		-1.0f, -0.9f,
		-0.9f, -0.9f,
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Load data to graphics card
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), squareVertices, GL_STATIC_DRAW); // (Buffer, size in bytes, data, usage)

	// Specify the layout of the vertex data
	//GLint posAttrib = glGetAttribLocation(squareShaderProgram, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // (index, num of numbers in coords , type of data, conversion?, stride/size of each vertex, offset)

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void Song3Btn() {
	// Create Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Set square vertices (x, y)
	float squareVertices[] = {
		-0.9f, -0.73f,
		-1.0f, -0.73f,
		-1.0f, -0.8f,
		-0.9f, -0.8f,
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Load data to graphics card
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), squareVertices, GL_STATIC_DRAW); // (Buffer, size in bytes, data, usage)

	// Specify the layout of the vertex data
	//GLint posAttrib = glGetAttribLocation(squareShaderProgram, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // (index, num of numbers in coords , type of data, conversion?, stride/size of each vertex, offset)

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void QuitBtn() {
	// Create Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Set square vertices (x, y)
	float squareVertices[] = {
		-0.9f, 0.93f,
		-1.0f, 0.93f,
		-1.0f, 1.0f,
		-0.9f, 1.0f,
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Load data to graphics card
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), squareVertices, GL_STATIC_DRAW); // (Buffer, size in bytes, data, usage)

	// Specify the layout of the vertex data
	//GLint posAttrib = glGetAttribLocation(squareShaderProgram, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // (index, num of numbers in coords , type of data, conversion?, stride/size of each vertex, offset)

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void whiteBtn() {
	// Create Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Set square vertices (x, y)
	float squareVertices[] = {
		-0.74f, -0.73f,
		-0.8f, -0.73f,
		-0.8f, -0.83f,
		-0.74f, -0.83f,
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Load data to graphics card
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), squareVertices, GL_STATIC_DRAW); // (Buffer, size in bytes, data, usage)

	// Specify the layout of the vertex data
	//GLint posAttrib = glGetAttribLocation(squareShaderProgram, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // (index, num of numbers in coords , type of data, conversion?, stride/size of each vertex, offset)

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void greenBtn() {
	// Create Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Set square vertices (x, y)
	float squareVertices[] = {
		-0.74f, -0.87f,
		-0.8f, -0.87f,
		-0.8f, -0.97f,
		-0.74f, -0.97f,
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Load data to graphics card
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), squareVertices, GL_STATIC_DRAW); // (Buffer, size in bytes, data, usage)

	// Specify the layout of the vertex data
	//GLint posAttrib = glGetAttribLocation(squareShaderProgram, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // (index, num of numbers in coords , type of data, conversion?, stride/size of each vertex, offset)

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void redBtn() {
	// Create Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Set square vertices (x, y)
	float squareVertices[] = {
		-0.65f, -0.87f,
		-0.71f, -0.87f,
		-0.71f, -0.97f,
		-0.65f, -0.97f,
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Load data to graphics card
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), squareVertices, GL_STATIC_DRAW); // (Buffer, size in bytes, data, usage)

	// Specify the layout of the vertex data
	//GLint posAttrib = glGetAttribLocation(squareShaderProgram, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // (index, num of numbers in coords , type of data, conversion?, stride/size of each vertex, offset)

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void blueBtn() {
	// Create Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Set square vertices (x, y)
	float squareVertices[] = {
		-0.65f, -0.73f,
		-0.71f, -0.73f,
		-0.71f, -0.83f,
		-0.65f, -0.83f,
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Load data to graphics card
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), squareVertices, GL_STATIC_DRAW); // (Buffer, size in bytes, data, usage)

	// Specify the layout of the vertex data
	//GLint posAttrib = glGetAttribLocation(squareShaderProgram, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // (index, num of numbers in coords , type of data, conversion?, stride/size of each vertex, offset)

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void playBtn() {
	// Create Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Set square vertices (x, y)
	float squareVertices[] = {
		0.1f, -0.2f,
		0.2f, -0.2f,
		0.2f, -0.27f,
		0.1f, -0.27f,
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Load data to graphics card
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), squareVertices, GL_STATIC_DRAW); // (Buffer, size in bytes, data, usage)

	// Specify the layout of the vertex data
	//GLint posAttrib = glGetAttribLocation(squareShaderProgram, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // (index, num of numbers in coords , type of data, conversion?, stride/size of each vertex, offset)

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void pauseBtn() {
	// Create Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Set square vertices (x, y)
	float squareVertices[] = {
		0.25f, -0.2f,
		0.35f, -0.2f,
		0.35f, -0.27f,
		0.25f, -0.27f,
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Load data to graphics card
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), squareVertices, GL_STATIC_DRAW); // (Buffer, size in bytes, data, usage)

	// Specify the layout of the vertex data
	//GLint posAttrib = glGetAttribLocation(squareShaderProgram, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // (index, num of numbers in coords , type of data, conversion?, stride/size of each vertex, offset)

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void restartBtn() {
	// Create Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Set square vertices (x, y)
	float squareVertices[] = {
		0.73f, -0.2f,
		0.87f, -0.2f,
		0.87f, -0.27f,
		0.73f, -0.27f,
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Load data to graphics card
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), squareVertices, GL_STATIC_DRAW); // (Buffer, size in bytes, data, usage)

	// Specify the layout of the vertex data
	//GLint posAttrib = glGetAttribLocation(squareShaderProgram, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // (index, num of numbers in coords , type of data, conversion?, stride/size of each vertex, offset)

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

// function inspired by Ramazan Bellek https://www.youtube.com/watch?v=elE__Nouv54
void drawText(const char* text, int length, float x, float y) {
	glMatrixMode(GL_PROJECTION);
	double* matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2f(x, y); //changed from 2i to 2f to allow for floats (specifies poisition)
	for (size_t i = 0; i < length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]); // (font, character)
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}
//----------------------------------------------------------------------------
//
// display
//

void drawUI() {

	whiteShader();

	Song1Btn();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	Song2Btn();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	Song3Btn();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	QuitBtn();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	pauseBtn();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	playBtn();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	pauseBtn();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	restartBtn();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	drawText(songName.data(), songName.size(), 0.5f, -0.15f);
	drawText("Settings", 8, -0.9f, -0.6f);
	drawText("Songs", 5, -0.99f, -0.7f);
	drawText("Colours", 7, -0.8f, -0.7f);

	whiteBtn();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


	greenShader();

	greenBtn();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	redShader();

	redBtn();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	blueShader();

	blueBtn();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	blackShader();

	drawText("Song 3", 6, -0.99f, -0.98f);
	drawText("Song 2", 6, -0.99f,-0.88f);
	drawText("Song 1", 6, -0.99f, -0.78f);
	drawText("Quit", 4, -0.99f, 0.95f);
	drawText("Pause", 5, 0.27f, -0.25f);
	drawText("Play", 4, 0.13f, -0.25f);
	drawText("Restart", 7, 0.76f, -0.25f);
}

// Places points for each coord in 3D space no matter how many points there are.
void plotPoints(vector<float> vertices, int numOfElements) {
	int sizeOfNew = pointsToShow;
	vector<float> newVertices;
	int pointsEnd = pointsStart + pointsToShow;

	if (pointsEnd > numOfElements)
	{
		pointsEnd = numOfElements;
		sizeOfNew = pointsEnd - pointsStart;
	}
	//cout << vertices[(3 * numberOfFrequences)] << " " << vertices[(3 * numberOfFrequences) * 2] << " " << vertices[(3 * numberOfFrequences) * 3] << " " << vertices[(3 * numberOfFrequences) * 3] - vertices[(3 * numberOfFrequences) * 2]  << "\n";

	for (int i = pointsStart; i < pointsEnd; i++)
	{

		// i mod 3 to find if data is time 
		if (i==0 || i % 3 == 0)
		{
			// Renormalize
			newVertices.push_back(normaliseValue(vertices[i], vertices[pointsStart], vertices[pointsEnd]));
			//cout << "Point: " << i << " Value: " << vertices[i] << "\n";
		}
		else {
			newVertices.push_back(vertices[i]);
		}
	}

	// Create Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Load data to graphics card
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeOfNew * sizeof(float), &newVertices[0], GL_STATIC_DRAW); // (Buffer, size in bytes, data, usage)

	// Specify the layout of the vertex data
	//GLint posAttrib = glGetAttribLocation(squareShaderProgram, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // (index, num of numbers in coords , type of data, conversion?, stride/size of each vertex, offset)

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfNew * 3 * 3 * sizeof(GLuint), NULL, GL_STATIC_DRAW);

	useSelectedShader();

	glPointSize(2);
	glDrawArrays(GL_POINTS, 0, (sizeOfNew/3));
	//glDrawArrays(GL_LINE_STRIP, 0, 4);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

//----------------------------------------------------------------------------
//
// Audio processing
//

// Sets everything on a scale of 0 - 1
float normaliseValue(float value, float min, float max) {
	return ((value - min) / (max - min));
}

vector<float> findFrequencyData() {
	std::string line = reader.getLine(0);
	vector<float> uneditedFrequencyData;
	vector<float> normalisedFrequencyData;
	
	int first = 12;
	int last = 0;

	for (size_t i = 12; i < line.length(); i++)
	{
		if (line[i] == ',')
		{
			last = i;

			// converts string to float
			uneditedFrequencyData.push_back(stof(line.substr(first, last - first)));

			first = last + 1;
		}
	}

	uneditedFrequencyData.push_back(stof(line.substr(first, line.length() - first)));

	for(size_t i = 0; i < uneditedFrequencyData.size() ; i++)
	{
		normalisedFrequencyData.push_back(normaliseValue(uneditedFrequencyData[i], uneditedFrequencyData[0], uneditedFrequencyData[uneditedFrequencyData.size() - 1]));
		//cout<< uneditedFrequencyData[i] << " converts to " << normalisedFrequencyData[i] << "\n";
	}

	return normalisedFrequencyData;
}

void findNumberOfFrequences() {
	string line = reader.getLine(0);
	numberOfFrequences = 0;

	// ever file starts with "Time in Sec," atm so we are skiping this
	for (size_t i = 12; i < line.length(); i++)
	{
		if (line[i] == ',')
		{
			numberOfFrequences++;
		}
	}

	//each line does not end with a , so we must + 1
	numberOfFrequences++;

	//return numberOfFrequences;
}

vector<float> findTimeData() {
	vector<float> uneditedTimeData;
	vector<float> normalisedTimeData;

	for (int i = 1; i < reader.lineCount(); i++) {

		std::string line = reader.getLine(i);

		int character = 0;
		while (line[character] != ',')
		{
			character++;
		}

		uneditedTimeData.push_back(stof(line.substr(0, character - 0)));

		//cout << "Time " << uneditedTimeData[i - 1] << "\n";
	}

	for (size_t i = 0; i < uneditedTimeData.size(); i++)
	{
		normalisedTimeData.push_back(normaliseValue(uneditedTimeData[i], uneditedTimeData[0], uneditedTimeData[uneditedTimeData.size() - 1]));
		//cout<< uneditedTimeData[i] << "converts to " << normalisedTimeData[i] << "\n";
	}

	return normalisedTimeData;
}

vector<float> findAmplitudeData() {
	vector<float> uneditedAmplitudeData;
	vector<float> normalisedAmplitudeData;

	float minAmplitude = 0;
	float maxAmplitude = 0;


	for (int i = 1; i < reader.lineCount(); i++) {

		std::string line = reader.getLine(i);

		int first = 0;
		int last = 0;

		// Prevent adding time to the data
		while (line[first] != ',')
		{
			first++;
		}

		first++;

		for (size_t x = first; x < line.length(); x++)
		{
			if (line[x] == ',')
			{
				last = x;

				// converts string to float
				uneditedAmplitudeData.push_back(stof(line.substr(first, last - first)));

				if (minAmplitude > stof(line.substr(first, last - first)))
				{
					minAmplitude = stof(line.substr(first, last - first));
				}
				if (maxAmplitude < stof(line.substr(first, last - first)))
				{
					maxAmplitude = stof(line.substr(first, last - first));
				}

				//cout << "Amp " << uneditedAmplitudeData[x] << "\n";

				first = last + 1;
			}
		}

		uneditedAmplitudeData.push_back(stof(line.substr(first, line.length() - first)));
	}

	for (size_t i = 0; i < uneditedAmplitudeData.size(); i++)
	{
		normalisedAmplitudeData.push_back(normaliseValue(uneditedAmplitudeData[i], minAmplitude, maxAmplitude));
		//cout << uneditedAmplitudeData[i] << " converts to " << normalisedAmplitudeData[i] << "\n";
	}

	return normalisedAmplitudeData;
}

void findNumberOfVertices() {
	findNumberOfFrequences();

	// -1 as the first line is not a time stamp
	numberOfVertices = numberOfFrequences * (reader.lineCount() - 1);
}

vector<float> createVertexData() {
	// These are set by me atm
	float minFrequency = 0;
	float maxFrequency = 22050;
	float minAmplitude = -100;
	float maxAmplitude = 100;

	// This needs to found as length of song changes
	float minTime = 0;
	float maxTime;

	//int numberOfVertices = findNumberOfVertices();
	//cout << "Number of Vertices: " << numberOfVertices;

	vector<float> frequencyData = findFrequencyData();
	vector<float> timeData = findTimeData();
	vector<float> amplitudeData = findAmplitudeData();

	vector<float> songVertexData;
	int a = 0;

	// x = time, y = amplitude, z = frequency
	for (size_t t = 0; t < timeData.size(); t++)
	{
		for (size_t f = 0; f < frequencyData.size(); f++)
		{
			songVertexData.push_back(timeData[t]);
			songVertexData.push_back(amplitudeData[a]);
			songVertexData.push_back(frequencyData[f]);
			a++;
		}
	}

	return songVertexData;
	//float songVertexData[numberOfVertices];

}

void changeSong() {
	// Loading screen

	reader.closeCurrentFile();
	//"media/Music/bensound-slowmotion.csv"
	reader.openFile("media/Music/" + songName + ".csv");

	songVertexData.clear();
	songVertexData = createVertexData();

	findNumberOfFrequences();
	pointsStart = 0;
	pointsToShow = numberOfFrequences * 3 * 60;

	findNumberOfVertices();

	keepIncreasing = false;

	//plotPoints(songVertexData, (songVertexData.size() / 3));
}

//----------------------------------------------------------------------------
//
// logic
//

void btnPressed(GLFWwindow* window) {
	char btn;
	double xpos, ypos;

	// Get cursor position
	glfwGetCursorPos(window, &xpos, &ypos);

	// Select song & Quit
	if (xpos >= 0 && xpos <= 95)
	{
		if (ypos >= 0 && ypos <= 37)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		else if (ypos >= 1040 && ypos <= 1080)
		{	
			// Code for song1 selection
			if (songName == "bensound-dubstep")
			{
				pointsStart = 0;
				keepIncreasing = true;
			}
			else {
				songName = "bensound-dubstep";
				changeSong();
			}
		}
		else if (ypos >= 987 && ypos <= 1027)
		{
			// Code for song2 selection
			if (songName == "bensound-energy")
			{
				pointsStart = 0;
				keepIncreasing = true;
			}
			else {
				songName = "bensound-energy";
				changeSong();
			}
		}
		else if (ypos >= 930 && ypos <= 970)
		{
			// Code for song3 selection
			if (songName == "bensound-slowmotion")
			{
				pointsStart = 0;
				keepIncreasing = true;
			}
			else {
				songName = "bensound-slowmotion";
				changeSong();
			}
		}
	}
	// 1st colour block
	else if (xpos >= 192 && xpos <= 250) {
		//white
		if (ypos >= 933 && ypos <= 989)
		{
			selectedShader = "white";
		}
		//green
		else if (ypos >= 1010 && ypos <= 1064) {
			selectedShader = "green";
		}
	}
	// 2nd colour block
	else if (xpos >= 280 && xpos <= 335)
	{
		//blue
		if (ypos >= 933 && ypos <= 989)
		{
			selectedShader = "blue";
		}
		//red
		else if (ypos >= 1010 && ypos <= 1064) {
			selectedShader = "red";
		}
	}
	// Play
	else if (xpos >= 1056 && xpos <= 1151) {
		if (ypos >= 648 && ypos <= 685)
		{
			keepIncreasing = true;
		}
	}
	// Pause
	else if (xpos >= 1200 && xpos <= 1296) {
		if (ypos >= 648 && ypos <= 685)
		{
			keepIncreasing = false;
		}
	}
	// Restart
	else if (xpos >= 1660 && xpos <= 1795) {
		if (ypos >= 648 && ypos <= 685)
		{
			pointsStart = 0;
			keepIncreasing = false;
		}
	}

	//cout << "Cursor Position at (" << xpos << " : " << ypos << endl;
}

//----------------------------------------------------------------------------
//
// main
//


int main(int argc, char** argv)
{
	// Initialise GLFW library (returns true if successful)
	glfwInit();

	// Create window (x, y, Title, NULL, NULL)
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Musical Enviroment", glfwGetPrimaryMonitor(), NULL);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	// Make current OpenGL context so can use OpenGL API
	glfwMakeContextCurrent(window);

	glutInit(&argc, argv);
	// Initialise GLEW library
	glewInit();

	songName = "test";
	changeSong();
	keepIncreasing = false;
	pointsToShow = (numberOfVertices * 3);

	while (!glfwWindowShouldClose(window))
	{
		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		drawUI();

		if (keepIncreasing)
		{
			pointsStart = pointsStart + (numberOfFrequences * 3);
		}

		if (pointsStart < numberOfVertices)
		{
			//cout << "Point at: " << pointsStart << "\n";
			plotPoints(songVertexData, songVertexData.size() - 3);
		}
		else
		{
			keepIncreasing = false;
			whiteShader();
			drawText("End of song", 11, 0.46f, 0.5f);
		}

		//plotPoints(songVertexData, 300);
		//plotPoints(songVertexData, songVertexData.size());

		// pass in size as can't find it in next function due to only being a pointer to first element
		//plotPoints(vertices, (sizeof(vertices) / sizeof(vertices[0])));

		glfwSwapBuffers(window); // Once rendered swap buffer
		glfwPollEvents(); // Process event

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
			btnPressed(window);
			//cout << "Cursor Position at (" << xpos << " : " << ypos << endl;
		}
	}

	//Destroy window
	glfwDestroyWindow(window);

	// Terminate GLFW when done
	glfwTerminate();
}