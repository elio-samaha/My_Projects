#include "p.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>

// 1.1 et 1.2 dans le terminal

// 1.3
int hashFile(char *source, char *dest) {
  char buff[1000];
  sprintf(buff, "cat %s | sha256sum > %s", source, dest);
  system(buff);
  return 0;
}

// 1.4
char *sha256file(char *file) {
  static char template[] = "/tmp/myfileXXXXXX";
  char fname[1000];
  strcpy(fname, template);
  int fd = mkstemp(fname);
  hashFile(file, fname);
  char *buff = (char *)malloc(1000 * sizeof(char));
  FILE *f = fopen(fname, "r");
  fgets(buff, sizeof(char) * 10000, f);
  close(fd);
  fclose(f);
  buff[strlen(buff) - 4] = '\0';
  return buff;
}

// 2.1
List *initList() {
  List *L = (List *)malloc(sizeof(List));
  *L = NULL;
  return L;
}

// 2.2
Cell *buildCell(char *ch) {
  Cell *cellule = (Cell *)malloc(sizeof(Cell));
  cellule->data = strdup(ch);
  cellule->next = NULL;
  return cellule;
}

// 2.3
void insertFirst(List *L, Cell *C) {
  C->next = *L;
  *L = C;
}

// 2.4
char *ctos(Cell *c) {
  if (!c)
    return "";
  return c->data;
}

char *ltos(List *L) {
  Cell *tmp = *L;
  char *res = (char *)malloc(sizeof(char) * 1000);
  strcpy(res, "");
  while (tmp) {
    strcat(res, ctos(tmp));
    strcat(res, "|");
    tmp = tmp->next;
  }
  res[strlen(res) - 1] = '\0';
  return res;
}

// 2.5
Cell *listGet(List *L, int i) {
  Cell *tmp = *L;
  int count = 0;
  while ((tmp) && (count < i)) {
    count++;
    tmp = tmp->next;
  }
  return tmp;
}

// 2.6
Cell *searchList(List *L, char *str) {
  if(!L) return NULL;
  Cell *tmp = *L;
  while ((tmp) && (strcmp(str, tmp->data) != 0)) {
    tmp = tmp->next;
  }
  return tmp;
}

// 2.7
List *stol(char *s) {
  List *L = initList();
  char *tmp = strdup(s);
  char *token = strtok(tmp, "|");
  while (token) {
    insertFirst(L, buildCell(token));
    token = strtok(NULL, "|");
  }
  free(tmp);
  return L;
}

// 2.8
void ltof(List *L, char *path) {
  FILE *f = fopen(path, "w");
  Cell *tmp = *L;
  while (tmp) {
    fprintf(f, "%s\n", ctos(tmp));
    tmp = tmp->next;
  }
  fclose(f);
}

//-

List *ftol(char *path) {
  FILE *f = fopen(path, "r");
  List *L = initList();
  char buff[1000];
  while (fgets(buff, sizeof(char) * 1000, f)) {
    buff[strlen(buff) - 1] = '\0';
    insertFirst(L, buildCell(buff));
  }
  fclose(f);
  return L;
}

// -
void liberer(List *L) {
  if(L){
    Cell *l = *L;
    while (l) {
      Cell *tmp = l->next;
      free(l->data);
      free(l);
      l = tmp;
    }
  free(L);
  }
}

// 3.1
List *listdir(char *root_dir) {
  List *L = initList();
  DIR *dp = opendir(root_dir);
  struct dirent *ep;
  if (dp) {
    while ((ep = readdir(dp))) {
      insertFirst(L, buildCell(ep->d_name));
    }
    closedir(dp);
  } else
    printf("Erreur (fonction listdir): Impossible d'ouvrir %s\n", root_dir);
  return L;
}

// 3.2
struct stat st = {0};

int fileExists(char *file) {
  struct stat buffer;
  return (stat(file, &buffer) == 0) ? 1 : 0;
}

// 3.3
void cp(char *to, char *from) {
  FILE *fr = fopen(from, "r");
  if (fr) {
    FILE *fw = fopen(to, "w");
    if (!fw){
      printf("Erreur (fonction cp): Impossible d'ouvrir %s\n", to);
    }
    int mode = getChmod(from);
    setMode(mode, to);
    char buff[10000];
    while (fgets(buff, 1000 * sizeof(char), fr)) {
      fprintf(fw, "%s", buff);
    }
    fclose(fr);
    fclose(fw);
  } else
    printf("Erreur (fonction cp): Impossible d'ouvrir %s\n", from);
}

// 3.4
char *hashToPath(char *hash) {
  char *path = (char *)malloc(sizeof(char) * (strlen(hash) + 4));
  snprintf(path, strlen(hash) + 2, "%c%c/%s%c", hash[0], hash[1], hash + 2,
           '\0');
  return path;
}

// 3.5
void blobFile(char *file) {
  char *hash = sha256file(file);
  char *ch2 = strdup(hash);
  ch2[2] = '\0';
  if (!fileExists(ch2)) {
    char buff[100];
    sprintf(buff, "mkdir %s", ch2);
    system(buff);
  }
  char *ch = hashToPath(hash);
  cp(ch, file);
  free(hash);
  free(ch2);
  free(ch);
}

void printList(List* L){
  Cell *curr = *L;
  while(curr){
    printf("%s -> ", curr->data);
    curr = curr->next;
  }
}