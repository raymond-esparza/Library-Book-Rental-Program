/*
Author: Raymond Esparza
Program: Library Book Rental Program
Section #02
File: checkout.cpp

This program is intended to simulate a library book rental system. It opens a textfile called books.txt that contains information
on every book in the library in the format:
	bookID (int)
	bookTitle (string)
	bookAuthor (string)
	bookCategory (string)

It then creates a Book object and fills a vector of pointers to each book. It does the same thing with a textfile called person.txt
with information in the format: userID(int) status(bool) firstname(string) lastname(string)
It creates a Person object and fills a vector of pointers to each person. Then it reads from another textfile called rentals.txt
that contains information on all books loaned out in the format:
bookID(int) borrowerID(int)
It then updates each book with the cardholder who is currently borrowing it. This program then allows the user to choose from a
range of options that a normal library would find useful such as checking out a book, returning a book, and managing a users
account. When the program is exited, it rewrites the person.txt and book.txt files with new information to use the next time it
is run.
*/


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "person.cpp"
#include "book.cpp"

using namespace std;


void printMenu();
void fill_books(vector<Book*>&);
void fill_cardholders(vector<Person*>&);
void readRentals(vector<Book*>&, vector<Person*>);
//void print_contents(vector<Book*>, vector<Person*>); //test function that helps with finding errors. Delete/comment out when running actual program
void checkout(vector<Book*>&, vector<Person*>&);
void return_book(vector<Book*>&);
void readBooks(vector<Book*>);
void books_out(vector<Book*>);
void user_rentals(vector<Book*>, vector<Person*>);
void openCard(vector<Person*>&);
void closeCard(vector<Person*>&);
void write_files(vector<Book*>, vector<Person*>);
void release_memory(vector<Book*>&, vector<Person*>&);




int main() {

    vector<Book*> books;
    vector<Person*> cardholders;
    int choice;

   fill_books(books);
   fill_cardholders(cardholders);
   readRentals(books, cardholders);
//   print_contents(books, cardholders); //test function
    
    do
    {
        printMenu();
        cin >> choice;
        switch(choice)
        {
            case 1: // Book checkout
            	cout << endl;
            	checkout(books, cardholders);            	
                break;

            case 2: // Book return
            	cout << endl;
            	return_book(books);
                break;

            case 3: // View all available books
            	cout << endl;
            	readBooks(books);
                break;

            case 4: // View all outstanding rentals
            	cout << endl;
            	books_out(books);
                break;

            case 5: // View outstanding rentals for a cardholder
            	cout << endl;
            	user_rentals(books, cardholders);
                break;

            case 6: // Open new library card
            	cout << endl;
            	openCard(cardholders);
                break;

            case 7: // Close library card
            	cout << endl;
            	closeCard(cardholders);
                break;
                
            case 8:   // Update records in files and clean up memory before exiting the program
				write_files(books, cardholders);
            	release_memory(books, cardholders);
                break;

            default:
                cout << "Invalid entry" << endl;
//            	print_contents(books, cardholders); //test function, delete/comment out when running actual program
                break;
        }
        cout << endl;
	} while(choice != 8);
	return 0;
}

//This function simply prints a menu for the user to respond
void printMenu() {
    cout << "----------Library Book Rental System----------" << endl;
    cout << "1.  Book checkout" << endl;
    cout << "2.  Book return" << endl;
    cout << "3.  View all available books" << endl;
    cout << "4.  View all outstanding rentals" << endl;
    cout << "5.  View outstanding rentals for a cardholder" << endl;
    cout << "6.  Open new library card" << endl;
    cout << "7.  Close library card" << endl;
    cout << "8.  Exit system" << endl;
    cout << "Please enter a choice: ";
}



//This function opens a textfile called books.txt and pulls information from it to fill a vector
//of pointers to the various books
void fill_books(vector<Book*> &books){
	string idstring;
	int id;
	string title;
	string author;
	string category;
	string nextline;
	ifstream readfile("books.txt");
		
	if ( readfile.is_open() ){
		while ( !readfile.eof() ){
			getline(readfile, idstring);  //for some reason, having trouble reading in text as an int, so read as string instead
			getline(readfile, title);
			getline(readfile, author);
			getline(readfile, category);
			getline(readfile, nextline);
			id = stoi(idstring);           //convert string to int so program runs as intended
			Book *temp = new Book(id, title, author, category);
			books.push_back(temp);
		}
		readfile.close();
	}
}


//This function opens a textfile called persons.txt and pulls informaion from it to fill a vector
//of pointers to he various cardholders
void fill_cardholders(vector<Person*> &cardholders){
	ifstream readfile("persons.txt");
	int id;
	bool active; //0 is not active, 1 is active
	string firstname;
	string lastname;

	while(readfile >> id >> active >> firstname >> lastname){		
		Person *temp = new Person(id, active, firstname, lastname);
		cardholders.push_back(temp);
	}
	readfile.close();

	return;
}


//This function opens a textfile called rentals.txt and pulls a books ID and a cardholders ID from it. It then searches all
//cardholders for an ID that matches the one pulled form the textfile to assign the checked out books their respective borrowers
void readRentals(vector<Book*> &books, vector<Person*> cardholders){
	ifstream readfile("rentals.txt");
	int booksize = books.size();
	int choldersize = cardholders.size();
	int customerID;
	int bookID;

	while(readfile >> bookID >> customerID){
		for(int i = 0; i != choldersize; ++i){
			if( customerID == (*cardholders.at(i)).getId() ){
				for(int j = 0; j != booksize; ++j){
					if( bookID == (*books.at(j)).getId() ){
						(*books.at(j)).setPersonPtr( cardholders.at(i) ); // set the book to be checked out by cardholder by setting its pointer to the cardholder
						break;
					}
					//else keep searching for matching bookID
				}
				break;
			}
			//else keep searching for matching customerID
		}
		//keep reading from the textfile
	}
	readfile.close();

	return;
}


/* This function prompts the user for their ID and then makes sure they are eligible to checkout a book. If they are, they enter
the desired book ID which is verified and if available, the book is assigned the cardholder as a borrower. Otherwise no action is
taken and function returns to the main menu.*/
void checkout(vector<Book*> &books, vector<Person*> &cardholders) {
	int userID;
	int bookoutID;
	int choldersize = cardholders.size();
	int booksize = books.size();

	cout << "Please enter your card ID: ";
	cin >> userID;
	for(int i = 0; i != choldersize; ++i){
		if( userID == (*cardholders.at(i)).getId() && (*cardholders.at(i)).isActive() ){ //check for a matching ID and if its active
			cout << "Cardholder: " << (*cardholders.at(i)).fullName() << endl;
			cout << "Please enter the book ID: ";
			cin >> bookoutID;
			for(int j = 0; j != booksize; ++j){
				if( bookoutID == (*books.at(j)).getId() ){ //check for a matching book to inputed ID
					if( (*books.at(j)).getPersonPtr() != NULL ){
						cout << "Book already checked out.\n";
						return;
					}
					cout << "Title: " << (*books.at(j)).getTitle() << endl;
					(*books.at(j)).setPersonPtr(cardholders.at(i));   //set the books pointer to the respective borrower
					cout << "Rental Complete. " << (*books.at(j)).getTitle() << " checked out to " << (*cardholders.at(i)).fullName() << endl << endl;
					return;
				}
				//else continue searching
			}
			cout << "Book not found.\n";
			return;
		}
		//else continue searching
	}
	cout << "Card ID not found or not active.\n";
	
    return;
}


//This function prompts the user for a book ID to return. If a valid ID is entered, the book releases its former borrower. Books
//are allowed to be returned even if they are not registered as being checked out.
void return_book(vector<Book*> &books){
	int returnID;
	int booksize = books.size();

	cout << "Please enter the returned books ID: ";
	cin >> returnID;
	for(int i = 0; i != booksize; ++i){
		if(returnID == (*books.at(i)).getId() ){
			cout << "Title: " << (*books.at(i)).getTitle() << endl;
			(*books.at(i)).setPersonPtr(NULL);
			cout << "Return complete.\n";
			return;
		}
	}
	cout << "Book ID not found.\n";
	return;
}


//This function searches the current borrower of all books. If the book has no borrower and is available, it is displayed to the user
void readBooks(vector<Book*> books){
	int booksize = books.size();

	cout << "Books available:\n\n";
	for(int i = 0; i != booksize; ++i){
		if( (*books.at(i)).getPersonPtr() == NULL ){ //if the book is not checked out to anyone...
			cout << "Book ID: " << (*books.at(i)).getId() << endl;
			cout << "Title: " << (*books.at(i)).getTitle() << endl;
			cout << "Author: " << (*books.at(i)).getAuthor() << endl;
			cout << "Category: " << (*books.at(i)).getCategory() << endl << endl;
		}
	}
	cout << "No other books available for checkout.\n";
	return;
}


//This function searches the current borrower of all books. If the book is loaned out, the book is displayed to the user
void books_out(vector<Book*> books){
	int booksize = books.size();
	for(int i = 0; i != booksize; ++i){
		if( (*books.at(i)).getPersonPtr() != NULL){
			cout << "Book ID: " << (*books.at(i)).getId() << endl;
			cout << "Title: " << (*books.at(i)).getTitle() << endl;
			cout << "Author: " << (*books.at(i)).getAuthor() << endl;
			cout << "Cardholder: " << (*(*books.at(i)).getPersonPtr()).fullName() << endl;
			cout << "Card ID: " << (*(*books.at(i)).getPersonPtr()).getId() << endl << endl;
		}
		//else keep searching books
	}
	cout << "End of outstanding books.\n";
	return;
}


//This function prompts the user for their ID and searches all books for a match. If one is found, the books checked out to them
//are displayed.
void user_rentals(vector<Book*> books, vector<Person*> cardholders){
	int userID;
	int choldersize = cardholders.size();
	int booksize = books.size();
	bool hasrental = false;

	cout << "Please enter your card ID: ";
	cin >> userID;
	cout << endl;
	for(int i = 0; i != choldersize; ++i){   //search cardholders
		if( userID == (*cardholders.at(i)).getId() && (*cardholders.at(i)).isActive() ){ //if input ID is valid and active
			cout << "Cardholder: " << (*cardholders.at(i)).fullName() << endl;
			for(int j = 0; j != booksize; ++j){                           //search books
				if( (*books.at(j)).getPersonPtr() == cardholders.at(i) ){ //if book's borrower matches the cardholder
					hasrental = true;
					cout << "Book ID: " << (*books.at(j)).getId() << endl;
					cout << "Title: " << (*books.at(j)).getTitle() << endl;
					cout << "Author: " << (*books.at(j)).getAuthor() << endl << endl;
				}
			}
		}
	}
	if(hasrental == false)
		cout << "No books currently checked out.\n";
	return;
}


/*This function prompts the user for their first and last name. It the searches all cardholders for a matching name in case the
user already has a card. If the card is inactive, the user can choose to activate it. If the card is already active, the function
returns to main. If the user is unregistered, this function creates a new ID for them and adds them to cardholders.*/
void openCard(vector<Person*> & cardholders){
	int choldersize = cardholders.size();
	int newid;
	string fname;
	string lname;
	char choice;

	cout << "Please enter your first name: ";
	cin >> fname;
	cout << "Please enter your last name: ";
	cin >> lname;

	for(int i = 0; i != choldersize; ++i){   //search cardholders...
		if( (fname == (*cardholders.at(i)).getFirstName() ) && (lname == (*cardholders.at(i)).getLastName()) ){ //...if first name and last name both match a current cardholder
			cout << "User already exists.\n";
			if( !(*cardholders.at(i)).isActive() ){   //if user has a card but it's inactive
				cout << "Activate? y/n ";
				cin >> choice;
				if(choice == 'y'){
					(*cardholders.at(i)).setActive(true);
					cout << "Card #" << (*cardholders.at(i)).getId() << " Activated.\n";
					return;
				}
				else if(choice == 'n'){
					cout << "Card will remain deactivated. No action taken.\n";
					return;
				}
				else{
					cout << "Not a valid entry. No action taken.\n";
					return;
				}
			}
			else{ //user exists and is active
				cout << "User is already active. No action taken.\n";
				return;
			}
		}
		//else keep searching
	}
	//no matching users found, so create a new user
	newid = (*cardholders.at(choldersize - 1)).getId() + 1;
	Person *temp = new Person(newid, true, fname, lname);
	cardholders.push_back(temp);
	cout << "Thank you " << fname << " for creating a new account. ";
	cout << "Your new ID is: " << newid << endl << endl;
	
}


//This function prompts the user for their ID and searches cardholders for a match. If one is found and the user is not already
//deactivated, the user is asked if they want their card deacivated. Otherwise, no action is taken.
void closeCard(vector<Person*> &cardholders){
	int choldersize = cardholders.size();
	int userID;
	char choice;

	cout << "Please enter the card ID: ";
	cin >> userID;
	for(int i = 0; i != choldersize; ++i){   //search cardholders
		if( (userID == (*cardholders.at(i)).getId()) ){ //if userID matches an active card
			if( (*cardholders.at(i)).isActive() ){   //if matched ID is currently active
				cout << "Cardholder: " << (*cardholders.at(i)).fullName() << endl;
				cout << "Are you sure you want to deactivate the card? y/n ";
				cin >> choice;
				if(choice == 'y'){
					(*cardholders.at(i)).setActive(false);
					cout << "Card ID deactivated. You are still responsible for any outstanding book rental returns.\n";
					return;
				}
				else if (choice == 'n'){
					cout << "Card still active. No action taken.\n";
					return;
				}
				else{
					cout << "Not a valid entry. No action taken.\n";
					return;
				}
			}
			else{   //matched ID is already deactivated
				cout << "Cardholder: " << (*cardholders.at(i)).fullName() << endl;
				cout << "Card ID is already inactive. No action taken.\n";
				return;
			}
		}
		//else keep searching
	}
	cout << "Card ID not found.\n";
	return;
}


//This function opens the textfile rentals.txt and writes to it the checked out book ID and their respective borrowers ID. It then
// opens persons.txt and writes to it each cardholders ID, active status, and full name for use next time this program is run.
void write_files(vector<Book*> books, vector<Person*> cardholders){
	int choldersize = cardholders.size();
	int booksize = books.size();
	ofstream writeRentals("rentals.txt");
	if( writeRentals.is_open() ){
		for(int i = 0; i != booksize; ++i){   //search books
			if( (*books.at(i)).getPersonPtr() != NULL ){ //if a book has a borrower
				writeRentals << (*books.at(i)).getId() << " " << (*(*books.at(i)).getPersonPtr()).getId() << endl; // write book ID then borrower ID
			}
			//else keep searching
		}
		writeRentals.close();
	}

	ofstream writePersons("persons.txt");
	if( writePersons.is_open() ){
		for(int i = 0; i != choldersize; ++i){
			writePersons << (*cardholders.at(i)).getId() << " " << (*cardholders.at(i)).isActive() << " " << (*cardholders.at(i)).fullName() << endl;
		}
		writePersons.close();
	}
	return;
}



//This function is the last one called before the program ends and it goes through all elements of the book and cardholder vectors
//and releases the memory each element points to
void release_memory(vector<Book*> &books, vector<Person*> &cardholders){
	int booksize = books.size();
	int choldersize = cardholders.size();

	for(int i = 0; i != booksize; ++i){
		delete books.at(i);
		books.at(i) = NULL;
	}

	for(int i = 0; i != choldersize; ++i){
		delete cardholders.at(i);
		cardholders.at(i) = NULL;
	}
	return;
}


/*//test function that prints contents of all cardholders and all books
void print_contents(vector<Book*> books, vector<Person*> cardholders){
	int choldersize = cardholders.size();
	int booksize = books.size();

	cout << "Names:\n";
	for(int i = 0; i != choldersize; ++i){
		cout << i << " " << (*cardholders.at(i)).fullName() << " " << (*cardholders.at(i)).getId() << endl;
	}
	cout << endl;
	cout << "Books:\n";
	for(int i = 0; i != booksize; ++i){
		cout << i << " " << (*books.at(i)).getId() << " " << (*books.at(i)).getTitle() << " " << (*books.at(i)).getPersonPtr() << endl;
	}
	cout << endl;
}
*/