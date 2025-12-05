#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    if (!list)
    {
        printf("empty list");
        return;
    }

    while (list)
    {
        printLetterList(list->word);
        printf(" ");
        list = list->next;
    }
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

int getListLength(WordNode *list)
{
    int len = 0;
    while (list)
    {
        len++;
        list = list->next;
    }
    return len;
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

    WordNode *toDelete = current->next;
    if (!toDelete)
        return -1;

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

    int res = strcmp(s1, s2);

    free(s1);
    free(s2);

    return res;
}

int compare_length(LetterNode *a, LetterNode *b)
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
        scanf(" %9s", input);

        if (strlen(input) == 1)
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
    printf("Enter Word: ");
    scanf(" %49s", msg);
}

int getInsertIndex(WordNode *list)
{
    char input[10];
    printf("Enter Index:");
    scanf(" %9s", input);

    for (int i = 0; input[i]; i++)
        if (!isdigit((unsigned char)input[i]))
            return -1;

    int index = atoi(input);
    int length = getListLength(list);

    if (index < 0 || index > length)
        return -1;

    return index;
}

int getDeleteIndex(WordNode *list)
{
    char input[10];
    printf("Enter Index:\n");
    scanf(" %9s", input);

    for (int i = 0; input[i]; i++)
        if (!isdigit((unsigned char)input[i]))
            return -1;

    int index = atoi(input);
    int length = getListLength(list);

    if (index < 0 || index >= length)
        return -1;

    return index;
}

int getAscOrDesc()
{
    char input[10];
    printf("Sort (a)scending or (d)escending?");
    scanf(" %9s", input);

    return (input[0] == 'd') ? 1 : 0;
}

int getAlphOrLen()
{
    char input[10];
    printf("Sort (a)lphabetically or by (l)ength?");
    scanf(" %9s", input);

    return (input[0] == 'l') ? 1 : 0;
}

int main()
{
    WordNode *wordList = NULL;

    while (1)
    {
        printf("\nChoose action: (a)ppend, (i)nsert, (d)elete, (s)ort, (p)rint, e(x)it:\n");

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
            int idx = getInsertIndex(wordList);
            if (idx == -1)
            {
                printf("Invalid index\n");
                break;
            }
            getWord(input);
            insertWord(&wordList, input, idx);
            break;
        }

        case 'd':
        {
            int idx = getDeleteIndex(wordList);
            if (idx == -1)
            {
                printf("Invalid index\n");
                break;
            }
            deleteWord(&wordList, idx);
            break;
        }

        case 's':
        {
            int type = getAlphOrLen();
            int order = getAscOrDesc();
            sort(wordList, type == 0 ? compareAlphabetical : compare_length, order);
            break;
        }

        case 'p':
            printWordList(wordList);
            break;

        case 'x':
            freeWordList(wordList);
            return 0;
        }
    }
}
