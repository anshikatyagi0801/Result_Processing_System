// ==========================================
// RESULT PROCESSING SYSTEM USING TEMPLATES
// Made with ❤️ by Anshika Tyagi
// ==========================================
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;
const int MAX_STUDENTS = 100;
template <class T>
class Student {
private:
    int id;
    string name;
    T score;
public:
    Student() { id = 0; name = ""; score = 0; }
    Student(int i, string n, T s) { id = i; name = n; score = s; }
    int getId() { return id; }
    string getName() { return name; }
    T getScore() { return score; }
    void display() {
        cout << left << setw(6) << id << setw(22) << name
             << setw(10) << score;
    }
};
template <class T>
T findLargest(T a, T b, T c) {
    T largest = a;
    if (b > largest) largest = b;
    if (c > largest) largest = c;
    return largest;
}
template <class T>
double calculateAverage(T scores[], int count) {
    if (count == 0) return 0;
    double total = 0;
    for (int i = 0; i < count; i++) total = total + scores[i];
    return total / count;
}
Student<int> integerStudents[MAX_STUDENTS];
Student<float> floatStudents[MAX_STUDENTS];
int type[MAX_STUDENTS];
int countStudents = 0;
void displayStudent(int i) {
    if (type[i] == 1) {
        integerStudents[i].display();
        cout << "Integer\n";
    } else {
        floatStudents[i].display();
        cout << "Float\n";
    }
}
void displayAll() {
    if (countStudents == 0) { cout << "No records available.\n"; return; }
    cout << left << setw(6) << "ID" << setw(22) << "Name"
         << setw(10) << "Score" << "Type\n";
    cout << "------------------------------------------------\n";
    for (int i = 0; i < countStudents; i++) displayStudent(i);
}
int findStudent(int id) {
    for (int i = 0; i < countStudents; i++) {
        if (type[i] == 1 && integerStudents[i].getId() == id) return i;
        if (type[i] == 2 && floatStudents[i].getId() == id) return i;
    }
    return -1;
}
void saveStudents() {
    ofstream file("students_simple.txt");
    if (!file) { cout << "Could not save the records.\n"; return; }
    file << countStudents << '\n';
    file << setprecision(9);
    for (int i = 0; i < countStudents; i++) {
        file << type[i] << '\n';
        if (type[i] == 1) {
            file << integerStudents[i].getName() << '\n';
            file << integerStudents[i].getScore() << '\n';
        } else {
            file << floatStudents[i].getName() << '\n';
            file << floatStudents[i].getScore() << '\n';
        }
    }
    if (!file) cout << "Error while writing the file.\n";
}
void readStudents() {
    ifstream file("students_simple.txt");
    if (!file) return;
    int savedCount;
    if (!(file >> savedCount) || savedCount < 0 || savedCount > MAX_STUDENTS) {
        cout << "Invalid saved file. Starting with empty records.\n";
        return;
    }
    Student<int> temporaryIntegers[MAX_STUDENTS];
    Student<float> temporaryFloats[MAX_STUDENTS];
    int temporaryTypes[MAX_STUDENTS];
    for (int i = 0; i < savedCount; i++) {
        int studentType;
        string name;
        if (!(file >> studentType) || (studentType != 1 && studentType != 2)) {
            cout << "Invalid saved file. Starting with empty records.\n";
            return;
        }
        file >> ws;
        if (!getline(file, name) || name.find_first_not_of(" \t") == string::npos) {
            cout << "Invalid saved file. Starting with empty records.\n";
            return;
        }
        temporaryTypes[i] = studentType;
        if (studentType == 1) {
            int score;
            if (!(file >> score) || score < 0 || score > 100) {
                cout << "Invalid saved file. Starting with empty records.\n";
                return;
            }
            temporaryIntegers[i] = Student<int>(i + 1, name, score);
        } else {
            float score;
            if (!(file >> score) || !(score >= 0 && score <= 100)) {
                cout << "Invalid saved file. Starting with empty records.\n";
                return;
            }
            temporaryFloats[i] = Student<float>(i + 1, name, score);
        }
    }
    countStudents = savedCount;
    for (int i = 0; i < countStudents; i++) {
        type[i] = temporaryTypes[i];
        integerStudents[i] = temporaryIntegers[i];
        floatStudents[i] = temporaryFloats[i];
    }
    cout << countStudents << " saved student record(s) loaded.\n";
}
int inputInteger(int minimum, int maximum) {
    int value;
    while (true) {
        if (cin >> value && value >= minimum && value <= maximum) {
            cin.ignore(10000, '\n');
            return value;
        }
        cout << "Invalid input. Try again: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}
float inputFloat() {
    float value;
    while (true) {
        if (cin >> value && value >= 0 && value <= 100) {
            cin.ignore(10000, '\n');
            return value;
        }
        cout << "Invalid score. Enter a value from 0 to 100: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}
void addStudent() {
    if (countStudents == MAX_STUDENTS) {
        cout << "Student limit reached.\n";
        return;
    }
    cout << "Score type (1=Integer, 2=Float): ";
    int studentType = inputInteger(1, 2);
    string name;
    do {
        cout << "Student name: ";
        getline(cin, name);
        if (name.find_first_not_of(" \t") == string::npos)
            cout << "Name cannot be empty.\n";
    } while (name.find_first_not_of(" \t") == string::npos);
    int id = countStudents + 1;
    type[countStudents] = studentType;
    if (studentType == 1) {
        cout << "Integer score (0-100): ";
        int score = inputInteger(0, 100);
        integerStudents[countStudents] = Student<int>(id, name, score);
    } else {
        cout << "Float score (0-100): ";
        float score = inputFloat();
        floatStudents[countStudents] = Student<float>(id, name, score);
    }
    countStudents++;
    saveStudents();
    cout << "Student added.\n";
}
void largestScores() {
    if (countStudents == 0) {
        cout << "No student records available.\n";
        return;
    }
    cout << "Compare (1=Integer, 2=Float): ";
    int selectedType = inputInteger(1, 2);
    bool found = false;
    float largest = 0;
    string largestName = "";
    float overallLargest = 0;
    string overallName = "";
    for (int i = 0; i < countStudents; i++) {
        float score;
        string name;
        if (type[i] == 1) {
            score = integerStudents[i].getScore();
            name = integerStudents[i].getName();
        } else {
            score = floatStudents[i].getScore();
            name = floatStudents[i].getName();
        }
        if (i == 0 || score > overallLargest) {
            overallLargest = score;
            overallName = name;
        }
        if (type[i] == selectedType) {
            if (!found) {
                largest = score;
                largestName = name;
                found = true;
            } else if (score > largest) {
                // Use the required function template to compare scores.
                largest = findLargest(largest, score, largest);
                largestName = name;
            }
        }
    }
    if (found) {
        if (selectedType == 1)
            cout << "Largest Integer Score: ";
        else
            cout << "Largest Float Score: ";
        cout << largest << " (" << largestName << ")\n";
    } else {
        cout << "No students in the selected category.\n";
    }
    cout << "Largest Overall Score: " << overallLargest
         << " (" << overallName << ")\n";
}
void showAverages() {
    int integerScores[MAX_STUDENTS];
    float floatScores[MAX_STUDENTS];
    int integerCount = 0, floatCount = 0;
    double total = 0;
    for (int i = 0; i < countStudents; i++) {
        if (type[i] == 1) {
            int score = integerStudents[i].getScore();
            integerScores[integerCount++] = score;
            total += score;
        } else {
            float score = floatStudents[i].getScore();
            floatScores[floatCount++] = score;
            total += score;
        }
    }
    cout << fixed << setprecision(2);
    if (integerCount > 0)
        cout << "Integer average: " << calculateAverage(integerScores, integerCount) << '\n';
    else cout << "Integer average: no records\n";
    if (floatCount > 0)
        cout << "Float average: " << calculateAverage(floatScores, floatCount) << '\n';
    else cout << "Float average: no records\n";
    if (countStudents > 0)
        cout << "Overall average: " << total / countStudents << '\n';
    else cout << "Overall average: no records\n";
    cout << defaultfloat;
}
void searchStudent() {
    if (countStudents == 0) { cout << "No records available.\n"; return; }
    cout << "Enter student ID: ";
    int id = inputInteger(1, countStudents);
    int position = findStudent(id);
    if (position == -1) cout << "Student not found.\n";
    else displayStudent(position);
}
int main() {
    readStudents();
    int choice;
    do {
        cout << "\nMade with Love by Anshika Tyagi\n";
        cout << "\n========== RESULT PROCESSING SYSTEM ==========\n";
        cout << "1. Add Student Result\n";
        cout << "2. Display All Student Results\n";
        cout << "3. Find Largest Score\n";
        cout << "4. Calculate Average Scores\n";
        cout << "5. Search Student Result\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        choice = inputInteger(1, 6);
        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: largestScores(); break;
            case 4: showAverages(); break;
            case 5: searchStudent(); break;
            case 6:cout << "\nThank you for using Result Processing System!\n";cout << "Made with Love by Anshika Tyagi <3\n";break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 6);
    return 0;
}