#include<iostream>
#include<limits>

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

//Method generates identity matrix
float** generateOnesMatrix(int size)
{
    float** matrix = createSquareMatrix(size);

    for (int i = 0; i < size; i++)
    {
        for (int y = 0; y < size; y++)
        {
            matrix[i][y] = (i == y) ? 1 : 0; //For all members if member on diagonal then 1
        }
    }

    return matrix;
}

//Method used for interactive filling matrix by user
float** askForMatrix(int size)
{
    float** matrix = createSquareMatrix(size);

    for(int i = 0; i < size; i++)
    {
        cout << "Row " + to_string(i + 1) + " : [insert number by number as you are asked]" << endl;
        for(int y = 0; y < size; y++)
        {
            matrix[i][y] = cinIntOnly("Insert number " + to_string(y + 1) + " of row " + to_string(i + 1) + ":");
        }
    }

    return matrix;
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

//Method selects first non zero member in column
int selectFirst(int size, float** matrix, int column)
{
    for(int i = column; i < size; i++) //todo
    {
        if (matrix[i][column] != 0)
        {
            return i;
        }
    }

    throw logic_error("Matrix is singular!"); //If there is no nonzero member then matrix must be singular. Throw
}

//Helper method for changing twho lines
void changeLines(float** matrix, int index, int index2)
{
    float* indexLine = matrix[index];
    matrix[index] = matrix[index2];
    matrix[index2] = indexLine;
}

//Method modificates param matrix acording to gauss elimination and aplies same steps to inverse param
void gauss(int size, float** matrix, float** inverse)
{
    for(int y = 0; y < size; y++) { //for all columns
        int index = selectFirst(size, matrix, y); //select first row and change for non null one
        changeLines(matrix, index, y);
        changeLines(inverse, index, y);

        int first = matrix[y][y]; //hold first member

        for(int i = y + 1; i < size; i ++) { //for all rows
            int firstInRow = matrix[i][y]; // hold first in row

            if (firstInRow == 0) //if zero there is nothing to do
            {
                continue;
            }

            for(int z = 0; z < size; z++) { //for all columns subtract multiplies. Lowest common multiple is waste of supplies
                matrix[i][z] = first*matrix[i][z] - firstInRow*matrix[y][z];
                inverse[i][z] = first*inverse[i][z] - firstInRow*inverse[y][z];
            }
        }
    }
}

//The same method for diagonalization and dividing rows by diagonal members
void backwardGauss(int size, float** matrix, float** inverse)
{
    for(int y = size - 1; y > 0; y--) {
        int first = matrix[y][y];

        for(int i = y - 1; i >= 0; i--) {
            int firstInRow = matrix[i][y];

            if (firstInRow == 0)
            {
                continue;
            }

            for(int z = size - 1; z >= 0; z--) {
                matrix[i][z] = (first*matrix[i][z] - firstInRow*matrix[y][z]);
                inverse[i][z] = (first*inverse[i][z] - firstInRow*inverse[y][z]);
            }
        }
    }

    for(int i = 0; i < size; i++)
    {
        for(int y = 0; y < size; y++)
        {
            inverse[i][y] = inverse[i][y] / matrix[i][i];
        }
    }
}

int main(void)
{
    try //try for potential errors and intended throws
    {
        int size = cinIntOnly("Insert size of matrix:"); //init-input part
        float** matrix = askForMatrix(size);
        float** ones = generateOnesMatrix(size);
        printMatrix(size, matrix, "input");

        gauss(size, matrix, ones); //modifications
        backwardGauss(size, matrix, ones);

        printMatrix(size, matrix, "diagonalized input"); //print output
        printMatrix(size, ones, "inverse matrix");
    }
    catch (const std::exception& ex) //handle errors
    {
        cout << "Error!" << endl;
        cout << ex.what();
    }

    return 0;
}

