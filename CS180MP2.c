#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define LENGTH 100

typedef struct attnode attnode;
typedef struct att att;

struct attnode
{
	char attname[LENGTH];
	int x;
	int y;
	int equivalent;
	int used;
	struct attnode * next;
};

struct att
{
	attnode * head;
	attnode * tail;
};


char * removeNewline(char * str);
void init(att * foo);
void addNode(att * foo, char attname[LENGTH], int x, int y, int equivalent);
void showList(att * foo);

int main()
{
	int i, j, k, l; // Iterators
	int valuesize;
	char strInput[LENGTH]; // Buffer for scanning string inputs
	int values[LENGTH][LENGTH]; // 2D Array to contain integer value equivalents
	int nattributes; // Number of attributes
	int nchoices; // Number of choices per attribute
	int nvalues; // Number of values in test data
	FILE * file;
	FILE * file2;
	att test;
	init(&test);

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
	valuesize=1;
	j=-1;
	for(k=0; k<nattributes; k++)
	{
		fgets(strInput, LENGTH, file);
		removeNewline(strInput);
		addNode(&test, strInput, 0, k, j);
		j--;

		fscanf(file, "%d\n", &nchoices);
		for(i=0; i<nchoices; i++)
		{
			fgets(strInput, LENGTH, file);
			removeNewline(strInput);
			addNode(&test, strInput, i+1, k, valuesize);
			valuesize++;
		}
	}

	showList(&test);

	/*
		Format of inputvalues.txt:
		5								// Number of values
		Sunny Hot High Light No 		// 1st input value
		Sunny Hot High Strong No 		// 2nd input value
		Overcast Hot High Light Yes 	// 3rd input value
		Rain Mild High Light Yes 		// 4th input value
		Rain Cool Normal Light Yes 		// 5th input value
	*/

	/*file2=fopen("inputvalues.txt", "r");
	if(file==NULL)
	{
		printf("Missing or empty file.\n");
		return;
	}

	fscanf(file2, "%d\n", &nvalues);
	for(i=0; i<nvalues; i++)
	{
		for(valuesize=0; valuesize<nattributes; valuesize++)
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
			values[i][valuesize]=curr->equivalent;

		}
	}

	printf("\n");

	for(i=0; i<nvalues; i++)
	{
		for(valuesize=0; valuesize<nattributes; valuesize++)
		{
			curr=head;
			while(curr!=NULL)
			{
				if(values[i][valuesize]==curr->equivalent)
				{
					break;
				}
				curr=curr->next;
			}
			printf("%s ", curr->attname);
		}
		printf("\n");
	}*/

	//positiveValues(NULL, 1, values, valuesize, nattributes, test, head);
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

void init(att * foo)
{
	foo->head=NULL;
	foo->tail=NULL;
}


void addNode(att * foo, char attname[LENGTH], int x, int y, int equivalent)
{
	struct attnode * node;
	struct attnode * curr;

	node=(attnode*)malloc(sizeof(attnode));
	strcpy(node->attname, attname);
	node->x=x;
	node->y=y;
	node->equivalent=equivalent;
	node->next=NULL;

	if(foo->head==NULL)
	{
		foo->head=node;
		foo->tail=node;
		foo->head->next=NULL;
	}
	else
	{
		curr=foo->head;
		while(curr->next!=NULL)
		{
			curr=curr->next;
		}
		curr->next=node;
		foo->tail=curr->next;
		foo->tail->next=NULL;
	}
}

void showList(att * foo)
{
	struct attnode * curr;
	curr=foo->head;
	if(foo->head==NULL)
	{
		printf("\nQueue: empty queue\n");
	}
	else
	{
		printf("\nQueue:\n");
		while(curr!=NULL)
		{
			printf("%s %d %d %d\n", curr->attname, curr->x, curr->y, curr->equivalent);
			curr=curr->next;
		}
	}
}

/*int positiveValues(int S, int attribute, int values[LENGTH][LENGTH], int valuesize, int nattributes, attnode * test)
{
	attnode * curr;
	attnode * yes;
	int i, j;
	if(S!=NULL)
	{
		// Stuff
	}
	else
	{
		curr=test->head;
		while(curr!=NULL)
		{
			if(curr->equivalent==attribute)
			{
				break;
			}
			curr=curr->next;
		}

		yes=test->head;
		while(yes!=NULL)
		{
			if(yes->attname=="Yes")
			{
				break;
			}
			yes=yes->next;
		}

		for(i=0; i<valuesize; i++)
		{
			for(j=0; j<nattributes; j++)
			{
				if(values[i][j]==attribute && values[i][nattributes-1]==yes->equivalent)
				{
					printf("Uy may isang yes.\n");
				}
			}
		}
	}

}

int negativeValues(int S, int attribute, int values[LENGTH][LENGTH], int valuesize, int nattributes)
{
	if(S!=NULL)
	{
		//Stuff
	}
	else
	{

	}
}*/