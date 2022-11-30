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

unsigned long translate(
        PageTableEntry *pageTable, // the page table itself
        unsigned long pageNum, // the virtual page being translated
        unsigned long iPageNum, // the page of the instruction address corresponding to this data
        // page; or zero if pageNum corresponds to an instruction address
        MemStruct *memStruct // information about the virtual memory
);

#define BUFFLEN 1024
#define PAGESIZE 4096
#define FRAMENUM 16  //change to 32 or 64 when necessary
#define FILENAME "testOne.atrace.out"


int main(){
    // Create and initialize structs
    MemStruct memStruct;

    memStruct.numFrames = FRAMENUM; //not sure about this
    memStruct.currentTime = 0;
    memStruct.policy = FIFO;
    memStruct.numPageFaults = 0;

    PageTableEntry pageTable[memStruct.numFrames];


    // variables for reading in from file
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
    //read until end of file
    while ( chp != NULL ) {

        //read data in from file, val1 is instruction address, val2 is data address
        nf = sscanf(buffer, "%lp: %c %lp", &val1, tmpbuf, &val2);
        if (nf == 3) {
            printf("%s", buffer);
            printf("val1 = %lu; val2 = %lu\n", val1, val2);
        }

        //translate val1 and val2 into page numbers
        unsigned long iPageNum = val1 / PAGESIZE;
        unsigned long dPageNum = val2 / PAGESIZE;

        //call translate function on instruction and then data
        translate(&pageTable, iPageNum, 0, &memStruct);
        translate(&pageTable, dPageNum, iPageNum, &memStruct);


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

    int frameNum = 0;
    int targetFrame = 0;
    int inTable = 0; //represents if page num is in page table or not
    for (int i = 0; i < memStruct->numFrames; ++i){
        if (pageTable[i].pageNum == pageNum || pageTable[i].pageNum == iPageNum){
            inTable = 1;
            frameNum = i;
        }
    }

    if (inTable == 1) {
        pageTable[frameNum].useTime = memStruct->currentTime;
        memStruct->currentTime++;
        return frameNum;
    } else {
        memStruct->numPageFaults++;
        // need to be sure in here that we don't replace the instruction page if we're loading the data
        // TODO: targetFrame = getFreeFrame()
        pageTable[targetFrame].pageNum = pageNum;
        pageTable[targetFrame].inUse = 1;
        pageTable[targetFrame].useTime = memStruct->currentTime;
        pageTable[targetFrame].inTime = memStruct->currentTime;
        memStruct->currentTime++;
        return targetFrame;
    }
}

