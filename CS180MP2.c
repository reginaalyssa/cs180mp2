#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define LENGTH 100

typedef struct att
{
	char attname[LENGTH];
	int x;
	int y;
	int equivalent;
	int used;
	struct att * next;
} att;

char * removeNewline(char * str);
int positiveValues(int S, int attribute);
int negativeValues(int S, int attribute);

int main()
{
	int i, j, k, l; // Iterators
	char strInput[LENGTH]; // Buffer for scanning string inputs
	int values[LENGTH][LENGTH]; // 2D Array to contain integer value equivalents
	int nattributes; // Number of attributes
	int nchoices; // Number of choices per attribute
	int nvalues; // Number of values in test data
	FILE * file;
	FILE * file2;
	att * test;
	att * head=NULL;
	att * tail=NULL;

	/*
		Format of input.txt:
			Outlook				// 1st Attribute
			3					// Number of values for 1st attribute
			Sunny				// 1st value of 1st attribute
			Overcast			// 2nd value of 1st attribute
			Rain				// 3rd value of 1st attribute
			Temperature			// 2nd Attribute
			3 					// Number of values for 2nd attribute
			Hot					// 1st value of 2nd attribute
			Mild				// 2nd value of 2nd attribute
			Cool 				// 3rd value of 2nd attribute
	*/

	file=fopen("input.txt", "r");
	if(file==NULL)
	{
		printf("Missing or empty file.\n");
		return;
	}

	fscanf(file, "%d\n", &nattributes);
	k=0;
	j=1;
	l=-1;
	for(k=0; k<nattributes; k++)
	{
		test=malloc(sizeof(att));
		fgets(strInput, LENGTH, file);
		removeNewline(strInput);
        strcpy(test->attname, strInput);
		test->x=0;
		test->y=k;
		test->equivalent=l;
		l--;

		test->next=NULL;
		if(head==NULL)
		{
			head=test;
		}
		else
		{
			tail->next=test;
		}
		tail=test;

		fscanf(file, "%d\n", &nchoices);
		for(i=0; i<nchoices; i++)
		{
			test=malloc(sizeof(att));
			fgets(strInput, LENGTH, file);
			removeNewline(strInput);
        	strcpy(test->attname, strInput);
			test->x=i+1;
			test->y=k;
			test->equivalent=j;

			test->next=NULL;
			if(head==NULL)
			{
				head=test;
			}
			else
			{
				tail->next=test;
			}
			tail=test;
			j++;
		}
	}
	att * curr;
	curr=head;
	while(curr!=NULL)
	{
		printf("Attribute[%d][%d]: %s %d\n", curr->x, curr->y, curr->attname, curr->equivalent);
		curr=curr->next;
	}

	/*
		Format of inputvalues.txt:
		5								// Number of values
		Sunny Hot High Light No 		// 1st input value
		Sunny Hot High Strong No 		// 2nd input value
		Overcast Hot High Light Yes 	// 3rd input value
		Rain Mild High Light Yes 		// 4th input value
		Rain Cool Normal Light Yes 		// 5th input value
	*/

	file2=fopen("inputvalues.txt", "r");
	if(file==NULL)
	{
		printf("Missing or empty file.\n");
		return;
	}

	fscanf(file2, "%d\n", &nvalues);
	for(i=0; i<nvalues; i++)
	{
		for(j=0; j<nattributes; j++)
		{
			fscanf(file2, "%s ", strInput);
			curr=head;
			while(curr!=NULL)
			{
				if(strcmp(strInput, curr->attname)==0)
				{
					break;
				}
				curr=curr->next;
			}
			values[i][j]=curr->equivalent;

		}
	}

	printf("\n");

	for(i=0; i<nvalues; i++)
	{
		for(j=0; j<nattributes; j++)
		{
			curr=head;
			while(curr!=NULL)
			{
				if(values[i][j]==curr->equivalent)
				{
					break;
				}
				curr=curr->next;
			}
			printf("%s ", curr->attname);
		}
		printf("\n");
	}
}

char * removeNewline(char * str)
{
	char * cptr=str;

	while(* cptr!='\0')
	{
		if(* cptr=='\n')
		{
			* cptr='\0';
		}
		cptr++;
	}
	return str;
}

int positiveValues(int S, int attribute)
{
	if(S!=NULL)
	{
		// Stuff
	}
	else
	{

	}

}

int negativeValues(int S, int attribute)
{
	if(S!=NULL)
	{
		//Stuff
	}
	else
	{

	}
}