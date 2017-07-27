//Anurag Misra 12144
//Made as a part of Computer Graphics Course

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <StopWatch.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

/* GLM */
// #define GLM_MESSAGES
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_utils.h>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <GL/glut.h>
#endif

#define NUM_SPHERES 17

GLFrame spheres[NUM_SPHERES];
GLFrame cyl[24][4];

struct cylin{
	
	float x; 
	float z;
};

cylin cy[24][4];
cylin sph[NUM_SPHERES];


int flag = 0;
GLFrame cu[12][12];


int m[12][12] = { { 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 }, { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 }, { 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1 }, { 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
{ 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1 }, { 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1 }, { 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
{ 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1 }, { 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1 }, { 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1 }, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1 } };


GLShaderManager		shaderManager;			// Shader Manager
GLMatrixStack		modelViewMatrix;		// Modelview Matrix
GLMatrixStack		projectionMatrix;		// Projection Matrix
GLFrustum			viewFrustum;			// View Frustum
GLGeometryTransform	transformPipeline;		// Geometry Transform Pipeline
GLFrame				cameraFrame; // Camera frame
GLTriangleBatch		torusBatch;
GLTriangleBatch		sphereBatch;
GLTriangleBatch		sphereBatch1;
GLTriangleBatch		diskBatch;
GLBatch cubeBatch;
GLFrame objectFrame;
GLBatch				floorBatch;
GLBatch				floorBatch1;
GLBatch				floorBatch2;
GLBatch				floorBatch3;
GLBatch				floorBatch4;

GLTriangleBatch		cylBatch;
GLBatch             ceilingBatch;
GLBatch             leftWallBatch;
GLBatch             rightWallBatch;
GLBatch             leftWallBatch1;
GLBatch             rightWallBatch1;
GLBatch				frontWallBatch;
GLBatch				frontWallBatch1;
GLBatch				frontWallBatch2;
GLBatch				frontWallBatch3;

GLuint				uiTextures[6];
/*
class Mesh {
private:
	GLuint vbo_vertices, vbo_normals, ibo_elements;
public:
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec3> normals;
	std::vector<GLushort> elements;
	glm::mat4 object2world;

	Mesh() : vbo_vertices(0), vbo_normals(0), ibo_elements(0), object2world(glm::mat4(1)) {}
	~Mesh() {
		if (vbo_vertices != 0)
			glDeleteBuffers(1, &vbo_vertices);
		if (vbo_normals != 0)
			glDeleteBuffers(1, &vbo_normals);
		if (ibo_elements != 0)
			glDeleteBuffers(1, &ibo_elements);
	}

}

void ObjectOrder()
{

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
	bool res = loadOBJ("ninga1.obj", vertices, uvs, normals);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);




}

*/

void ObjectOrder()
{
 	static GLfloat vWhite[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static GLfloat vLightPos[] = { -40.0f, 75.0f, 90.0f, 1.0f };
	M3DVector4f	vLightTransformed;
	M3DMatrix44f mCamera;
	modelViewMatrix.GetMatrix(mCamera); 
	m3dTransformVector4(vLightTransformed, vLightPos, mCamera);
	glBindTexture(GL_TEXTURE_2D, uiTextures[3]);

	modelViewMatrix.PushMatrix();
	modelViewMatrix.MultMatrix(objectFrame);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF,
	modelViewMatrix.GetMatrix(),
	transformPipeline.GetProjectionMatrix(),
	vLightTransformed,
	vWhite,
	0);
	sphereBatch1.Draw();
	modelViewMatrix.PopMatrix();


}

void WorldOrder2()
{
	static GLfloat vWhite[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static GLfloat vLightPos[] = { -10.0f, 30.0f, -80.0f, 1.0f };
	static GLfloat vLightPos1[] = { -10.0f, 50.0f, -380.0f, 1.0f };
	// Get the light position in eye space
	M3DVector4f	vLightTransformed, vLightTransformed1;
	M3DMatrix44f mCamera;
	modelViewMatrix.GetMatrix(mCamera);
	m3dTransformVector4(vLightTransformed, vLightPos, mCamera);

	m3dTransformVector4(vLightTransformed1, vLightPos, mCamera);
	// Draw the light source
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Translatev(vLightPos);
	shaderManager.UseStockShader(GLT_SHADER_FLAT,
		transformPipeline.GetModelViewProjectionMatrix(),
		vWhite);
	sphereBatch.Draw();
	modelViewMatrix.PopMatrix();


	
	glBindTexture(GL_TEXTURE_2D, uiTextures[1]);

	for (int i = 0; i < 24; i++) {
		for (int j = 0; j < 4; j++)
		{
			modelViewMatrix.PushMatrix();
			modelViewMatrix.MultMatrix(cyl[i][j]);
			modelViewMatrix.Rotate(-90.0f, 1.0f, 0.0f, 0.0f);
			shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF,
				modelViewMatrix.GetMatrix(),
				transformPipeline.GetProjectionMatrix(),
				vLightTransformed,
				vWhite,
				0);
			
			diskBatch.Draw();

			modelViewMatrix.PopMatrix();
		}
	}

	for (int i = 0; i < 24; i++) {
		for (int j = 0; j < 4; j++)
		{
			modelViewMatrix.PushMatrix();
			modelViewMatrix.MultMatrix(cyl[i][j]);
			modelViewMatrix.Rotate(90.0f, 1.0f, 0.0f, 0.0f);
			
			shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF,
				modelViewMatrix.GetMatrix(),
				transformPipeline.GetProjectionMatrix(),
				vLightTransformed,
				vWhite,
				0);
			cylBatch.Draw();
		
			

			modelViewMatrix.PopMatrix();
		}
	}

	
	 
	glBindTexture(GL_TEXTURE_2D, uiTextures[5]);

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++)
		{
			modelViewMatrix.PushMatrix();
			modelViewMatrix.MultMatrix(cu[i][j]);
			/*shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF,
				modelViewMatrix.GetMatrix(),
				transformPipeline.GetProjectionMatrix(),
				vLightTransformed1,
				vWhite,
				0);
				*/

			shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE,
				transformPipeline.GetModelViewProjectionMatrix(), 0);



			if (m[i][j]==1)
			cubeBatch.Draw();

			modelViewMatrix.PopMatrix();
		}
	}


}

void WorldOrder1(GLfloat yRot)	
	{
	static GLfloat vWhite[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static GLfloat vLightPos[] = { 0.0f, 3.0f, 0.0f, 1.0f };
	
	// Get the light position in eye space
	M3DVector4f	vLightTransformed;
	M3DMatrix44f mCamera;
	modelViewMatrix.GetMatrix(mCamera);
	m3dTransformVector4(vLightTransformed, vLightPos, mCamera);
	
	// Draw the light source
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Translatev(vLightPos);
	shaderManager.UseStockShader(GLT_SHADER_FLAT, 
								 transformPipeline.GetModelViewProjectionMatrix(),
								 vWhite);
	sphereBatch.Draw();


	modelViewMatrix.PopMatrix();
    
	
	
	modelViewMatrix.Translate(0.0f, 0.6f, -2.5f);
	modelViewMatrix.PushMatrix();	// Saves the translated origin
	modelViewMatrix.Rotate(yRot, 0.0f, 1.0f, 0.0f);
	
	// Draw stuff relative to the camera
	glBindTexture(GL_TEXTURE_2D, uiTextures[1]);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF,
								 modelViewMatrix.GetMatrix(),
								 transformPipeline.GetProjectionMatrix(),
								 vLightTransformed, 
								 vWhite,
								 0);
	
	torusBatch.Draw();
	modelViewMatrix.PopMatrix(); // Erased the rotate
	
	
	modelViewMatrix.Rotate(yRot * -1.0f, 0.0f, 1.0f, 0.0f);
	modelViewMatrix.Translate(2.0f, 0.0f, 0.0f);
	
	glBindTexture(GL_TEXTURE_2D, uiTextures[2]);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF,
								 modelViewMatrix.GetMatrix(),
								 transformPipeline.GetProjectionMatrix(),
								 vLightTransformed, 
								 vWhite,
								 0);
	sphereBatch.Draw();
	

	glBindTexture(GL_TEXTURE_2D, uiTextures[2]);
	for (int i = 0; i < NUM_SPHERES; i++) {
		modelViewMatrix.PushMatrix();
		modelViewMatrix.MultMatrix(spheres[i]);
		//spheres[i].SetOrigin(0.0f, 0.2f, -2.5f);
		//modelViewMatrix.Translate(r, 0.0f, 0.0f);
		shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF,
			modelViewMatrix.GetMatrix(),
			transformPipeline.GetProjectionMatrix(),
			vLightTransformed,
			vWhite,
			0);
		sphereBatch.Draw();
		modelViewMatrix.PopMatrix();
	}
	}
	
	
bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
	{
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	

	// Read the texture bits
	pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
	if(pBits == NULL) 
		return false;
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, nWidth, nHeight, 0,
				 eFormat, GL_UNSIGNED_BYTE, pBits);
	
    free(pBits);

    if(minFilter == GL_LINEAR_MIPMAP_LINEAR || 
       minFilter == GL_LINEAR_MIPMAP_NEAREST ||
       minFilter == GL_NEAREST_MIPMAP_LINEAR ||
       minFilter == GL_NEAREST_MIPMAP_NEAREST)
        glGenerateMipmap(GL_TEXTURE_2D);
            
	return true;
	}

        
//////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
	// Make sure OpenGL entry points are set
	glewInit();

	// Initialze Shader Manager
	shaderManager.InitializeStockShaders();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	cameraFrame.MoveForward(-65.0f);
	cameraFrame.MoveUp(1.0f);
	

	glClearColor(0.196078f, 0.6f, 0.8f, 0.5f);

	// This makes a torus
	gltMakeTorus(torusBatch, 1.0f, 0.3f, 40, 20);

	// This makes a sphere
	gltMakeSphere(sphereBatch, 0.5f, 26, 13);
	gltMakeSphere(sphereBatch1, 0.5f, 26, 13);
	gltMakeCylinder(cylBatch, 0.5f, 0.5f, 3.0f, 20, 10);
	gltMakeDisk(diskBatch, 0.0f, 0.5f, 20, 10);
	gltMakeCube(cubeBatch, 2.0f);


	// Make the solid ground
	GLfloat texSize = 1.0f;
	GLfloat texSize1 = 10.0f;

	floorBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
	floorBatch.MultiTexCoord2f(0.0, 0.0f, 0.0f);
	floorBatch.Vertex3f(-300.0f, -0.41f, 270.0f);

	floorBatch.MultiTexCoord2f(0, texSize1, 0.0f);
	floorBatch.Vertex3f(300.0f, -0.41f, 270.0f);

	floorBatch.MultiTexCoord2f(0, texSize1, texSize1);
	floorBatch.Vertex3f(300.0f, -0.41f, -70.0f);

	floorBatch.MultiTexCoord2f(0, 0.0f, texSize1);
	floorBatch.Vertex3f(-300.0f, -0.41f, -70.0f);
	floorBatch.End();


	floorBatch1.Begin(GL_TRIANGLE_FAN, 4, 1);
	floorBatch1.MultiTexCoord2f(0.0, 0.0f, 0.0f);
	floorBatch1.Vertex3f(-2.5f, -0.41f, -70.0f);

	floorBatch1.MultiTexCoord2f(0, texSize, 0.0f);
	floorBatch1.Vertex3f(2.5f, -0.41f, -70.0f);

	floorBatch1.MultiTexCoord2f(0, texSize, texSize);
	floorBatch1.Vertex3f(2.5f, -0.41f, -130.0f);

	floorBatch1.MultiTexCoord2f(0, 0.0f, texSize);
	floorBatch1.Vertex3f(-2.5f, -0.41f, -130.0f);
	floorBatch1.End();

	floorBatch2.Begin(GL_TRIANGLE_FAN, 4, 1);
	floorBatch2.MultiTexCoord2f(0.0, 0.0f, 0.0f);
	floorBatch2.Vertex3f(-2.5f, -0.41f, -375.0f);

	floorBatch2.MultiTexCoord2f(0, texSize, 0.0f);
	floorBatch2.Vertex3f(2.5f, -0.41f, -375.0f);

	floorBatch2.MultiTexCoord2f(0, texSize, texSize);
	floorBatch2.Vertex3f(2.5f, -0.41f, -420.0f);

	floorBatch2.MultiTexCoord2f(0, 0.0f, texSize);
	floorBatch2.Vertex3f(-2.5f, -0.41f, -420.0f);
	floorBatch2.End();

	


	floorBatch3.Begin(GL_TRIANGLE_FAN, 4, 1);
	floorBatch3.MultiTexCoord2f(0.0, 0.0f, 0.0f);
	floorBatch3.Vertex3f(-300.0f, -0.41f, -420.0f);

	floorBatch3.MultiTexCoord2f(0, texSize1, 0.0f);
	floorBatch3.Vertex3f(300.0f, -0.41f, -420.0f);

	floorBatch3.MultiTexCoord2f(0, texSize1, texSize1);
	floorBatch3.Vertex3f(300.0f, -0.41f, -600.0f);

	floorBatch3.MultiTexCoord2f(0, 0.0f, texSize1);
	floorBatch3.Vertex3f(-300.0f, -0.41f, -600.0f);
	floorBatch3.End();

	floorBatch4.Begin(GL_TRIANGLE_FAN, 4, 1);
	floorBatch4.MultiTexCoord2f(0.0, 0.0f, 0.0f);
	floorBatch4.Vertex3f(-300.0f, -30.41f, -130.0f);

	floorBatch4.MultiTexCoord2f(0, texSize1, 0.0f);
	floorBatch4.Vertex3f(300.0f, -30.41f, -130.0f);

	floorBatch4.MultiTexCoord2f(0, texSize1, texSize1);
	floorBatch4.Vertex3f(300.0f, -30.41f, -420.0f);

	floorBatch4.MultiTexCoord2f(0, 0.0f, texSize1);
	floorBatch4.Vertex3f(-300.0f, -30.41f, -420.0f);
	floorBatch4.End();

	/*
	//Make other grounds
	ceilingBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
	ceilingBatch.MultiTexCoord2f(0.0f, 0.0f, 0.0f);
	ceilingBatch.Vertex3f(-20.0f, 7.0f, -70.0f);

	ceilingBatch.MultiTexCoord2f(0.0f, texSize, 0.0f);
	ceilingBatch.Vertex3f(20.0f, 7.0f, -70.0f);

	ceilingBatch.MultiTexCoord2f(0.0f, texSize, texSize);
	ceilingBatch.Vertex3f(20.0f, 7.0f, -100.0f);

	ceilingBatch.MultiTexCoord2f(0.0f, 0.0f, texSize);
	ceilingBatch.Vertex3f(-20.0f, 7.0f, -100.0f);
	ceilingBatch.End();
	*/
	leftWallBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
	leftWallBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
	leftWallBatch.Vertex3f(-2.5f, -0.41f, -70.0f);

	leftWallBatch.MultiTexCoord2f(0.0f, texSize,0.0f);
	leftWallBatch.Vertex3f(-2.5f, -200.0f, -70.0f);

	leftWallBatch.MultiTexCoord2f(0.0f, texSize, texSize);
	leftWallBatch.Vertex3f(-2.5f, -0.41f, -130.0f);

	leftWallBatch.MultiTexCoord2f(0.0f, 0.0f, texSize);
	leftWallBatch.Vertex3f(-2.5f, -200.0f, -130.0f);

	leftWallBatch.End();

	leftWallBatch1.Begin(GL_TRIANGLE_FAN, 4, 1);
	leftWallBatch1.MultiTexCoord2f(0, 0.0f, 0.0f);
	leftWallBatch1.Vertex3f(-2.5f, -0.41f, -375.0f);

	leftWallBatch1.MultiTexCoord2f(0.0f, texSize, 0.0f);
	leftWallBatch1.Vertex3f(-2.5f, -200.0f, -375.0f);

	leftWallBatch1.MultiTexCoord2f(0.0f, texSize, texSize);
	leftWallBatch1.Vertex3f(-2.5f, -0.41f, -420.0f);

	leftWallBatch1.MultiTexCoord2f(0.0f, 0.0f, texSize);
	leftWallBatch1.Vertex3f(-2.5f, -200.0f, -420.0f);

	leftWallBatch1.End();

	rightWallBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
	rightWallBatch.MultiTexCoord2f(0.0f, 0.0f, 0.0f);
	rightWallBatch.Vertex3f(2.5f, -0.41f, -70.0f);

	rightWallBatch.MultiTexCoord2f(0.0f, texSize, 0.0f);
	rightWallBatch.Vertex3f(2.5f, -200.0f, -70.0f);

	rightWallBatch.MultiTexCoord2f(0.0f, texSize, texSize);
	rightWallBatch.Vertex3f(2.5f, -0.41f, -130.0f);

	rightWallBatch.MultiTexCoord2f(0.0f, 0.0f, texSize);
	rightWallBatch.Vertex3f(2.5f, -200.0f, -130.0f);

	rightWallBatch.End();
	
	rightWallBatch1.Begin(GL_TRIANGLE_FAN, 4, 1);
	rightWallBatch1.MultiTexCoord2f(0.0f, 0.0f, 0.0f);
	rightWallBatch1.Vertex3f(2.5f, -0.41f, -375.0f);

	rightWallBatch1.MultiTexCoord2f(0.0f, texSize, 0.0f);
	rightWallBatch1.Vertex3f(2.5f, -200.0f, -375.0f);

	rightWallBatch1.MultiTexCoord2f(0.0f, texSize, texSize);
	rightWallBatch1.Vertex3f(2.5f, -0.41f, -420.0f);

	rightWallBatch1.MultiTexCoord2f(0.0f, 0.0f, texSize);
	rightWallBatch1.Vertex3f(2.5f, -200.0f, -420.0f);

	rightWallBatch1.End();

	frontWallBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
	frontWallBatch.MultiTexCoord2f(0.0f, 0.0f, 0.0f);
	frontWallBatch.Vertex3f(-300.0f, -0.41f, -70.0f);

	frontWallBatch.MultiTexCoord2f(0.0f, texSize1, 0.0f);
	frontWallBatch.Vertex3f(-300.0f, -200.0f, -70.0f);

	frontWallBatch.MultiTexCoord2f(0.0f, texSize1, texSize1);
	frontWallBatch.Vertex3f(300.0f, -0.41f, -70.0f);

	frontWallBatch.MultiTexCoord2f(0.0f, 0.0f, texSize1);
	frontWallBatch.Vertex3f(300.0f, -200.0f, -70.0f);

	frontWallBatch.End();

	frontWallBatch1.Begin(GL_TRIANGLE_FAN, 4, 1);
	frontWallBatch1.MultiTexCoord2f(0.0f, 0.0f, 0.0f);
	frontWallBatch1.Vertex3f(-300.0f, -0.41f, -420.0f);

	frontWallBatch1.MultiTexCoord2f(0.0f, texSize1, 0.0f);
	frontWallBatch1.Vertex3f(-300.0f, -200.0f, -420.0f);

	frontWallBatch1.MultiTexCoord2f(0.0f, texSize1, texSize1);
	frontWallBatch1.Vertex3f(500.0f, -0.41f, -420.0f);

	frontWallBatch1.MultiTexCoord2f(0.0f, 0.0f, texSize1);
	frontWallBatch1.Vertex3f(500.0f, -200.0f, -420.0f);

	frontWallBatch1.End();
	frontWallBatch2.Begin(GL_TRIANGLE_FAN, 4, 1);
	frontWallBatch2.MultiTexCoord2f(0.0f, 0.0f, 0.0f);
	frontWallBatch2.Vertex3f(-2.5f, -0.41f, -375.0f);

	frontWallBatch2.MultiTexCoord2f(0.0f, texSize, 0.0f);
	frontWallBatch2.Vertex3f(-2.5f, -200.0f, -375.0f);

	frontWallBatch2.MultiTexCoord2f(0.0f, texSize, texSize);
	frontWallBatch2.Vertex3f(2.5f, -0.41f, -375.0f);

	frontWallBatch2.MultiTexCoord2f(0.0f, 0.0f, texSize);
	frontWallBatch2.Vertex3f(2.5f, -200.0f, -375.0f);

	frontWallBatch2.End();
	frontWallBatch3.Begin(GL_TRIANGLE_FAN, 4, 1);
	frontWallBatch3.MultiTexCoord2f(0.0f, 0.0f, 0.0f);
	frontWallBatch3.Vertex3f(-2.5f, -0.41f, -130.0f);

	frontWallBatch3.MultiTexCoord2f(0.0f, texSize1, 0.0f);
	frontWallBatch3.Vertex3f(-2.5f, -200.0f, -130.0f);

	frontWallBatch3.MultiTexCoord2f(0.0f, texSize, texSize);
	frontWallBatch3.Vertex3f(2.5f, -0.41f, -130.0f);

	frontWallBatch3.MultiTexCoord2f(0.0f, 0.0f, texSize);
	frontWallBatch3.Vertex3f(2.5f, -200.0f, -130.0f);

	frontWallBatch3.End();

	// Make 5 texture objects
	glGenTextures(5, uiTextures);

	// Load Texture
	glBindTexture(GL_TEXTURE_2D, uiTextures[0]);
	LoadTGATexture("water.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);

	// Load Texture
	glBindTexture(GL_TEXTURE_2D, uiTextures[1]);
	LoadTGATexture("stone.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);

	// Load Texture
	glBindTexture(GL_TEXTURE_2D, uiTextures[2]);
	LoadTGATexture("moonlike.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);


	// Load Texture
	glBindTexture(GL_TEXTURE_2D, uiTextures[3]);
	LoadTGATexture("sand.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);

	// Load Texture
	glBindTexture(GL_TEXTURE_2D, uiTextures[4]);
	LoadTGATexture("grass.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);


	// Load Texture
	glBindTexture(GL_TEXTURE_2D, uiTextures[5]);
	LoadTGATexture("wall.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT);

	objectFrame.SetOrigin(0.0f, 0.3f, 63.0f);
	
	// Randomly place the spheres
	for (int i = 0; i < NUM_SPHERES; i++) {
		GLfloat x = ((GLfloat)((rand() % 800) - 400) * 0.1f);
		GLfloat z = ((GLfloat)((rand() % 800) - 400) * 0.1f);
		spheres[i].SetOrigin(x, 0.3f, z);
		sph[i].x = x;
		sph[i].z = z;
	}

		for (int i = 0; i < 24; i++) {
			for (int j = 0; j < 4; j++)
			{
				cy[i][j].x = -5.0*(j - 2);
				cy[i][j].z = -140.0 - 10.0*i;
				cyl[i][j].SetOrigin(-5.0*(j - 2), -0.41f, -140.0 - 10.0*i);
			}
		}

		for (int i = 0; i < 12; i++)
		{
			for (int j = 0; j < 12; j++)
				cu[i][j].SetOrigin(-22.0 + (4.0*j), 1.59f, -430 - 4.0*i);
		}
	
}

////////////////////////////////////////////////////////////////////////
// Do shutdown for the rendering context
void ShutdownRC(void)
    {
	glDeleteTextures(6, uiTextures);
    }



        
// Called to draw scene
void RenderScene(void)
	{
	static CStopWatch	rotTimer;
	float yRot = rotTimer.GetElapsedSeconds() * 60.0f;
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	modelViewMatrix.PushMatrix();	
	M3DMatrix44f mCamera;
	cameraFrame.GetCameraMatrix(mCamera);
	modelViewMatrix.MultMatrix(mCamera);


	modelViewMatrix.PushMatrix();
	modelViewMatrix.Scale(1.0f, -1.0f, 1.0f); // Flips the Y Axis
	modelViewMatrix.Translate(0.0f, 30.41f, 0.0f); // Scootch the world down a bit...
	glFrontFace(GL_CW);

	WorldOrder2();

	glFrontFace(GL_CCW);
	modelViewMatrix.PopMatrix();
	
	// Draw the world upside down
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Scale(1.0f, -1.0f, 1.0f); // Flips the Y Axis
	modelViewMatrix.Translate(0.0f, 0.8f, 0.0f); // Scootch the world down a bit...
	glFrontFace(GL_CW);
	ObjectOrder();
	WorldOrder1(yRot);
	
	glFrontFace(GL_CCW);
	modelViewMatrix.PopMatrix();
	
	static GLfloat vFloorColor[] = { 1.0f, 1.0f, 1.0f, 0.75f };


	glBindTexture(GL_TEXTURE_2D, uiTextures[4]);

	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE,
		transformPipeline.GetModelViewProjectionMatrix(), 0);

	floorBatch1.Draw();
	floorBatch2.Draw();
	leftWallBatch.Draw();
	leftWallBatch1.Draw();
	rightWallBatch.Draw();
	rightWallBatch1.Draw();
	frontWallBatch3.Draw();
	frontWallBatch2.Draw();
	glBindTexture(GL_TEXTURE_2D, uiTextures[3]);

	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE,
		transformPipeline.GetModelViewProjectionMatrix(), 0);

	floorBatch3.Draw();
	//glBindTexture(GL_TEXTURE_2D, uiTextures[3]);
	//shaderManager.UseStockShader(GLT_SHADER_TEXTURE_RECT_REPLACE, transformPipeline.GetModelViewProjectionMatrix(), 0);
	frontWallBatch1.Draw();


	// Draw the solid ground
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, uiTextures[0]);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shaderManager.UseStockShader(GLT_SHADER_TEXTURE_MODULATE,
								 transformPipeline.GetModelViewProjectionMatrix(),
								 vFloorColor,
								 0); 
	
	frontWallBatch.Draw();
	floorBatch.Draw();
	floorBatch4.Draw();

	
	glDisable(GL_BLEND);
	
	ObjectOrder();
	WorldOrder2();
	WorldOrder1(yRot);

	modelViewMatrix.PopMatrix();
	
        
    // Do the buffer Swap
    glutSwapBuffers();
        
    // Do it again
    glutPostRedisplay();
    }


bool check(float xtot, float ztot, int mov)
{
	float xstart = -22.0;
	float cubewidth = 2.0f;
	float zstart = -430.0f;
	float sphererad =0.5;
	float z;
	float x;
	float i, j;
	int inti, intj;
	
	if (mov == 1)
	{
		z = ztot - sphererad;
		i = -(z + 428) / 4;
		inti = floor(i);
		intj = floor((xtot+24)/4);


	}
	else if (mov == 2)
	{
		z = ztot + sphererad;
		i = -(z + 428) / 4;
	
		inti = floor(i);
		intj = floor((xtot + 24) / 4);
	}
	else if (mov == 3)
	{
		x = xtot - sphererad;
		j = (x + 24) / 4;
		intj = floor(j);
		inti = floor(-(ztot + 428) / 4);
	}
	else
	{
		x = xtot + sphererad; 
		j = (x + 24) / 4;
		intj = floor(j);
		inti = floor(-(ztot + 428) / 4);
	}
	
	if (inti < 0 || intj < 0)
		return FALSE;
	else
	{
		if (m[inti][intj] == 1)
			return FALSE;
		else return TRUE;
	}


}

bool colDetect(GLfloat xtot, GLfloat ztot)
{
	int flag = 0;

	int i;
	for (i = 0; i < NUM_SPHERES; i++)
	{
		if (sqrt(pow(xtot - sph[i].x, 2) + pow(ztot - sph[i].z, 2)) <= 1.0f)
			flag++;

	}
	if (flag == 0)
		return FALSE;
	else return TRUE;

}

float angle=0.0;
float angle2 = 0.0;
float ztot = 61.0f;
float xtot = 0.0;
float gt= 0;
int flag2 = 0;
int ii = -1;
int ji= 0;
int flag3 = 0;
// Respond to arrow keys by moving the camera frame of reference
void SpecialKeys(int key, int x, int y)
    {
	float linear = 0.3f;
	float angular = float(m3dDegToRad(5.0f));
	float perpendicular = float(m3dDegToRad(90.0f));
	
	if (key == GLUT_KEY_UP)
	{
		if (ztot > -100.0f)
		{
			if (flag == 0 && (flag3 == 0))
			{
				cameraFrame.MoveForward(linear*cos(angle2));
				objectFrame.MoveForward(linear*cos(angle2));
				ztot -= linear*cos(angle2);
				cameraFrame.MoveRight(linear*sin(angle2));;
				objectFrame.MoveRight(linear*sin(angle2));
				xtot += linear*sin(angle2);
				angle2 = 0.0f;
				//if (colDetect(xtot,ztot))
					//flag3 = 1;
			}
		}
		else if (ztot > cy[23][1].z && ztot <-115.0f)
		{
			
				ii++;
				ji = 3;
				
				cameraFrame.SetOrigin(cy[ii][ji].x, 1.0f, cy[ii][ji].z+2.0f);
				objectFrame.SetOrigin(cy[ii][ji].x, 0.3f, cy[ii][ji].z);

				//cameraFrame.MoveForward(30.0f);
				//objectFrame.MoveForward(30.0f);
				float t= cy[ii][ji].z-ztot;
				ztot += t;
			}
		else if (ztot < cy[23][1].z - 10.0f)
		{
			if (ztot < -428.0f)
			{
				if (check(xtot, ztot - linear, 1))
				{
					cameraFrame.MoveForward(linear);
					objectFrame.MoveForward(linear);
					ztot -= linear;
				}
				/*
				if (!check(xtot, ztot - linear, 1))
				{
					cameraFrame.MoveForward(-linear);
					objectFrame.MoveForward(-linear);
					ztot += linear;
				}*/
			}

			else
			{
				cameraFrame.MoveForward(linear);
				objectFrame.MoveForward(linear);
				ztot -= linear;
			}
			
		}
		else
		{
			flag2=1;
		}
		}
	
		


	else if (key == GLUT_KEY_DOWN)
	{
		if (ztot > -100.0f)
		{
			if (flag == 0&&(flag3==0))
			{
				cameraFrame.MoveForward(-linear*cos(angle2));
				objectFrame.MoveForward(-linear*cos(angle2));
				ztot -= -linear*cos(angle2);
				cameraFrame.MoveRight(-linear*sin(angle2));;
				objectFrame.MoveRight(-linear*sin(angle2));
				xtot += -linear*sin(angle2);
				angle2 = 0.0f;
				//if (colDetect(xtot, ztot))
					//flag3 = 1;
		
			}
			
		}
		else if (ztot > cy[23][1].z && ztot <-115.0f)
		{
		
				ii++;
				ji = 2;
				
				cameraFrame.SetOrigin(cy[ii][ji].x, 1.0f, cy[ii][ji].z + 2.0f);
				objectFrame.SetOrigin(cy[ii][ji].x, 0.3f, cy[ii][ji].z);
				//cameraFrame.MoveForward(30.0f);
				//objectFrame.MoveForward(30.0f);
				float t = cy[ii][ji].z - ztot;
				ztot += t;
			}
		else if (ztot < cy[23][1].z-10.0f)
		{
			if (ztot < -428.0f)
			{
				if (check(xtot, ztot + linear, 1))
				{
					cameraFrame.MoveForward(-linear);
					objectFrame.MoveForward(-linear);
					ztot += linear;
				}
				/*
				if (!check(xtot, ztot + linear, 1))
				{
					cameraFrame.MoveForward(linear);
					objectFrame.MoveForward(linear);
					ztot -= linear;
				}*/
			}
			else
			{
				cameraFrame.MoveForward(-linear);
				objectFrame.MoveForward(-linear);
				ztot += linear;
			}
			
		}
		else
		{
			flag2 = 1;
		}
		}
		
	
	else if (key == GLUT_KEY_LEFT)
	{
		if (ztot > -100.0f)
		{
			if (flag == 0)
			{	
				if (angle > -m3dDegToRad(30.0f))
				{
					angle2 -= angular;
					angle -= angular;
					cameraFrame.RotateWorld(angular, 0.0f, 1.0f, 0.0f);
					objectFrame.RotateWorld(angular, 0.0f, 1.0f, 0.0f);
				}
			}
		}
		else if (ztot > cy[23][1].z && ztot <-115.0f)
		{
			ii++;
			ji = 1;

			cameraFrame.SetOrigin(cy[ii][ji].x, 1.0f, cy[ii][ji].z + 2.0f);
			objectFrame.SetOrigin(cy[ii][ji].x, 0.3f, cy[ii][ji].z);
			//cameraFrame.MoveForward(30.0f);
			//objectFrame.MoveForward(30.0f);
			float t = cy[ii][ji].z - ztot;
			ztot += t;

		}
		else if (ztot < cy[23][1].z-10.0f)
		{

			if (ztot < -428.0f)
			{
				if (check(xtot-linear, ztot, 1))
				{
					cameraFrame.MoveRight(linear);
					objectFrame.MoveRight(linear);
					xtot -= linear;
				}
			}
			else
			{
				cameraFrame.MoveRight(linear);
				objectFrame.MoveRight(linear);
				xtot-= linear;
			}
				}
		else{
			flag2 = 1;
		}

		
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		if (ztot > -100.0f)
		{
			if (flag == 0)
			{
				if (angle < m3dDegToRad(30.0f))
				{
					angle2 += angular;
					angle += angular;
					cameraFrame.RotateWorld(-angular, 0.0f, 1.0f, 0.0f);
					objectFrame.RotateWorld(-angular, 0.0f, 1.0f, 0.0f);
				}
			}
		}
		else if (ztot > cy[23][1].z && ztot <-115.0f)
		{
			ii++;
			ji = 0;

			cameraFrame.SetOrigin(cy[ii][ji].x, 1.0f, cy[ii][ji].z + 2.0f);
			objectFrame.SetOrigin(cy[ii][ji].x, 0.3f, cy[ii][ji].z);
			//cameraFrame.MoveForward(30.0f);
			//objectFrame.MoveForward(30.0f);
			float t = cy[ii][ji].z - ztot;
			ztot += t;
		}
		else if (ztot < cy[23][1].z-10.0f)
		{
			if (ztot < -428.0f)
			{
				if (check(xtot+linear, ztot - linear, 1))
				{
					cameraFrame.MoveRight(-linear);
					objectFrame.MoveRight(-linear);
					xtot += linear;
				}
			}
			else
			{
				cameraFrame.MoveRight(-linear);
				objectFrame.MoveRight(-linear);
				xtot += linear;
			}
			
	
		}

		else{
			flag2 = 1;
		}
		
	}
	else if (key == GLUT_KEY_INSERT&&(flag==0))
	{

		cameraFrame.RotateWorld(-angle, 0.0f, 1.0f, 0.0f);
		objectFrame.RotateWorld(-angle, 0.0f, 1.0f, 0.0f);
		angle = 0.0;
		cameraFrame.MoveForward(40.0f);
		cameraFrame.RotateWorld(-float(m3dDegToRad(90.0f)), 1.0f, 0.0f, 0.0f);
	
		cameraFrame.MoveForward(-160.0f);
		flag = 1;
	}
	else if (key == GLUT_KEY_INSERT && (flag == 1))
	{
		flag = 0;
		cameraFrame.MoveForward(160.0f);
		cameraFrame.RotateWorld(float(m3dDegToRad(90.0f)), 1.0f, 0.0f, 0.0f);
		cameraFrame.MoveForward(-40.0f);
	}
	else if (key == GLUT_KEY_END&&flag2 == 1)
	{
		
	
		cameraFrame.SetOrigin(0.0f, 1.0f, ztot-18.0f);
		objectFrame.SetOrigin(0.0f, 0.3f, ztot- 20.0f);
		ztot -= 20.0f;
		xtot = 0.0f;
		flag2 = 0;
	}

	
	}


void ChangeSize(int nWidth, int nHeight)
    {
	glViewport(0, 0, nWidth, nHeight);
	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
	
	viewFrustum.SetPerspective(45.0f, float(nWidth)/float(nHeight), 0.01f, 200.0f);
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
	modelViewMatrix.LoadIdentity();
	}

int main(int argc, char* argv[])
    {
	gltSetWorkingDirectory(argv[0]);
		

	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1366,768);
  
    glutCreateWindow("Anurag Misra");
 
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKeys);
	 
    SetupRC();
    glutMainLoop();    
    ShutdownRC();
    return 0;
    }
