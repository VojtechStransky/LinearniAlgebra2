#include<iostream>
#include<limits>
#include<optional>
#include<tgmath.h>

using namespace std;

//Method used for cin int based on printed instruction
int cinIntOnly(string instruction)
{
    cout << instruction + " [integer only]" << endl;
    int x = 0;

    while(!(cin >> x)){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input.  Try again: ";
    }

    return x;
}

//Method used for creatig square matrix based on size parameter
float** createSquareMatrix(int size)
{
    float** matrix = new float*[size];

    for (int i = 0; i < size; i++) //For all rows create array to represent columns
    {
        matrix[i] = new float[size];
    }
    return matrix;
}

//Method used for interactive filling matrix by user
pair<float**, float**> askForMatrixes(int size)
{
    float** matrix = createSquareMatrix(size);
    float** matrix1 = createSquareMatrix(size);

    cout << "Follow instructions to create input matrix" << endl;

    for(int i = 0; i < size; i++)
    {
        cout << "Row " + to_string(i + 1) + " : [insert number by number as you are asked]" << endl;
        for(int y = 0; y < size; y++)
        {
            float value = cinIntOnly("Insert number " + to_string(y + 1) + " of row " + to_string(i + 1) + ":");
            matrix[i][y] = value;
            matrix1[i][y] = value;
        }
    }

    return pair(matrix, matrix1);
}

//Method used for interactive creating vector
float* askForVector(int size)
{
    float* vector = new float[size];

    cout << "Follow instructions to create input vector" << endl;

    for(int i = 0; i < size; i++)
    {
        float value = cinIntOnly("Insert number " + to_string(i + 1) + ":");
        vector[i] = value;
    }

    return vector;
}

//Metod used to print matrix
void printMatrix(int size, float** matrix, string name = "")
{
    cout << "\nMatrix \"" + name + "\":" << endl;

    for(int i = 0; i < size; i++)
    {
        for(int y = 0; y < size; y++)
        {
            cout << matrix[i][y] << "\t"; //For all rows and columns output numer and tab to format
        }

        cout << endl;
    }
}

//Metod used to print a vector
void printVector(int size, float* vector, string name = "")
{
    cout << "\nVector \"" + name + "\":" << endl;

    for(int i = 0; i < size; i++)
    {
        cout << vector[i] << "\t";

        cout << endl;
    }
}
//Method selects first non zero member in column
optional<int> selectFirst(int size, float** matrix, int column)
{
    for(int i = column; i < size; i++) //todo
    {
        if (matrix[i][column] != 0)
        {
            return i;
        }
    }

    return nullopt;
}

//Helper method for changing twho lines
void changeLines(float** matrix, int index, int index2)
{
    float* indexLine = matrix[index];
    matrix[index] = matrix[index2];
    matrix[index2] = indexLine;
}

//Method calculates determinant of the inputed matrix by gauss elimination and multiplication on diagonal members
float determinant(int size, float** matrix)
{
    float result = 1;

    for(int y = 0; y < size; y++) { //for all columns
        optional<int> optIndex = selectFirst(size, matrix, y); //select first row and change for non null one

        if (optIndex == nullopt) return 0; //if singular - zero

        int index = optIndex.value();

        changeLines(matrix, index, y);

        float first = matrix[y][y]; //hold first member
        result *= first; //multiply result by another diagonal member

        for(int i = y + 1; i < size; i ++) { //for all rows
            int firstInRow = matrix[i][y]; // hold first in row

            if (firstInRow == 0) //if zero there is nothing to do
            {
                continue;
            }

            for(int z = 0; z < size; z++) { //for all columns subtract multiplies. Lowest common multiple is waste of supplies
                matrix[i][z] = matrix[i][z] - firstInRow*matrix[y][z]/first;
            }
        }
    }

    return result;
}

float** submatrix(int size, int x, int y, float** matrix)
{
    float** submatrix = createSquareMatrix(size - 1);

    for (int i = 0; i < size - 1; i++)
    {
        for (int z = 0; z < size - 1; z++)
        {
            int k = (i >= x) ? 1 : 0;
            int l = (z >= y) ? 1 : 0; //member offset

            submatrix[i][z] = matrix[i+k][z+l];
        }
    }

    return submatrix;
}

float** changeColumn(int size, float** matrix, float* vector, int column)
{
    float** toReturn = createSquareMatrix(size);

    for (int i = 0; i < size; i++)
    {
        for (int y = 0; y < size; y++)
        {
            toReturn[i][y] = (y == column) ? vector[i] : matrix[i][y]; //if selected to change, change, otherwise copy
        }
    }

    return toReturn;
}

float** calculateInverse(int size, float originalDeterminant, float** matrix)
{
    float** inverse = createSquareMatrix(size);

    for (int i = 0; i < size; i++)
    {
        for (int y = 0; y < size; y++)
        {
            inverse[i][y] = pow(-1, i+y)*determinant(size-1, submatrix(size, y, i, matrix)) /originalDeterminant; //Calculation using adjunged matrix
        }
    }

    return inverse;
}

float* solve(int size, float originalDeterminant, float** matrix, float* vector)
{
    float* solution = new float[size];

    for (int i = 0; i < size; i++)
    {
        float** modifiedMatrix = changeColumn(size, matrix, vector, i);
        solution[i] = determinant(size, modifiedMatrix)/originalDeterminant;
    }

    return solution;
}

int main(void)
{
    try //try for potential errors
    {
        int size = cinIntOnly("Insert size of matrix:"); //init-input part

        pair<float**, float**> matrixes = askForMatrixes(size);
        printMatrix(size, matrixes.first, "inserted matrix");

        float* vector = askForVector(size);
        printVector(size, vector, "inserted vector");

        float originalDeterminant = determinant(size, matrixes.first);

        if (originalDeterminant == 0) //if determinant = 0, then singular, then no inverse exists
        {
            cout << "Input matrix is singular!";
            return 0;
        };

        float** inverse = calculateInverse(size, originalDeterminant, matrixes.second);
        float* solution = solve(size, originalDeterminant, matrixes.second, vector);

        printMatrix(size, inverse, "inverse matrix");
        printVector(size, solution, "solution vector");
    }
    catch (const std::exception& ex) //handle errors
    {
        cout << "Error!" << endl;
        cout << ex.what();
    }

    return 0;
}

