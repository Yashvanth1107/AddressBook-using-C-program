#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "populate.h"
void listContacts(AddressBook *addressBook,int sortCriteria) 
{
    Contact temp;
    for (int i = 0; i < addressBook->contactCount - 1; i++) 
    {
        for (int j = i + 1; j < addressBook->contactCount; j++)
        {
            int res = 0;
            switch (sortCriteria) 
            {
                case 1: 
                    res = strcmp(addressBook->contacts[i].name, addressBook->contacts[j].name);
                    break;
                case 2: 
                    res = strcmp(addressBook->contacts[i].phone, addressBook->contacts[j].phone);
                    break;
                case 3: 
                    res = strcmp(addressBook->contacts[i].email, addressBook->contacts[j].email);
                    break;
                default:
                    printf("Invalid sorting criteria.\n");
                    return;
            }
            if (res > 0) 
            {
                temp = addressBook->contacts[i];
                addressBook->contacts[i] = addressBook->contacts[j];
                addressBook->contacts[j] = temp;
            }
        }
    }
    // Display the sorted contacts
    printf("Contact List:\n");
    printTop();
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        printContactDetails(addressBook, i);
    }
}

void initialize(AddressBook *addressBook) 
{
    addressBook->contactCount = 0;
    populateAddressBook(addressBook);
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) 
{
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}

void createContact(AddressBook *addressBook)
{
	int verify_name, verify_num, verify_mail;
    int index = addressBook->contactCount; // index value
    char new_name[100], new_num[100],new_mail[100];
    // checking name
    do{
        getchar();
        printf("Enter the name: ");
        scanf("%[^\n]",new_name);
        verify_name = check_name(new_name);
        if (verify_name != 1)
        {
            printf("Error : Invalid name : Name should only in alphabets\n");
        }
    } while (verify_name != 1); // until 1 the loop runs
    strcpy(addressBook->contacts[addressBook->contactCount].name,new_name);
    //store the validate name in the address book
    // checking number
    do{
        getchar();
        printf("Enter the number: ");
        scanf("%[^\n]",new_num);
        verify_num=check_number(new_num,addressBook);
    } while (verify_num != 0);
    strcpy(addressBook->contacts[index].phone,new_num);
    // store the validate phone number in the address book.
    // checking email
    do{
        getchar();
        printf("Enter email id: ");
        scanf("%[^\n]",new_mail);
        verify_mail=check_email(new_mail,addressBook);
    } while(verify_mail!=0);
    strcpy(addressBook->contacts[index].email,new_mail);
        //store the validated email in the address book
    addressBook->contactCount++; // for the index
    printf("Contact created Successfully!\n");
}

int check_name(char *new_name)
{
    char *name = new_name;
    int i = 0;
    
    if (!(name[0] >= 'A' && name[0] <= 'Z')) 
    {
        return 0;
    }
    
    while (name[i])
    {
        if (!( (name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z') || (name[i] == ' ') ))
        {
            return 0;
        }
        i++;
    }
    return 1;
}

int check_number(char *new_num,AddressBook *addressBook)
{
    char *currentnum = new_num;
    int i = 0;
    int count = 0;
    while (currentnum[i])
    {
        if(currentnum[i] >= '0' && currentnum[i] <= '9')
        {
            count++;
        }
        else
        {
            printf("Phone number must contain only digit!\n");
            return 1;
        }
        i++;
    }
    if(count != 10)
    {
        printf("Number should contain 10 digit\n");
        return 1;
    }
    int flag = 0;
    for(int i = 0; i < addressBook->contactCount; i++)
    {
        if(!(strcmp(currentnum,addressBook->contacts[i].phone)))
        {
            flag=1;
            break;
        }
    }
    if(flag == 1)
    {
        printf("Phone number already exists!\n");
        return 1;
    }
    else
    {
        return 0;
    }
}

int check_email(char * new_mail, AddressBook *addressBook)
{
    char *currentmail = new_mail;
    int i = 0;
    int t = 0, flag = 0;
    char str[5] = ".com";  // make sure enough space for null-terminator

    if(currentmail[0]=='@' || (currentmail[0]>='0' && currentmail[0]<='9'))
    {
        printf("Invalid email format!!!\n");
        return 1;
    }

    // Count '@' and check allowed characters
    while (currentmail[i])
    {
        if ((currentmail[i] >= 'a' && currentmail[i] <= 'z') ||
            (currentmail[i] >= '0' && currentmail[i] <= '9') ||
            currentmail[i] == '@' || currentmail[i] == '.')
        {
            if (currentmail[i] == '@')
            {
                if(currentmail[i+1]=='.')
                {
                    printf("Invalid email format!!!\n");
                    return 1;
                }
                t++;
            }
        }
        else
        {
            printf("Invalid email format!!!\n");
            return 1;
        }
        i++;
    }

    if (t != 1)
    {
        printf("Invalid email format!!! @ should be present once!!!\n");
        return 1;
    }

    // Checking the one .com present or not
    if (!(strstr(currentmail, str)))
    {
        printf(".com is not present!!! invalid email!!!\n");
        return 1;
    }

    // Count occurrences of ".com"
    int com_count = 0;
    char *ptr = currentmail;
    while ((ptr = strstr(ptr, str)) != NULL)
    {
        com_count++;
        ptr = ptr + 4; 
    }
    if (com_count != 1)
    {
        printf("Invalid email format!!! Only one .com is allowed.\n");
        return 1;
    }

    // Check for duplicate emails in address book
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcmp(currentmail, addressBook->contacts[i].email) == 0)
        {
            flag = 1;
            break;
        }
    }

    if (!flag)
    {
        return 0;  // valid email
    }
    else
    {
        printf("Mail Id Already Exists! Please try another mail.\n");
        return 1;
    }
}

int searchbyname(AddressBook *addressBook, int *arr)
{
    char name[100];
    getchar();
    printf("Enter the name: ");
    scanf("%[^\n]", name);
    int s = 0, flag = 0;
    //loop through structure to find matching name
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcasestr(addressBook->contacts[i].name, name))
        {
            flag = 1;
            arr[s] = i;//store the index of matching names
            s++;
        }
    }
    if (flag)
    {
        return s;
    }
    else
    {
        return -1;
    }
}

int searchbynum(AddressBook *addressBook)
{
    char num[15];
    getchar();
    printf("Enter the Number: ");
    scanf("%[^\n]", num);
    int flag = 0, i;
    for (i = 0; i < addressBook->contactCount; i++)
    {
        if (!strcmp(num, addressBook->contacts[i].phone))
        {
            flag = 1;
            break;
        }
    }
    if (flag)
    {
        return i;
    }
    else
    {
        return -1;
    }
}

int searchbymail(AddressBook *addressBook)
{
    char mail[100];
    getchar();
    printf("Enter the Email: ");
    scanf("%[^\n]", mail);
    int flag = 0, i;
    for (i = 0; i < addressBook->contactCount; i++)
    {
        if (!strcmp(mail, addressBook->contacts[i].email))
        {
            flag = 1;
            break;
        }
    }
    if (flag)
    {
        return i;
    }
    else
    {
        return -1;
    }
}

void searchContact(AddressBook *addressBook) 
{
    int choice,index;
    int arr[100];
    getchar();
    printf("Search Contact by\n1.Name \n2.Phone no \n3.Email\n");
    printf("Enter your choice:");
    scanf("%d", &choice);
    switch (choice)
    {
        case 1:
        index = searchbyname(addressBook,arr);
        if(index!=-1)
        {
            printTop();
            for(int i=0;i<index;i++)
            {
                printf("%d",i+1);
                printContactDetails(addressBook,arr[i]);
            }
        }
        else
            printf("Name not found\n");
            
        case 2:
        index = searchbynum(addressBook);
        if(index!=-1)
        {
            printTop();
            printf("1.");
            printContactDetails(addressBook,index);
        }
        else
            printf("Number not found\n");
            break;
        case 3:
        index = searchbymail(addressBook);
        if(index!=-1)
        {
            printTop();
            printf("1.");
            printContactDetails(addressBook,index);
        }
        else
            printf("Email not found\n");
            break;
    }
}

void editContact(AddressBook *addressBook)
{
	int choice,index,res;
    int arr[100];
    int s=0,selected;
    getchar();
    printf("Search Contact by\n1. Name\n2. Phone no\n3. Email\n");
    printf("Enter your option: ");
    scanf("%d",&choice);
    switch (choice)
    {
        case 1:
        res=searchbyname(addressBook,arr);
        printTop();
        for (int i = 0; i < res; i++)
        {
            printf("%d.", i+1);
            printContactDetails(addressBook,arr[i]);
        }
        break;
        case 2:
        index=searchbynum(addressBook);
        if(index != -1)
        {
            printTop();
            printf("1."); 
            printContactDetails(addressBook,index);
        }
        break;
        case 3:
        index=searchbymail(addressBook);
        if(index != -1)
        {
            printTop();
            printf("1."); 
            printContactDetails(addressBook,index);
        }
        break;
        default:
        printf("Invalid input\n");
        break;
    }
    if(res == -1 || index == -1)
    {
        printf("Contact not found :)\n");
        editContact(addressBook);
    }
    else
    {
        if(choice == 1)
        {
            if(res>1)
            {
                getchar();
                printf("Select the contact to edit: ");
                scanf("%d",&selected);
                index=arr[selected-1];
            }
            else
            {
                index=arr[0];
            }
        }
        int choice;
        getchar();
        printf("Contact found! What you want to edit!\n1. Name\n2. Phone no\n3. email\n");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:
            int verify_name;
            char newname[100];
            do{
                getchar();
                printf("Enter the name: ");
                scanf("%[^\n]",newname);
                verify_name = check_name(newname);
                if (verify_name != 1)
                printf("Error : Invalid name :) enter or give me only Alphabets\n");
            } while (verify_name != 1); 
            strcpy(addressBook->contacts[index].name,newname);
            printf("Name change is Succesful!\n");     
            break;

            case 2:
            int res_num;
            char new_num[500];
            do{
                getchar();
                printf("Enter the number: ");
                scanf("%[^\n]",new_num);
                res_num=check_number(new_num,addressBook);
            } while(res_num!=0);
            strcpy(addressBook->contacts[index].phone,new_num);
            printf("Phone number change is Successful!\n");
            break;

            case 3:
            int res_email;
            char new_email[500];
            do{
                getchar();
                printf("Enter the email: ");
                scanf("%[^\n]",new_email);
                res_email=check_email(new_email,addressBook);
            } while(res_email!=0);
            strcpy(addressBook->contacts[index].email,new_email);
            printf("Email change is Successful!\n");
            break;
        }
    }
}

void deleteContact(AddressBook *addressBook)
{ 
	int choice,index,res;
    int arr[100];
    int selected;
    getchar();
    printf("Search contact ny\n1.Name\n2.Phone no\n3.Email\n");
    printf("Enter your option:");
    scanf("%d",&choice);
    switch (choice)
    {
        case 1:
        res = searchbyname(addressBook,arr);
        printTop();
        for(int i=0;i < res;i++)
        {
            printf("%d",i+1);
            printContactDetails(addressBook,arr[i]);
        }
        break;

        case 2:
        index = searchbynum(addressBook);
        if(index != -1)
        {
            printTop();
            printf("1.");
            printContactDetails(addressBook,index);
        }
        break;
        default:
        printf("Invalid input\n");
        break;
    }
    int c = addressBook->contactCount;
    if( res == -1 || index == -1 )
    {
        printf("Contact not found:\n");
        deleteContact(addressBook);
    }
    else
    {
        if(choice==1)
        {
            if(res>1)
            {
                getchar();
                printf("Select the contact to delete:");
                index=arr[selected-1];
            }
            else
            {
                index=arr[0];
            }
        }
        int choice;
        getchar();
        printf("Do you want to delete the contact?\n1.Yes 2.No\nEnter the choice:");
        scanf("%d",&choice);
        if(choice==1)
        {
            for(int i=index;i<addressBook->contactCount;i++)
            {
                addressBook->contacts[i]=addressBook->contacts[i+1];
            }
            addressBook->contactCount--;
            if(addressBook->contactCount==c-1)
            printf("Contact Deleted.\n");
        }
        else
             printf("Contact not deleted\n");
    }  
}

void printTop()
{  
    printf("%-25s %-15s %-30s\n", "Name","phone no","Email");
    printf("-----------------------------------------------------\n");
}

void printContactDetails(AddressBook *addressBook,int index)
{
    printf("%-25s",addressBook->contacts[index].name);
    printf("%-15s",addressBook->contacts[index].phone);
    printf("%-30s\n",addressBook->contacts[index].email);
}