#include "p.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>


WorkTree *mergeWorkTrees(WorkTree *wt1, WorkTree *wt2, List **conflicts) {
  WorkTree *wt = initWorkTree();
  WorkFile wf1;
  WorkFile wf2;
  Cell *c;
  
  if(!wt1){
    printf("Erreur (fonction mergeWorkTrees): wt1 est NULL\n");
    if (!wt2){
      printf("Erreur (fonction mergeWorkTrees): wt2 est NULL\n");
    }
    freeWorkTree(wt);
    return wt2;
  }
  if (!wt2){
    printf("wt2 est NULL\n");
    freeWorkTree(wt);
    return wt1; 
  }
  
  for (int i = 0; i < wt1->n; i++) {
    wf1 = wt1->tab[i];
    for (int j = 0; j < wt2->n; j++) {
      wf2 = wt2->tab[j];
      if ((strcmp(wf1.name, wf2.name) == 0) && (strcmp(wf1.hash, wf2.hash) != 0)){
        insertFirst(*conflicts, buildCell(wf1.name));
        break;
      }
    }
  }
  for (int i = 0; i < wt1->n; i++) {
    wf1 = wt1->tab[i];
    c = searchList(*conflicts, wf1.name);
    if (!c)
      appendWorkTreeWf(wt, &wf1);
  }
  for (int j = 0; j < wt2->n; j++) {
    wf2 = wt2->tab[j];
    c = searchList(*conflicts, wf2.name);
    if (!c)
      appendWorkTreeWf(wt, &wf2);
  }
  return wt;
}

WorkTree* btwt(char *branch){
  char *br_ref = getRef(branch);
  char *hash_br = hashToPathCommit(br_ref);
  Commit* c_br = ftc(hash_br);
  char* hash_tree = commitGet(c_br , "tree");
  char* hp_tree = hashToPath(hash_tree); 
  strcat(hp_tree , ".t");
  WorkTree * wt =  ftwts(hp_tree);
  if (br_ref && strlen(br_ref)>0) free(br_ref);
  free(hash_br); freeCommit(c_br) ; free(hp_tree); 
  return wt;
}

List *merge(char *remote_branch, char *message) {
  char* curr_br = getCurrentBranch();
  
  WorkTree * wt_current = btwt(curr_br); 
  if(!wt_current) printf("Erreur (fonction merge): worktree current branch est NULL\n");
  
  WorkTree * wt_remote = btwt(remote_branch);
  if(!wt_remote) printf("Erreur (fonction merge): worktree remote branch est NULL\n");
  
  List *conflicts = initList();
  
  WorkTree *wt = mergeWorkTrees(wt_current, wt_remote, &conflicts);
  //printList(conflicts);
  if (*conflicts){
    printf("conflit\n");
    free(curr_br);
    freeWorkTree(wt_current);
    freeWorkTree(wt_remote);
    freeWorkTree(wt);
    return conflicts;
  }
  char *curr_ref = getRef(curr_br);
  
  char *rb_ref = getRef(remote_branch);
  
  //char *hash = saveWorkTree(wt, ".");
  char *hash = blobWorkTree(wt);  
  
  Commit *c = createCommit(hash);
  commitSet(c, "message", message);
  commitSet(c, "predecessor", curr_ref); //branche courante
  commitSet(c, "merged_predecessor", rb_ref); //remote branch
  
  char* hash_c = blobCommit(c);
  
  createUpdateRef(curr_br, hash_c);
  createUpdateRef("HEAD", hash_c);
  
  deleteRef(remote_branch);
  restoreWorkTree(wt, ".");

  free(curr_br);
  if (curr_ref && strlen(curr_ref)>0) free(curr_ref);
  if (rb_ref && strlen(rb_ref)>0) free(rb_ref);
  free(hash);
  free(hash_c);
  
  freeWorkTree(wt_current);
  freeWorkTree(wt_remote);
  freeWorkTree(wt);
  liberer(conflicts);
  freeCommit(c);
 
  return NULL;
}

void createDeletionCommit(char* branch, List* conflicts, char* message){
  char* curr = getCurrentBranch();
  myGitCheckoutBranch(branch);
  WorkTree * wt = btwt(branch);
  if (fileExists(".add")) system("rm .add");
  if (wt){
    for (int i = 0 ; i < wt->n ; i++){
      Cell * c = searchList(conflicts , wt->tab[i].name);
      if(!c){
         myGitAdd(wt->tab[i].name);
      }
    }
    myGitCommit(branch , message);
    myGitCheckoutBranch(curr);
    freeWorkTree(wt);
  }
  free(curr);
}
