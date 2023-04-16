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

float** copyMatrix(int size, float** matrix)
{
    float** copy = createSquareMatrix(size);

    for (int i = 0; i < size; i++) //For all rows create array to represent columns
    {
        for (int y = 0; y < size; y++) //For all rows create array to represent columns
        {
            copy[i][y] = matrix[i][y];
        }
    }

    return copy;
}

//Method used for interactive filling matrix by user
float** askForMatrix(int size)
{
    float** matrix = createSquareMatrix(size);

    cout << "Follow instructions to create input matrix" << endl;

    for(int i = 0; i < size; i++)
    {
        cout << "Row " + to_string(i + 1) + " : [insert number by number as you are asked]" << endl;
        for(int y = 0; y < size; y++)
        {
            float value = cinIntOnly("Insert number " + to_string(y + 1) + " of row " + to_string(i + 1) + ":");
            matrix[i][y] = value;
        }
    }

    return matrix;
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

float** askForVectors(int size)
{
    float** vectors = new float*[size];

    cout << "Follow instructions to create array of input vectors" << endl;

    for(int i = 0; i < size; i++)
    {
        vectors[i] = askForVector(size);
    }

    return vectors;
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
void printVectors(int size, float** vectors, string name = "")
{
    cout << "\nVectors \"" + name + "\":" << endl;

    for(int i = 0; i < size; i++)
    {
        cout << "( ";

        for(int y = 0; y < size; y++)
        {
            cout << to_string(vectors[i][y]) + ((y == size - 1) ? "" : ", ");
        }

        cout << ")" << endl;
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
bool checkByDeterminant(int size, float** matrix, bool checkIndependence = false)
{
    float result = 1;

    for(int y = 0; y < size; y++) { //for all columns
        optional<int> optIndex = selectFirst(size, matrix, y); //select first row and change for non null one

        if (optIndex == nullopt)
        {
            cout << ((checkIndependence) ? "Vectors are linearly dependent!" : "Myltiplication matrix is not positive definitive!") << endl;

            return false; //if singular - zero
        }

        int index = optIndex.value();

        if (index != y)
        {
            changeLines(matrix, index, y);
            result *= -1;
        }

        float first = matrix[y][y]; //hold first member
        result *= first; //multiply result by another diagonal member

        if (!checkIndependence && (result <= 0)) {
            cout << "Multiplication matrix is not positive definite!" << endl;
            return false;
        }

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

    return true;
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

bool checkMultiplicationMatrix(int size, float** matrix)
{
    for (int i = 0; i < size; i++)
    {
        for (int y = 0; y < i; y++)
        {
            if (matrix[i][y] != matrix[y][i])
            {
                cout << "Multiplication matrix is not symetric!" << endl;
                return false;
            }
        }
    }

    return checkByDeterminant(size, matrix);
}

float multiply(int size, float* vectorOne, float* vectorTwo, float** matrix)
{
    int result = 0;

    for (int i = 0; i < size; i++)
    {
        int helper = 0;

        for (int y = 0; y < size; y++)
        {
            helper += (matrix[i][y] * vectorTwo[y]);
        }

        result += helper * vectorOne[i];
    }

    return result;
}

float* addVectors(int size, float* vectorOne, float* vectorTwo, float vectorTwoCoefficient = 1)
{
    float* vector = new float[size];

    for(int i = 0; i < size; i++)
    {
        vector[i] = vectorOne[i] + vectorTwoCoefficient * vectorTwo[i];
    }

    return vector;
}

float** makeOrtogonal(int size, float** matrix, float** vectors)
{
    float** result = createSquareMatrix(size);
    float* norms = new float[size];

    result[0] = vectors[0];

    for (int i = 1; i < size; i++)
    {
        result[i] = vectors[i];
        norms[i-1] = multiply(size, result[i-1], result[i-1], matrix);

        for (int y = 0; y < i; y++)
        {
            float coefficient =  multiply(size, vectors[i], result[y], matrix)/norms[y];
            result[i] = addVectors(size, result[i], result[y], -coefficient);
        }
    }

    return result;
}

int main(void)
{
    try //try for potential errors
    {
        int size = cinIntOnly("Insert size of multiplication matrix:"); //init-input part

        float** matrix = askForMatrix(size);
        printMatrix(size, matrix, "inserted matrix");

        if (!checkMultiplicationMatrix(size, copyMatrix(size, matrix)))
        {
            return 0;
        }

        float** vectors = askForVectors(size);
        printVectors(size, vectors, "inserted vectors");

        if (!checkByDeterminant(size, copyMatrix(size, vectors), true))
        {
            return 0;
        }

        float** ortogonal = makeOrtogonal(size, matrix, vectors);
        printVectors(size, ortogonal, "ortogonal vectors");
    }
    catch (const std::exception& ex) //handle errors
    {
        cout << "Error!" << endl;
        cout << ex.what();
    }

    return 0;
}

