#include "p.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>

#define TAILLE 10
#define SIZE_COMMIT 10

// 6.1
kvp *createKeyVal(char *key, char *val) {
  kvp *new = (kvp *)malloc(sizeof(kvp));
  if (!val && !key)
    return NULL;
  new->key = key ? strdup(key) : NULL;
  new->value = val ? strdup(val) : NULL;
  return new;
}

void freeKeyVal(kvp *kv) {
  if (kv) {
    free(kv->key);
    free(kv->value);
  }
  free(kv);
}

// 6.2
char *kvts(kvp *k) {
  if (!k)
    return NULL;
  char *str = (char *)malloc(100 * sizeof(char));
  snprintf(str, 100, "%s : %s", k->key, k->value);
  return str;
}

kvp *stkv(char *str) {
  if (!str)
    return NULL;
  char key[256], val[256];
  sscanf(str, "%s : %s", key, val);
  kvp *new_kv = createKeyVal(key, val);
  return new_kv;
}

// 6.3
Commit *initCommit() {
  Commit *commit = (Commit *)malloc(sizeof(Commit));
  commit->n = 0;
  commit->size = SIZE_COMMIT;
  commit->T = (kvp **)malloc(SIZE_COMMIT * sizeof(kvp *));
  for (int i = 0; i < commit->size; i++) {
    commit->T[i] = NULL;
  }
  return commit;
}

// 6.4
static unsigned long sdbm(unsigned char *str) {
  unsigned long hash = 0;
  int c;
  while ((c = *(str++)))
    hash = c + (hash << 6) + (hash << 16) - hash;
  return hash;
}
// 6.5
void commitSet(Commit *c, char *key, char *value) {
  unsigned long hash = sdbm((unsigned char *)key) % c->size;
  if (c->n == c->size) {
    printf("Le commit est saturé\n");
    return;
  }
  while (c->T[hash]) {
    hash = (hash + 1) % c->size;
  }
  c->T[hash] = createKeyVal(key, value);
  (c->n)++;
}

// 6.6
Commit *createCommit(char *hash) {
  Commit *commit = initCommit();
  commitSet(commit, "tree", hash);
  return commit;
}

// 6.7
char *commitGet(Commit *c, char *key) {
  int i = sdbm((unsigned char *)key) % c->size;
  int cpt = 0;
  while (c->T[i] && cpt < c->size) {
    if (strcmp(c->T[i]->key, key) == 0)
      return c->T[i]->value;
    i = (i + 1) % c->size;
    cpt++;
  }
  return NULL;
}

void freeCommit(Commit *c) {
  int i = 0;
  while (i < c->size) {
    if (c->T[i])
      freeKeyVal(c->T[i]);
    i++;
  }
  free(c->T);
  free(c);
}

// 6.8
char *cts(Commit *c) {
  char *s = (char *)malloc(c->n * 100 * sizeof(char));
  s[0] = '\0';
  for (int i = 0; i < c->size; i++) {
    if (!(c->T[i]))
      continue;
    char *si = kvts(c->T[i]);
    strcat(s, si);
    strcat(s, "\n");
    free(si);
  }
  s[strlen(s) - 1] = '\0';
  return s;
}

Commit *stc(char *ch) {
  Commit *c = initCommit();
  char *token = strtok(ch, "\n");
  while (token) {
    kvp *tmp = stkv(token);
    if (tmp) {
      commitSet(c, tmp->key, tmp->value);
      freeKeyVal(tmp);
    }
    token = strtok(NULL, "\n");
  }
  return c;
}

// 6.9
void ctf(Commit *c, char *file) {
  FILE *f = fopen(file, "w");
  if (!f) {
    printf("Erreur (fonction ctf): Impossible d'ouvrir %s\n", file);
    return;
  }
  char *s = cts(c);
  // printf("dans ctf s est %s\n", s);
  fputs(s, f);
  fclose(f);
  free(s);
}

Commit *ftc(char *file) {
  FILE *f = fopen(file, "r");
  if (!f) {
    printf("Erreur (fonction ftc): Impossible d'ouvrir  %s\n", file);
    return NULL;
  }
  char buff[100]; // on stock dedans chaque ligne
  char s[1000];   // on stock dedans le tout pour apres le transformer en commit
  s[0] = '\0';
  while (fgets(buff, 100, f)) {
    strcat(s, buff);
    
  }
  fclose(f);
  Commit *c = stc(s);
  return c;
}

// 6.10
char *blobCommit(Commit *c) {
  char fname[100] = "/tmp/myfileXXXXXX";
  int fd = mkstemp(fname);
  ctf(c, fname);
  char *hash = sha256file(fname);
  char *ch = hashToFile(hash);
  strcat(ch, ".c");
  cp(ch, fname);
  free(ch);
  return hash;
}

// 7.1
void initRefs() {
  if (!fileExists(".refs")) {
    system("mkdir .refs");
    FILE *f = fopen(".refs/master", "w");
    fclose(f);
    FILE *f1 = fopen(".refs/HEAD", "w");
    fclose(f1);
    // OR : system("truncate -s 0 .refs/master");
    // OR : system("truncate -s 0 .refs/HEAD");
  }
}

// 7.2
void createUpdateRef(char *ref_name, char *hash) {
  char buff[256];
  sprintf(buff, ".refs/%s", ref_name);
  if (!fileExists(buff)) {
    sprintf(buff, ".refs/%s", ref_name);
    FILE *f = fopen(buff, "w");
    fclose(f);
  }
  char h[100];
  if (hash) {
    strcpy(h, hash);
  } else {
    strcpy(h, "");
  }
  sprintf(buff, "echo %s > .refs/%s", h, ref_name);
  system(buff);
}

// 7.3
void deleteRef(char *ref_name) {
  char buff[256];
  sprintf(buff, ".refs/%s", ref_name);
  if (!fileExists(buff)) {
    printf("Erreur (fonction deleteRef): %s n'existe pas", ref_name);
  } else {
    sprintf(buff, "rm .refs/%s", ref_name);
    system(buff);
  }
}

//7.4
char *getRef(char *ref_name) {

  char path[256];
  sprintf(path, ".refs/%s", ref_name);
  if (!fileExists(path)) {
    printf("Erreur (fonction getRef): %s n'existe pas\n", ref_name);
    return NULL;
  }

  char *buff = (char *)malloc(sizeof(char) * 256);

  FILE *fr = fopen(path, "r");
  if (!fr) {
    printf("Erreur (fonction getRef): Impossible d'ouvrir %s\n", path);
    return NULL;
  }
  if (fgets(buff, 1000, fr) == NULL) {
    free(buff);
    fclose(fr);
    return ""; // fichier vide
  }
  fclose(fr);
  if (buff[strlen(buff) - 1] == '\n')
    buff[strlen(buff) - 1] = '\0';
  return buff;
}

// 7.5
void myGitAdd(char *file_or_folder) {
  WorkTree *wt;
  if (!fileExists(".add")) {
    system("touch .add");
    wt = initWorkTree();
  } else {
    wt = ftwts(".add");
  }
  if (!fileExists(file_or_folder)) {
    printf("Erreur (fonction myGitAdd): %s n'existe pas\n", file_or_folder);
    freeWorkTree(wt);
    return;
  }
  appendWorkTree(wt, file_or_folder, NULL, 0);
  wttf(wt, ".add");
  freeWorkTree(wt);
}

// 7.6
void myGitCommit(char *branch_name, char *message) {
  if (!fileExists(".refs")) {
    printf("Erreur (fonction myGitCommit): les références du projet ne sont "
           "pas initialisées\n");
    return;
  }
  char buff[256];
  sprintf(buff, ".refs/%s", branch_name);
  if (!fileExists(buff)) {
    printf("Erreur (fonction myGitCommit): la branche %s n'existe pas\n",
           branch_name);
    return;
  }
  char *branch_ref = getRef(branch_name); // contenu du dernier commit de la branche
  char *head_ref = getRef("HEAD");
  if (strcmp(head_ref, branch_ref) != 0) {
    printf("Erreur (fonction myGitCommit): HEAD doit pointer sur le dernier "
           "Commit de l branche\n");
    if (branch_ref && strlen(branch_ref)>0) free(branch_ref);
    if (head_ref && strlen(head_ref)>0) free(head_ref);
    return;
  }
  if (!fileExists(".add")) {
    printf("Erreur (fonction myGitCommit): Le fichier .add n'existe pas\n");
    if (branch_ref && strlen(branch_ref)>0) free(branch_ref);
    if (head_ref && strlen(head_ref)>0) free(head_ref);
    return;
  }
  WorkTree *wt = ftwts(".add");
  system("rm .add");

  //on remplit le champ tree avec le hash du worktree associe
  char *hash = saveWorkTree(wt, "."); 
  Commit *c = createCommit(hash); 
  
  //on remplit les champs message et predecessor s'ils existent
  if (strlen(branch_ref) != 0)
    commitSet(c, "predecessor", branch_ref); //lien avec le commit precedemment referencé
  if (message)
    commitSet(c, "message", message); //description du commit

  //mise a jour references HEAD et ref sur ce dernier commit
  free(hash);
  hash = blobCommit(c);
  createUpdateRef(branch_name, hash);
  createUpdateRef("HEAD", hash);

  if (branch_ref && strlen(branch_ref)>0) free(branch_ref);
  if (head_ref && strlen(head_ref)>0) free(head_ref);
  free(hash);
  freeWorkTree(wt);
  freeCommit(c);
}

// 7.7 -> (myGit.c)
