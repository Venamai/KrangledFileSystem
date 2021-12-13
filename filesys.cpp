#include <bits/stdc++.h>
#include <stdio.h>
#include "filesys.h"
using namespace std;

void printdir(){
fstream dirs;
dirs.open("directories.txt", fstream::in | fstream::binary);
struct dir* tempdir = (struct dir*)malloc(sizeof(dir));
cout << "name" << '\t' << "len" << '\t' << "index" << endl;
while (1){
    dirs.read((char*)tempdir, sizeof(dir));
    if (dirs.eof())
        break;
        if(tempdir->deleted == 0)
            cout << tempdir->filename << '\t' << tempdir->filelen << '\t' << tempdir->index << endl;//'\t' << "deleted=" << tempdir->deleted << endl;
}
dirs.close();
free(tempdir);
}

int dircount(){
fstream dirs;
dirs.open("directories.txt", fstream::in | fstream::binary);
dirs.seekg(0, fstream::end);
int fsize = dirs.tellg()/sizeof(dir);
#ifdef debug
//cout << "number of dirs= " << fsize << endl;
#endif // debug
return fsize;
}

struct dir* dirsearch(char *filename){
fstream dirs;
dirs.open("directories.txt", fstream::in | fstream::binary);
struct dir* tempdir = (struct dir*)malloc(sizeof(dir));
while (1){
    dirs.read((char*)tempdir, sizeof(dir));
    if (dirs.eof())
        break;
    #ifdef debug
    //cout << "reading" << " tempdir name = " << tempdir->filename;
    #endif // debug
    if (tempdir->deleted)
        continue;
    if (!(strcmp((const char*)filename, (const char*)tempdir->filename))){
        dirs.close();
        return tempdir;
    }
}
dirs.close();
free(tempdir);
return NULL;
}

struct dir* dircreate(char *filename){
struct dir* tempdir;
tempdir=dirsearch(filename);
if (tempdir) {
    free(tempdir);
    return NULL;
}
tempdir = (struct dir*)malloc(sizeof(dir));
strcpy((char*)(tempdir->filename), filename);
#ifdef debug
//cout << "created file " << tempdir->filename;
#endif // debug
tempdir->filelen=0;
tempdir->inode[0]=0; // need to be defined
tempdir->deleted=0;
tempdir->index = dircount()+1;
fstream dirs;
dirs.open("directories.txt", fstream::app | fstream::binary);
dirs.write((char*)tempdir, sizeof(dir));
dirs.close();
return tempdir;
}

struct File* openfile(char *filename, int mode){
struct File* tempfile;
#ifdef debug
//cout << "calling openfile " << " filename =" << filename << " mode=" << mode << endl;
#endif // debug
if (filename == NULL)
    return NULL;
struct dir* tempdir;
switch (mode){
    case 1 : // 1 == readonly
        tempdir = dirsearch(filename);
        #ifdef debug
        //cout << "calling search" << " tempdir name=" << tempdir->filename;
        #endif
        if (!tempdir){
            return NULL;
        }
        tempfile = (struct File*)malloc(sizeof(File));
        strcpy((char*)&(tempfile->filename), filename);
        tempfile->filelen = tempdir->filelen;
        tempfile->mode = mode;
        tempfile->data[0] = 0;
        tempfile->curpos = 0;
        tempfile->endof = 0;
        memcpy((void*)(tempfile->inode), (void*)(tempdir->inode), sizeof(tempdir->inode));
        free(tempdir);
        return tempfile;
        break;
    case 2 : // 2 == writeonly
        tempdir = dirsearch(filename);
        if (!tempdir)
            tempdir=dircreate(filename);
        tempfile = (struct File*)malloc(sizeof(File));
        strcpy((char*)&(tempfile->filename), filename);
        tempfile->filelen = 0;
        tempfile->mode = mode;
        tempfile->data[0] = 0;
        tempfile->curpos = 0;
        free(tempdir);
        return tempfile;
        break;
    default:
        return NULL;
        break;
}
}

void closefile(struct File* tempfile){
    if (tempfile->mode == 2)
        fileflush(tempfile);
    free(tempfile);
}

int rmfile(char *filename){
struct dir* tempdir;
tempdir = dirsearch(filename);
if (!tempdir)
    return -1;
//strcpy(tempdir->filename, "hello");
tempdir->deleted = 1;
#ifdef debug
//cout << "del index =" << tempdir->index << " index*size=" << tempdir->index*sizeof(dir) << endl;
#endif // debug
FILE* dirs;
dirs = fopen("directories.txt", "rb+");
fseek(dirs,(((tempdir->index)-1)*sizeof(dir)), 0);
#ifdef debug
//cout << "ftell =" << ftell(dirs) << " sizeof(dir)=" << sizeof(dir) << endl;
//cout << tempdir->filename << tempdir->index << tempdir->deleted << endl;
#endif // debug
fwrite((void*)tempdir, sizeof(dir), 1, dirs);
fclose(dirs);
return 0;
}

int filewrite(void* buf, int writesize, struct File* fileptr){
if (fileptr->mode != 2)
    return -1;
FILE* data;
data = fopen("filedata.txt", "rb+");
if (!data)
    return -1;
long t;
if ((fileptr->filelen) == 0){
    fseek(data, 0, SEEK_END);
    t=ftell(data);
    int a = (t/4096);
    int b = t%4096;
    //cout<<"a=" << a << " b=" << b << " t=" << t << endl;
    if (b) {
        fwrite(fileptr->data, 4096-b, 1, data);
        a++;
    }
    (fileptr->inode[0])=a;
}
int a = (fileptr->curpos)/4096; // inode[number]
int b = (fileptr->inode[a])*4096+(fileptr->curpos)%4096;
fseek(data, b, 0);
//cout << "b=" << b <<endl;
int c = 4096-(fileptr->curpos)%4096; // left in block
if (writesize<=c){
    fwrite(buf, writesize, 1, data);
    if ((fileptr->curpos)+writesize > fileptr->filelen)
        fileptr->filelen = (fileptr->curpos)+writesize;
    fileptr->curpos+=writesize;
    fclose(data);
    return 0;
} else {
    fwrite(buf, c, 1, data);
    if ((fileptr->curpos)+c > fileptr->filelen)
        fileptr->filelen = (fileptr->curpos)+c;
    fileptr->curpos+=c;
}
writesize -= c;
buf = (char*)buf+c;
fseek(data, SEEK_END, 0);
c=4096-ftell(data)%4096;
if (c!=4096) {
    fwrite((void*)fileptr->data, c, 1, data);
}
#ifdef debug
cout << "curpos =" << b << " buf =" << (char*)buf << endl;
#endif // debug
int j=0;
for (int i=0; i < (writesize/4096); i++){
    fwrite((void*)((char*)buf+i*4096), 4096, 1, data);
    fileptr->inode[i+a]=(ftell(data)/4096);
    j=i;
#ifdef debug
cout << "im in the loop" << endl;
#endif // debug
}
j+=1;
fwrite((void*)((char*)buf+j*4096), writesize%4096, 1, data);
fileptr->inode[j+a] = (ftell(data)/4096);
fileptr->filelen += writesize;
fileptr->curpos += writesize;
fclose(data);
return 0;
}

int fileflush(struct File* fileptr){
struct dir* tempdir;
tempdir = dirsearch(fileptr->filename);
if (!tempdir)
    return -1;
//strcpy(tempdir->filename, "hello");
tempdir->filelen = fileptr->filelen;
#ifdef debug
cout << "size int*inode=" <<sizeof(int)*sizeof(fileptr->inode) << endl;
//cout << "del index =" << tempdir->index << " index*size=" << tempdir->index*sizeof(dir) << endl;
#endif // debug
memcpy((void*)(tempdir->inode), (void*)(fileptr->inode), sizeof(fileptr->inode));
FILE* dirs;
dirs = fopen("directories.txt", "rb+");
fseek(dirs,(((tempdir->index)-1)*sizeof(dir)), 0);
#ifdef debug
//cout << "ftell =" << ftell(dirs) << " sizeof(dir)=" << sizeof(dir) << endl;
//cout << tempdir->filename << tempdir->index << tempdir->deleted << endl;
#endif // debug
fwrite((void*)tempdir, sizeof(dir), 1, dirs);
fclose(dirs);
return 0;
}

int fileread(void* buf, int readsize, struct File* fileptr){
#ifdef debug
//cout << "mode =" << fileptr->mode << endl;
#endif // debug
if (fileptr->mode != 1)
    return -1;
FILE* data;
data = fopen("filedata.txt", "rb");
    if (!data)
        return -2;
if ((fileptr->curpos)+readsize > fileptr->filelen)
    readsize = (fileptr->filelen) - (fileptr->curpos);
int rsize=readsize;
if (rsize == 0){
    fileptr->endof = 1;
    return 0;
}
while(1){
    int a = (fileptr->curpos)/4096; // inode[number]
    int b = (fileptr->inode[a])*4096+(fileptr->curpos)%4096;
    fseek(data, b, 0);
    int c = 4096-(fileptr->curpos)%4096; // left in block

    if (readsize<=c){
        fread(buf, readsize, 1, data);
        fileptr->curpos+=readsize;
        fileptr->endof = 1;
        return rsize;
    } else {
        fread(buf, c, 1, data);
        fileptr->curpos+=c;
    }
    readsize -= c;
    if (readsize <= 0){
        fileptr->endof = 1;
        return rsize;
    }
    buf = (char*)buf + c;
}

}
