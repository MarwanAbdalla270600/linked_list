#include "stdio.h"
#include "stdlib.h"

typedef struct letternode
{
    char letter;
    struct letternode *next;
} LetterNode;

typedef struct wordnode
{
    LetterNode *word;
    struct wordnode *next;
} WordNode;

LetterNode *newLetterNode(char ch)
{
    LetterNode *node = malloc(sizeof(LetterNode));
    node->letter = ch;
    node->next = NULL;
    return node;
}

WordNode *newWordNode(LetterNode *letters)
{
    WordNode *node = malloc(sizeof(WordNode));
    node->word = letters;
    node->next = NULL;
    return node;
}

void printLetterList(LetterNode *list)
{
    LetterNode *current = list;
    while (current != NULL)
    {
        printf("%c", current->letter);
        current = current->next;
    }
}

LetterNode *createLetterListFromString(char *word)
{
    if (word == NULL || word[0] == '\0')
        return NULL;
    LetterNode *head = newLetterNode(word[0]);
    LetterNode *current = head;

    for (int i = 1; word[i] != '\0'; i++)
    {
        current->next = newLetterNode(word[i]);
        current = current->next;
    }
    return head;
}

void printWordList(WordNode *list)
{
    WordNode *current = list;
    while (current != NULL)
    {
        printLetterList(current->word);
        printf(" ");
        current = current->next;
    }
    printf("\n");
}

void appendWord(WordNode **list, char *word)
{
    LetterNode *letters = createLetterListFromString(word);
    WordNode *newNode = newWordNode(letters);

    // Fall 1: Liste ist leer → neuer Head
    if (*list == NULL)
    {
        *list = newNode;
        return;
    }

    // Fall 2: ans Ende laufen
    WordNode *current = *list;
    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = newNode;
}

int insertWord(WordNode **list, char *word, int index)
{
    if (index < 0)
        return -1;

    LetterNode *letters = createLetterListFromString(word);
    WordNode *newNode = newWordNode(letters);

    // Insert at head
    if (index == 0)
    {
        newNode->next = *list;
        *list = newNode;
        return 0;
    }

    WordNode *current = *list;

    // Run to the node BEFORE insertion point
    for (int i = 0; i < index - 1; i++)
    {
        if (current == NULL) // list too short
            return -1;

        current = current->next;
    }

    if (current == NULL) // DON'T TOUCH NULL
        return -1;

    // Insert
    newNode->next = current->next;
    current->next = newNode;

    return 0;
}

int deleteWord(WordNode **list, int index)
{
    if (*list == NULL || index < 0)
        return -1;

    // Fall 1: Kopf löschen
    if (index == 0)
    {
        *list = (*list)->next;
        return 0;
    }

    WordNode *current = *list;

    // Zu Node vor der zu löschenden Position
    for (int i = 0; i < index - 1; i++)
    {
        if (current == NULL || current->next == NULL)
            return -1; // Index out of bounds

        current = current->next;
    }

    // current->next ist der zu löschende Node
    if (current->next == NULL)
        return -1;

    // Verkettung überspringen
    current->next = current->next->next;

    return 0;
}




int main()
{

    WordNode *wordList = NULL;
    appendWord(&wordList, "Hallo");
    appendWord(&wordList, "mein");
    appendWord(&wordList, "name");
    appendWord(&wordList, "ist");
    appendWord(&wordList, "Momo");

    insertWord(&wordList, "marwan", 6);

    printWordList(wordList);
    return 0;
}