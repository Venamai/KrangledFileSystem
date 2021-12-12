struct dir {
char filename[128];
int filelen;
int deleted;
int index;
int inode[1024];
};

struct File {
char filename[128];
int filelen;
int mode;
char data[4096];
int inode[1024];
unsigned int curpos;
int endof;
};

struct dir* dirsearch(char *filename);

struct dir* dircreate(char *filename);

struct File* openfile(char *filename, int mode);

void closefile(struct File*);

void printdir();

int rmfile(char *filename);

int filewrite(void* buf, int writesize, struct File* fileptr);

int fileflush(struct File* fileptr);

int fileread(void* buf, int readsize, struct File* fileptr);

//#define debug = ;
