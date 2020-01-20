#include <vector>
#include <iostream>

/*
 - Library provided from https://github.com/OneLoneCoder
 - Used to draw shapes to the console 
*/
#include "olcConsoleGameEngine.h"


#define CUBE_DEMO

/**********************************************
class Matrix
@brief Class used to represent matrices with
overloaded operator functions for proper
matrix operations
************************************************/
class Matrix
{
public:
	Matrix(int row, int col)
	{
		r = row;
		c = col;
		value.resize(row, std::vector<float>(col));
	}

	// Copies vector to this classes vector
	void Assign(std::vector< std::vector<float> > v)
	{
		// Check for proper dimensions
		if (value.size() == v.size() && value[0].size() == v[0].size())
		{
			value = v;
		}
		else
			throw "Maxtrices are not of same dimension!";
	}

	// Reduce the amount of columns by 1
	void clip()
	{
		for (auto vec : value)
			vec.pop_back();
		c--;
	}


	Matrix operator + (Matrix const& obj)
	{
		Matrix res(r, c);
		std::vector< std::vector<float> > result;
		result.resize(r, std::vector<float>(c));

		// Check for proper dimensions
		if (value.size() == obj.value.size() && value[0].size() == obj.value[0].size())
		{
			for (int i = 0; i < r; i++)
				for (int j = 0; j < c; j++)
					result[i][j] = value[i][j] + obj.value[i][j];
		}
		else
			throw "Maxtrices are not of same dimension!";

		res.Assign(result);

		return res;

	}

	Matrix operator - (Matrix const& obj)
	{
		Matrix res(r, c);
		std::vector< std::vector<float> > result;
		result.resize(r, std::vector<float>(c));

		// Check for proper dimensions
		if (value.size() == obj.value.size() && value[0].size() == obj.value[0].size())
		{
			for (int i = 0; i < r; i++)
				for (int j = 0; j < c; j++)
					result[i][j] = value[i][j] - obj.value[i][j];
		}
		else
			throw "Maxtrices are not of same dimension!";

		res.Assign(result);

		return res;

	}

	// Implements the dot product between two matrices
	Matrix operator * (Matrix const& obj)
	{
		Matrix res(r, obj.c);
		std::vector< std::vector<float> > result;
		result.resize(r, std::vector<float>(obj.c));

		// Check for proper dimensions
		if (value[0].size() == obj.value.size())
		{
			
				for (int i = 0; i < r; i++)
				{
					for (int k = 0; k < obj.c; k++)
					{
						for (int j = 0; j < obj.r; j++)
							result[i][k] += value[i][j] * obj.value[j][k];
					}
				}
		}
		else
			throw "Maxtrices are not of same dimension!";

		res.Assign(result);

		return res;
	}

	Matrix operator / (float divisor)
	{
		Matrix res(r, c);
		std::vector< std::vector<float> > result;
		result.resize(r, std::vector<float>(c));
		result = value;

		if (divisor != 0.0f)
		{

			for (int i = 0; i < r; i++)
			{

				for (int j = 0; j < c; j++)
					result[i][j] /= divisor;
			}

			res.Assign(result);
		}
		else
		{
			return *this;
		}

		return res;
	}

	// allows value to be accessed easier
	float operator () (int i, int j)
	{
		return value[i][j];
	}

	



	// Holds values in proper structure
	std::vector< std::vector<float> > value;

private:
	// Holds the values for rows and columns of this object
	int r;
	int c;

};

class Triangle
{
public:
	
	Triangle(std::vector< std::vector<float > > v) : vertices(3, 3)
	{
		vertices.Assign(v);
	}

	Matrix vertices;
};

std::vector<Triangle> cube_demo()
{
	return
	{
		Triangle({ { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } }),
		Triangle({ { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } }),

		Triangle({ { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } }),
		Triangle({ { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f } }),

		Triangle({ { 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f } }),
		Triangle({ { 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } }),

		Triangle({ { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } }),
		Triangle({ { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } }),

		Triangle({ { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } }),
		Triangle({ { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f } }),

		Triangle({ { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } }),
		Triangle({ { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } }),
	};
}

class EmazingEngine : public olcConsoleGameEngine
{
public:
	bool OnUserCreate() override
	{
#ifdef CUBE_DEMO
		object = cube_demo();		
#endif  CUBE_DEMO

		return true;

	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Clear screen to redraw
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

		rotate_angle += 1.0f * fElapsedTime;

		// Projection matrices
		Matrix pro1(1, 3);
		Matrix pro2(1, 3);
		Matrix pro3(1, 3);

		Matrix rX1(1, 3);
		Matrix rX2(1, 3);
		Matrix rX3(1, 3);

		Matrix rZ1(1, 3);
		Matrix rZ2(1, 3);
		Matrix rZ3(1, 3);

		Matrix l1(1, 3);
		Matrix l2(1, 3);
		Matrix normal(1, 3);

		

		for (auto t : object)
		{
			

			rZ1 = z_axis_rotation(t.vertices(0, 0), t.vertices(0, 1), t.vertices(0, 2));
			rZ2 = z_axis_rotation(t.vertices(1, 0), t.vertices(1, 1), t.vertices(1, 2));
			rZ3 = z_axis_rotation(t.vertices(2, 0), t.vertices(2, 1), t.vertices(2, 2));

			rX1 = x_axis_rotation(rZ1(0, 0), rZ1(0, 1), rZ1(0, 2));
			rX2 = x_axis_rotation(rZ2(0, 0), rZ2(0, 1), rZ2(0, 2));
			rX3 = x_axis_rotation(rZ3(0, 0), rZ3(0, 1), rZ3(0, 2));

			rX1.value[0][2] += 3.0f;
			rX2.value[0][2] += 3.0f;
			rX3.value[0][2] += 3.0f;

			// Construct line 1 of the triangle
			l1.value[0][0] = rX2(0, 0) - rX1(0, 0);
			l1.value[0][1] = rX2(0, 1) - rX1(0, 1);
			l1.value[0][2] = rX2(0, 2) - rX1(0, 2);

			// Contstruct line 2 of the triangle
			l2.value[0][0] = rX3(0, 0) - rX1(0, 0);
			l2.value[0][1] = rX3(0, 1) - rX1(0, 1);
			l2.value[0][2] = rX3(0, 2) - rX1(0, 2);

			// Calculate normal vector of the traingle
			normal.value[0][0] = l1(0, 1) * l2(0, 2) - l1(0, 2) * l2(0, 1);
			normal.value[0][1] = l1(0, 2) * l2(0, 0) - l1(0, 0) * l2(0, 2);
			normal.value[0][2] = l1(0, 0) * l2(0, 1) - l1(0, 1) * l2(0, 0);

			if (normal(0, 2) > 0)
			{
				pro1 = coordinate_projection(rX1(0, 0), rX1(0, 1), rX1(0, 2));
				pro2 = coordinate_projection(rX2(0, 0), rX2(0, 1), rX2(0, 2));
				pro3 = coordinate_projection(rX3(0, 0), rX3(0, 1), rX3(0, 2));

				pro1.value[0][0] += 1.0f;
				pro1.value[0][1] += 1.0f;

				pro2.value[0][0] += 1.0f;
				pro2.value[0][1] += 1.0f;

				pro3.value[0][0] += 1.0f;
				pro3.value[0][1] += 1.0f;

				pro1.value[0][0] *= 0.5f * (float)ScreenWidth();
				pro1.value[0][1] *= 0.5f * (float)ScreenHeight();

				pro2.value[0][0] *= 0.5f * (float)ScreenWidth();
				pro2.value[0][1] *= 0.5f * (float)ScreenHeight();

				pro3.value[0][0] *= 0.5f * (float)ScreenWidth();
				pro3.value[0][1] *= 0.5f * (float)ScreenHeight();

				DrawTriangle(pro1(0, 0), pro1(0, 1), pro2(0, 0), pro2(0, 1), pro3(0, 0), pro3(0, 1), PIXEL_SOLID, FG_WHITE);
			}
	
		}

		return true;
	}

	// Map 3D coordinates to 2D space
	Matrix coordinate_projection(float x, float y, float z)
	{
		// Put coordinates in vector format
		Matrix coordinate_vec(1, 4);
		coordinate_vec.Assign({ { x, y, z, 1.0f } }); // 1 is added so that -z_near*q can be subtracted from z*q

		Matrix projection_matrix(4, 4);
		projection_matrix.Assign({ { aspect_ratio * scaling_factor, 0.0f, 0.0f, 0.0f }, { 0.0f, scaling_factor, 0.0f, 0.0f }, { 0.0f, 0.0f, q, 1.0f } , { 0.0f, 0.0f, -z_near * q, 0.0f } });

		Matrix res = coordinate_vec * projection_matrix;

		// Dived entire matrix by the last value to convert it back to 3D space
		// Also satistfies dividing the terms by z
		res = res / res.value[0][3];

		// Change matrix to 1X3
		res.clip();

		return res;
	}

	Matrix x_axis_rotation(float x, float y, float z)
	{
		// Put coordinates in vector format
		Matrix coordinate_vec(1, 4);
		coordinate_vec.Assign({ { x, y, z, 1.0f } });

		Matrix x_rotation(4, 4);
		x_rotation.Assign({ {1, 0, 0, 0}, {0, cosf(rotate_angle * 0.5f), sinf(rotate_angle * 0.5f), 0}, {0, -sinf(rotate_angle * 0.5f), cosf(rotate_angle * 0.5f), 0}, {0, 0, 0, 1} });

		Matrix res = coordinate_vec * x_rotation;

		// Dived entire matrix by the last value to convert it back to 3D space
		// Also satistfies dividing the terms by z
		res = res / res.value[0][3];

		// Change matrix to 1X3
		res.clip();

		return res;
	}

	Matrix z_axis_rotation(float x, float y, float z)
	{
		// Put coordinates in vector format
		Matrix coordinate_vec(1, 4);
		coordinate_vec.Assign({ { x, y, z, 1.0f } });

		Matrix z_rotation(4, 4);
		z_rotation.Assign({ {cosf(rotate_angle), sinf(rotate_angle), 0, 0}, {-sinf(rotate_angle), cosf(rotate_angle), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} });

		Matrix res = coordinate_vec * z_rotation;

		// Dived entire matrix by the last value to convert it back to 3D space
		// Also satistfies dividing the terms by z
		res = res / res.value[0][3];

		// Change matrix to 1X3
		res.clip();

		return res;
	}

private:
	std::vector<Triangle> object;
	float z_far = 1000.0f; // represents the distance from the theoretical distance in the screen to the users face
	float z_near = 0.1f; // represents the distance from the users face to the screen
	float q = z_far / (z_far - z_near);

	float theta = 90.0f; // the field of view for the player

	float scaling_factor = 1.0f / tanf(theta * 0.5f / 180.0f * 3.14159f); // amount needed to scale coordinates based on the fov
	float aspect_ratio = (float)ScreenHeight() / (float)ScreenWidth();

	float rotate_angle;
};


int main()
{
	EmazingEngine game;
	if (game.ConstructConsole(250, 250, 4, 4))
		game.Start();

	/*Matrix test(2, 3);
	test.Assign({ { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } });

	Matrix yeet(3, 4);
	yeet.Assign({ { 2.0f, 3.0f, 0.0f, 5.0f }, { 5.0f, 1.0f, 4.0f, 7.8f }, { 1.0f, 1.0f, 2.0f, 3.5f } });

	Matrix res = test * yeet;*/
	/*EmazingEngine game;

	Matrix res = game.coordinate_projection(0.0f, 1.0f, 0.0f);

	std::cout << res.value[0][3] << std::endl << res.value[0][0] << std::endl << res.value[0][1] << std::endl << res.value[0][2];*/

	/*for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout << res.value[i][j];
		std::cout << std::endl;
	}*/



	return 0;
}
