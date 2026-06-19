#include<iostream>
#include<fstream>
#include<string>
#include<limits>
#include<iomanip>
#include<vector>
#include<algorithm>
#include<sstream>
using namespace std;

struct Student {
    string name;
    int roll;
    int maths,science, english;
    float avg;
    char grade;
};

int binarySearch(vector<Student>& s, int roll) {
    int left = 0, right = s.size() - 1;

    while(left <= right) {
        int mid = (left + right) / 2;

        if(s[mid].roll == roll)
            return mid;
        else if(s[mid].roll < roll)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return -1;
}

// Sort (ascending by roll)
void sortStudents(vector<Student>& s) {
    sort(s.begin(), s.end(), [](Student a, Student b) {
        return a.roll < b.roll;
    });
}



void calculateResult(Student &s) {
    int total = s.maths + s.science + s.english;
    s.avg = total / 3.0;

    if(s.avg >= 90) s.grade = 'A';
    else if(s.avg >= 75) s.grade = 'B';
    else if(s.avg >= 50) s.grade = 'C';
    else s.grade = 'F';
}

// Save to file 
void saveToFile(vector<Student>& s) {
    ofstream out("students.txt");  // overwrite

    for(int i = 0; i < s.size(); i++) {
        out << s[i].roll << "|"
            << s[i].name << "|"
            << s[i].maths << "|"
            << s[i].science << "|"
            << s[i].english << "|"
            << s[i].avg << "|"
            << s[i].grade << endl;
    }

    out.close();
}

// Load from file 
vector<Student> loadFromFile() {
    vector<Student> s;
    ifstream in("students.txt");

    string line;

    while(getline(in, line)) {

        if(line.empty()) continue;   //  skip blank lines

        Student temp;
        stringstream ss(line);
        string data;

        try {
            getline(ss, data, '|');
            temp.roll = stoi(data);

            getline(ss, temp.name, '|');

            getline(ss, data, '|');
            temp.maths = stoi(data);

            getline(ss, data, '|');
            temp.science = stoi(data);

            getline(ss, data, '|');
            temp.english = stoi(data);

            getline(ss, data, '|');
            temp.avg = stof(data);

            getline(ss, data, '|');
            temp.grade = data[0];

            s.push_back(temp);
        }
        catch(...) {
            cout << "Skipping invalid record: " << line << endl;
        }
    }

    in.close();
    return s;
}

bool isDuplicate(vector<Student>& s, int roll) {
    for(int i = 0; i < s.size(); i++) {
        if(s[i].roll == roll) {
            return true;
        }
    }
    return false;
}

// Add Students
void addStudents() {
    vector<Student> s = loadFromFile();

    int n;
    cout << "Enter number of new students: ";
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int added = 0;
    int attempts = 0;

    while(added < n) {
        cout << "\nEnter details of student " << s.size() + 1 << endl;

        int roll;
        cout << "Roll: ";
        cin >> roll;

        if(isDuplicate(s, roll)) {
            cout << "Roll number already exists! Try again.\n";
            attempts++;

            if(attempts == 3) {
                cout << "Too many failed attempts. Skipping...\n";
                break;
            }
            continue;
        }

        attempts = 0;

        Student temp;
        temp.roll = roll;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Name: ";
        getline(cin, temp.name);

        
        do {
            cout << "Enter Maths Marks (0-100): ";
            cin >> temp.maths;

            cout << "Enter Science Marks (0-100): ";
            cin >> temp.science;

            cout << "Enter English Marks (0-100): ";
            cin >> temp.english;

            if(temp.maths < 0 || temp.maths > 100 ||
               temp.science < 0 || temp.science > 100 ||
               temp.english < 0 || temp.english > 100) {
                cout << "Invalid marks! Please enter values between 0 and 100.\n";
            }

        } while(temp.maths < 0 || temp.maths > 100 ||
                temp.science < 0 || temp.science > 100 ||
                temp.english < 0 || temp.english > 100);

        //  correct calculation
        calculateResult(temp);

        s.push_back(temp);

        added++;
    }

    sortStudents(s);
    saveToFile(s);

    cout << "\nStudents added & sorted successfully\n";
}
// View
void viewStudents() {
    vector<Student> s = loadFromFile();

    if(s.empty()) {
        cout << "\nNo student records found!\n";
        return;
    }

    sortStudents(s);

    cout << "\n\n=========== STUDENT LIST ===========\n\n";

    //  UPDATED HEADER
    cout << left << setw(8) << "Roll"
         << setw(15) << "Name"
         << setw(10) << "Maths"
         << setw(10) << "Science"
         << setw(10) << "English"
         << setw(10) << "Avg"
         << setw(10) << "Grade" << endl;

    cout << "-------------------------------------------------------------------\n";

    //  UPDATED DATA PRINT
    for(int i = 0; i < s.size(); i++) {
        cout << setw(8) << s[i].roll
             << setw(15) << s[i].name
             << setw(10) << s[i].maths
             << setw(10) << s[i].science
             << setw(10) << s[i].english
             << setw(10) << s[i].avg
             << setw(10) << s[i].grade
             << endl;
    }

    cout << "===================================================================\n";
}
// Search
void searchStudent() {
    vector<Student> s = loadFromFile();  // vector आता है अब

    if(s.empty()) {
        cout << "No students found!\n";
        return;
    }

    
    sortStudents(s);

    int roll;
    cout << "\nEnter roll number: ";
    cin >> roll;

    int index = binarySearch(s, roll);

    if(index == -1) {
        cout << " Student not found!\n";
    } else {
        cout << "\n========= STUDENT DETAILS =========\n\n";

        cout << "Roll No  : " << s[index].roll << endl;
        cout << "Name     : " << s[index].name << endl;

        cout << "\n--- Marks ---\n";
        cout << "Maths    : " << s[index].maths << endl;
        cout << "Science  : " << s[index].science << endl;
        cout << "English  : " << s[index].english << endl;

        cout << "\n--- Result ---\n";
        cout << "Average  : " << s[index].avg << endl;
        cout << "Grade    : " << s[index].grade << endl;

        cout << "===================================\n";
        
    }
}

void searchByName() {
    vector<Student> s = loadFromFile();  // vector now

    if(s.empty()) {
        cout << "No students found!\n";
        return;
    }

    string search;
    cout << "Enter name to search: ";
    cin.ignore();
    getline(cin, search);

    bool found = false;

    for(int i = 0; i < s.size(); i++) {

        // check substring
        if(s[i].name.find(search) != string::npos) {
            cout << "\nName: " << s[i].name << endl;
            cout << "Roll: " << s[i].roll << endl;
            cout << "Avg: " << s[i].avg << endl;
            cout << "Grade: " << s[i].grade << endl;

            found = true;
        }
    }

    if(!found) {
        cout << "No matching student found!\n";
    }
}

// Topper
void showTopper() {
    vector<Student> s = loadFromFile();  // vector now

    if(s.empty()) {
        cout << "No students found!\n";
        return;
    }

    int topperIndex = 0;

    for(int i = 1; i < s.size(); i++) {
        if(s[i].avg > s[topperIndex].avg) {
            topperIndex = i;
        }
    }

    cout << "\n Topper Details:\n";
    cout << "Roll: " << s[topperIndex].roll << endl;
    cout << "Name: " << s[topperIndex].name << endl;
    cout << "Average Marks: " << s[topperIndex].avg << endl;
}
// Average
void showAverage() {
    vector<Student> s = loadFromFile();  

    if(s.empty()) {
        cout << "No students found!\n";
        return;
    }

    float total = 0;

    for(int i = 0; i < s.size(); i++) {
        total += s[i].avg;
    }

    cout << "\nAverage: " << (total / s.size()) << endl;
}

// Top 3
void showTop3() {
    vector<Student> s = loadFromFile();  

    if(s.size() < 3) {
        cout << "Not enough students!\n";
        return;
    }

    for(int i = 0; i < 3; i++) {
        int maxIndex = i;

        for(int j = i + 1; j < s.size(); j++) {
            if(s[j].avg > s[maxIndex].avg) {
                maxIndex = j;
            }
        }

        // swap
        swap(s[i], s[maxIndex]);
    }

    cout << "\n Top 3 Students:\n";
    for(int i = 0; i < 3; i++) {
        cout << "\nRank " << i+1 << endl;
        cout << "Roll: " << s[i].roll << endl;
        cout << "Name: " << s[i].name << endl;
        cout << "Marks: " << s[i].avg << endl;
    }
}

void deleteStudent() {
    vector<Student> s = loadFromFile();   

    if(s.size() == 0) {
        cout << "No students found!\n";
        return;
    }

    int roll;
    cout << "\nEnter roll number to delete: ";
    cin >> roll;

    int index = -1;

    //  Find student
    for(int i = 0; i < s.size(); i++) {
        if(s[i].roll == roll) {
            index = i;
            break;
        }
    }

    if(index == -1) {
        cout << "Student not found!\n";
        return;
    }

    //  Show student details
    cout << "\nStudent Found:\n";
    cout << "Name: " << s[index].name << endl;
    cout << "Roll: " << s[index].roll << endl;
    cout << "Marks: " << s[index].avg << endl;
    cout << "Grade: " << s[index].grade << endl;

    //  Confirmation
    char confirm;
    cout << "\nAre you sure you want to delete this student? (y/n): ";
    cin >> confirm;

    if(confirm != 'y' && confirm != 'Y') {
        cout << "Deletion cancelled.\n";
        return;
    }

    //  DELETE (NO SHIFTING NEEDED)
    s.erase(s.begin() + index);

    //  Save updated data
    saveToFile(s);

    cout << "Student deleted successfully!\n";
}

void updateStudent() {
    vector<Student> s = loadFromFile();

    if(s.empty()) {
        cout << "No students found!\n";
        return;
    }

    int roll;
    cout << "Enter roll number to update: ";
    cin >> roll;

    int index = -1;

    for(int i = 0; i < s.size(); i++) {
        if(s[i].roll == roll) {
            index = i;
            break;
        }
    }

    if(index == -1) {
        cout << "Student not found!\n";
        return;
    }

    cout << "\nWhat do you want to update?\n";
    cout << "1. Maths\n2. Science\n3. English\n4. All Subjects\n5. Name\n";
    
    int choice;
    cin >> choice;

    if(choice == 1) {
        do {
            cout << "Enter new Maths marks: ";
            cin >> s[index].maths;
        } while(s[index].maths < 0 || s[index].maths > 100);
    }
    else if(choice == 2) {
        do {
            cout << "Enter new Science marks: ";
            cin >> s[index].science;
        } while(s[index].science < 0 || s[index].science > 100);
    }
    else if(choice == 3) {
        do {
            cout << "Enter new English marks: ";
            cin >> s[index].english;
        } while(s[index].english < 0 || s[index].english > 100);
    }
    else if(choice == 4) {
        do {
            cout << "Enter Maths: ";
            cin >> s[index].maths;

            cout << "Enter Science: ";
            cin >> s[index].science;

            cout << "Enter English: ";
            cin >> s[index].english;

        } while(s[index].maths < 0 || s[index].maths > 100 ||
                s[index].science < 0 || s[index].science > 100 ||
                s[index].english < 0 || s[index].english > 100);
    }else if(choice == 5){
        cin.ignore();
        cout<<"\nCurrent Name: "<<s[index].name<<endl;
        cout<<"\nEnter new name:";
        getline(cin, s[index].name);
    }
    else {
        cout << "Invalid choice!\n";
        return;
    }

    //  VERY IMPORTANT
    calculateResult(s[index]);  // avg + grade update

    sortStudents(s);
    saveToFile(s);

    cout << "\nStudent updated successfully!\n";
}

void subjectTopperMenu(vector<Student>& s) {
    if(s.empty()) {
        cout << "No data available!\n";
        return;
    }

    cout << "\n===== SUBJECT TOPPER =====\n";
    cout << "1. Maths\n2. Science\n3. English\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    int topperIndex = 0;

    if(choice == 1) {
        for(int i = 1; i < s.size(); i++) {
            if(s[i].maths > s[topperIndex].maths)
                topperIndex = i;
        }

        cout << "\nMaths Topper: " << s[topperIndex].name
             << " (" << s[topperIndex].maths << ")\n";
    }
    else if(choice == 2) {
        for(int i = 1; i < s.size(); i++) {
            if(s[i].science > s[topperIndex].science)
                topperIndex = i;
        }

        cout << "\nScience Topper: " << s[topperIndex].name
             << " (" << s[topperIndex].science << ")\n";
    }
    else if(choice == 3) {
        for(int i = 1; i < s.size(); i++) {
            if(s[i].english > s[topperIndex].english)
                topperIndex = i;
        }

        cout << "\nEnglish Topper: " << s[topperIndex].name
             << " (" << s[topperIndex].english << ")\n";
    }
    else {
        cout << "Invalid choice!\n";
    }
}

void showPerformanceReport() {
    vector<Student> s = loadFromFile();

    if(s.empty()) {
        cout << "No data available!\n";
        return;
    }

    int sum = 0;
    int maxMarks = s[0].avg;
    int minMarks = s[0].avg;
    int topIndex = 0;
    int lowIndex = 0;

    int pass = 0, fail = 0;
    int gradeA = 0, gradeB = 0, gradeC = 0, gradeF = 0;

    for(int i = 0; i < s.size(); i++) {
        sum += s[i].avg;

        // Topper
        if(s[i].avg > maxMarks) {
            maxMarks = s[i].avg;
            topIndex = i;
        }

        // Lowest
        if(s[i].avg < minMarks) {
            minMarks = s[i].avg;
            lowIndex = i;
        }

        // Pass/Fail
        if(s[i].avg >= 50) pass++;
        else fail++;

        // Grade Distribution
        if(s[i].avg >= 90) gradeA++;
        else if(s[i].avg >= 75) gradeB++;
        else if(s[i].avg >= 50) gradeC++;
        else gradeF++;
    }

    float avg = (float)sum / s.size();
    float passPercent = ((float)pass / s.size()) * 100;

    cout << "\n\n========== CLASS PERFORMANCE REPORT ==========\n";

    cout << "\nTotal Students: " << s.size();
    cout << "\nAverage Marks: " << avg;

    cout << "\n\n Topper: " << s[topIndex].name 
         << " (" << s[topIndex].avg << ")";

    cout << "\n Lowest: " << s[lowIndex].name 
         << " (" << s[lowIndex].avg << ")";

    cout << "\n\nPass Students: " << pass;
    cout << "\nFail Students: " << fail;
    cout << "\nPass Percentage: " << passPercent << "%";

    cout << "\n\nGrade Distribution:";
    cout << "\nA: " << gradeA;
    cout << "\nB: " << gradeB;
    cout << "\nC: " << gradeC;
    cout << "\nF: " << gradeF;

    cout << "\n=============================================\n";
}

void showBelowAverage() {
    vector<Student> s = loadFromFile();

    if(s.empty()) {
        cout << "No data available!\n";
        return;
    }

    int sum = 0;
    for(int i = 0; i < s.size(); i++) {
        sum += s[i].avg;
    }

    float avg = (float)sum / s.size();

    cout << "\nAverage Marks: " << avg;
    cout << "\n\nStudents below average:\n";

    for(int i = 0; i < s.size(); i++) {
        if(s[i].avg < avg) {
            cout << s[i].name << " (" << s[i].avg << ")\n";
        }
    }
}

void performanceMenu() {
    int choice;

    do {
        cout << "\n\n======  PERFORMANCE ANALYSIS ======";
        cout << "\n1.  Show Topper";
        cout << "\n2.  Show Average Marks";
        cout << "\n3.  Top 3 Students";
        cout << "\n4.  Subject-wise Topper";
        cout << "\n5.  Full Performance Report";
        cout << "\n6.  Below Average Students";
        cout << "\n7.  Back to Main Menu";
        cout << "\nEnter choice: ";
        cin >> choice;

        switch(choice) {
            case 1:
                showTopper();
                break;

            case 2:
                showAverage();
                break;

            case 3:
                showTop3();
                break;
            
            case 4: {
                vector<Student> s = loadFromFile();  
                subjectTopperMenu(s);
                break;
                }

            case 5:
                showPerformanceReport(); // combined report
                break;

            case 6:
                showBelowAverage();
                break;

            case 7:
                cout << "Returning to Main Menu...\n";
                break;

            default:
                cout << " Invalid choice! Try again.\n";
        }

    } while(choice != 7);
}

// MAIN MENU
int main() {
    int choice;

    do {
        cout << "\n\n===== STUDENT PERFORMANCE ANALYZER =====";
        cout << "\n1. Add Students";
        cout << "\n2. View Students";
        cout << "\n3. Search Student";
        cout << "\n4. Delete Student";
        cout << "\n5. Update Student";
        cout << "\n6. Performance Analysis";
        cout << "\n7. Exit";
        cout << "\nEnter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: addStudents(); break;
            case 2: viewStudents(); break;
            case 3: {
            int searchChoice;

            cout << "\n1. Search by Roll";
            cout << "\n2. Search by Name";
            cout << "\nEnter choice: ";
            cin >> searchChoice;

            if(searchChoice == 1) searchStudent();
            else if(searchChoice == 2) searchByName();
            else cout << "Invalid choice\n";

            break;
        }
            case 4: deleteStudent(); break;
            case 5: updateStudent(); break;
            case 6: performanceMenu();break;
            case 7: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice\n";
        }

    } while(choice != 7);

    return 0;
}