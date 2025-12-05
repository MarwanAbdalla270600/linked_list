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
    WordNode *node = malloc(sizeof(WordNode) + sizeof(LetterNode));
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
        printf(" -> ");
        current = current->next;
    }
    printf("\n");
}

void appendWord(WordNode *list, char *word)
{
    WordNode *current = list;

    while (current->next != NULL)
    {
        current = current->next;
    }

    LetterNode *letterList = createLetterListFromString(word);
    WordNode *wordNode = newWordNode(letterList);
    current->next = wordNode;
}

int insertWord(WordNode *list, char *word, int index)
{
    // 0. Sonderfall: Einfügen an Index 0
    if (index == 0)
        return -1; // oder: neuen head zurückgeben

    WordNode *current = list;

    // 1. Zum Node VOR dem Einfügepunkt laufen
    for (int i = 0; i < index - 1; i++)
    {
        if (current == NULL)
            return -1; // Index out of bounds

        current = current->next;
    }

    // 2. Neuen WordNode erstellen
    LetterNode *letters = createLetterListFromString(word);
    WordNode *newNode = newWordNode(letters);

    // 3. Verkettung richtig setzen
    newNode->next = current->next;
    current->next = newNode;

    return 0;
}

int main()
{
    char *hello = "Hallo";
    LetterNode *letterList = createLetterListFromString(hello);
    WordNode *wordList = newWordNode(letterList);
    appendWord(wordList, "mein");
    appendWord(wordList, "name");
    appendWord(wordList, "ist");
    appendWord(wordList, "Momo");
    insertWord(wordList, "zweiter", 3);

    printWordList(wordList);
    return 0;
}