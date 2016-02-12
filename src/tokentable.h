#ifndef TOKENTABLE_H
#define TOKENTABLE_H

#define TABLESIZE 20

struct TokenEntry {
    char* name;
    struct Token* token;
    struct TokenEntry* next;
};

struct TokenTable {
    struct TokenEntry* table[TABLESIZE];
    struct TokenTable* subtable;
};

#endif
