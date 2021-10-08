#pragma once
#include <cstring>
#include <iostream>
#include "List.h"
class GradesReport
{
	struct Student
	{
		std::string lastName;
		std::string firstName;
		std::string group;
		double GPA = 0;
		virtual std::string Serialize();
		virtual void Deserialize(char* str);
		void PrintStudentInfo();
		~Student(){};
	};
	List<Student>* list = new List<Student>();
	
	
public:
	void Add(std::string lstname, std::string fstname, std::string grp, double gpa);
	void RemoveByName(std::string lstname, std::string fstname);
	int RemoveByLessGPA(double lessThGPA);
	void GetStudentInfoByLastName(std::string lstname);
	void GetStudentInfoByGPA(double GPA);
	int NumberOfStudents();
	void Save(std::string path);
	void Load(std::string path);
	void Print();
	~GradesReport() {};
	//{delete list;}
};
