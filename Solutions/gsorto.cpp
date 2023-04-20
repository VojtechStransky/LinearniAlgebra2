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

//Method use to copy matrix member by member
float** copyMatrix(int size, float** matrix)
{
    float** copy = createSquareMatrix(size);

    for (int i = 0; i < size; i++)
    {
        for (int y = 0; y < size; y++)
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

//Method used for interactive creating multiple vectors input array
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

//Metod used to print vectors array
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
//Then it can check linear independence or positive definition
bool checkByDeterminant(int size, float** matrix, bool checkIndependence = false)
{
    float result = 1;

    for(int y = 0; y < size; y++) { //for all columns
        optional<int> optIndex = selectFirst(size, matrix, y); //select first row and change for non null one

        if (optIndex == nullopt)
        {
            cout << ((checkIndependence) ? "Vectors are linearly dependent!" : "Myltiplication matrix is not positive definitive!") << endl;

            return false; //if singular - false
        }

        int index = optIndex.value();

        if (index != y)
        {
            changeLines(matrix, index, y);
            result *= -1;
        }

        float first = matrix[y][y]; //hold first member
        result *= first; //multiply result by another diagonal member

        if (!checkIndependence && (result <= 0)) { //If one is negative or zero at least one of subdeterminants is negatice or zero - false
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

//Method uset to check symetricity member by member for all memberst under diagonal by comparation with complementar members, then check if positive definite
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

//Method used to multiply vector1^T * matrix * vector2. Used for applying dot product
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

//Method adds to first vector coef*second vector. Member by member
float* addVectors(int size, float* vectorOne, float* vectorTwo, float vectorTwoCoefficient = 1)
{
    float* vector = new float[size];

    for(int i = 0; i < size; i++)
    {
        vector[i] = vectorOne[i] + vectorTwoCoefficient * vectorTwo[i];
    }

    return vector;
}

//Gram-Schmidt ortogonalization process
float** makeOrtogonal(int size, float** matrix, float** vectors)
{
    float** result = createSquareMatrix(size); //Array of arrays for result
    float* norms = new float[size]; //helper array to hold norms to save processes number

    result[0] = vectors[0]; //first vector of ortogonal base is first inserted

    for (int i = 1; i < size; i++) //for all inserted vectors. Starts at second
    {
        result[i] = vectors[i]; //copy vector to results
        norms[i-1] = multiply(size, result[i-1], result[i-1], matrix); //make norm of previous vector like preparation to be used

        for (int y = 0; y < i; y++) //for all vectors in sum
        {
            float coefficient =  multiply(size, vectors[i], result[y], matrix)/norms[y]; //calculate coefficient according to formula. Dot product / norm fro array
            result[i] = addVectors(size, result[i], result[y], -coefficient); //instead of adding in sum and then subtracting subtract each from result
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

        if (!checkMultiplicationMatrix(size, copyMatrix(size, matrix))) //if multiplication matrix is not correct, end
        {
            return 0;
        }

        float** vectors = askForVectors(size);
        printVectors(size, vectors, "inserted vectors");

        if (!checkByDeterminant(size, copyMatrix(size, vectors), true)) //if vectors are linearly dependent, end
        {
            return 0;
        }

        float** ortogonal = makeOrtogonal(size, matrix, vectors); //create ortogonal base
        printVectors(size, ortogonal, "ortogonal vectors"); //print result
    }
    catch (const std::exception& ex) //handle errors
    {
        cout << "Error!" << endl;
        cout << ex.what();
    }

    return 0;
}

