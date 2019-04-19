/*
Author: Raymond Esparza
Program: Library Book Rental Program
Section #02
File: person.cpp

This file contains the member functions of the Person class. It is intended to be used by checkout.cpp and this file
uses file person.h
*/


#include "person.h"

Person::Person(int cardNo, bool act, string fName, string lName) {
    cardID = cardNo;
    active = act;
    firstName = fName;
    lastName = lName;
}

string Person::getFirstName() {
    return firstName;
}

string Person::getLastName() {
    return lastName;
}

int Person::getId() {
    return cardID;
}

void Person::setActive(bool act) {
	active = act;
}

bool Person::isActive() {
    return active;
}

string Person::fullName() {
    return firstName + " " + lastName;
}