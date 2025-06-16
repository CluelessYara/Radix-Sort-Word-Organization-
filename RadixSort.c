//Yara Wael Obaid 1212482
//DATA STRUCTURES AND ALGORITHMS FIRST PROJECT FALL/WINTER 2023
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define maxsize 100
#define stringmax 31
#define numlists 128 //128 is the total number off all ascii values that a character could have, and in radix sorting it serves as the size of the array

// declare a doubly linked list data structure
struct Node
{
    char data[stringmax];
    struct Node* prev;
    struct Node* next;

};

// declare a data structure for an array of doubly linked lists
struct ArrayOfLists
{
    struct Node* head;
    struct Node* tail;
};

// a function to append a new node with the given data to the end of a doubly linked list
struct Node* insertAtEnd(struct Node* L, char* ch)
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    strcpy(newNode->data, ch);  // Copy the string into the node
    newNode->next = NULL;

    if (L == NULL)
    {
        newNode->prev = NULL;
        return newNode;
    }

    struct Node* current = L;
    while (current->next != NULL)
    {
        current = current->next;
    }

    newNode->prev = current;
    current->next = newNode;

    return L;
}

//a function to delete a node with a specified data value from the linked list
void Delete(struct Node** list, const char word[])
{
    struct Node* current = *list;
    while (current != NULL)
    {
        if (strcmp(current->data, word) == 0)
        {
            struct Node* temp = current;

            if (temp->prev != NULL)
            {
                temp->prev->next = temp->next;
            }
            else
            {
                // If the node to delete is the head
                *list = temp->next;
            }

            if (temp->next != NULL)
            {
                temp->next->prev = temp->prev;
            }

            free(temp);
            return;
        }

        current = current->next;
    }
}

// a function that traverses the linked list and prints its data
void printList(struct Node* head)
{
    struct Node* current = head;
    while (current != NULL)
    {
        printf("%s", current->data);
        printf(" <-> ");
        current = current->next;
    }
    printf("NULL\n");
}

//a function to create and initialize an array of linked lists
struct ArrayOfLists* createArrayOfLists(int numLists)
{
    struct ArrayOfLists* array = (struct ArrayOfLists*)malloc(numLists * sizeof(struct ArrayOfLists));
    if (array == NULL)
    {
        printf("out of memory!\n");
        exit(1);
    }

    for (int i = 0; i < numLists; ++i)
    {
        array[i].head = NULL;
        array[i].tail = NULL;
    }

    return array;
}


//a function that inserts data into a specified index of an array of linked lists
void addToEnd(struct ArrayOfLists* array, int listIndex, char* ch)
{
    array[listIndex].head = insertAtEnd(array[listIndex].head, ch);
    if (array[listIndex].tail == NULL)
        array[listIndex].tail = array[listIndex].head;

}

//a function to print the elements of each list in the array
void printArrayOfLists(struct ArrayOfLists* array, int numLists)
{
    for (int i = 0; i < numLists; ++i)
    {
        printf("List %d: ", i);
        printList(array[i].head);
    }
}

//a function that gets the length of the longest string stored in a linked list, to be used later for padding strings
int LongestString(struct Node* list)
{
    int maxLength = 0;
    while(list!=NULL)
    {
        if (strlen(list->data) > maxLength)
            maxLength = strlen(list->data);
        list = list->next;
    }
    return maxLength;

}

//a function that makes all the strings in a LL the same length as the longest string so they can be compared properly in radix sort
void PadStrings(struct Node* list, int maxLength)
{
    while (list != NULL)
    {
        int currentLength = strlen(list->data);
        if (currentLength < maxLength)
        {
            // Calculate the number of spaces to add
            int spacesToAdd = maxLength - currentLength;

            // Append spaces to the string
            for (int i = 0; i < spacesToAdd; ++i)
            {
                strcat(list->data, " ");
            }
        }
        list = list->next;
    }
}

/*a function that pads a single word. had to use this because i faced a problem in the delete function, as the function deletes nodes
 with a data value that matches the value given by the user. all the strings in the linked list are priorly padded so the word given
 by the user to delete has to be padded as well before passing it to the delete function or else it wouldn't accurately match the
 string value in the linked list */
void PadWord(char word[],int maxLength)
{
    int currentLength = strlen(word);
    if (currentLength < maxLength)
    {
        // Calculate the number of spaces to add
        int spacesToAdd = maxLength - currentLength;

        // Append spaces to the string
        for (int i = 0; i < spacesToAdd; ++i)
        {
            strcat(word, " ");
        }
    }
}

//a function that reads strings from a file, pads all strings to be the same length and creates a doubly LL to store the strings
struct Node* readFile(char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Failed to open file.\n");
        exit(-1);
    }
    struct Node* FirstLL = NULL;
    char line[stringmax];
    //process each line and insert it to the linked list
    while (fgets(line, sizeof(line), file) != NULL)
    {
        // remove newline character at the end
        if (line[0] != '\0' && line[strlen(line) - 1] == '\n')
        {
            line[strlen(line) - 1] = '\0';
        }
        FirstLL = insertAtEnd(FirstLL,line);
    }
    PadStrings(FirstLL,LongestString(FirstLL));
    fclose(file);
    return FirstLL;
}

//a function to copy contents from an array of lists to a single doubly linked list. is to be used later in radix sort
struct Node* fromArrayToList(struct ArrayOfLists* array, int numLists)
{
    struct Node* result = NULL;

    for (int i = 0; i < numLists; ++i)
    {
        struct Node* current = array[i].head;

        while (current != NULL)
        {
            result = insertAtEnd(result, current->data);
            current = current->next;
        }
    }

    return result;
}


/*the radix sort function. simply implemented through a nested loop: an inner while loop that traverses the linked list and adds strings
from the linked list into the array of lists, an outer loop that goes through all the characters in each string starting from
the last character until theres no more characters to compare. with each iteration, the linked list and the array of
linked lists are emptied again to store the strings in a different order until they are completely sorted*/
struct Node* Sort(struct Node* list, struct ArrayOfLists* array, int numLists)
{
    // 'i' here represents the length of the longest string in the list, which is the number of times the outer loop will iterate
    for(int i=(LongestString(list)-1); i>=0; --i)
    {
        freeArrayOfLists(array, numLists);
        array = createArrayOfLists(numLists);
        while(list!=NULL)
        {
            addToEnd(array,(int)list->data[i],list->data);//add strings from LL to array at the index that matches ASCII value of the character
            list = list->next;
        }

        list = fromArrayToList(array,numlists);
    }
    return list;

}

//a function that prints the contents of a linked lists to an output file with a given name
void printToFile(struct Node* head, const char* fileName)
{
    FILE* outputFile = fopen(fileName, "w");
    if (outputFile == NULL)
    {
        printf("Failed to open output file.\n");
        exit(1);
    }

    struct Node* current = head;
    while (current != NULL)
    {
        fprintf(outputFile, "%s", current->data);
        fprintf(outputFile, " <-> ");
        current = current->next;
    }
    fprintf(outputFile, "NULL\n");

    fclose(outputFile);
}


// function to free the memory allocated for a doubly linked list
void freeList(struct Node* head)
{
    struct Node* current = head;
    while (current != NULL)
    {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
}

// function to free the memory allocated for an array of doubly linked lists
void freeArrayOfLists(struct ArrayOfLists* array, int numLists)
{
    for (int i = 0; i < numLists; ++i)
    {
        freeList(array[i].head);
    }
    free(array);
}

//a function to display the menu with different options of operation
void printMenu()
{
    printf("\nplease choose your desired operation\n\n");
    printf("2. print the strings pre sorting\n");
    printf("3. sort the strings\n");
    printf("4. print strings after sorting\n");
    printf("5. add a new word and sort it\n");
    printf("6. delete a word from the sorted strings\n");
    printf("7. save strings to output file\n");
    printf("8. exit\n");
}

//a function that scans and processes each choice entered by the user
void processChoices(int choice,struct Node* FirstLL,char filename[])
{
    struct ArrayOfLists* arr = createArrayOfLists(numlists);
    char word[stringmax];
    switch(choice)
    {
    case 2:
        printf("the strings before sorting are:\n");
        printList(FirstLL);
        break;

    case 3:

        FirstLL = Sort(FirstLL,arr,numlists);
        printf("strings sorted successfully!!\n");
        break;

    case 4:

        FirstLL = Sort(FirstLL,arr,numlists);
        printf("the strings after sorting are:\n");
        printList(FirstLL);
        break;

    case 5:


        printf("please type in the string you want to add below:\n");
        scanf("%s",&word);
        if(((int)word[0])>=48 && ((int)word[0])<=57) //if the ascii value of the first char of the entered string lies between 48-57, then its a number
           {
              printf("oops! your string cannot start with a number, try again\n");
           }
        else{
        PadWord(word,LongestString(FirstLL));
        FirstLL = insertAtEnd(FirstLL,word);
        FirstLL = Sort(FirstLL,arr,numlists);
        printf("here are the sorted strings with the new string:\n");
        printList(FirstLL);
        }
        break;

    case 6:
        printf("please type in the string you want deleted:\n");
        scanf("%s",&word);
        PadWord(word,LongestString(FirstLL));
        Delete(&FirstLL,word);
        FirstLL=Sort(FirstLL,arr,numlists);
        printf("the updated list is:\n");
        printList(FirstLL);
        break;

    case 7:
        printf("please type the name of the file you want store the strings inside:\n");
        scanf("%s",filename);
        FirstLL=Sort(FirstLL,arr,numlists);
        printToFile(FirstLL,filename);
        printf("successfully saved output!\n");
        break;
    case 8:
        printf("have a great day :)\n");
        break;
    }
}

    int main(){
    struct Node* FirstLL = NULL;
    char filename[stringmax];
    printf("hello!! welcome to my program\n\n");
    printf("click any number to load strings from a file\n");
    int whatever;
    scanf("%d",&whatever);
        printf("please type the name of the file you want to load from\n");
        scanf("%s",filename);
        FirstLL = readFile(filename);
        printf("strings loaded successfully!!\n");
    int choice;
    do {
        printMenu();
        scanf("%d",&choice);
        processChoices(choice,FirstLL,filename);
       }
    while (choice!=8);
    return 0;
    }
