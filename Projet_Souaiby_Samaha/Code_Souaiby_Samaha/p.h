#ifndef P_H
#define P_H

typedef struct cell {
  char *data;
  struct cell *next;
} Cell;

typedef Cell *List;

typedef struct {
  char *name;
  char *hash;
  int mode;
} WorkFile;

typedef struct {
  WorkFile *tab;
  int size;
  int n;
} WorkTree;

typedef struct key_value_pair {
  char *key;
  char *value;
} kvp;

typedef struct hash_table {
  kvp **T;
  int n;
  int size;
} HashTable;

typedef HashTable Commit;

// s1.c
// exo1
int hashFile(char *source, char *dest);
char *sha256file(char *file);
// exo2
List *initList();
Cell *buildCell(char *ch);
void insertFirst(List *L, Cell *C);
char *ctos(Cell *c);
char *ltos(List *L);
Cell *listGet(List *L, int i);
Cell *searchList(List *L, char *str);
List *stol(char *s);
void ltof(List *L, char *path);
List *ftol(char *path);
void liberer(List *L);
void printList(List* L);
// exo3
List *listdir(char *root_dir);
int fileExists(char *file);
void cp(char *to, char *from);
char *hashToPath(char *hash);
void blobFile(char *file);

// s2.c
int octalToDecimal(int octal);
int decimal(int num);
int octal(int num);
void setMode(int mode, char *path);
int getChmod(const char *path);
// exo4
WorkFile *createWorkFile(char *name);
char *wfts(WorkFile *wf);
WorkFile *stwf(char *ch);
WorkTree *initWorkTree();
int inWorkTree(WorkTree *wt, char *name);
int appendWorkTree(WorkTree *wt, char *name, char *hash, int mode);
int appendWorkTreeWf(WorkTree *wt, WorkFile *wf);
char *wtts(WorkTree *wt);
WorkTree *stwt(char *ch);
int wttf(WorkTree *wt, char *file);
WorkTree *ftwt(char *file);
WorkTree *ftwts(char *file);
// exo5
char *hashToFile(char *hash);
char *blobWorkTree(WorkTree *wt);
void freeWorkTree(WorkTree *wt);
void freeWorkFile(WorkFile *wf);
int isDirectory(const char *path);
void restoreWorkTree(WorkTree *wt, char *path);
char *saveWorkTree(WorkTree *wt, char *path);

// s3.c
// exo6
kvp *createKeyVal(char *key, char *val);
void freeKeyVal(kvp *kv);
char *kvts(kvp *k);
kvp *stkv(char *str);
Commit *initCommit();
static unsigned long sdbm(unsigned char *str);
void commitSet(Commit *c, char *key, char *value);
Commit *createCommit(char *hash);
char *commitGet(Commit *c, char *key);
void freeCommit(Commit *c);
char *cts(Commit *c);
Commit *stc(char *ch);
void ctf(Commit *c, char *file);
Commit *ftc(char *file);
char *blobCommit(Commit *c);
// exo7
void initRefs();
void createUpdateRef(char *ref_name, char *hash);
void deleteRef(char *ref_name);
char *getRef(char *ref_name);
void myGitAdd(char *file_or_folder);
void myGitCommit(char *branch_name, char *message);

// s4.c
// exo8
void initBranch();
int branchExists(char *branch);
void createBranch(char *branch);
char *getCurrentBranch();
char *hashToPathCommit(char *hash);
void printBranch(char *branch);
List *branchList(char *branch);
List *getAllCommits();
// exo9
void restoreCommit(char *hash_commit);
void myGitCheckoutBranch(char *branch);
List *filterList(List *L, char *pattern);
void myGitCheckoutCommit(char *pattern);

// s5.c
// exo11
WorkTree* btwt(char *branch);
WorkTree *mergeWorkTrees(WorkTree *wt1, WorkTree *wt2, List **conflicts);
List *merge(char *remote_branch, char *message);
void createDeletionCommit(char* branch, List* conflicts, char* message);


#endif
