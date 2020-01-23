#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <strstream>

/*
 - Library provided from https://github.com/OneLoneCoder
 - Used to draw shapes to the console 
*/
#include "olcConsoleGameEngine.h"

#include "Matrix.h"

//#define CUBE_DEMO

//class Triangle
//{
//public:
//
//	Triangle() : vertices(3) {
//		Triangle({ {0.0f, 0.0f, 0.f}, {0.0f, 0.0f, 0.f}, {0.0f, 0.0f, 0.f} });
//	}
//	
//	Triangle(std::vector< std::vector<float > > v) : vertices(3)
//	{
//		vertices[0] = v[0];
//		vertices[1] = v[1];
//		vertices[2] = v[2];
//	}
//
//	std::vector<Vector3D> vertices;
//
//	wchar_t symbol;
//	short colour;
//};

struct Vector3D {
	// vector coordinates
	float x, y, z;

	void cross(Vector3D& line, Vector3D& normal)
	{
		// Calculate normal vector of the traingle
		normal.x = y * line.z - z * line.y;
		normal.y = z * line.x - x * line.z;
		normal.z = x * line.y - y * line.x;
	}

	float dot(Vector3D& vec)
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	void normalize()
	{
		float normalize = sqrtf(pow(x, 2) + pow(y, 2) + pow(z, 2));

		x /= normalize;
		y /= normalize;
		z /= normalize;
	}
};

struct Triangle {
	Vector3D vertices[3];

	wchar_t symbol;
	short colour;
};

struct operation_matrix
{
	Matrix m(4, 4);
};

std::vector<Triangle> cube_demo()
{
	return {

		// SOUTH
		{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		// EAST                                                      
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		// NORTH                                                     
		{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

		// WEST                                                      
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

		// TOP                                                       
		{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

		// BOTTOM                                                    
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

	};
	
}

operation_matrix projection_matrix;
operation_matrix x_rotation;
operation_matrix z_rotation;

class EmazingEngine : public olcConsoleGameEngine
{
public:
	bool OnUserCreate() override
	{
#ifdef CUBE_DEMO
		object = cube_demo();	
#else
		object = LoadOBJFile("3DTestObjects/teapot.obj");
#endif  CUBE_DEMO

		projection_matrix.m.Assign({ { aspect_ratio * scaling_factor, 0.0f, 0.0f, 0.0f }, { 0.0f, scaling_factor, 0.0f, 0.0f }, { 0.0f, 0.0f, q, 1.0f } , { 0.0f, 0.0f, -z_near * q, 0.0f } });

		lighting.x = 0.0f; lighting.y = 0.0f; lighting.z = -1.0f;

		lighting.normalize();

		cam.x = 0.0f; cam.y = 0.0f; cam.z = 0.0f;
		
		return true;

	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Clear screen to redraw
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

		rotate_angle += 1.0f * fElapsedTime;

		x_rotation.m.Assign({ {1, 0, 0, 0}, {0, cosf(rotate_angle * 0.5f), sinf(rotate_angle * 0.5f), 0}, {0, -sinf(rotate_angle * 0.5f), cosf(rotate_angle * 0.5f), 0}, {0, 0, 0, 1} });
		z_rotation.m.Assign({ {cosf(rotate_angle), sinf(rotate_angle), 0, 0}, {-sinf(rotate_angle), cosf(rotate_angle), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} });


		for (auto o : object)
		{
			
			// Rotate around z-axis
			coordinate_projection(o.vertices[0], matRotZ, rZ.vertices[0]);
			coordinate_projection(o.vertices[1], matRotZ, rZ.vertices[1]);
			coordinate_projection(o.vertices[2], matRotZ, rZ.vertices[2]);

			// Rotate around x-axis
			coordinate_projection(rZ.vertices[0], matRotX, rX.vertices[0]);
			coordinate_projection(rZ.vertices[1], matRotX, rX.vertices[1]);
			coordinate_projection(rZ.vertices[2], matRotX, rX.vertices[2]);

			// move object back so it is in view of the camera
			rX.vertices[0].z += 8.0f;
			rX.vertices[1].z += 8.0f;
			rX.vertices[2].z += 8.0f;

			// Construct line 1 of the triangle
			l1.x = rX.vertices[1].x - rX.vertices[0].x;
			l1.y = rX.vertices[1].y - rX.vertices[0].y;
			l1.z = rX.vertices[1].z - rX.vertices[0].z;

			// Contstruct line 2 of the triangle
			l2.x = rX.vertices[2].x - rX.vertices[0].x;
			l2.y = rX.vertices[2].y - rX.vertices[0].y;
			l2.z = rX.vertices[2].z - rX.vertices[0].z;

			// calculate normal of triangle face
			l1.cross(l2, normal);

			normal.normalize();

			// Calculate the angle betwwen the normal and the camera projection to determine if the triangle is visible
			if ( (normal.x * ( rX.vertices[0].x - cam.x )
				+ normal.y * ( rX.vertices[0].y - cam.y )
				+ normal.z * ( rX.vertices[0].z - cam.z ) ) < 0.0f )
			{
				// Dot product is used to determine how direct the light is hitting the traingle to determine what shade it should be
				float dotProd = normal.dot(lighting);

				// The larger dot product in this case means the more lit up the triangle face will be 
				CHAR_INFO colour = GetColour(dotProd);
				
				// Project all the coordinates to 2D space
				coordinate_projection(rX.vertices[0], matProj, pro.vertices[0]);
				coordinate_projection(rX.vertices[1], matProj, pro.vertices[1]);
				coordinate_projection(rX.vertices[2], matProj, pro.vertices[2]);

				// Center the points and change the scale
				pro.vertices[0].x += 1.0f;
				pro.vertices[0].y += 1.0f;

				pro.vertices[1].x += 1.0f;
				pro.vertices[1].y += 1.0f;

				pro.vertices[2].x += 1.0f;
				pro.vertices[2].y += 1.0f;

				pro.vertices[0].x *= 0.5f * (float)ScreenWidth();
				pro.vertices[0].y *= 0.5f * (float)ScreenHeight();

				pro.vertices[1].x *= 0.5f * (float)ScreenWidth();
				pro.vertices[1].y *= 0.5f * (float)ScreenHeight();

				pro.vertices[2].x *= 0.5f * (float)ScreenWidth();
				pro.vertices[2].y *= 0.5f * (float)ScreenHeight();

				pro.symbol = colour.Char.UnicodeChar;
				pro.colour = colour.Attributes;

				renderTriangles.push_back(pro);	
			}
	
		}

		// sort triangles by their average z value
		// higher z values mean the traingle should be rendered later
		std::sort(renderTriangles.begin(), renderTriangles.end(), [](Triangle& tri1, Triangle& tri2) {
			float avg_z1 = (tri1.vertices[0].z + tri1.vertices[1].z + tri1.vertices[2].z) / 3.0f;
			float avg_z2 = (tri2.vertices[0].z + tri2.vertices[1].z + tri2.vertices[2].z) / 3.0f;

			if (avg_z1 > avg_z2)
				return true;
			else
				return false;
			});
					
		// render all the triangles in order now 
		for (auto t : renderTriangles) 
		{
			FillTriangle(t.vertices[0].x, t.vertices[0].y, t.vertices[1].x, t.vertices[1].y, t.vertices[2].x, t.vertices[2].y, t.symbol, t.colour);
		}
		
		// vector needs to be empty for next redraw
		renderTriangles.clear();

		return true;
	}

	// Map 3D coordinates to 2D space
	void coordinate_projection(Vector3D& vertex, operation_matrix& operation, Vector3D& outVec)
	{
		outVec.x = vertex.x * operation.m.(0, 0) + vertex.y * operation.m.value[1][0] + vertex.z * operation.m.value[2][0] + operation.m.value[3][0];
		outVec.y = vertex.x * operation.m.value[0][1] + vertex.y * operation.m.value[1][1] + vertex.z * operation.m.value[2][1] + operation.m.value[3][1];
		outVec.z = vertex.x * operation.m.value[0][2] + vertex.y * operation.m.value[1][2] + vertex.z * operation.m.value[2][2] + operation.m.value[3][2];

		float leftOver = vertex.x * operation.m.value[0][3] + vertex.y * operation.m.value[1][3] + vertex.z * operation.m.value[2][3] + operation.m.value[3][3];

		// Dived entire matrix by the last value to convert it back to 3D space
		// Also satistfies dividing the terms by z

		if (leftOver != 0.0f)
		{
			outVec.x /= leftOver;
			outVec.y /= leftOver;
			outVec.z /= leftOver;
		}

	}

	// Loads vertex and face data from txt file realting to obj file
	std::vector<Triangle> LoadOBJFile(std::string fname)
	{
		std::ifstream readFile;
		readFile.open(fname);

		if (!readFile.is_open())
		{
			std::cout << "Cannot open file!";
		}

		Vector3D vertex;
		std::vector<Vector3D> vertices;

		std::vector<Triangle> triangles;

		std::string line;

		char startingChar; // Stores the starting character of the line

		int i1, i2, i3; // indexes of the vertices

		// iterate through all lines in file
		while (std::getline(readFile, line))
		{
			std::strstream st;
			st << line;

			// indicates vertex data
			if (line[0] == 'v')
			{
				st >> startingChar >> vertex.x >> vertex.y >> vertex.z;
				vertices.push_back(vertex);
			}
			// indicates traingle face data
			else if (line[0] == 'f')
			{
				st >> startingChar >> i1 >> i2 >> i3;
				triangles.push_back(
					{vertices[i1 - 1], vertices[i2 - 1], vertices[i3 - 1]} );
			}
		}

		return triangles;
	}

	

private:
	std::vector<Triangle> object;
	float z_far = 1000.0f; // represents the distance from the theoretical distance in the screen to the users face
	float z_near = 0.1f; // represents the distance from the users face to the screen
	float q = z_far / (z_far - z_near);

	float theta = 90.0f; // the field of view for the player

	float scaling_factor = 1.0f / tanf(theta * 0.5f / 180.0f * 3.14159f); // amount needed to scale coordinates based on the fov
	float aspect_ratio = (float)ScreenHeight() / (float)ScreenWidth();

	float rotate_angle; // perodically changing to give the appearance that the object is rotating

	// Projection matrices
	Triangle pro;

	Triangle rX;

	Triangle rZ;

	// Vectors to calculate the normal of triangle faces.
	Vector3D l1;
	Vector3D l2;
	Vector3D normal;

	// Position of camera aka user's view
	Vector3D cam;

	// Direction that the light would be pointing in game
	Vector3D lighting;

	std::vector<Triangle> renderTriangles;
	
};

int main()
{
	EmazingEngine game;
	if (game.ConstructConsole(264, 250, 4, 4))
		game.Start();
		
	return 0;
}
