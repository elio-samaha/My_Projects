#include "p.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>
#define TAILLE 10

int main(int argc, char **argv) {
  // s1.c--------------------------------------------------------------------------------------

  /*
  //exo1
  system("ls"); //(1.2)

  int h = hashFile(argv[1],argv[2]); //(1.3)

  char *sha256 = sha256file(argv[1]);
  printf("%s\n", sha256); //(1.4)

  free(sha256);

  //exo2
  List* L = initList(); //(2.1)

  Cell* c1 = buildCell("chaine1"); //(2.2)
  Cell* c2 = buildCell("chaine2");
  Cell* c3 = buildCell("chaine3");

  insertFirst(L, c3); //(2.3)
  insertFirst(L, c2);
  insertFirst(L, c1);

  Cell* c = listGet(L, 2); //(2.5)
  printf("%s\n", ctos(c));

  Cell* c4 = searchList(L, "chaine2"); //(2.6)
  printf("%s\n", ctos(c4));
  char *chaine = ltos(L);
  printf("%s\n",chaine);
  List *L1 = stol(chaine);

  ltof(L1, "L.txt"); //(2.4-2.7-2.8)

  List *L2 = ftol("L.txt"); //(2.8)

  liberer(L);
  liberer(L1);
  liberer(L2);

  //exo3
  char *ch = hashToPath("maisouetdoncornicar"); //(3.4)
  printf("%s\n", ch);

  cp("elio.txt", "christina.txt"); //(3.3)

  blobFile("test.c");//(3.5)

  free(ch);
  free(chaine);
  */

  // s2.c--------------------------------------------------------------------------------------

  //exo4
  WorkFile *wf1 = createWorkFile("wf1"); //(4.1)
  WorkFile *wf2 = createWorkFile("wf2");
  wf1->mode = 777;
  wf2->mode = 707;
  wf1->hash = strdup("sarah");
  wf2->hash = strdup("tina");

  char *s1 = wfts(wf1); //(4.2)
  char *s2 = wfts(wf2);
  // WorkFile *wf3 = stwf(s1);
  WorkFile *wf3 = createWorkFile("wf3");
  char *s3 = wfts(wf3);
  printf("s1 : %s\n", s1);
  printf("wf3 name : %s\n", wf3->name);

  WorkTree *wt1 = initWorkTree(); //(4.4)

  appendWorkTreeWf(wt1, wf1); //(4.5-4.6)
  appendWorkTreeWf(wt1, wf2);
  appendWorkTreeWf(wt1, wf3);

  char *st = wtts(wt1); //(4.7)
  printf("%s\n", st);
  printf("wt : %d %s\n", wf2->mode, st);

  WorkTree *wt2 = stwt(st); //(4.3-4.8)
  printf("%d \n", wt2->n);

  wttf(wt1, "elio_l_batal"); //(4.9)

  WorkTree *wt3 = ftwts("elio_l_batal"); //(4.10)
  printf("%d \n", wt3->n);

  //exo5
  //char* hs = blobWorkTree(wt1); //(5.1)
  //printf("%s\n" , hs);
  //cp("wf4" , "wf1");

  char *sv = saveWorkTree(wt1, "."); //(5.2)
  FILE *wf1f = fopen("wf1", "a");
  fprintf(wf1f, "elio\n");
  fclose(wf1f);
  FILE *wf2f = fopen("wf2/wftest", "a");
  fprintf(wf2f, "elio\n");
  fclose(wf2f);

  restoreWorkTree(wt1, "."); //(5.3)

  // free(hs);
  free(sv);
  freeWorkTree(wt3);
  free(s1);
  free(s2);
  free(s3);
  free(st);
  freeWorkFile(wf1);
  freeWorkFile(wf2);
  freeWorkFile(wf3);
  freeWorkTree(wt1);
  freeWorkTree(wt2);
  
  
  // s3.c--------------------------------------------------------------------------------------
  //exo6
  /*
  kvp *kv1 = createKeyVal("21102782", "Souaiby_Christina"); //(6.1)

  char *s1 = kvts(kv1); //(6.2)
  printf("%s\n", s1);
  kvp *kv2 = stkv(s1);

  Commit *c1 = initCommit(); //(6.3)

  commitSet(c1, kv1->key, kv1->value); //(6.4-6.5)
  commitSet(c1, kv2->key, kv2->value); // insertion de 2 identiques pour verifier le probing

  Commit *christina = createCommit("hashchristina"); //(6.6)
  commitSet(c1, "21105733", "Samaha_Elio");

  char *s2 = commitGet(c1, "21105733"); //(6.7)
  printf("21105733 : %s\n", s2);

  char *s3 = cts(c1); //(6.8)
  printf("-Commit 1-\n%s\n", s3);
  Commit *c2 = stc(s3);
  char f1[10] = "fichier1";
  char f2[10] = "fichier2";
  char *s5 = cts(c2); //(6.8)
  printf("-Commit 2-\n%s\n", s5);
  ctf(c1, f1); //(6.9)
  ctf(c2, f2); // verifier que les 2 fichiers ont le meme contenu
  Commit *c3 = ftc(f2);

  char *s4 = blobCommit(c3); //(6.10)
  printf("BlobCommit(c3) :\n%s\n", s4);

  free(s1);
  free(s3);
  free(s4);
  free(s5);
  freeKeyVal(kv1);
  freeKeyVal(kv2);
  freeCommit(c1);
  freeCommit(c2);
  freeCommit(c3);
  freeCommit(christina);
  */
  
  return 0;
}
