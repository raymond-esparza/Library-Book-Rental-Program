/*
Author: Raymond Esparza
Program: Library Book Rental Program
Section #02
File: book.h

This file is the header file for all books in the library database. Creates each new object as type Book.
 This file is intended to work by being included by book.cpp and this file includes the file person.h
*/

#ifndef BOOK_H
#define BOOK_H

#include <string>
#include "person.h"

class Book
{
private:
    string   title;
    string   author;
    string   category;
    int      bookID;
    Person * personPtr = nullptr; //pointer to the Person who's checking it out

public:
    Book(int id, string bookName, string auth, string cat);
    string   getTitle();
    string   getAuthor();
    string   getCategory();
    int      getId();
    void     setPersonPtr(Person * ptr);
    Person * getPersonPtr();
};
#endif // end book.h