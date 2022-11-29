// Assignment 5
// CS 201
// Created by Lars Jensen on 11/28/22.

#include <stdio.h>
#include <stdlib.h>

// Enum for replacement policies
enum ReplacementPolicy {FIFO = 0, LRU = 1, RANDOM = 2};

// Structure for the page-table entries
typedef struct {
    int inUse; // whether there is actually an active page in this frame
    int inTime; // the time that the page is this frame was loaded
    int useTime; // the most recent access time for the page in this frame
    unsigned long pageNum; // the page in this frame
} PageTableEntry;

// Structure to keep track of the frames
typedef struct {
    int numFrames; // number of page frames available
    unsigned int currentTime; // a counter; increment it each time translate() is called
    enum ReplacementPolicy policy; // an enum representing which replacement alg to use
    int numPageFaults; // the total number of page faults
} MemStruct;

#define BUFFLEN 1024

#define FILENAME "testOne.atrace.out"


int main(){
    char buffer[BUFFLEN];
    char tmpbuf[64];
    unsigned long val1, val2;
    int nf;
    FILE *fp;
    char *chp;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("cannot open file '%s' for reading\n", FILENAME);
        return 8;
    }

    chp = fgets(buffer, BUFFLEN, fp);
    while ( chp != NULL ) {
        nf = sscanf(buffer, "%lp: %c %lp", &val1, tmpbuf, &val2);
        if (nf == 3) {
            printf("%s", buffer);
            printf("val1 = %lu; val2 = %lu\n", val1, val2);
        }
        chp = fgets(buffer, BUFFLEN, fp);
    }

    fclose(fp);



    return 0;
}

unsigned long translate(
        PageTableEntry *pageTable, // the page table itself
        unsigned long pageNum, // the virtual page being translated
        unsigned long iPageNum, // the page of the instruction address corresponding to this data
        // page; or zero if pageNum corresponds to an instruction address
        MemStruct *memStruct // information about the virtual memory
    ){


}

