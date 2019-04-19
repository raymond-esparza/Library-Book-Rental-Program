/*
Author: Raymond Esparza
Program: Library Book Rental Program
Section #02
File: person.h

This file is the header file for all cardholders in the library database. It creates each new cardholder
as type Person. This file is intended to be included by files person.cpp and book.h
*/


#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>
using namespace std;

class Person
{
private:
    string firstName;
    string lastName;
    int    cardID;
    bool   active;

public:
    Person (int cardNo, bool act, string fName, string lName);
    string getFirstName();
    string getLastName();
    int    getId();
	void   setActive(bool act);
	bool   isActive();
    string fullName();
};
#endif // end person.h
