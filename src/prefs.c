#include "prefs.h"

static PlayerPrefs* tPrefs;

static int find_entry_index(const char *key) {
    for (size_t i = 0; i < tPrefs->count; ++i) {
        if (strcmp(tPrefs->entries[i].key, key) == 0) {
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

PlayerPrefs* PrefsInit() {
    tPrefs = (PlayerPrefs*)RL_MALLOC(sizeof(PlayerPrefs));
    tPrefs->count = 0;
    tPrefs->dirty = true;
    return tPrefs;
}

int PrefsSet(const char *key, int value) {
    if (!tPrefs || !key || key[0] == '\0') return 0;

    int idx = find_entry_index(key);
    if (idx >= 0) {
        tPrefs->entries[idx].value = value;
        return 1;
    }

    if (tPrefs->count >= PREFS_MAX_ENTRIES) {
        return 0; // no space left
    }

    strncpy(tPrefs->entries[tPrefs->count].key, key, PREFS_KEY_MAX_LEN - 1);
    tPrefs->entries[tPrefs->count].key[PREFS_KEY_MAX_LEN - 1] = '\0';
    tPrefs->entries[tPrefs->count].value = value;
    tPrefs->count++;
    tPrefs->dirty = true;
    return 1;
}

int PrefsGet(const char *key, int *outValue) {
    if (!tPrefs || !key || !outValue) return 0;

    int idx = find_entry_index(key);
    if (idx < 0) return 0;

    *outValue = tPrefs->entries[idx].value;
    return 1;
}

int PrefsSave(const char *path) {
    if (!tPrefs || !path) return 0;

    FILE *f = fopen(path, "w");
    if (!f) return 0;

    for (size_t i = 0; i < tPrefs->count; ++i) {
        fprintf(f, "%s %d\n", tPrefs->entries[i].key, tPrefs->entries[i].value);
    }

    fclose(f);
    return 1;
}

int PrefsLoad(const char *path) {
    if (!tPrefs || !path) return 0;

    FILE *f = fopen(path, "r");
    if (!f) return 0;

    tPrefs->count = 0;

    char line[128];
    while (fgets(line, sizeof(line), f)) {
        trim_trailing_whitespace(line);
        if (line[0] == '\0' || line[0] == '#') continue;

        char key[PREFS_KEY_MAX_LEN];
        int value;
        if (sscanf(line, "%31s %d", key, &value) == 2) {
            PrefsSet(key, value);
        }
    }

    tPrefs->dirty = true;

    fclose(f);
    return 1;
}