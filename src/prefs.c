#include "prefs.h"

static PlayerPrefs* prefs;

PlayerPrefs* PrefsInit() {
    prefs->count = 0;
    return prefs;
}

int PrefsSet(PlayerPrefs *prefs, const char *key, int value) {
    if (!prefs || !key || key[0] == '\0') return 0;

    int idx = find_entry_index(prefs, key);
    if (idx >= 0) {
        prefs->entries[idx].value = value;
        return 1;
    }

    if (prefs->count >= PREFS_MAX_ENTRIES) {
        return 0; // no space left
    }

    strncpy(prefs->entries[prefs->count].key, key, PREFS_KEY_MAX_LEN - 1);
    prefs->entries[prefs->count].key[PREFS_KEY_MAX_LEN - 1] = '\0';
    prefs->entries[prefs->count].value = value;
    prefs->count++;
    return 1;
}

int PrefsGet(const PlayerPrefs *prefs, const char *key, int *outValue) {
    if (!prefs || !key || !outValue) return 0;

    int idx = find_entry_index(prefs, key);
    if (idx < 0) return 0;

    *outValue = prefs->entries[idx].value;
    return 1;
}

int PrefsSave(const PlayerPrefs *prefs, const char *path) {
    if (!prefs || !path) return 0;

    FILE *f = fopen(path, "w");
    if (!f) return 0;

    for (size_t i = 0; i < prefs->count; ++i) {
        fprintf(f, "%s %d\n", prefs->entries[i].key, prefs->entries[i].value);
    }

    fclose(f);
    return 1;
}

int PrefsLoad(PlayerPrefs *prefs, const char *path) {
    if (!prefs || !path) return 0;

    FILE *f = fopen(path, "r");
    if (!f) return 0;

    prefs->count = 0;

    char line[128];
    while (fgets(line, sizeof(line), f)) {
        trim_trailing_whitespace(line);
        if (line[0] == '\0' || line[0] == '#') continue;

        char key[PREFS_KEY_MAX_LEN];
        int value;
        if (sscanf(line, "%31s %d", key, &value) == 2) {
            PrefsSet(prefs, key, value);
        }
    }

    fclose(f);
    return 1;
}

static int find_entry_index(const PlayerPrefs *prefs, const char *key) {
    for (size_t i = 0; i < prefs->count; ++i) {
        if (strcmp(prefs->entries[i].key, key) == 0) {
            return (int)i;
        }
    }
    return -1;
}

static void trim_trailing_whitespace(char *str) {
    size_t len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[len - 1] = '\0';
        len--;
    }
}