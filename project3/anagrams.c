/************************************************************************
 * filename: anagrams.c													*
 *																		*
 * Reads from a dictionary of words and builds lists of anagrams		*
 * (see example dictionary files and output produced for each 			*
 * input/dictionary file).												*
 *																		*
 * An anagram is a word formed by rearranging the letters of another	*
 * word such as the word "cinema" formed from the word "iceman".		*
 *																		*
 * Author(s): Chad Teitsma and Jaxon Wright								*
 ***********************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_ARRAY_SIZE 10000
#define MAX_WORD_SIZE 50

// node in a linked list
struct node {
	char *text;
	struct node *next;
};

typedef struct node Node;

// structure used for an array element
//		- head field points to the first element of a linked list
//		- size field stores number of nodes in the linked list
typedef struct {
	int size;
	Node *head;
} AryElement;

/************************************************************************
 * YOU MUST NOT DEFINE ANY GLOBAL VARIABLES (i.e., OUTSIDE FUNCTIONS).  *
 * COMMUNICATION BETWEEN FUNCTIONS MUST HAPPEN ONLY VIA PARAMETERS.     *
 ************************************************************************/

/************************************************************************
 * Function declarations/prototypes										*
 ************************************************************************/

AryElement *buildAnagramArray(char *infile, int *aryLen);

void printAnagramArray(char *outfile, AryElement *ary, int aryLen);

void freeAnagramArray(AryElement *ary, int aryLen);

bool areAnagrams(char *word1, char *word2);

Node *createNode(char *word);


/************************************************************************
 * Main driver of the program.											*
 * The input file is assumed to contain one word (of lower case			*
 * letters only) per line.												*
 * Use the following commands to compile and run the program:			*
 *		$ gcc -Wall -std=c99 -o anagrams anagrams.c						*
 * 		$ ./anagrams  dictionary1.txt  output1.txt						*
 ************************************************************************/
int main(int argc, char *argv[]){
	AryElement *ary;
	int aryLen;

	if (argc != 3) {
		printf("Wrong number of arguments to program.\n");
		printf("Usage: ./anagrams infile outfile\n");
		exit(EXIT_FAILURE);
	}

	char *inFile = argv[1];
	char *outFile = argv[2];
	
	ary = buildAnagramArray(inFile,&aryLen);

	printAnagramArray(outFile,ary,aryLen);

	freeAnagramArray(ary,aryLen);

	return EXIT_SUCCESS;
}

/************************************************************************
 * Takes a filename that contains one word (of lower case letters) per	*
 * line, reads the file contents, and builds an array of linked lists	*
 * and returns a pointer to this array. It also sets the aryLen 		*
 * parameter to size/length of the array returned from the function.	*
 ************************************************************************/
AryElement *buildAnagramArray(char *infile, int *aryLen){
	AryElement *ary = NULL;				// stores pointer to dynamically allocated array of structures
	char word[MAX_WORD_SIZE];			// stores a word read from the input file
	int curAryLen = INITIAL_ARRAY_SIZE;	// stores current length/size of the array
	int nbrUsedInAry = 0;				// stores number of actual entries in the array

	// prepare the input file for reading
	FILE *fp = fopen(infile,"r");
	if (fp == NULL) {
		fprintf(stderr,"Error opening file %s\n", infile);
		exit(EXIT_FAILURE);
	}
	
	ary = malloc(sizeof(AryElement)*INITIAL_ARRAY_SIZE);
	//AryElement: int size, Node *head
	//Node: char *text, node *next
	
	while (fscanf(fp, "%s", word) != EOF){
		//check if we are at the limit of our allocated space
		if(nbrUsedInAry == curAryLen){
			//reallocate our space
			curAryLen = curAryLen*2;
			ary = realloc(ary,sizeof(AryElement)*curAryLen);
		}
		//search array for anagrams
		bool foundAnagram = false;
		for(int i=0;i<nbrUsedInAry;i++){
			AryElement* aryElement = &ary[i];
			Node* temp = aryElement->head;
			if(areAnagrams(temp->text, word)){
				//the two words are anagrams
				ary[i].size++;
				//find the end of the current linked list
				while(temp->next != NULL){
					temp = temp->next;
				}
				//create node at the end of the current linked list
				temp->next = createNode(word);
				foundAnagram = true;
			}
			//word and ary[i] words aren't anagrams
		}
		if(foundAnagram == false){
			//there are no anagram matches for the current word. Create a new array element.
			//AryElement *newElem = malloc(sizeof(AryElement));
			ary[nbrUsedInAry].size = 1;
			ary[nbrUsedInAry].head = createNode(word);
			nbrUsedInAry++;
		}
	} //done scanning words from file
	
	//get rid of unused memory in array
	ary = realloc(ary,sizeof(AryElement)*nbrUsedInAry);
	//printf("Used %d out of %d total\n", nbrUsedInAry, curAryLen);
	fclose(fp);
	
	*aryLen = nbrUsedInAry;

    return ary;
}

/************************************************************************
 * Takes a filename used for output, a pointer to the array, and size	*
 * of the array, and prints	the list of anagrams (see sample output) 	*
 * to the file specified.												*
 ************************************************************************/
void printAnagramArray(char *outfile, AryElement *ary, int aryLen){
	FILE *fp = fopen(outfile, "w");
	//printf("Printing Array\n");
	if (fp == NULL) {
		fprintf(stderr,"Error opening file %s\n", outfile);
		exit(EXIT_FAILURE);
	}
	bool firstnl = false;
	//printf("Array is length: %d\n",aryLen);
	for(int i = 0;i<aryLen;i++){
		//only look at elements that have more than 2 words
		if(ary[i].size >= 2){
			
			if(firstnl == true){
				fprintf(fp, "\n");
			}
			firstnl = true;
			
			Node* temp = ary[i].head;
			//iterate through the linked list
			while(temp != NULL){
				fprintf(fp, "%s ",temp->text);
				temp = temp->next;
			}
			
		}
	}

	fclose(fp);
}

/************************************************************************
 * Releases memory allocated for the array and all the linked lists.	*
 * This involves releasing the memory allocated for each Node of each	*
 * linked list and the array itself. Before freeing up memory of a Node *
 * object, make sure to release the memory allocated for the 			*
 * "text" field of that node first.										*
 ************************************************************************/
void freeAnagramArray(AryElement *ary, int aryLen){
	Node *ptr, *tmp;
	for (int i = 0; i < aryLen; i++) {
		if (ary[i].size > 0) {
			ptr = ary[i].head;
			while (ptr != NULL) {
				free(ptr->text);
				tmp = ptr->next;
				free(ptr);
				ptr = tmp;
			}
		}
	}
	free(ary);
}

/************************************************************************
 * Allocates memory for a Node object and initializes the "text" field	*
 * with the input string/word and the "next" field to NULL. Returns a	*
 * pointer to the Node object created.									*
 ************************************************************************/
Node *createNode(char *word){
	
	Node *node = malloc(sizeof(Node));
	char* copy = malloc(strlen(word)+1);
	strcpy(copy,word);
	node->text = copy;
	node->next = NULL;
	return node;
}

/************************************************************************
 * Returns true if the input strings are anagrams, false otherwise.		*
 * Assumes the words contain only lower case letters.					*
 ************************************************************************/
bool areAnagrams(char *word1, char *word2){
	int alphabet[26];
	
	
	memset(alphabet, 0, sizeof(alphabet));
	
	//the words are not the same length
	if (strlen(word1) != strlen(word2)){ 
		return false;
	}
  
	for(int i=0; i < strlen(word1); i++) {
		alphabet[word1[i]-'a']++;
		alphabet[word2[i]-'a']--;
	}
	
	for(int j=0; j < 26; j++) {
		if(alphabet[j] != 0){
			return false;
		}
	}
	return true;
}