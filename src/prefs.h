#pragma once
#ifndef PREFS_H
#define PREFS_H

#include "includes.h"

#define PREFS_MAX_ENTRIES 64
#define PREFS_KEY_MAX_LEN 32

typedef struct {
    char key[PREFS_KEY_MAX_LEN];
    int value;
} PrefEntry;

typedef struct {
    PrefEntry entries[PREFS_MAX_ENTRIES];
    size_t count;
    bool dirty;
} PlayerPrefs;

PlayerPrefs* PrefsInit();
int PrefsSet(const char *key, int value);
int PrefsGet(const char *key, int *outValue);
int PrefsSave(const char *path);
int PrefsLoad(const char *path);
int ApplyPreferences(void);

#endif