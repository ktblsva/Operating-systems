#define _CRT_SECURE_NO_WARNINGS
#include "GradesReport.h"
#include <sstream>
#include <fstream>
#include <cstdio>


std::string GradesReport::Student::Serialize()
{
	std::stringstream ss;
	ss << lastName << "|" << firstName << "|" << group << "|" << GPA;
	return ss.str();
}

void GradesReport::Student::Deserialize(char* str)
{
	lastName = strtok(str, "|");
	firstName = strtok(NULL, "|");
	group = strtok(NULL, "|");
	std::string _gpa = strtok(NULL, "");
	GPA = atof(_gpa.c_str());
}

void GradesReport::Student::PrintStudentInfo()
{
	std::cout << "Last Name: " << lastName << std::endl;
	std::cout << "First Name: " << firstName << std::endl;
	std::cout << "Group: " << group << std::endl;
	std::cout << "GPA: " << GPA << std::endl;
	std::cout << "___________________________________________" << std::endl;
}

void GradesReport::Add(std::string lstname, std::string fstname, std::string grp, double gpa)
{
	Student* tmp = new Student();
	tmp->lastName = lstname;
	tmp->firstName = fstname;
	tmp->group = grp;
	tmp->GPA = gpa;
	list->pushBack(tmp);
}

void GradesReport::RemoveByName(std::string lstname, std::string fstname)
{
	if (list->getSize() <= 0)
	{
		return;
	}
	for (int i = 0; i < (int)list->getSize(); i++)
	{
		if (lstname == list->getObjByIndex(i)->lastName)
		{
			if (fstname == list->getObjByIndex(i)->firstName)
			{
				list->deleteAt(i);
				return;
			}
		}
	}
}

int GradesReport::RemoveByLessGPA(double lessThGPA)
{
	int count = 0;
	if (list->getSize() <= 0)
	{
		return 0;
	}
	int sizeOfRep = (int)list->getSize();
	for (int i = 0; i < sizeOfRep; i++)
	{
		if (lessThGPA > list->getObjByIndex(i)->GPA)
		{
			list->deleteAt(i);
			count++;
			sizeOfRep--;
		}
	}
	return count;
}

void GradesReport::GetStudentInfoByLastName(std::string lstname)
{
	if (list->getSize() <= 0)
	{
		return;
	}
	for (int i = 0; i < (int)list->getSize(); i++)
	{
		if (lstname == list->getObjByIndex(i)->lastName)
		{
			list->getObjByIndex(i)->PrintStudentInfo();
			return;
		}
	}
}

void GradesReport::GetStudentInfoByGPA(double GPA)
{
	if (list->getSize() <= 0)
	{
		return;
	}
	for (int i = 0; i < (int)list->getSize(); i++)
	{
		if (GPA <= list->getObjByIndex(i)->GPA)
		{
			list->getObjByIndex(i)->PrintStudentInfo();
		}
	}
}

int GradesReport::NumberOfStudents()
{
	return (int)list->getSize();
}

void GradesReport::Save(std::string path)
{
	std::ofstream outf(path);
	if (!outf)
	{
		std::cerr << "This file can't be open!" << std::endl;
		exit(1);
	}
	outf << list->serialize();
}

void GradesReport::Load(std::string path)
{
	std::ifstream inf(path, std::ios::binary);
	if (!inf)
	{
		std::cerr << "This file can't be open!" << std::endl;
		exit(1);
	}
	inf.seekg(0, std::ios::end);
	size_t filesize = inf.tellg();
	inf.seekg(0);
	std::string dataFromFile(filesize, ' ');
	inf.read(&dataFromFile[0], filesize);
	list->deserialize(dataFromFile);
}

void GradesReport::Print()
{
	if (list->getSize() <= 0)
	{
		return;
	}
	for (int i = 0; i < (int)list->getSize(); i++)
	{
		std::cout << i + 1 << ". " << std::endl;
		list->getObjByIndex(i)->PrintStudentInfo();
	}
}
