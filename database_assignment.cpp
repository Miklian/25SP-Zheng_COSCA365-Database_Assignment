#include <iostream>
#include <fstream>
#include <string>
using namespace std;

fstream file;
string filename = "data.txt";

struct Person {
    string id, firstName, lastName;
    int income;
};

struct Person *search(string id) {
    struct Person *p2 = new Person;
    char empId[6], firstName[15], lastName[15], incomeStr[10];

    file.clear();
    file.seekg(0, ios::beg);

    while (file.read(empId, 5)) {
        empId[5] = '\0';
        p2->id = empId;
        if (p2->id == id) {
          // Reading rest of data
            file.read(firstName, 15); // read 15 chars
            file.read(lastName, 15); // read 15 chars
            file.read(incomeStr, 10); // read 10 chars
            // Save to structure Person
            p2->firstName = firstName;
            p2->lastName = lastName;
            p2->income = stof(incomeStr); // convert incomeStr to float
            return p2; // return pointer to Person struct
        }
        file.seekg(40, ios::cur); // why 40? //It needs to skip the firstName, lastName, and incomeStr (15+15+10=40) to be at the next empId
    }
    return 0;
}

void get_employee() {
    string empId;
    cout << "Please enter employee ID: " << endl;
    cin >> empId;

    struct Person *idPtr = new Person;
    idPtr = search(empId);

    if (idPtr) {
        cout << "Employee name: " << idPtr->firstName;
        cout << " " << idPtr->lastName << endl;
        cout << "Employee income: " << idPtr->income << endl;
    } else {
        cout << "Employee was not found." << endl;
    }
}
void list() {
    char id[6], firstName[15], lastName[15], incomeStr[10];
    file.clear();
    file.seekg(0, ios::beg); // rewind file to begining
    while (file.read(id, 5)) {
        // read rest of data
        file.read(firstName, 15);
        file.read(lastName, 15);
        file.read(incomeStr, 10);
        // save data to Person struct
        float income = atof(incomeStr); // convert string to float
        // display Person struct (record)
        cout << id << " " << firstName << " " << lastName << " " << income << endl;
    }
}

void write_data(Person p) {
    // write data to file:
    file.clear();
    file.write(p.id.c_str(), 5); // write 5 chars for id
    file.write(p.firstName.c_str(), 15); // write 15 chars for first name
    file.write(p.lastName.c_str(), 15); // write 15 chars for last name
    file.write(to_string(p.income).c_str(), 10); // write 10 chars after converting to string
    file.flush();
}

void new_employee() {
    string empId;
    while (1) {
        cout << "\n*** New Employee ***\n";
        cout << "Enter employee id (enter ID as -1 to end): ";
        cin >> empId;
        if (empId == "-1") break; // stop when user enters -1

        struct Person *idPtr = new Person;
        idPtr = search(empId);

        if (idPtr) {
            cout << "Employee with that ID already exists." << endl;
            return;
        }

        // Save data to Person struct
        Person p;
        p.id = empId;
        cout << "Enter first name: ";
        cin >> p.firstName;
        cout << "Enter last name: ";
        cin >> p.lastName;
        cout << "Enter income: ";
        cin >> p.income;
        cout << "Saving data: ";
        write_data(p);
    }
}

int main() {
    file.open(filename, ios::in | ios::out); // open file for read and write
    if (!file) {
        file.open(filename, ios::out); // create the file
        file.close();
        file.open(filename, ios::in | ios::out);
    }

    // Menu:
    int option = -1;

    while (1) {
        cout << "*** Menu ***" << endl;
        cout << "1) New employee\n";
        cout << "2) Search employee information\n";
        cout << "3) List employee\n";
        cout << "option: ";
        cin >> option;
        switch (option) {
            case 1: new_employee(); break;
            case 2: get_employee(); break;
            case 3: list(); break;
            default: // any other number will exit the menu loop
                return 0;
        }
    }

    // Close files
    file.close();
    return 0;
}