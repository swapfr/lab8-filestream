#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <string>

using namespace std;

void create_2d_array_and_write_it_to_the_file(const int ROWS, const int COLS, string &filename);
void show_2d_array(const vector<vector<int>>& _arr);
void write_array_to_the_file(const vector<vector<int>>& _arr, const int ROWS, const int COLS, const string& filename);

vector<vector<int>> read_array_from_the_file(const string& file);

int main()
{

    system("chcp 1251 > nul");
    
    srand(time(NULL));
    
    int rows, cols;;
    

    cout << "кол-во строк = "; cin >> rows;
    cout << "кол-во колонок = "; cin >> cols;

    string file_name = "input.txt";
    string result_file_name = "result.txt";

    create_2d_array_and_write_it_to_the_file(rows, cols, file_name);

    vector<vector<int>> A = read_array_from_the_file("ewewewew.txt");

    if (A.size() > 1) {

        show_2d_array(A);

        //Варіант 3 В масиві А знайти значення максимального елементу і
        //замінити ним кожен елемент в другому стовпці


        int max_var = A[0][0];

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (A[i][j] > max_var) {
                    max_var = A[i][j];
                }
            }
        }

        cout << max_var << endl;

        for (int i = 0; i < rows; i++) {
            A[i][2] = max_var;
        }

        write_array_to_the_file(A, rows, cols, result_file_name);

    }
    return 0;
}



void create_2d_array_and_write_it_to_the_file(const int ROWS, const int COLS, string& filename) {

    vector<vector<int>> arr(ROWS, vector<int>(COLS));
    
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            arr[i][j] = rand() % 40 - 20;
        }
    }

    ofstream outfile(filename);

    outfile << ROWS << endl;
    outfile << COLS << endl;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            outfile << arr[i][j] << " ";
        }
        outfile << endl;
    }
    outfile.close();

}


vector<vector<int>> read_array_from_the_file(const string& file) {

    ifstream infile(file);
    
    if (!infile.is_open()) {
        
        cout << "файла нет" << endl;

        return {{0}};
    }

    int rows, cols;

    infile >> rows;
    infile >> cols;

    vector<vector<int>> arr(rows, vector<int>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            infile >> arr[i][j];
        }
    }

    infile.close();

    return arr;

}


void show_2d_array(const vector<vector<int>>& _arr) {

    cout << endl << endl; for (int i = 0; i < _arr.size(); i++) {
        for (int j = 0; j < _arr[i].size(); j++) {cout << _arr[i][j] << "\t| "; }
        cout << endl;}
cout << endl << endl;}



void write_array_to_the_file(const vector<vector<int>>& _arr, const int ROWS, const int COLS, const string& filename) {

    ofstream outfile(filename);

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            outfile << _arr[i][j] << " ";
        }
        outfile << endl;
    }

    outfile.close();
}
