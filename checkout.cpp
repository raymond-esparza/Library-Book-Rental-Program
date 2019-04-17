/*
// Raymond Esparza
// Section #02
File: checkout.cpp

This program as it is right now is unfinished. It should run and compile without errors, but
during the running of this program, it somehow overwrites the rentals.txt and persons.txt files
and deletes their contents. I think the problem is in the readRentals or write_files function
because they were the last ones I was working on before I noticed this bug, but I ran out of time
to continue troubleshooting the problem. I also was unable to implement the findPerson function
shown at the end of class, so the code may be more excessive than it needs to be.

*/


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "person.cpp"
#include "book.cpp"

using namespace std;

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


void fill_books(vector<Book*>&);
void fill_cardholders(vector<Person*>&);
void readRentals(vector<Book*>&, vector<Person*>);
//void print_contents(vector<Book*>, vector<Person*>); //test function that helps with finding errors
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
        // If you use cin anywhere, don't forget that you have to handle the <ENTER> key that 
        // the user pressed when entering a menu option. This is still in the input stream.
        printMenu();
        cin >> choice;
        switch(choice)
        {
            case 1: // Book checkout
            	checkout(books, cardholders);
            	print_contents(books, cardholders); //test function, delete when done
            	
                break;

            case 2: // Book return
            	return_book(books);
                break;

            case 3: // View all available books
            	readBooks(books);
                break;

            case 4: // View all outstanding rentals
            	books_out(books);
                break;

            case 5: // View outstanding rentals for a cardholder
            	user_rentals(books, cardholders);
                break;

            case 6: // Open new library card
            	openCard(cardholders);
                break;

            case 7: // Close library card
            	closeCard(cardholders);
                break;
                
            case 8:   // Must update records in files here before exiting the program
		write_files(books, cardholders);
            	release_memory(books, cardholders);
                break;

            default:
                cout << "Invalid entry" << endl;
                break;
        }
        cout << endl;
	} while(choice != 8);
	return 0;
}


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



void fill_cardholders(vector<Person*> &cardholders){
	ifstream readfile("persons.txt");
	int id;
	bool active; //0 is not active, 1 is active
	string firstname;
	string lastname;
	if ( readfile.is_open() ){
		while (true){
			readfile >> id >> active >> firstname >> lastname;
			Person *temp = new Person(id, active, firstname, lastname);
			cardholders.push_back(temp);
			if( readfile.eof() ) break;
		}
		readfile.close();
	}
}


//read rentals.txt and assign all checked out books their respective renters
void readRentals(vector<Book*> &books, vector<Person*> cardholders){
	ifstream readfile("rentals.txt");
	int booksize = books.size();
	int choldersize = cardholders.size();
	int customerID;
	int bookID;

	while(true){ //read in what books are checked out. search customers for a matching ID and assign the book to that person until no more books remain
		readfile  >> bookID >> customerID;
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
		if( readfile.eof() ){
			readfile.close();
			break;
		}
		//keep reading from the textfile
	}
	return;
}


void checkout(vector<Book*> &books, vector<Person*> &cardholders) {
cout << "Cardholders: " << cardholders.size() << endl << "Books: " << books.size() << endl; //testing purposes only, delete when done
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
					(*books.at(j)).setPersonPtr(cardholders.at(i));   //set the books pointer to the respective renter
					cout << "Rental Complete. " << (*books.at(j)).getTitle() << " checked out to " << (*cardholders.at(i)).fullName() << endl << endl;
					cout << "Address stored in book of the person who checked it out: " << (*books.at(j)).getPersonPtr() << endl; //For testing purposes only. Delete when everything works properly
					cout << "Address of actual person who checked the book out:       " << cardholders.at(i) << endl << endl;//For testing purposes only. Delete when everything works out properly
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


void readBooks(vector<Book*> books){
	int booksize = books.size();

	cout << "\nBooks available:\n\n";
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


//prompt user for ID and display the books checked out to them
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
				if( (*books.at(j)).getPersonPtr() == cardholders.at(i) ){ //if book's renter matches cardholder
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
					cout << "Card Activated.\n";
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
					cout << "Card ID deactivated.\n";
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


void write_files(vector<Book*> books, vector<Person*> cardholders){
	int choldersize = cardholders.size();
	int booksize = books.size();
	ofstream writeRentals("rentals.txt");
	if( writeRentals.is_open() ){
		for(int i = 0; i != booksize; ++i){   //search books
			if( (*books.at(i)).getPersonPtr() != NULL ){ //if a book has a renter
				writeRentals << (*books.at(i)).getId() << " " << (*(*books.at(i)).getPersonPtr()).getId() << endl; // write book ID then renter ID
			}
			//else keep searching
		}
		writeRentals.close();
	}

	ofstream writePersons("persons.txt");
	if( writeRentals.is_open() ){
		for(int i = 0; i != choldersize; ++i){
			writeRentals << (*cardholders.at(i)).getId() << " " << (*cardholders.at(i)).isActive() << " " << (*cardholders.at(i)).fullName() << endl;
		}
		writeRentals.close();
	}
	return;
}


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


//test function
void print_contents(vector<Book*> books, vector<Person*> cardholders){
	int choldersize = cardholders.size();
	int booksize = books.size();

	cout << "Names:\n";
	for(int i = 0; i != choldersize; ++i){
		cout << i << " " << (*cardholders.at(i)).fullName() << endl;
	}
	cout << endl;
	cout << "Books:\n";
	for(int i = 0; i != booksize; ++i){
		cout << i << " " << (*books.at(i)).getId() << " " << (*books.at(i)).getTitle() << " " << (*books.at(i)).getPersonPtr() << endl;
	}
	cout << endl;
}
