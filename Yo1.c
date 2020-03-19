#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

void clearInput(void);
void addNewcontact(void);
void listAll(void);
void deletecontact(void);
void modifycontact(void);
int findcontact(void);
int prompt(void);
int findnum (int);


 typedef struct contact {
    int number;
    char name[20];
    char address[50];
    char phone[15];
    char email[20];
    struct contact *next;
    int count;
} Contact;

void bubbleSort(Contact*);
//void swap(Contact*, Contact*);
//void swap(Contact*, int, int);
void swap(Contact* , Contact*, Contact* );

Contact *firstc,*currentc,*newc;
int cnum = 0;

int main()
{
    FILE *datafile;
    char *filename = "conts.dat";
    char ch;
    firstc = NULL;
    datafile = fopen(filename,"r");
    if(datafile)
    {
	    firstc = (struct contact *)malloc(sizeof(struct contact));
	    currentc = firstc;
	    while(1)
     	    {
		    newc = (struct contact *)malloc(sizeof(struct contact));
		    fread(currentc,sizeof(struct contact),1,datafile);
		    if(currentc->next == NULL)
			    break;
		    currentc->next = newc;
            currentc->count=0;
		    currentc = newc;
	    }
	    fclose(datafile);
	    cnum = currentc->number;

    }

    do
    {
	fflush(stdin);
        puts("\nWelcome To The Contact Database");
        puts("-- -----------------------------");
        puts("1 - Add a new contact");
        puts("2 - Delete contact");
        puts("3 - List all contacts");
        puts("4 - Modify contact");
        puts("5 - Find a contact by name");
        puts("-- -----------------------------");
        puts("0 - Save and quit\n");
        printf("Your choice:");
        ch = getchar();
        switch(ch)
        {
            case '1':
                puts("Add a new contact\n");
	        fflush(stdin);
                addNewcontact();//call addNewcontact function
                break;
	    case '2':
		puts("Delete a contact\n");
		deletecontact();
		break;
	    case '3':
		puts("List all contacts\n");
		listAll();
		break;
	    case '4':
		puts("Modify a contact\n");
		modifycontact();
		break;
	    case '5':
		puts("Find a contact by name\n");
		findcontact();
		break;
            case '0':
                puts("Save and quit\n");
                default:
                break;
        }
    }
    while(ch != '0');

    currentc = firstc;

    if(currentc == NULL)
	    return(0);

    datafile = fopen(filename,"w");

    if(datafile == NULL)
    {
	    printf("Error writing to %s\n",filename);
	    return(1);
    }
    while(currentc != NULL)
    {
	    fwrite(currentc,sizeof(struct contact),1,datafile);
	    currentc = currentc->next;
    }
    fclose(datafile);
    return(0);
}

void addNewcontact(void)
{
    newc = (struct contact *)malloc(sizeof(struct contact));

    cnum++;
    printf("%27s: %5i\n","contact number",cnum);
    newc->number = cnum;

    printf("%27s: ","Enter contact name");
    gets(newc->name);

    do{
    printf("%12s: ","Enter Contact Phone number");
    gets(newc->phone);
    }while((strlen(newc->phone)>11) || checknum(newc,strlen(newc->phone))==0);

    printf("%27s: ","Enter contact email");
    gets(newc->email);

    printf("%27s: ","Enter address");
    gets(newc->address);

    printf("\nContact Added Successfully!\n\n");
    newc->count=0;

    newc->next = NULL;

    if(firstc==NULL)
        firstc = newc;
    else
    {
        Contact* temp=firstc;
        Contact* prev=NULL;
        while(temp!=NULL && strcmpi(newc->name,temp->name)>0)
        {
                prev=temp;
                temp=temp->next;
        }
        if(prev==NULL)
        {
            currentc=newc;
            currentc->next=firstc;
            firstc=currentc;
        }
        else{
            currentc=newc;
            prev->next=currentc;
            currentc->next=temp;
        }
    }




    //bubbleSort(firstc);
}

void listAll()
{
    if(firstc==NULL)
        puts("There are no contacts to display!");

    else
    {
	    printf("%6s %-20s %-15s %-20s %-40s\n","Acct#","Name","Phone","Email","    Address");
        puts("------ -------------------- ------------- ------------------- ---------------------------------");

        currentc=firstc;

        do
        {
            printf("%6d: %-20s %-15s %-20s %-40s\n",\
                currentc->number,\
                currentc->name,\
                currentc->phone,\
                currentc->email,\
                currentc->address);
        }

        while((currentc=currentc->next) != NULL);
    }
}

void deletecontact(void)
{
    int record;
    struct contact *previousa;

    if(firstc==NULL)
    {
        puts("There are no contacts to delete!");
	return;
    }

    listAll();
    printf("Enter contact account number to delete: ");
    scanf("%d",&record);
    currentc = firstc;

    while(currentc != NULL)
    {
        if(currentc->number == record)
	{
	    if(currentc == firstc)
		firstc=currentc->next;
	    else
		previousa->next = currentc->next;

        free(currentc);
	    printf("contact %d deleted!\n",record);
	    return;
	}

	else
	{
        previousa = currentc;
	    currentc = currentc->next;
	}
    }
    printf("contact %d not found!\n",record);
 }

void modifycontact(void)
{
    int record, result;

    if(firstc==NULL)
    {
        puts("There are no contacts to modify!");
	return;
    }

    listAll();
    printf("Enter contact account number to modify or change: ");
    scanf("%d",&record);

    result = findnum(record);

    if( result >0 ){
	    printf("Contact %d:\n",currentc->number);
	    printf("Name: %s\n",currentc->name);
	    if(prompt())
		    gets(currentc->name);
	    printf("Phone: %s\n",currentc->phone);
	    if(prompt())
		    {do{gets(currentc->phone);}while((strlen(currentc->phone)>11) || checknum(currentc,strlen(currentc->phone))==0);}
	    printf("Email: %s\n",currentc->email);
	    if(prompt())
		    gets(currentc->email);
        printf("Email: %s\n",currentc->address);
        if(prompt())
		    gets(currentc->address);
	    return;
	}
    printf("contact %d was not found!\n",record);
}

int findnum (int recordnum)
{
    int record;
    record = recordnum;
    currentc = firstc;
    while(currentc != NULL)
    {

        if(currentc->number == record)
	     {
           return 1;
         }

        else
        {
            currentc = currentc->next;
        }
    }
    return -1;
}

int findcontact(void)
{
     int flag=0;
     char buff[20];
     if(firstc==NULL)
	{
        puts("There are no contacts to find!");
	    return 1;
    }

    printf("Enter contact name: ");
    fflush(stdin);
    gets(buff);

    currentc = firstc;
    while(currentc != NULL)
    {

        if( strcmp(currentc->name, buff) == 0 )
	    {
			printf("%6s %-20s %-15s %-20s %-40s\n","Acct#","Name","Phone","Email","     Address");
            printf("%6d: %-20s %-15s %-20s %-40s\n",\
            currentc->number,\
            currentc->name,\
            currentc->phone,\
            currentc->email,\
            currentc->address);
			currentc = currentc->next;
            flag=1;
	    }
		else
		{
			currentc = currentc->next;
		}
    }
    if(flag==0) printf("contact %s was not found!\n",buff);
          return 1;
}

int prompt(void)
{
	char ch;
    fflush(stdin);
	printf("Press Y to Update?");
	ch = getchar();
	ch = toupper(ch);
	fflush(stdin);
	if(ch == 'Y')
	{
		printf("Enter new value: ");
		return(1);
	}
	else
		return(0);
}

int checknum(Contact* node,int x){

    int flag=1;
    int count=0;
    int o;
    while(count<x){
        o=node->phone[count];
        if(o>57 || o<48){
            flag=0;
        }
        count++;
    }
    return flag;

}

void bubbleSort(Contact *start)
{
    int swapped, i;
    Contact *ptr1;
    Contact *lptr = NULL;

    /* Checking for empty list */
    if (start == NULL)
        return;

    do
    {
        swapped = 0;
        ptr1 = start;

        while (ptr1->next != lptr)
        {
            if (strcmp(ptr1->name,ptr1->next->name)>0)
            {
                swap(start,ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);
}



void swap(Contact *head_ref, Contact* x, Contact* y)
{

    Contact* temp;
    temp=head_ref;
    while(temp->next!=x){
        temp=temp->next;
    }
    temp->next=y;
    x->next=y->next;
    y->next=x;
    if(head_ref==x)
        head_ref=y;
}
/*
void swap(Contact *head_ref, Contact* x, Contact* y)
{
   // Nothing to do if x and y are same
   if (x == y) return;

   // Search for x (keep track of prevX and CurrX
   Contact *prevX = NULL;
   Contact *currX = head_ref;
   while (currX && currX->number != x)
   {
       prevX = currX;
       currX = currX->next;
   }

   // Search for y (keep track of prevY and CurrY
   Contact *prevY = NULL, *currY = head_ref;
   while (currY && currY->number != y)
   {
       prevY = currY;
       currY = currY->next;
   }

   // If either x or y is not present, nothing to do
   if (currX == NULL || currY == NULL)
       return;

   // If x is not head of linked list
   if (prevX != NULL)
       prevX->next = currY;
   else // Else make y as new head
       head_ref = currY;

   // If y is not head of linked list
   if (prevY != NULL)
       prevY->next = currX;
   else  // Else make x as new head
       head_ref = currX;

   // Swap next pointers
   Contact *temp = currY->next;
   int temp1 = currY->next->number;
   currY->next = currX->next;
   currY->next->number=currX->next->number;
   currX->next  = temp;
   currX->next->number=temp1;
}*/

/*void swap(Contact *a, Contact *b)
{
    int temp = a->number;
    a->number = b->number;
    b->number = temp;
}*/

/*int count(Contact *list)
{
    int nodes = 0;

    while (list != NULL)
    {
        nodes++;
        list = list->next;
    }

    return nodes;
}


int swap(Contact *list, int pos1, int pos2)
{
    Contact *node1, *node2, *prev1, *prev2, *temp;
    int i;

    // Get the far position among both
    const int maxPos = (pos1 > pos2) ? pos1 : pos2;

    // Get total nodes in the list
    const int totalNodes = count(list);

    // Validate swap positions
    if ((pos1 <= 0 || pos1 > totalNodes) || (pos2 <= 0 || pos2 > totalNodes))
    {
        return -1;
    }

    // If both positions are same then no swapping required
    if (pos1 == pos2)
    {
        return 1;
    }


    // Identify both nodes to swap
    i = 1;
    temp  = list;
    prev1 = NULL;
    prev2 = NULL;

    // Find nodes to swap
    while (temp != NULL && (i <= maxPos))
    {
        if (i == pos1 - 1)
            prev1 = temp;
        if (i == pos1)
            node1 = temp;

        if (i == pos2 - 1)
            prev2 = temp;
        if (i == pos2)
            node2 = temp;

        temp = temp->next;
        i++;
    }

    // If both nodes to swap are found.
    if (node1 != NULL && node2 != NULL)
    {
        // Link previous of node1 with node2
        if (prev1 != NULL)
            prev1->next = node2;

        // Link previous of node2 with node1
        if (prev2 != NULL)
            prev2->next = node1;

        // Swap node1 and node2 by swapping their
        // next node links
        temp        = node1->next;
        node1->next = node2->next;
        node2->next = temp;

        // Make sure to swap head node when swapping
        // first element.
        if (prev1 == NULL)
            head = node2;
        else if (prev2 == NULL)
            head = node1;
    }

    return 1;
}*/
