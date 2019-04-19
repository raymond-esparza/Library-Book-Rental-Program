/*
Author: Raymond Esparza
Program: Library Book Rental Program
Section #02
File: book.cpp

This file contains the member functions of the Book class. It is intended to work by being
included by checkout.cpp and this file includes book.h
*/


#include "book.h"

Book::Book(int id, string bookName, string auth, string cat) {
    bookID = id;
    title = bookName;
    author = auth;
    category = cat;
}

string Book::getTitle() {
    return title;
}

string Book::getAuthor() {
    return author;
}

string Book::getCategory() {
    return category;
}

int Book::getId() {
    return bookID;
}
void Book::setPersonPtr(Person * ptr) {
	personPtr = ptr;
	return;
}

Person * Book::getPersonPtr() {
    return personPtr; // complete
}