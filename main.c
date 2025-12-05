#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 50

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

void freeLetterList(LetterNode *list)
{
    while (list)
    {
        LetterNode *tmp = list;
        list = list->next;
        free(tmp);
    }
}

void freeWordList(WordNode *list)
{
    while (list)
    {
        WordNode *tmp = list;
        list = list->next;
        freeLetterList(tmp->word);
        free(tmp);
    }
}

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
    int len = 0;
    LetterNode *c = letters;

    while (c)
    {
        len++;
        c = c->next;
    }

    char *str = malloc(len + 1);
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
    while (list)
    {
        printf("%c", list->letter);
        list = list->next;
    }
}

void printWordList(WordNode *list)
{
    while (list)
    {
        printLetterList(list->word);
        printf(" ");
        list = list->next;
    }
    printf("\n");
}

LetterNode *createLetterListFromString(char *word)
{
    if (!word || word[0] == '\0')
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

void appendWord(WordNode **list, char *word)
{
    LetterNode *letters = createLetterListFromString(word);
    WordNode *newNode = newWordNode(letters);

    if (*list == NULL)
    {
        *list = newNode;
        return;
    }

    WordNode *current = *list;
    while (current->next)
        current = current->next;

    current->next = newNode;
}

int insertWord(WordNode **list, char *word, int index)
{
    if (index < 0)
        return -1;

    LetterNode *letters = createLetterListFromString(word);
    WordNode *newNode = newWordNode(letters);

    if (index == 0)
    {
        newNode->next = *list;
        *list = newNode;
        return 0;
    }

    WordNode *current = *list;
    for (int i = 0; i < index - 1; i++)
    {
        if (!current)
            return -1;
        current = current->next;
    }

    if (!current)
        return -1;

    newNode->next = current->next;
    current->next = newNode;

    return 0;
}

int deleteWord(WordNode **list, int index)
{
    if (!*list || index < 0)
        return -1;

    if (index == 0)
    {
        WordNode *tmp = *list;
        *list = (*list)->next;
        freeLetterList(tmp->word);
        free(tmp);
        return 0;
    }

    WordNode *current = *list;
    for (int i = 0; i < index - 1; i++)
    {
        if (!current || !current->next)
            return -1;
        current = current->next;
    }

    if (!current->next)
        return -1;

    WordNode *toDelete = current->next;
    current->next = toDelete->next;

    freeLetterList(toDelete->word);
    free(toDelete);

    return 0;
}

int compareAlphabetical(LetterNode *a, LetterNode *b)
{
    char *s1 = wordToString(a);
    char *s2 = wordToString(b);

    for (int i = 0; s1[i]; i++)
        s1[i] = tolower(s1[i]);
    for (int i = 0; s2[i]; i++)
        s2[i] = tolower(s2[i]);

    int result = strcmp(s1, s2);

    free(s1);
    free(s2);

    return result;
}

int compareLength(LetterNode *a, LetterNode *b)
{
    int lenA = 0, lenB = 0;

    while (a)
    {
        lenA++;
        a = a->next;
    }
    while (b)
    {
        lenB++;
        b = b->next;
    }

    return lenA - lenB;
}

void sort(WordNode *list, int (*compare)(LetterNode *, LetterNode *), int descending)
{
    if (!list)
        return;

    int swapped;
    do
    {
        swapped = 0;
        WordNode *current = list;

        while (current->next)
        {
            int cmp = compare(current->word, current->next->word);

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
        printf("Invalid input, try again:\n");
    }
}

void getWord(char *msg)
{
    printf("Enter word: ");
    scanf(" %49s", msg);
}

int getIndex()
{
    char input[10];
    printf("Enter index: ");
    scanf(" %9s", input);
    return atoi(input);
}

int getAscOrDesc()
{
    char input[10];
    printf("Sort (a)scending or (d)escending? ");
    scanf(" %9s", input);

    if (input[0] == 'a')
        return 0;
    if (input[0] == 'd')
        return 1;
    return -1;
}

int getAlphOrLen()
{
    char input[10];
    printf("Sort (a)lphabetically or (l)ength? ");
    scanf(" %9s", input);

    if (input[0] == 'a')
        return 0;
    if (input[0] == 'l')
        return 1;
    return -1;
}

int main()
{
    WordNode *wordList = NULL;

    while (1)
    {
        printf("\nChoose action: (a)ppend, (i)nsert, (d)elete, (s)ort, (p)rint, e(x)it\n");

        char action = getMenu();
        char input[MAX_WORD_LEN];

        switch (action)
        {
        case 'a':
            getWord(input);
            appendWord(&wordList, input);
            break;

        case 'i':
        {
            int idx = getIndex();
            getWord(input);
            insertWord(&wordList, input, idx);
            break;
        }

        case 'd':
        {
            int idx = getIndex();
            deleteWord(&wordList, idx);
            break;
        }

        case 's':
        {
            int type = getAlphOrLen();
            int order = getAscOrDesc();

            if (type == 0)
                sort(wordList, compareAlphabetical, order);
            else
                sort(wordList, compareLength, order);
            break;
        }

        case 'p':
            printWordList(wordList);
            break;

        case 'x':
            freeWordList(wordList);
            printf("Goodbye!\n");
            return 0;
        }
    }
}
