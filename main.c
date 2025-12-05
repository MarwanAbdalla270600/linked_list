#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"

#define MAX_WORD_LEN 20

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

char *wordToString(LetterNode *letters)
{
    // Länge bestimmen
    int len = 0;
    LetterNode *c = letters;
    while (c != NULL)
    {
        len++;
        c = c->next;
    }

    // Buffer erzeugen
    char *str = malloc(len + 1);

    // Füllen
    c = letters;
    for (int i = 0; i < len; i++)
    {
        str[i] = c->letter;
        c = c->next;
    }

    str[len] = '\0';
    return str;
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

int compareAlphabetical(LetterNode *a, LetterNode *b)
{
    char *s1 = wordToString(a);
    char *s2 = wordToString(b);

    // Beide Wörter lowercase machen
    for (int i = 0; s1[i]; i++)
        s1[i] = tolower(s1[i]);
    for (int i = 0; s2[i]; i++)
        s2[i] = tolower(s2[i]);

    int result = strcmp(s1, s2);

    return result;
}

int compareLength(LetterNode *a, LetterNode *b)
{
    int lenA = 0, lenB = 0;

    LetterNode *c = a;
    while (c)
    {
        lenA++;
        c = c->next;
    }

    c = b;
    while (c)
    {
        lenB++;
        c = c->next;
    }

    return lenA - lenB;
}

void sort(WordNode *list, int (*compare)(LetterNode *, LetterNode *), int descending)
{
    if (list == NULL)
        return;

    int swapped;

    do
    {
        swapped = 0;
        WordNode *current = list;

        while (current->next != NULL)
        {
            // Compare über externe Funktion
            int cmp = compare(current->word, current->next->word);

            // ascending → cmp > 0 bedeutet: tauschen
            // descending → cmp < 0 bedeutet: tauschen
            int mustSwap = descending ? (cmp < 0) : (cmp > 0);

            if (mustSwap)
            {
                LetterNode *tmp = current->word;
                current->word = current->next->word;
                current->next->word = tmp;
                swapped = 1;
            }

            current = current->next;
        }
    } while (swapped);
}

// Liest die Benutzeraktion ein (a, i, d, s, p, x)
char getMenu()
{
    char input[10];

    while (1)
    {
        if (scanf(" %9s", input) == 1 && strlen(input) == 1)
        {
            char c = input[0];

            if (c == 'a' || c == 'i' || c == 'd' || c == 's' || c == 'p' || c == 'x')
                return c;
        }

        printf("Input invalid! Try again:\n");
    }
}

// Fragt Benutzer nach Textnachricht
void getWord(char *msg)
{
    printf("Enter Word: ");
    scanf(" %31s", msg);
}

int getIndex()
{
    printf("Enter Index: ");
    char input[10];
    scanf(" %9s", input);
    return atoi(input);
}

int getAscOrDesc()
{
    printf("Sort (a)scending or (d)escending? ");
    char input[10];
    scanf(" %9s", input);
    char c = input[0];
    if (c == 'a')
    {
        return 0;
    }
    else if (c == 'd')
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

int getAlphOrLen()
{
    printf("Sort (a)lphabetically or (l)ength? ");
    char input[10];
    scanf(" %9s", input);
    char c = input[0];
    if (c == 'a')
    {
        return 0;
    }
    else if (c == 'l')
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

int main()
{
    WordNode *wordList = NULL;

    while (1)
    {
        printf("\nChoose action: (a)ppend, (i)nsert, (d)elete, (s)ort, (p)rint, e(x)it\n");

        char action = getMenu();
        char input[MAX_WORD_LEN + 1];

        switch (action)
        {
        case 'a':
            getWord(input);
            appendWord(&wordList, input);
            break;

        case 'i':
        {
            int index = getIndex();
            getWord(input);
            insertWord(&wordList, input, index);
            break;
        }

        case 'd':
        {
            int index = getIndex();
            deleteWord(&wordList, index);
            break;
        }

        case 's':
        {
            int alphOrLen = getAlphOrLen();
            int ascOrDesc = getAscOrDesc();

            if (alphOrLen == 0)
                sort(wordList, compareAlphabetical, ascOrDesc);
            else
                sort(wordList, compareLength, ascOrDesc);

            break;
        }

        case 'p':
            printWordList(wordList);
            break;

        case 'x':
            printf("Bye!\n");
            exit(0);
            break;
        }
    }

    return 0;
}
