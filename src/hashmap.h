#ifndef PNP_HASHMAP_H_
#define PNP_HASHMAP_H_

#include <stdlib.h>

#define String_Hashmap_Size 4096

typedef struct String_Hashmap_Entry {
	char *key;
	char *value;
	struct String_Hashmap_Entry *next;
} String_Hashmap_Entry;

typedef struct String_Hashmap {
	String_Hashmap_Entry *table[String_Hashmap_Size];
} String_Hashmap;

unsigned int String_Hash(char *);
void String_Hashmap_Insert(String_Hashmap *, char *, char *);
int String_Hashmap_Find(String_Hashmap *, char *);
char *String_Hashmap_Lookup(String_Hashmap *, char *);

// [sdbm hash](http://www.cse.yorku.ca/~oz/hash.html#sdbm)
unsigned int String_Hash(char *key) {
	unsigned long long hash = 0;
	for (unsigned int i = 0; i < strlen(key); ++i)
		hash = key[i] + (hash<<6) + (hash<<16) - hash;
	return hash;
}

void String_Hashmap_Insert(String_Hashmap *hashmap, char *key, char *value) {
	unsigned int hash = String_Hash(key)%String_Hashmap_Size;
	String_Hashmap_Entry *entry = (String_Hashmap_Entry *)malloc(sizeof(String_Hashmap_Entry));
    if (!entry) {
	    printf("error: hashmap entry allocation failed\n");
		exit(EXIT_FAILURE);
	}
    entry->key = strdup(key);
    entry->value = strdup(value);
	entry->next = hashmap->table[hash];
    hashmap->table[hash] = entry;
}

int String_Hashmap_Find(String_Hashmap *hashmap, char *key) {
	unsigned int hash = String_Hash(key)%String_Hashmap_Size;
	String_Hashmap_Entry *itr = hashmap->table[hash];
	while (itr) {
		if (strcmp(itr->key, key) == 0) return 1;
		itr = itr->next;
	}
	return 0;
}

char *String_Hashmap_Lookup(String_Hashmap *hashmap, char *key) {
	unsigned int hash = String_Hash(key)%String_Hashmap_Size;
	String_Hashmap_Entry *itr = hashmap->table[hash];
	while (itr) {
		if (strcmp(itr->key, key) == 0) return strdup(itr->value);
		itr = itr->next;
	}
	return NULL;
}

#endif // PNP_HASHMAP_H_
