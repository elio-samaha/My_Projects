#include "p.h"
#include <dirent.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>

#define TAILLE 10

int octalToDecimal(int octalMode) {
  int decimalMode = 0, i = 0;
  while (octalMode != 0) {
    decimalMode += (octalMode % 10) * (1 << (i * 3));
    octalMode /= 10;
    ++i;
  }
  return decimalMode;
}

int decimal(int num) {
  int decimal_num = 0, base = 1, rem;
  while (num > 0) {
    rem = num % 10;
    decimal_num += rem * base;
    num /= 10;
    base *= 2;
  }
  return decimal_num;
}

int octal(int num) {
  int octal_num = 0, base = 1, rem;
  while (num > 0) {
    rem = num % 8;
    octal_num += rem * base;
    num /= 8;
    base *= 10;
  }
  return octal_num;
}

int getChmod(const char *path) {
  struct stat ret;

  if (stat(path, &ret) == -1) {
    return -1;
  }
  return (ret.st_mode & S_IRUSR) | (ret.st_mode & S_IWUSR) |
         (ret.st_mode & S_IXUSR) | /*owner*/
         (ret.st_mode & S_IRGRP) | (ret.st_mode & S_IWGRP) |
         (ret.st_mode & S_IXGRP) | /*group*/
         (ret.st_mode & S_IROTH) | (ret.st_mode & S_IWOTH) |
         (ret.st_mode & S_IXOTH); /*other*/
}

void setMode(int mode, char *path) {
  char buff[100];
  sprintf(buff, "chmod %o %s", mode, path);
  system(buff);
}

// 4.1
WorkFile *createWorkFile(char *name) {
  WorkFile *wf = (WorkFile *)malloc(sizeof(WorkFile));
  wf->name = strdup(name);
  wf->hash = NULL;
  wf->mode = 0;
  return wf;
}

// 4.2
char *wfts(WorkFile *wf) {
  if (wf == NULL) {
    return "";
  }
  char temp[1000];
  sprintf(temp, "%s\t%s\t%o%c", wf->name, wf->hash, wf->mode, '\0');
  char *res = strdup(temp);
  return res;
}

// 4.3
WorkFile *stwf(char *ch) {
  char buff[100];
  char buff2[100];
  int buff3;
  sscanf(ch, "%s\t%s\t%o", buff, buff2, &buff3);
  WorkFile *res = createWorkFile(buff);
  res->hash = strdup(buff2);
  res->mode = buff3;
  return res;
}

// 4.4
WorkTree *initWorkTree() {
  WorkTree *wt = (WorkTree *)malloc(sizeof(WorkTree));
  wt->size = TAILLE;
  wt->n = 0;
  wt->tab = (WorkFile *)malloc(TAILLE * sizeof(WorkFile));
  return wt;
}

// 4.5
int inWorkTree(WorkTree *wt, char *name) {
  WorkFile *tmp = wt->tab;
  int pos = 0;
  while (pos < wt->n) {
    if (strcmp(tmp[pos].name, name) == 0)
      return pos;
    pos++;
  }
  return -1;
}

// 4.6
int appendWorkTree(WorkTree *wt, char *name, char *hash, int mode) {
  if (inWorkTree(wt, name) != -1) //le workfile de ce nom est deja dedans
    return 0;
  if (wt->n >= wt->size) //le worktree est rempli
    return -1;

  (wt->tab)[wt->n].name = name?strdup(name):NULL;
  (wt->tab)[wt->n].hash = hash?strdup(hash):NULL;
  (wt->tab)[wt->n].mode = mode;
  (wt->n)++;

  return 1;
}

int appendWorkTreeWf(WorkTree *wt, WorkFile *wf) {
  return appendWorkTree(wt, wf->name, wf->hash, wf->mode);
}

// 4.7
char *wtts(WorkTree *wt) {
  if (wt->n == 0)
    return "";
  char *s = (char *)malloc(sizeof(char) * TAILLE * 1000);
  char *temp = wfts(&(wt->tab[0]));
  strcpy(s, temp);
  for (int i = 1; i < wt->n; i++) {
    char *temp2 = wfts(&(wt->tab[i]));
    strcat(s, "\n");
    strcat(s, temp2);
    free(temp2);
  }
  strcat(s, "\n");
  free(temp);
  return s;
}

// 4.8
WorkTree *stwt(char *ch) {
  WorkTree *wt = initWorkTree();
  char *tmp = strdup(ch);
  char *token = strtok(tmp, "\n");
  int i = 0;
  while (token && i < wt->size) {
    WorkFile *wf = stwf(token);
    appendWorkTreeWf(wt, wf);
    token = strtok(NULL, "\n");
    freeWorkFile(wf);
    i++;
  }
  free(tmp);
  return wt;
}

// 4.9
int wttf(WorkTree *wt, char *file) {
  FILE *f = fopen(file, "w");
  char *temp = wtts(wt);
  fprintf(f, "%s", temp);
  fclose(f);
  free(temp);
  return 0;
}

// 4.10
WorkTree *ftwts(char *file) {
  FILE *fr;
  if ((fr = fopen(file, "r")) == NULL)
    return NULL;
  WorkTree *wt = initWorkTree();
  char buf[256];
  while (fgets(buf, 256, fr) != NULL) {
    WorkFile *wf = stwf(buf);
    if (wf != NULL) {
      appendWorkTree(wt, wf->name, wf->hash, wf->mode);
      freeWorkFile(wf);
    }
  }
  fclose(fr);
  return wt;
}

// 5.1

char *hashToFile(char *hash) {
  char *ch2 = strdup(hash);
  ch2[2] = '\0';
  struct stat st;
  if (stat(ch2, &st) == -1) {
    mkdir(ch2, 0700);
  }
  free(ch2);
  return hashToPath(hash);
}

char *blobWorkTree(WorkTree *wt) {
  char fname[100] = "/tmp/myfileXXXXXX";
  mkstemp(fname);
  wttf(wt, fname);
  char *hash = sha256file(fname);
  char *ch = hashToFile(hash);
  strcat(ch, ".t");
  cp(ch, fname);
  setMode(0777, ch);
  free(ch);
  return hash;
}

void freeWorkFile(WorkFile *wf) {
  if (wf) {
    free(wf->name);
    free(wf->hash);
    free(wf);
  }
  return;
}

void freeWorkTree(WorkTree *wt) {
  int i = 0;
  WorkFile *p = wt->tab;
  while (i < wt->n) {
    WorkFile *wf = &(wt->tab[i]);
    free(wf->name);
    free(wf->hash);
    i++;
  }
  free(p);
  free(wt);
}

//5.2
int isDirectory(const char *path) {
  //retourne 1 si c'est un repertoire, 0 sinon
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISDIR(path_stat.st_mode);
}

char *concat_paths(char *path1, char *path2) {
  char *result = malloc(strlen(path1) + strlen(path2) + 2);
  strcpy(result, path1); // Copy path1 to result
  strcat(result, "/");   // Append a slash to result
  strcat(result, path2); // Append path2 to result
  return result;
}

char *saveWorkTree(WorkTree *wt, char *path) {
  for (int i = 0; i < wt->n; i++) {
    WorkFile *wf = &(wt->tab[i]);
    char *absPath = concat_paths(path, wf->name);
    if (isDirectory(absPath) == 0) { //fichier
      blobFile(absPath);
      if ((wt->tab[i]).hash) free((wt->tab[i]).hash);
      wt->tab[i].hash = sha256file(absPath);
      wt->tab[i].mode = getChmod(absPath); //mise a jour hash et mode

    } else { //repertoire
      WorkTree *wt2 = initWorkTree();
      List *L = listdir(absPath);
      for (Cell *ptr = *L; ptr != NULL; ptr = ptr->next) {
        if (ptr->data[0] == '.')
          continue; //on ignore les fichiers cachés
        appendWorkTree(wt2, ptr->data, NULL, 0);
      }
      liberer(L);
      if ((wt->tab[i]).hash) free((wt->tab[i]).hash);
      wt->tab[i].hash = saveWorkTree(wt2, absPath); //appel recursif
      freeWorkTree(wt2);
      wt->tab[i].mode = getChmod(absPath);//mise a jour hash et mode
    }
    free(absPath);
  }
  return blobWorkTree(wt);
}


//5.3
void restoreWorkTree(WorkTree *wt, char *path) {
  char commande[200];
  sprintf(commande, "mkdir -p %s", path);
  system(commande); //on cree le repertoire cible de restauration au cas ou il n existe pas

  for (int i = 0; i < wt->n; i++) {
    char *hash_path = hashToPath(wt->tab[i].hash);
    char *buff = concat_paths(path, wt->tab[i].name);
    //distinction fichier/repertoire, equivalent a isDirectory
    if (access(hash_path, F_OK) == 0) { //fichier
      cp(buff, hash_path);
    } else { //repertoire
      strcat(hash_path, ".t");
      WorkTree *new_wt = ftwts(hash_path);
      restoreWorkTree(new_wt, wt->tab[i].name);//appel recursif
      freeWorkTree(new_wt); 
    }
    free(buff);
    free(hash_path);
  }
}
