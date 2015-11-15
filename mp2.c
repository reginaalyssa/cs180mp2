#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define LENGTH 100

typedef struct attnode attnode;
typedef struct att att;
typedef struct tree tree;

struct attnode
{
	char attname[LENGTH];
	int x;
	int y;
	int equivalent;
	int used;
	struct attnode * next;
	struct attnode * LSON;
	struct attnode * RSON;  
};

struct att
{
	attnode * head;
	attnode * tail;
};

struct tree
{
	attnode * top;
};


char * removeNewline(char * str);
void init(att * foo);
void initTree(tree * foo);
void addNode(att * foo, char attname[LENGTH], int x, int y, int equivalent);
void showList(att * foo);
int positiveValues(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test);
int negativeValues(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test);
float getEntropy(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test);
float getGain(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test);
int getMaxGain(int S, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test);
int isUsed(int attribute, att * test);
void addSubtree(int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test, tree * decisionTree);


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
	tree decisionTree;
	init(&test);
	initTree(&decisionTree);

	/*
		Format of input.txt:
			Outlook             // 1st Attribute
			3                   // Number of values for 1st attribute
			Sunny               // 1st value of 1st attribute
			Overcast            // 2nd value of 1st attribute
			Rain                // 3rd value of 1st attribute
			Temperature         // 2nd Attribute
			3                   // Number of values for 2nd attribute
			Hot                 // 1st value of 2nd attribute
			Mild                // 2nd value of 2nd attribute
			Cool                // 3rd value of 2nd attribute
	*/

	file=fopen("input.txt", "r");
	if(file==NULL)
	{
		printf("Missing or empty file.\n");
		return 0;
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
		5                               // Number of values
		Sunny Hot High Light No         // 1st input value
		Sunny Hot High Strong No        // 2nd input value
		Overcast Hot High Light Yes     // 3rd input value
		Rain Mild High Light Yes        // 4th input value
		Rain Cool Normal Light Yes      // 5th input value
	*/

	file2=fopen("inputvalues.txt", "r");
	if(file==NULL)
	{
		printf("Missing or empty file.\n");
		return 0;
	}

	fscanf(file2, "%d\n", &nvalues);
	struct attnode * curr;
	for(i=0; i<nvalues; i++)
	{
		for(j=0; j<nattributes; j++)
		{
			fscanf(file2, "%s ", strInput);
			curr=test.head;
			while(curr!=NULL)
			{
				if(strcmp(strInput, curr->attname)==0)
				{
					//printf("%s ", curr->attname);
					values[i][j]=curr->equivalent;
					break;
				}
				curr=curr->next;
			}

		}
		//printf("\n");
	}

	printf("\n");   

	/*for(i=0; i<nvalues; i++)
	{
		for(j=0; j<nattributes; j++)
		{
			printf("%d ", values[i][j]);
		}
		printf("\n");
	}*/

	//getGain(0, -1, values, nvalues, nattributes, &test);
	//i=getMaxGain(0, values, nvalues, nattributes, &test);
	//printf("Node with maximum gain is %d\n", i);


	addSubtree(0, values, nvalues, nattributes, &test, &decisionTree);
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

void initTree(tree * foo)
{
	foo->top=NULL;
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
	node->used=0;
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
	if(foo->head!=NULL)
	{
		while(curr!=NULL)
		{
			printf("%s %d %d %d\n", curr->attname, curr->x, curr->y, curr->equivalent);
			curr=curr->next;
		}
	}
}

int positiveValues(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test)
{
	struct attnode * curr;
	struct attnode * yes;
	int i, j, k;
	int positivectr=0;
	if(S==0)
	{
		yes=test->head;
		while(yes!=NULL)
		{
			if(strcmp(yes->attname, "Yes")==0)
			{
				break;
			}
			yes=yes->next;
		}

		for(i=0; i<nvalues; i++)
		{
			if(attribute==0)
			{
				if(values[i][nattributes-1]==yes->equivalent)
				{
					positivectr++;
				}
			}
			else
			{
				for(j=0; j<nattributes; j++)
				{
					if(values[i][j]==attribute && values[i][nattributes-1]==yes->equivalent)
					{
						positivectr++;
					}
				}
			}
		}
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
			if(strcmp(yes->attname, "Yes")==0)
			{
				break;
			}
			yes=yes->next;
		}

		for(i=0; i<nvalues; i++)
		{
			for(j=0; j<nattributes; j++)
			{
				for(k=0; k<nattributes; k++)
				{
					if(values[i][k]==S && values[i][j]==attribute && values[i][nattributes-1]==yes->equivalent)
					{
						positivectr++;
					}
				}
			}
		}
	}
	//printf("Positive: %d\n", positivectr);
	return positivectr;

}

int negativeValues(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test)
{
	struct attnode * curr;
	struct attnode * no;
	int i, j, k;
	int negativectr=0;
	if(S==0)
	{
		no=test->head;
		while(no!=NULL)
		{
			if(strcmp(no->attname, "No")==0)
			{
				break;
			}
			no=no->next;
		}

		for(i=0; i<nvalues; i++)
		{
			if(attribute==0)
			{
				if(values[i][nattributes-1]==no->equivalent)
				{
					negativectr++;
				}
			}
			else
			{
				for(j=0; j<nattributes; j++)
				{
					if(values[i][j]==attribute && values[i][nattributes-1]==no->equivalent)
					{
						negativectr++;
					}
				}
			}
		}
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

		no=test->head;
		while(no!=NULL)
		{
			if(strcmp(no->attname, "No")==0)
			{
				break;
			}
			no=no->next;
		}

		for(i=0; i<nvalues; i++)
		{
			for(j=0; j<nattributes; j++)
			{
				for(k=0; k<nattributes; k++)
				{
					if(values[i][k]==S && values[i][j]==attribute && values[i][nattributes-1]==no->equivalent)
					{
						negativectr++;
					}   
				}
			}
		}
	}
	//printf("Negative: %d\n", negativectr);
	return negativectr;
}

float getEntropy(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test)
{
	float entropy;
	int positive=positiveValues(S, attribute, values, nvalues, nattributes, test);
	int negative=negativeValues(S, attribute, values, nvalues, nattributes, test);
	float pos;
	float neg;
	float logpos;
	float logneg;
	int total;

	total=positive+negative;
	pos=(float)positive/total;
	neg=(float)negative/total;
	logpos=log2(pos);
	logneg=log2(neg);
	if(positive==0||negative==0)
	{
		entropy=0;
	}
	else if(positive==negative)
	{
		entropy=1;
	}
	else
	{
		entropy=(-pos*logpos)-(neg*logneg);
	}
	//printf("%d %lf %lf\n", total, pos, neg);

	//printf("\nanswer: %lf\n", entropy);
	return entropy;
}

float getGain(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test)
{
	struct attnode * curr;
	struct attnode * curr2;
	float entropy;
	float entropy2;
	float sum=0;
	int denominator=positiveValues(S, S, values, nvalues, nattributes, test)+negativeValues(S, S, values, nvalues, nattributes, test);
	int numerator;
	float gain;
	entropy=getEntropy(S, S, values, nvalues, nattributes, test);
	//printf("ooh yas %lf\n", entropy);

	curr=test->head;
	while(curr!=NULL)
	{
		if(curr->equivalent==attribute)
		{
			//printf("Found curr: %s %d %d %d\n", curr->attname, curr->x, curr->y, curr->equivalent);
			break;
		}
		curr=curr->next;
	}

	curr2=curr->next;
	while(curr2!=NULL)
	{
		if(curr2->y==curr->y)
		{
			//printf("Found curr2: %s %d %d %d\n", curr2->attname, curr2->x, curr2->y, curr2->equivalent);
			entropy2=getEntropy(S, curr2->equivalent, values, nvalues, nattributes, test);
			//printf("Entropy: %lf\n", entropy2);
			numerator=positiveValues(S, curr2->equivalent, values, nvalues, nattributes, test)+negativeValues(S, curr2->equivalent, values, nvalues, nattributes, test);
			//printf("Numerator: %d\n", numerator);
			//printf("Denominator: %d\n", denominator);
			sum+=(float)numerator/denominator*entropy2;
			//printf("Sum ryt nao: %lf\n", sum);
		}
		curr2=curr2->next;
	}
	gain=entropy-sum;
	printf("Gain: %lf\n", gain);
	return gain;
}

int getMaxGain(int S, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test)
{
	float max=0;
	int maxAtt;
	int i;
	float gain;

	for(i=-1; i>-nattributes; i--)
	{
		if(isUsed(i, test)==0)
		{
			gain=getGain(S, i, values, nvalues, nattributes, test);
			printf("Gain of %d and %d is %lf\n", S, i, gain);
			if(gain>max)
			{
				max=gain;
				maxAtt=i;
			}
		}
	}
	if (max==0){
		int pos=positiveValues(0, S, values, nvalues, nattributes, test);
		int neg=negativeValues(0, S, values, nvalues, nattributes, test);
		if (pos>neg){
			maxAtt=100;
		}
		else{
			maxAtt=101;
		}
	}
	return maxAtt;
}

int isUsed(int attribute, att * test)
{
	attnode * curr;
	curr=test->head;
	while(curr!=NULL)
	{
		if(curr->equivalent==attribute && curr->used==1)
		{
			printf("Node is used\n");
			return 1;
		}
		curr=curr->next;
	}
	printf("Node hasnt been used yet\n");
	return 0;
}

void addSubtree(int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test, tree * decisionTree)
{
	attnode * curr;
	attnode * curr2;
	attnode * decurr;
	attnode * first;
	attnode * from;
	int toAdd;
	int i=0;
	int numRSON;

	// check if tree is null

	if (decisionTree->top==NULL)
	{
		// getMaxGain
		toAdd = getMaxGain(0, values, nvalues, nattributes, test);

		curr=test->head;
		while (curr!=NULL){
			if (curr->equivalent == toAdd){
				curr->used=1;
				decisionTree->top=curr;
				decisionTree->top->LSON=NULL;
				decisionTree->top->RSON=NULL;

				break;
			}
			curr=curr->next;
		}
		decurr=decisionTree->top;
		printf("\nTop nung dec tree: %s\n", decurr->attname);
		//addSubtree(decisionTree, test, decisionTree->top->equivalent, nattributes);

		// Append values of winning attribute
		curr2=curr->next;
		//printf("decurr: %s\n", decurr->attname);
		//printf("curr: %s\n", curr->attname);
		while(curr2!=NULL)
		{
			//printf("curr2: %s\n", curr2->attname);
			if(curr2->y==curr->y)
			{
				//printf("curr2 is a value\n");
				i++;
				if (i==1)
				{
					printf("i=1 (first value)\n");
					decurr->LSON=curr2;
					decurr->RSON=NULL;
					printf("%s -LSON-> %s", decurr->attname, decurr->LSON->attname);
					decurr=decurr->LSON;
					first=decurr;
				}
				else
				{
					printf("i>1 (next value)\n");
					decurr->LSON=NULL;
					decurr->RSON=curr2;
					printf("%s -RSON-> %s", decurr->attname, decurr->RSON->attname);
					decurr=decurr->RSON;
				}
			}
			curr2=curr2->next;
		}

		decurr=first;
		
		printf("\nfirst: %s\n", first->attname);
		while(decurr!=NULL)
		{
			//printf("\ndecurr: %s\n", decurr->attname);
			addSubtree(decurr->equivalent, values, nvalues, nattributes, test, decisionTree);
			decurr=decurr->RSON;
		}
	}

	else
	{
		printf("\nAttribute: %d\n", attribute);
		toAdd = getMaxGain(attribute, values, nvalues, nattributes, test);
		if (toAdd==100||toAdd==101){
			printf("\n>>Append yes/no na node (backlog muna)\n");
		}
		//printf("%d\n", toAdd);
		else{
			// find decurr
			decurr=decisionTree->top;
			curr=test->head;
			while (curr!=NULL){
				if (curr->equivalent == toAdd){
					curr->used=1;
					decurr=curr;
					decurr->LSON=NULL;
					decurr->RSON=NULL;
					break;
				}
				curr=curr->next;
			}

			printf("\n>>Ia-add ang %s", decurr->attname);
			
			// find attribute node
			curr=test->head;
			while (curr!=NULL){
				if (curr->equivalent == attribute){
					curr->used=1;
					from=curr;
					break;
				}
				curr=curr->next;
			}

			printf(" as LSON ni %s", from->attname);

			from->LSON=decurr;

			//printf("\n%s(top)-LSON->%s-LSON->%s\n", decisionTree->top->attname, decisionTree->top->LSON->attname, decisionTree->top->LSON->LSON->attname);
		}
	}
}