#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) 
{
    FILE *fptr = fopen("contacts.csv","w");
    if (fptr == NULL) 
    {
        printf("Error opening file!\n");
        return;
    }

    // Counting the valid contacts
    int validCount = 0;
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        if (addressBook->contacts[i].name[0] != '\0') 
        {
            validCount++;
        }
    }

    // Writing a number of contacts in a first line
    fprintf(fptr, "#%d\n", validCount);

    // Write contact details
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        if (addressBook->contacts[i].name[0] == '\0') 
        {
            continue;
        }
        fprintf(fptr, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fptr);
}
