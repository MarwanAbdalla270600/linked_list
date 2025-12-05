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
    struct Wordnode *next;
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
    printf("\n");
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

int main()
{
    char *hello = "HalloWelt";
    LetterNode *letterList = createLetterListFromString(hello);
    printLetterList(letterList);
    return 0;
}