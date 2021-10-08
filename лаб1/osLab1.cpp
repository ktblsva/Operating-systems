#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include "GradesReport.h"
using namespace std;

GradesReport* report = new GradesReport();
void menu()
{
	system("clear");
	while (1)
	{
		//system("clear");
		cout << "1. Add student" << endl;
		cout << "2. Get student info by lastname" << endl;
		cout << "3. Get student info by GPA (>=)" << endl;
		cout << "4. Size of grades report" << endl;
		cout << "5. Remove student by name" << endl;
		cout << "6. Save to file" << endl;
		cout << "7. Load from file" << endl;
		cout << "8. Print report" << endl;
		cout << "9. Remove students with less GPA" << endl;
		cout << "0. to exit" << endl;
		cout << endl << "Enter: ";
		int n;
		cin >> n;
		system("clear");
		if (n == 0)
		{
			delete report;
			break;
		}
		if (n == 1)
		{
			system("clear");
			string tLN, tFN, tG;
			double gpa = 0;
			cout << "Enter lastname: ";
			cin >> tLN;
			cout << "Enter firstname: ";
			cin >> tFN;
			cout << "Enter group: ";
			cin >> tG;
			cout << "Enter gpa: ";
			cin >> gpa;
			report->Add(tLN, tFN, tG, gpa);
			cout << endl << endl;
		}
		if (n == 2)
		{
			system("clear");
			string tLN;
			cout << "Get student info by lastname. Enter lastname: " << endl;
			cin >> tLN;
			cout << endl;
			report->GetStudentInfoByLastName(tLN);
			cout << endl << endl;
		}
		if (n == 3)
		{
			system("clear");
			cout << "Get student info by GPA. Enter GPA: " << endl;
			double gpa = 0;
			cin >> gpa;
			report->GetStudentInfoByGPA(gpa);
			cout << endl << endl;
		}
		if (n == 4)
		{
			system("clear");
			cout << "Size of grades report: " << report->NumberOfStudents() << endl << endl;
		}
		if (n == 5)
		{
			system("clear");
			string tLN, tFN;
			cout << "Remove student. " << endl;
			cout << "Enter last name: ";
			cin >> tLN;
			cout << "Enter firts name: ";
			cin >> tFN;
			report->RemoveByName(tLN, tFN);
			cout << "Size of grades report: " << report->NumberOfStudents() << endl << endl;
		}
		if (n == 6)
		{
			system("clear");
			cout << "Enter filename: ";
			string filename;
			cin >> filename;
			cout << filename << endl;
			if (report->NumberOfStudents() > 0) report->Save(filename);
			cout << "Saved!" << endl << endl;
		}
		if (n == 7)
		{
			system("clear");
			cout << "Enter filename: ";
			string filename;
			cin >> filename;
			cout << filename << endl;
			if (report->NumberOfStudents() > 0)
			{
				delete report;
				report = new GradesReport();
			}
			report->Load(filename);
			report->Print();
			cout << endl << endl;
		}
		if (n == 8)
		{
			system("clear");
			report->Print();
			cout << endl << endl;
		}
		if (n == 9)
		{
			system("clear");
			double gpa = 0;
			cout << "Enter GPA: " << endl;
			cin >> gpa;
			cout << report->RemoveByLessGPA(gpa) << " report(s) have been deleted (GPA below: " << gpa << ")." << endl << endl;
		}
	}
}

int main()
{
	// GradesReport* report = new GradesReport();
	// report->Add("Smith", "Lisa", "A911", 4.345);
	// report->Add("Kan", "Daniel", "A911", 3.8);
	// report->Add("Kan", "Susan", "B921", 4.0);
	// report->Print();
	// cout << "===================" << endl;
	// cout << "Get student info by lastname: " << endl;
	// report->GetStudentInfoByLastName("Smith");
	// cout << "===================" << endl;
	// cout << "Get student info by GPA: " << endl;
	// report->GetStudentInfoByGPA(4.0);
	// cout << "===================" << endl;
	// cout << "Size of grades report: " << report->NumberOfStudents() << endl;
	// cout << report->RemoveByLessGPA(4.0) << " report(s) have been deleted (GPA below: " << 4.0 << ")." << endl;

	// cout << "===================" << endl;
	// report->Add("Black", "Michael", "A913", 4.2);
	// cout << "Size of grades report: " << report->NumberOfStudents() << endl;
	// report->RemoveByName("Black", "Michael");
	// cout << "===================" << endl;
	// cout << "Size of grades report: " << report->NumberOfStudents() << endl;
	// report->Save("reports.txt");
	// cout << "=====================" << endl;
	// GradesReport* reportTMP = new GradesReport();
	// reportTMP->Load("reports.txt");
	// reportTMP->Print();
	// delete reportTMP;
	// delete report;
	menu();
}
