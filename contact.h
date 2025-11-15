#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100

typedef struct 
{
    char name[50];
    char phone[20];
    char email[50];
} Contact;

typedef struct 
{
    Contact contacts[MAX_CONTACTS];
    int contactCount;
} AddressBook;

//Declarations
void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact (AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook, int sortCriteria);

//Initialization and File Operations
void initialize(AddressBook *addressBook);
void saveContactsToFile(AddressBook *AddressBook);

//Validation Functions
int check_name(char *new_name);
int check_number(char * new_num,AddressBook *addressBook);
int check_email(char * new_mail, AddressBook *addressBook);

//Utility Functions
void printContactDetails(AddressBook *addressBook,int index);
void printTop();

#endif