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

//8.1
void initBranch(){
  if (!fileExists(".current_branch")) {
    FILE* f = fopen(".current_branch" , "w");
    fclose(f);
  }
  system("echo master > .current_branch");
}

//8.2
int branchExists(char* branch){
  //retourne 1 si la branche existe, 0 sinon
  if(branch == NULL || strlen(branch)==0) return 0;
  List *l = listdir(".refs");
  if (searchList(l, branch) == NULL) {
    liberer(l);
    return 0;
  }
  liberer(l);
  return 1;
}

//8.3
void createBranch(char* branch){
  if(branch == NULL || strlen(branch)==0) return;
  char* hash = getRef("HEAD");
  createUpdateRef(branch, hash);
  if (hash && strlen(hash)>0) free(hash);
}

//8.4
char* getCurrentBranch(){
  FILE* f = fopen(".current_branch","r");
  char* branch_name = malloc(sizeof(char)*1000);
  fscanf(f, "%s",branch_name);
  fclose(f);
  return branch_name;
}

//8.5
char* hashToPathCommit(char*hash){
  char * path = malloc(sizeof(char)*1000);
  char* tmp = hashToPath(hash);
  sprintf(path, "%s.c", tmp);
  free(tmp);
  return path;
}

void printBranch(char* branch){
  if(!branch) return;
  char * commit_hash = getRef(branch);
  if ((commit_hash == NULL)||strlen(commit_hash)==0){
    printf("Erreur (fonction printBranch): la branche est vide\n");
    if (commit_hash && strlen(commit_hash)>0) free(commit_hash);
    return;
  }
  char * commit_path =hashToPathCommit(commit_hash);
  if(!commit_path) {
    printf("Erreur (fonction printBranch): pas de chemin vers un commit dans la branche\n"); 
    return;
  }
  Commit * c = ftc(commit_path);
  char* message;
  while(c){
    //afichage du message de chaque commit
    message = commitGet(c,"message");
    if(message) printf("%s -> %s \n",commit_hash, message);
    else printf("%s \n", commit_hash);
   
    if(commitGet(c, "predecessor")==NULL) break; //la liste des commit pour cette branche est terminee
    else{
      //recuperation du fichier contenant le commit precedent a partir de son hash
      char *ref = commit_hash;
      commit_hash= commitGet(c, "predecessor");
      free(ref);
      
      char *path = commit_path;
      commit_path= hashToPathCommit(commit_hash);
      free(path);

      //creation du commit associe
      c =ftc(commit_path);
    }
  }
  freeCommit(c);
  if (commit_hash && strlen(commit_hash)>0) free(commit_hash);
  free(commit_path);
}

//8.6
List *branchList(char *branch) {
  List* L =initList();
  char * commit_hash = getRef(branch);
  if (!commit_hash || (strlen(commit_hash)) <= 5){
    liberer(L);
    return NULL;
  }
 
  char * commit_path =hashToPathCommit(commit_hash);
  Commit * c = ftc(commit_path);
 
  while(c){
    insertFirst(L, buildCell(commit_hash));
   
    if(commitGet(c, "predecessor") == NULL) break; //la liste des commit pour cette branche est terminee
      
    else{
      //recuperation du fichier contenant le commit precedent a partir de son hash
      char *ref = commit_hash;
      commit_hash= commitGet(c, "predecessor");
      free(ref);
      
      char *path = commit_path;
      commit_path= hashToPathCommit(commit_hash);
      free(path);
      if(c) freeCommit(c);
      //creation du commit associe
      c =ftc(commit_path);
    }
  }
  if (commit_hash && strlen(commit_hash)>0) free(commit_hash);
  free(commit_path);
  if(c) freeCommit(c);
  return L;
}

//8.7
List *getAllCommits(){
  List *L = initList();
  List* list = listdir(".refs");
  Cell *content = *list;
  while (content){
    if (content->data[0]!='.'){
      List* list2 = branchList(content->data); //recupere la liste des commits de chaque branche
      if (!list2) {
        content = content->next;
        continue;
      } 
      Cell *cell = *list2;
      while (cell){
        if (!searchList(L, cell->data)) { //on verifie avant d ajouter chaque commit qu'il n est pas deja dedans
          Cell* cellule = buildCell(cell->data);
          insertFirst(L, cellule);
        }
        cell = cell->next;
      }
      liberer(list2);
    }
    content = content->next;
  }
  liberer(list);
  return L;
}
//9.1
void restoreCommit(char * hash_commit){
  char* path_commit = hashToPathCommit(hash_commit);
  Commit* c = ftc(path_commit); 
  //on retrouve le worktree correspondant au commit et on le restaure
  char* tree_path = (char*)malloc(1000*sizeof(char));
  char* hash = commitGet(c, "tree"); 

  char* path = hashToPath(hash);
  sprintf(tree_path, "%s.t", path);

  WorkTree * wt = ftwts(tree_path);
  restoreWorkTree(wt, ".");
  free(path_commit);
  freeCommit(c);
  free(tree_path);
  free(path);
  freeWorkTree(wt);
}

//9.2
void myGitCheckoutBranch(char* branch){
    FILE * f = fopen(".current_branch", "w");
    if(!f) printf("Erreur (fonction myGitCheckoutBranch): Impossible d'ouvrir .current_branch\n");
    fprintf(f, "%s", branch); //on met a jour la branche courante
    fclose(f);
    char* hash_commit =  getRef(branch);
    createUpdateRef("HEAD", hash_commit); //on met a jour HEAD pour pointer sur le dernier commit de la nouvelle branche courante
  
    restoreCommit(hash_commit);
    if (hash_commit && strlen(hash_commit)>0) free(hash_commit);
}

//9.3
List* filterList(List* L, char* pattern){ 
  List* List_filtered = initList();
  int len_pattern = strlen(pattern);
  Cell* c = *L;
  while(c){
    if (strncmp(pattern, c->data, len_pattern) == 0){ //on recupere les commits qui commencent par pattern
      insertFirst(List_filtered, buildCell(c->data));
    }
    c = c->next;
  }
  return List_filtered;
}

//9.4
void myGitCheckoutCommit(char* pattern){
  List* L = getAllCommits();
  List* L_filtered = filterList(L, pattern);

  Cell* c = *L_filtered;

  if(!c){
    printf("Erreur (fonction myGitCheckoutCommit): aucun commit ne correspond a cette requete\n");
    liberer(L_filtered);
    liberer(L);
    return;
  }

  if(!c->next){ //un seul commit correspond
    char * commit_hash = c->data;
    createUpdateRef ("HEAD", commit_hash);
    restoreCommit (commit_hash);
    liberer(L_filtered);
    liberer(L);
    return;
  }

  printf("Plusieurs commits correspondent à votre requête, lequel recherchez vous?\n");
  char* str = ltos(L_filtered);
  printf("%s\n", str);
  liberer(L_filtered);
  liberer(L);
  free(str);
  return;
}

