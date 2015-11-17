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
	int positivetracker;
	int negativetracker;
	struct attnode * next;
	struct attnode * testnext;
	struct attnode * globnext;
	struct attnode * LSON;
	struct attnode * RSON;
	struct attnode * parent;
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
int positiveValues(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * train);
int negativeValues(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * train);
float getEntropy(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * train);
float getGain(int parent, int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * train);
int getMaxGain(int parent, int S, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * train);
int isUsed(int attribute, att * train);
void addSubtree(int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * train, tree * decisionTree);
void updatePosNeg(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * train);

int classify(att * test, att * train, tree * decisionTree);

att glob;

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
	att train;
	att test;
	tree decisionTree;
	init(&train);
	init(&test);
	initTree(&decisionTree);
	init(&glob);
	attnode * node;

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

	file=fopen("joinorg.txt", "r");
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
		addNode(&train, strInput, 0, k, j);
		j--;

		fscanf(file, "%d\n", &nchoices);
		for(i=0; i<nchoices; i++)
		{
			fgets(strInput, LENGTH, file);
			removeNewline(strInput);
			addNode(&train, strInput, i+1, k, valuesize);
			valuesize++;
		}
	}

	showList(&train);

	/*
		Format of inputvalues.txt:
		5                               // Number of values
		Sunny Hot High Light No         // 1st input value
		Sunny Hot High Strong No        // 2nd input value
		Overcast Hot High Light Yes     // 3rd input value
		Rain Mild High Light Yes        // 4th input value
		Rain Cool Normal Light Yes      // 5th input value
	*/

	file2=fopen("inputsurvey.txt", "r");
	if(file==NULL)
	{
		printf("Missing or empty file.\n");
		return 0;
	}

	fscanf(file2, "%d\n", &nvalues);
	attnode * curr;
	for(i=0; i<nvalues; i++)
	{
		for(j=0; j<nattributes; j++)
		{
			fscanf(file2, "%s ", strInput);
			curr=train.head;
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

	addSubtree(0, values, nvalues, nattributes, &train, &decisionTree);
	node=glob.head;
	while(node!=NULL){
		addSubtree(node->equivalent, values, nvalues, nattributes, &train, &decisionTree);
		node=node->globnext;
	}

	//Male 4th 1-2 Extrovert 18-21 3
	int testing[6]={1, 6, 9, 14, 17, 22};
	int testingctr=6;
	attnode * findtest;
	attnode * testcurr;

	testcurr=test.head;
	for(i=0; i<testingctr; i++)
	{
		findtest=train.head;
		while(findtest!=NULL)
		{
			if(findtest->equivalent==testing[i])
			{
				break;
			}
			findtest=findtest->next;
		}
		if(test.head==NULL){
			test.head=findtest;
			test.tail=findtest;
			test.head->testnext=NULL;
		}
		else
		{
			testcurr=test.head;
			while(testcurr->testnext!=NULL)
			{
				testcurr=testcurr->testnext;
			}
			findtest->testnext=NULL;
			testcurr->testnext=findtest;
		}
	}

	testcurr=test.head;
	while(testcurr!=NULL)
	{
		printf("%s ", testcurr->attname);
		testcurr=testcurr->testnext;
	}


	classify(&test, &train, &decisionTree);
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
	attnode * node;
	attnode * curr;

	node=(attnode*)malloc(sizeof(attnode));
	strcpy(node->attname, attname);
	node->x=x;
	node->y=y;
	node->equivalent=equivalent;
	node->used=0;
	node->positivetracker=0;
	node->negativetracker=0;
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
	attnode * curr;
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

int positiveValues(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * train)
{
	attnode * curr;
	attnode * yes;
	int i, j, k;
	int positivectr=0;
	if(S==0)
	{
		yes=train->head;
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
		curr=train->head;
		while(curr!=NULL)
		{
			if(curr->equivalent==attribute)
			{
				break;
			}
			curr=curr->next;
		}

		yes=train->head;
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

int negativeValues(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * train)
{
	attnode * curr;
	attnode * no;
	int i, j, k;
	int negativectr=0;
	if(S==0)
	{
		no=train->head;
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
		curr=train->head;
		while(curr!=NULL)
		{
			if(curr->equivalent==attribute)
			{
				break;
			}
			curr=curr->next;
		}

		no=train->head;
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

float getEntropy(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * train)
{
	float entropy;
	int positive;
	int negative;
	attnode * curr;
	curr=train->head;
	while(curr!=NULL)
	{
		if(curr->equivalent==attribute)
		{
			break;
		}
		curr=curr->next;
	}
	if(S==0)
	{
		positive=positiveValues(S, attribute, values, nvalues, nattributes, train);
		negative=negativeValues(S, attribute, values, nvalues, nattributes, train);
	}
	else
	{
		updatePosNeg(S, attribute, values, nvalues, nattributes, train);
		positive=curr->positivetracker;
		negative=curr->negativetracker;
	}
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
		printf("entropy of %d wrt %d=-%d/%dlg%d/%d - %d/%dlg%d/%d=%lf\n", attribute, S, positive, total, positive, total, negative, total, negative, total, entropy);
	}
	//printf("%d %lf %lf\n", total, pos, neg);

	//printf("\nanswer: %lf\n", entropy);
	return entropy;
}

float getGain(int parent, int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * train)
{
	attnode * curr;
	attnode * curr2;
	float entropy;
	float entropy2;
	float sum=0;
	int denominator;
	//=positiveValues(S, S, values, nvalues, nattributes, train)+negativeValues(S, S, values, nvalues, nattributes, train);
	int numerator;
	float gain;
	int positive;
	int negative;
	entropy=getEntropy(parent, S, values, nvalues, nattributes, train);
	//printf("ooh yas %lf\n", entropy);

	curr=train->head;
	while(curr!=NULL)
	{
		if(curr->equivalent==S)
		{
			break;
		}
		curr=curr->next;
	}
	if(S==0)
	{
		positive=positiveValues(S, S, values, nvalues, nattributes, train);
		negative=negativeValues(S, S, values, nvalues, nattributes, train);	
	}
	else
	{
		updatePosNeg(parent, S, values, nvalues, nattributes, train);
		positive=curr->positivetracker;
		negative=curr->negativetracker;

	}
	denominator=positive+negative;

	curr=train->head;
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
			entropy2=getEntropy(S, curr2->equivalent, values, nvalues, nattributes, train);
			//printf("Entropy: %lf\n", entropy2);
			if (entropy2!=0)
			{
				if(S==0)
				{
					positive=positiveValues(S, curr2->equivalent, values, nvalues, nattributes, train);
					negative=negativeValues(S, curr2->equivalent, values, nvalues, nattributes, train);
				}
				else
				{
					updatePosNeg(S, curr2->equivalent, values, nvalues, nattributes, train);
					positive=curr2->positivetracker;
					negative=curr2->negativetracker;
				}
				numerator=positive+negative;
				//positiveValues(S, curr2->equivalent, values, nvalues, nattributes, train)+negativeValues(S, curr2->equivalent, values, nvalues, nattributes, train);
				//printf("Numerator: %d\n", numerator);
				//printf("Denominator: %d\n", denominator);
				sum+=(float)numerator/denominator*entropy2;
				printf("Gain+=%d/%d * %lf\n", numerator, denominator, entropy2);
				//printf("Sum ryt nao: %lf\n", sum);
			}
		}
		curr2=curr2->next;
	}
	gain=entropy-sum;
	printf("Gain: %lf\n", gain);
	return gain;
}

int getMaxGain(int parent, int S, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * train)
{
	float max=0;
	int maxAtt;
	int i;
	int usedctr=0;
	float gain;

	for(i=-1; i>-nattributes; i--)
	{
		if(isUsed(i, train)==0)
		{
			gain=getGain(parent, S, i, values, nvalues, nattributes, train);
			printf("Gain of %d and %d is %lf\n", S, i, gain);
			if(gain>max)
			{
				max=gain;
				maxAtt=i;
			}
		}
		else{
			usedctr++;
		}
	}

	if(usedctr==nattributes-1){
		printf("Wala na pwedeng idagdag na attribute awthu\n");
		maxAtt=102;
	}

	else{
		if (max==0)
		{
			int pos=positiveValues(0, S, values, nvalues, nattributes, train);
			int neg=negativeValues(0, S, values, nvalues, nattributes, train);
			if (pos>neg)
			{
				maxAtt=100;
			}
			else
			{
				maxAtt=101;
			}
		}
	}
	printf("lalabas ng fcn na maxgain with maxatt %d.\n", maxAtt);
	return maxAtt;
}

int isUsed(int attribute, att * train)
{
	attnode * curr;
	curr=train->head;
	while(curr!=NULL)
	{
		if(curr->equivalent==attribute)
		{
			if(curr->used==1)
			{
				printf("%s is used\n", curr->attname);
				return 1;
			}
			else
			{
				printf("%s hasn't been used yet\n", curr->attname);
				return 0;
			}
		}
		curr=curr->next;
	}
	return 0;
}

void addSubtree(int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * train, tree * decisionTree)
{
	attnode * curr;
	attnode * curr2;
	attnode * decurr;
	attnode * from;
	attnode * globcurr;
	attnode * first;
	attnode * yes;
	attnode * no;
	int toAdd;
	int i=0;
	int numRSON;

	// check if tree is null

	if (decisionTree->top==NULL)
	{
		// getMaxGain
		toAdd = getMaxGain(0, 0, values, nvalues, nattributes, train);

		curr=train->head;
		while (curr!=NULL){
			if (curr->equivalent == toAdd){
				curr->used=1;
				decisionTree->top=curr;
				decisionTree->top->LSON=NULL;
				decisionTree->top->RSON=NULL;
				decisionTree->top->parent=NULL;

				break;
			}
			curr=curr->next;
		}
		decurr=decisionTree->top;

		printf("\nTop nung dec tree: %s\n", decurr->attname);
		//addSubtree(decisionTree, train, decisionTree->top->equivalent, nattributes);

		first=decurr;

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
					curr2->parent=decurr;
					updatePosNeg(curr2->parent->equivalent, curr2->equivalent, values, nvalues, nattributes, train);
					decurr->RSON=NULL;
					printf("%s -LSON-> %s\n", decurr->attname, decurr->LSON->attname);
					decurr=decurr->LSON;
					if (glob.head==NULL){
						//printf("Null glob");
						decurr->globnext=NULL;
						glob.head=decurr;
						printf("\nAdded %s to glob\n", decurr->attname);
					}
					else{
						globcurr=glob.head;
						while(globcurr->globnext!=NULL){
							globcurr=globcurr->globnext;
						}
						decurr->parent=first;
						decurr->globnext=NULL;
						globcurr->globnext=decurr;
						printf("\nAdded %s to glob\n", decurr->attname);
					}
				}
				else
				{
					printf("\ni>1 (next value)\n");
					decurr->LSON=NULL;
					decurr->RSON=curr2;

					curr2->parent=first;

					updatePosNeg(curr2->parent->equivalent, curr2->equivalent, values, nvalues, nattributes, train);
					printf("%s -RSON-> %s\n", decurr->attname, decurr->RSON->attname);
					decurr=decurr->RSON;
					if (glob.head==NULL){
						decurr->globnext=NULL;
						glob.head=decurr;
						printf("\nAdded %s to glob\n", decurr->attname);
					}
					else{
						globcurr=glob.head;
						while(globcurr->globnext!=NULL){
							globcurr=globcurr->globnext;
						}
						decurr->parent=first;
						decurr->globnext=NULL;
						globcurr->globnext=decurr;
						printf("\nAdded %s to glob\n", decurr->attname);
					}
				}
			}
			curr2=curr2->next;
		}
		/*
		while(decurr!=NULL)
		{
			//printf("\ndecurr: %s\n", decurr->attname);
			addSubtree(decurr->equivalent, values, nvalues, nattributes, train, decisionTree);
			decurr=decurr->RSON;
		}
		*/

		/*
		// PRINT SOME PARTS OF THE TREE
		printf("%s-LSON->%s\n", decisionTree->top->attname, decisionTree->top->LSON->attname);
		printf("%s-RSON->%s\n", decisionTree->top->attname, decisionTree->top->RSON->attname);
		printf("%s(lson ng tree)-LSON->%s\n", decisionTree->top->LSON->attname, decisionTree->top->LSON->LSON->attname);
		printf("%s(lson ng tree)-RSON->%s\n", decisionTree->top->LSON->attname, decisionTree->top->LSON->RSON->attname);
		printf("%s(top->LSON->RSON->RSON)-LSON->%s\n", decisionTree->top->LSON->RSON->RSON->attname, decisionTree->top->LSON->RSON->RSON->LSON->attname);
		printf("%s-LSON->%s\n", decisionTree->top->LSON->RSON->RSON->LSON->attname, decisionTree->top->LSON->RSON->RSON->LSON->LSON->attname);
		printf("%s-RSON->%s\n", decisionTree->top->LSON->RSON->RSON->LSON->LSON->attname, decisionTree->top->LSON->RSON->RSON->LSON->LSON->RSON->attname);
		*/
	}

	else
	{
		printf("\nAttribute: %d\n", attribute);

		decurr=decisionTree->top;
		curr=train->head;
		while (curr!=NULL){
			if (curr->equivalent == attribute){
				break;
			}
			curr=curr->next;
		}

		if (curr->positivetracker == 0){
			printf("APPEND NO\n");
			no=train->head;
			while(no!=NULL)
			{
				if(strcmp(no->attname, "No")==0)
				{
					break;
				}
				no=no->next;
			}
			curr->LSON=no;
			no->parent=curr;

		}
		else if (curr->negativetracker == 0){
			printf("APPEND YES\n");
			yes=train->head;
			while(yes!=NULL)
			{
				if(strcmp(yes->attname, "Yes")==0)
				{
					break;
				}
				yes=yes->next;
			}
			curr->LSON=yes;
			yes->parent=curr;
		}

		else
		{
			//if (toAdd==100||toAdd==101){
				//printf("\n>>Append %d yes/no na node (backlog muna)\n", attribute);
				//decurr = addNode(att * foo, char attname[LENGTH], int x, int y, int equivalent);
			//}
			//printf("%d\n", toAdd);
			//else{
				// find decurr
			curr=train->head;
			while(curr!=NULL)
			{
				if(curr->equivalent==attribute)
				{
					break;
				}
				curr=curr->next;
			}

			printf("Kunin toAdd\n");
			toAdd = getMaxGain(curr->parent->equivalent, curr->equivalent, values, nvalues, nattributes, train);
			/*
			decurr=decisionTree->top;
			curr=train->head;
			while (curr!=NULL){
				if (curr->equivalent == toAdd){
					//curr->used=1;
					decurr=curr;
					decurr->LSON=NULL;
					decurr->RSON=NULL;
					break;
				}
				curr=curr->next;
			}
			*/

			if (toAdd==102){
				printf("\nDahil used na si %s:\n", curr->attname);
				curr=train->head;
				while (curr!=NULL){
					if (curr->equivalent == attribute){
						break;
					}
				curr=curr->next;
				}

				if (curr->positivetracker>curr->negativetracker){
					printf("APPEND YES %d dahil wala na (No: %d)\n", curr->positivetracker, curr->negativetracker);
					yes=train->head;
					while(yes!=NULL)
					{
						if(strcmp(yes->attname, "Yes")==0)
						{
							break;
						}
						yes=yes->next;
					}
					curr->LSON=yes;
					yes->parent=curr;
				}
				else{
					printf("APPEND NO %d dahil wala na (Yes: %d)\n", curr->negativetracker, curr->positivetracker);
					no=train->head;
					while(no!=NULL)
					{
						if(strcmp(no->attname, "No")==0)
						{
							break;
						}
						no=no->next;
					}
					curr->LSON=no;
					no->parent=curr;
				}
			}

			else{
				
				decurr=decisionTree->top;
				curr=train->head;
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
				curr->used=1;

				printf("\n>>Ia-add ang %s", decurr->attname);
				
				// find attribute node
				curr=train->head;
				while (curr!=NULL){
					if (curr->equivalent == attribute){
						//curr->used=1;
						from=curr;
						break;
					}
					curr=curr->next;
				}

				//first=from;

				printf(" as LSON ni %s", from->attname);

				from->LSON=decurr;
				decurr->parent=from;
				updatePosNeg(decurr->parent->equivalent, decurr->equivalent, values, nvalues, nattributes, train);
				curr=decurr;

				first=decurr;

				//printf("\n%s(top)-LSON->%s-LSON->%s\n", decisionTree->top->attname, decisionTree->top->LSON->attname, decisionTree->top->LSON->LSON->attname);

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
							decurr->LSON=curr2;
							curr2->parent=decurr;
							updatePosNeg(curr2->parent->equivalent, curr2->equivalent, values, nvalues, nattributes, train);
							decurr->RSON=NULL;
							printf("\n%s -LSON-> %s\n", decurr->attname, decurr->LSON->attname);
							decurr=decurr->LSON;
							if (glob.head==NULL){
								decurr->globnext=NULL;
								glob.head=decurr;
								printf("\nAdded %s to glob\n", decurr->attname);
							}
							else{
								globcurr=glob.head;
								while(globcurr->globnext!=NULL){
									globcurr=globcurr->globnext;
								}
								decurr->parent=first;
								decurr->globnext=NULL;
								globcurr->globnext=decurr;
								printf("\nAdded %s to glob\n", decurr->attname);
							}
						}
						else
						{
							//printf("i>1 (next value)\n");
							decurr->LSON=NULL;
							decurr->RSON=curr2;

							curr2->parent=first;

							printf("\n%s -RSON-> %s", decurr->attname, decurr->RSON->attname);
							decurr=decurr->RSON;
							updatePosNeg(decurr->parent->equivalent, decurr->equivalent, values, nvalues, nattributes, train);
							if (glob.head==NULL){
							glob.head=decurr;
							printf("\nAdded %s to glob\n", decurr->attname);
							}
							else{
								globcurr=glob.head;
								while(globcurr->globnext!=NULL){
									globcurr=globcurr->globnext;
								}
								decurr->parent=first;
								decurr->globnext=NULL;
								globcurr->globnext=decurr;
								printf("\nAdded %s to glob\n", decurr->attname);
							}
						}
					}
					curr2=curr2->next;
				}
				/*
				while(decurr!=NULL)
				{
					//printf("\ndecurr: %s\n", decurr->attname);
					addSubtree(decurr->equivalent, values, nvalues, nattributes, train, decisionTree);
					decurr=decurr->RSON;
				}
				*/
			}
		}
	}
}

void updatePosNeg(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * train)
{
	attnode * curr;
	attnode * curr2;
	attnode * findS;
	attnode * realnode;
	attnode * yes;
	attnode * no;
	int parents[LENGTH];
	int parentsctr=0;
	int positivectr=0;
	int negativectr=0;
	int i, j, k;

	findS=train->head;
	while(findS!=NULL)
	{
		if(findS->equivalent==S)
		{
			break;
		}
		findS=findS->next;
	}

	curr=train->head;
	while(curr!=NULL)
	{
		if(curr->equivalent==attribute)
		{
			parents[parentsctr]=curr->equivalent;
			parentsctr++;
			realnode=curr;
			//printf("%s's parent is %s\n", curr->attname, findS->attname);
			if(findS->equivalent>0)
			{
				parents[parentsctr]=findS->equivalent;
				//printf("%d appended to parents with counter %d\n", findS->equivalent, parentsctr);
				parentsctr++;				
			}
			//Append sa list
			break;
		}
		curr=curr->next;
	}

	curr=findS;
	while(curr!=NULL)
	{
		//printf("%s's parent is %s\n", curr->attname, curr->parent->attname);
		if(curr->parent!=NULL && curr->parent->equivalent>0)
		{
			parents[parentsctr]=curr->parent->equivalent;
			//printf("%d appended to parents with counter %d\n", curr->parent->equivalent, parentsctr);
			parentsctr++;				
		}
		curr=curr->parent;
		//Append sa list
	}
	/*printf("Array consists of ");

	for(i=0; i<parentsctr; i++)
	{
		printf("%d, ", parents[i]);
	}
	printf("Parentsctr: %d\n", parentsctr);*/

	yes=train->head;
	while(yes!=NULL)
	{
		if(strcmp(yes->attname, "Yes")==0)
		{
			break;
		}
		yes=yes->next;
	}

	no=train->head;
	while(no!=NULL)
	{
		if(strcmp(no->attname, "No")==0)
		{
			break;
		}
		no=no->next;
	}

	int breakouter=0;
	int matchingctr;
	// Check 2D array na nandun lahat
	for(i=0; i<nvalues; i++)
	{
		matchingctr=0;
		for(j=0; j<nattributes; j++)
		{
			for(k=0; k<parentsctr; k++)
			{
				//printf("%d==%d?\n", parents[k], values[i][j]);
				if(parents[k]==values[i][j])
				{
					matchingctr++;
					//printf("Nagmatch yung value ni parents[%d]=%d and values[%d][%d]=%d\n", k, parents[k], i, j, values[i][j]);
				}
			}
		}
		if(matchingctr==parentsctr)
		{
			if(values[i][nattributes-1]==no->equivalent)
			{
				negativectr++;
			}
			else if(values[i][nattributes-1]==yes->equivalent)
			{
				positivectr++;
			}
			//printf("Nandito yung value sa row %d\n", i+1);
		}
	}
	// printf("Positive Rows: %d\nNegative Rows: %d\n\n", positivectr, negativectr);
	if(realnode->equivalent<0)
	{
		realnode->positivetracker=-1;
		realnode->negativetracker=-1;
	}
	else
	{
		realnode->positivetracker=positivectr;
		realnode->negativetracker=negativectr;
	}
	// Pagkaexit, before icheck yung gain check if + or - tracker ==0 if yes, wag na idagdag yung node
}


int classify(att * test, att * train, tree * decisionTree)
{
	attnode * curr;
	attnode * curr2;
	attnode * decurr;
	attnode * yes;
	attnode * no;

	yes=train->head;
	while(yes!=NULL)
	{
		if(strcmp(yes->attname, "Yes")==0)
		{
			break;
		}
		yes=yes->next;
	}

	no=train->head;
	while(no!=NULL)
	{
		if(strcmp(no->attname, "No")==0)
		{
			break;
		}
		no=no->next;
	}
	
	decurr=decisionTree->top;
	printf("\nNasa top tayo ng decision tree with value %s\n", decurr->attname);
	while(decurr->equivalent!=yes->equivalent && decurr->equivalent!=no->equivalent)
	{
		printf("Di pa nagyes/no. %d==%d? or %d==%d\n", decurr->equivalent, yes->equivalent, decurr->equivalent, no->equivalent);
		curr=test->head;
		while(curr!=NULL)
		{
			if(curr->y==decurr->y)
			{
				printf("Found matching input with equivalent y value %s.\n", curr->attname);
				break;
			}
			curr=curr->testnext;
		}
		decurr=decurr->LSON;
		printf("Move to left son with value %s and decurr eq is %d\n", decurr->attname, decurr->equivalent);
		while(curr->equivalent!=decurr->equivalent)
		{
			decurr=decurr->RSON;
			printf("Move to right son with value %s\n", decurr->attname);
		}
		decurr=decurr->LSON;
		printf("Value is now %s\n", decurr->attname);
	}
	printf("Nag%s na!\n", decurr->attname);
	if(decurr->equivalent==no->equivalent)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}