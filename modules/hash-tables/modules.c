#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash-tables.h"
int MAXSIZE = 10;


HashTable* HTCreate(int dataSize, bool isString)
{
    HashTable* table = malloc(sizeof(HashTable));
    table->size = 0;
    table->dataSize = dataSize;
    table->isString = isString;
    table->maxSize = MAXSIZE;
    table->array = malloc(table->maxSize*sizeof(HTNode*));
    for(int i=0; i<table->maxSize; i++)
    {
        table->array[i] = NULL;
    }
    return table;
}

int HTSize(HashTable* hash)
{
    return hash->size;
}

bool HTGet(HashTable* hash, char* key, HTItem* pitem)
{
    HTNode* temp;

    for(int i=0; i<hash->maxSize; i++)
    {
        if(hash->array[i] != NULL)
        {
            temp = hash->array[i];
            while(temp != NULL)
            {
                if(strcmp(temp->key, key)==0)
                {
                    pitem = &temp->item;
                    return true;
                }
                temp = temp->next;    
            }
        }
    }
    return false;
}

int HashFunction(char* key, int max)
{
    return *key%max;
}

void RecursiveInsertAndDelete(HTNode* node, HashTable* hash)
{
    if(node == NULL)
        return;

    RecursiveInsertAndDelete(node->next, hash);
    HTInsert(hash, node->key, node->item);
    free(node);
}

HashTable* reHashing(HashTable* hash)
{
    HTNode* list;
    MAXSIZE = MAXSIZE*2;
    HashTable* table = HTCreate(hash->dataSize ,hash->isString);

    printf("Rehashing the hash due to number of nodes \n");
    
    for(int i=0; i<MAXSIZE/2; i++)
    {
        list = hash->array[i];
        RecursiveInsertAndDelete(list, table);
    } 

    return table;
}

HashTable* HTInsert(HashTable* hash, char* key, HTItem item)
{
    HTNode* temp;
    HTNode* node = malloc(sizeof(HTNode));

    node->key = malloc(sizeof(char*));
    strcpy(node->key, key);
    node->item = item;
    node->next = NULL;

    int index = HashFunction(key, hash->maxSize);
    if(hash->array[index] == NULL)
    {
        hash->array[index] = node;
        hash->size++;
    }
    else
    {
        temp = hash->array[index];
        while(temp->next != NULL)
        {
            if(temp->key == key)
            {
                temp->item = node->item;
                node->key = NULL;
                hash->size++;
            }
            temp = temp->next;
        }
        if(node->key != NULL)
        {
            temp->next = node;
            hash->size++;
        }
    }
    if(hash->size/hash->maxSize > 0.9)
        hash = reHashing(hash);

    return hash;       
}

void HTRemove(HashTable* hash, char* key)
{
    HTNode* slow;
    HTNode* fast;

    int index = HashFunction(key, hash->maxSize);

    fast = hash->array[index];

     while(strcmp(fast->key ,key) != 0)
    {
        slow = fast;
        fast = fast->next;
    }
    if(fast == hash->array[index])
    {
        if(fast->next == NULL)
            hash->array[index] = NULL;
        else
            hash->array[index] = fast->next;
    }
    else
        slow->next = fast->next;
 
    hash->size--;
    free(fast);
}

void HTVisit(HashTable* hash, void (*visit)(HashTable* hash, char* key, HTItem pitem))
{
    HTNode* temp;
 
    for(int i=0; i<hash->maxSize; i++)
    {
        if(hash->array[i] != NULL)
        {
            temp = hash->array[i];
            while(temp != NULL)
            {
                visit(hash, temp->key, temp->item);
                temp = temp->next;    
            }
        }
    }
}

void destroy(HTNode* node)
{
    if(node == NULL)
        return;
 
    destroy(node->next);   
    free(node);
}


void HTDestroy(HashTable* hash)
{
    HTNode* list;

    for(int i=0; i<hash->maxSize; i++)
    {
        list = hash->array[i];
        if(list!=NULL)
          destroy(list);
    }
    free(hash->array);
    free(hash);
}


void printht(HashTable* hash) {
    HTNode* temp;

    for(int i=0; i<MAXSIZE; i++) {
         temp = hash->array[i];
         if(temp != NULL)
         {
            printf("%s -->", temp->key);
            if(temp->next != NULL) {
                while(temp->next != NULL) {
                    temp = temp->next;
                    printf("%s -->", temp->key);
                }
            }
            printf("\n");
         }
    }
}


void printstring(HashTable* hash) {
    HTNode* temp;

    for(int i=0; i<MAXSIZE; i++) {
         temp = hash->array[i];
         if(temp != NULL)
         {
            printf("%s -->", (char*)temp->item);
            if(temp->next != NULL) {
                while(temp->next != NULL) {
                    temp = temp->next;
                    printf("%s -->", (char*)temp->item);
                }
            }
            printf("\n");
         }
    }
}
