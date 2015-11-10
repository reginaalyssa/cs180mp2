#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define LENGTH 100

typedef struct dataentry
{
    char * gender;
    char * yearlevel;
    char * freetime;
    char * socialskills;
    char * units;
    int orgs;
    char * join;
    struct dataentry * next;
} dataentry;

char * removeNewline(char *str);
int main()
{
	int i, j, k;
	char strInput[LENGTH];
	char * tokens[LENGTH];
	char * token;
	FILE * file;
	dataentry * test;
    dataentry * head=NULL;
    dataentry * tail=NULL;

	file=fopen("input.txt", "r");
	if(file==NULL)
	{
		printf("Missing or empty file.\n");
		return;
	}

	while(fgets(strInput, LENGTH, file)!=NULL)
	{
		test=malloc(sizeof(dataentry));
		removeNewline(strInput);

		test->gender=strInput;
		printf("|%s|", strInput);

		fgets(strInput, LENGTH, file);
		removeNewline(strInput);

		test->yearlevel=strInput;
		printf("|%s|", strInput);

		fgets(strInput, LENGTH, file);
		removeNewline(strInput);

		test->freetime=strInput;
		printf("|%s|", strInput);

		fgets(strInput, LENGTH, file);
		removeNewline(strInput);

		test->socialskills=strInput;
		printf("|%s|", strInput);

		fgets(strInput, LENGTH, file);
		removeNewline(strInput);

		test->units=strInput;
		printf("|%s|", strInput);

		fgets(strInput, LENGTH, file);
		removeNewline(strInput);

		test->orgs=atoi(strInput);
		printf("|%s|", strInput);

		fgets(strInput, LENGTH, file);
		removeNewline(strInput);

		test->join=strInput;
		printf("|%s|", strInput);
		
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

		/*for(j=0; j<i; j++)
		{
			printf("|%s|, ", tokens[j]);
		}*/
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
