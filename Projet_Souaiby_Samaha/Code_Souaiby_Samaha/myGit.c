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

int main(int argc, char **argv) { //(7.7-10)

  if (strcmp(argv[1], "init") == 0) {
    initRefs();   //(7.1)
    initBranch(); //(8.1)
  }

  if (strcmp(argv[1], "list-refs") == 0) {
    if (fileExists(".refs")) {
      Cell *tmp = *listdir(".refs");
      while (tmp) {
        if (tmp->data[0] != '.') {
          char *content = getRef(tmp->data); //(7.4)
          printf("%s :\t%s\n", tmp->data, content);
          free(content);
        }
        tmp = tmp->next;
      }
    }
  }

  if (strcmp(argv[1], "create-ref") == 0) {
    if (argc == 4) {
      createUpdateRef(argv[2], argv[3]); //(7.2)
      createUpdateRef("HEAD", argv[3]);
    } else if (argc == 3) {
      createUpdateRef(argv[2], NULL); //(7.2)
      createUpdateRef("HEAD", NULL);
    }
  }
  if (strcmp(argv[1], "delete-ref") == 0) {
    deleteRef(argv[2]); //(7.3)
  }

  if (strcmp(argv[1], "add") == 0) {
    for (int i = 2; i < argc; i++)
      myGitAdd(argv[i]); //(7.5)
  }

  if (strcmp(argv[1], "list-add") == 0) {
    if (fileExists(".add")) {
      WorkTree *wt = ftwts(".add");
      char *s = wtts(wt);
      printf("%s\n", s);
      free(s);
      freeWorkTree(wt);
    }
  }

  if (strcmp(argv[1], "clear-add") == 0) {
    if (fileExists(".add")) system("rm .add");
  }

  if (strcmp(argv[1], "commit") == 0) {
    if ((argc == 5) && (argv[3][0] == '-') && (argv[3][1] == 'm'))
      myGitCommit(argv[2], argv[4]); //(7.6)
    else
      myGitCommit(argv[2], NULL);
  }

  if (strcmp(argv[1], "get-current-branch") == 0) {
    char *current_branch = getCurrentBranch(); //(8.4)
    printf("%s\n", current_branch);
    free(current_branch);
  }

  if (strcmp(argv[1], "branch") == 0) {
    if (branchExists(argv[2]))
      printf("La branche %s existe deja\n", argv[2]); //(8.2)
    else
      createBranch(argv[2]); //(8.3)
  }

  if (strcmp(argv[1], "branch-print") == 0) {
    if (!branchExists(argv[2]))
      printf("La branche %s n'existe pas\n", argv[2]);
    else
      printBranch(argv[2]); //(8.5)
  }

  if (strcmp(argv[1], "checkout-branch") == 0) {
    if (!branchExists(argv[2]))
      printf("La branche %s n'existe pas\n", argv[2]);
    else{
       myGitCheckoutBranch(argv[2]); //(9.2)
    }
  }

  /*if (strcmp(argv[1], "checkout-commit") == 0)
    myGitCheckoutCommit(argv[2]); //(9.4) */
 if (strcmp(argv[1], "checkout-commit") == 0) {
    if (argc != 3) {
      printf("Erreur: il faut 1 argument apres la commande checkout-commit\n");
    } else
      myGitCheckoutCommit(argv[2]);
    // les msgs d erreur sont deja dans la fonction
  }

  if (strcmp(argv[1], "getall-commits") == 0){
    List *l = getAllCommits();
    char *s = ltos(l);
    printf("%s\n",s);
    free(s);
    liberer(l);
  }  

  if (strcmp(argv[1], "branch-list") == 0){
    List *l = branchList(argv[2]);
    char *s = ltos(l);
    printf("%s\n",s);
    free(s);
    liberer(l);
  }
  if (strcmp(argv[1], "merge") == 0) {
    char *branch = argv[2];
    char *message = argv[3];
    char *curr_branch = getCurrentBranch();
    List *Liste_conflits = merge(branch, curr_branch);
    if (!Liste_conflits || (Liste_conflits && *Liste_conflits == NULL)) {
      printf("La fusion s'est bien passée !\n");
      if (Liste_conflits) free(Liste_conflits);
      free(curr_branch);
    } else {
        printf("LA LISTE DES CONFLITS:\n");
        printList(Liste_conflits);
        printf("Pour garder les fichiers de la branche courante et supprimer ceux de la branche %s, tapez 1\2:Pour garder les fichiers de la branche %s et supprimer ceux de la blanche courante, tapez 2\nPour choisir conflit par conflit la version à garder, tapez 3\n" , branch , branch);
        int choix;
        scanf("%d", &choix);
        if (choix == 1) {
          createDeletionCommit(branch, Liste_conflits, message);
          merge(branch, message);
        }
        if (choix == 2) {
          createDeletionCommit(curr_branch, Liste_conflits, message);
          merge(branch, message);
        }
        if (choix == 3) {
          Cell *c = *Liste_conflits;
          List *list_curr = initList();
          List *list_branch = initList();
          printf("LA LISTE DES CONFLITS:\n");
          printList(Liste_conflits);
          Cell* tmp;
          while (c) {
            tmp = buildCell(c->data);
            printf("Quelle version du fichier %s souhaitez vous garder?\nPour garder celle de la branche courante, tapez 1\nPour garder celle de la branche %s, tapez 2",c->data, branch);
            int choix_branch;
            scanf("%d", &choix_branch);
            if (choix_branch == 1) {
              insertFirst(list_branch, tmp);
            }
            if (choix_branch == 2) {
              insertFirst(list_curr, tmp);
            }
            c = c->next;
          }
          createDeletionCommit(curr_branch, list_curr, message);
          createDeletionCommit(branch, list_branch, message);
          merge(branch, message);
  
          free(curr_branch);
          liberer(list_curr);
          liberer(list_branch);
          liberer(Liste_conflits);
        }
      }
    }
  return 0;
}