#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define LENGTH 100

typedef struct dataentry
{
    char gender[LENGTH];
    char yearlevel[LENGTH];
    char freetime[LENGTH];
    char socialskills[LENGTH];
    char units[LENGTH];
    int orgs;
    char join[LENGTH];
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

	test=malloc(sizeof(dataentry));
	fscanf(file, "%s, ", &test->gender);
	printf("%s\n", test->gender);
	fscanf(file, "%s, ", &test->yearlevel);
	printf("%s\n", test->yearlevel);
	fscanf(file, "%s, ", &test->freetime);
	printf("%s\n", test->freetime);
	fscanf(file, "%s, ", &test->socialskills);
	printf("%s\n", test->socialskills);
	fscanf(file, "%s, ", &test->units);
	printf("%s\n", test->units);
	fscanf(file, "%d, ", &test->orgs);
	printf("%d\n, ", test->orgs);
	fscanf(file, "%s, ", &test->join);
	printf("%s\n", test->join);
	//fscanf(file, "%s, %s, %s, %s, %s, %d, %s\n", &test->gender, &test->yearlevel, &test->freetime, &test->socialskills, &test->units, &test->orgs, &test->join);
	//printf("%s, %s, %s, %s, %s, %d, %s\n", test->gender, test->yearlevel, test->freetime, test->socialskills, test->units, test->orgs, test->join);
	//fgets(strInput, LENGTH, file);
	//printf("%s\n", strInput);
	/*while(fgets(strInput, LENGTH, file)!=NULL)
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

		for(j=0; j<i; j++)
		{
			printf("|%s|, ", tokens[j]);
		}
	}*/
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
