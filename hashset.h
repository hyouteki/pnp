#ifndef PNP_HASHSET_H_
#define PNP_HASHSET_H_

#include <stdlib.h>

#define String_Hashset_Size 4096

typedef struct String_Hashset_Entry {
	char *key;
	struct String_Hashset_Entry *next;
} String_Hashset_Entry;

typedef struct String_Hashset {
	String_Hashset_Entry *table[String_Hashset_Size];
} String_Hashset;

unsigned int String_Hash(char *);
void String_Hashset_Insert(String_Hashset *, char *);
int String_Hashset_Find(String_Hashset *, char *);

// [sdbm hash](http://www.cse.yorku.ca/~oz/hash.html#sdbm)
unsigned int String_Hash(char *key) {
	unsigned long long hash = 0;
	for (unsigned int i = 0; i < strlen(key); ++i)
		hash = key[i] + (hash<<6) + (hash<<16) - hash;
	return hash;
}

void String_Hashset_Insert(String_Hashset *hashset, char *key) {
	unsigned int hash = String_Hash(key)%String_Hashset_Size;
	String_Hashset_Entry *entry = (String_Hashset_Entry *)malloc(sizeof(String_Hashset_Entry));
    if (!entry) {
	    printf("error: hashset entry allocation failed\n");
		exit(EXIT_FAILURE);
	}
    entry->key = strdup(key);
	entry->next = hashset->table[hash];
    hashset->table[hash] = entry;
}

int String_Hashset_Find(String_Hashset *hashset, char *key) {
	unsigned int hash = String_Hash(key)%String_Hashset_Size;
	String_Hashset_Entry *itr = hashset->table[hash];
	while (itr) {
		if (strcmp(itr->key, key) == 0) return 1;
		itr = itr->next;
	}
	return 0;
}

#endif // PNP_HASHSET_H_
