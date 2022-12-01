// Assignment 5
// CS 201
// Created by Lars Jensen on 11/28/22.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


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

int getFreeFrame(PageTableEntry *pageTable, // the page table itself
                 unsigned long pageNum, // the virtual page being translated
                 unsigned long iPageNum, //instruction page number, 0 if first is instruction
                 MemStruct *memStruct // info about virtual memory
                 );


#define BUFFLEN 1024
#define PAGESIZE 4096
#define FRAMENUM 16
#define POLICY RANDOM

#define FILENAME "/Users/larsjensen/Desktop/CS 201/Assignment5/small-loop.atrace.out"


int main(){
    // Create and initialize structs
    MemStruct memStruct;

    memStruct.numFrames = FRAMENUM; //not sure about this
    memStruct.currentTime = 0;
    memStruct.policy = POLICY;
    memStruct.numPageFaults = 0;

    PageTableEntry pageTable[memStruct.numFrames];

    for (int i = 0; i < memStruct.numFrames; ++i){
        pageTable[i].pageNum = 0;
        pageTable[i].inUse = 0;
        pageTable[i].useTime = 0;
        pageTable[i].inTime = 0;
    }

    // variables for reading in from file
    char buffer[BUFFLEN];
    char tmpbuf[64];
    unsigned long val1, val2;
    int nf;
    FILE *fp;
    char *chp;

    fp = fopen(FILENAME, "r"); // i had to use my path - C:\Users\aidan\CLionProjects\Assignment5
    if (fp == NULL) {
        printf("cannot open file '%s' for reading\n", FILENAME);
        return 8;
    }

    chp = fgets(buffer, BUFFLEN, fp);
    //read until end of file
    while ( chp != NULL ) {

        //read data in from file, val1 is instruction address, val2 is data address
        nf = sscanf(buffer, "%lx: %c %lx", &val1, tmpbuf, &val2);
        if (nf == 3) {
            printf("%s", buffer);
            printf("val1 = %lu; val2 = %lu\n", val1, val2);
        }
        //translate val1 and val2 into page numbers
        unsigned long iPageNum = val1 / PAGESIZE;
        unsigned long dPageNum = val2 / PAGESIZE;

        //call translate function on instruction and then data
        translate((PageTableEntry *) &pageTable, iPageNum, 0, &memStruct);
        translate((PageTableEntry *) &pageTable, dPageNum, iPageNum, &memStruct);


        chp = fgets(buffer, BUFFLEN, fp);
    }

    fclose(fp);



    printf("Number of Page faults: %d", memStruct.numPageFaults);

    return 0;
}

unsigned long translate(
        PageTableEntry *pageTable, // the page table itself
        unsigned long pageNum, // the virtual page being translated
        unsigned long iPageNum, // the page of the instruction address corresponding to this data
        // page; or zero if pageNum corresponds to an instruction address
        MemStruct *memStruct // information about the virtual memory
    ){

    printf("Translating...\n");
    int frameNum = 0;
    int targetFrame;
    int inTable = 0; //represents if page num is in page table or not
    for (int i = 0; i < memStruct->numFrames; ++i){
        if (pageTable[i].pageNum == pageNum){
            inTable = 1;
            frameNum = i;
        }
    }
    // If there is any entry in the page table matching the page number
    // of the address that is being translated (and for which the inUse flag is not zero (DON'T GET THIS))
    if (inTable == 1) {
        printf("Already in the table!\n");
        pageTable[frameNum].useTime = memStruct->currentTime;
        memStruct->currentTime++;
        return frameNum;
    }
    else
    {
        memStruct->numPageFaults++;
        // need to be sure in here that we don't replace the instruction page if we're loading the data
        targetFrame = getFreeFrame(pageTable, pageNum, iPageNum, memStruct);
        if (targetFrame == -1){
            printf("Error in getting free frame\n");
            return -1;
        }
        printf("%d\n", targetFrame);

        pageTable[targetFrame].pageNum = pageNum;
        pageTable[targetFrame].inUse = 1;
        pageTable[targetFrame].useTime = memStruct->currentTime;
        pageTable[targetFrame].inTime = memStruct->currentTime;
        memStruct->currentTime++;
        return targetFrame;
    }
}

int getFreeFrame(PageTableEntry *pageTable,
                 unsigned long pageNum,
                 unsigned long iPageNum,
                 MemStruct *memStruct
                 ){

    // check if there's a free frame, if so return it
    for (int i = 0; i < memStruct->numFrames; ++i){
        if (pageTable[i].inUse == 0){
            return i;
        }
    }

    int frameNum = 99;
    // if not, replace a frame based on the replacement policy
    if (memStruct->policy == RANDOM){
        //randomly choose a frame to replace
        frameNum = rand() % memStruct->numFrames;

        // make sure we're not replacing the instruction address associated with the data
        while (pageTable[frameNum].pageNum == iPageNum){
            frameNum = rand() % memStruct->numFrames;
        }
        return frameNum;
    }
    else if (memStruct->policy == FIFO){
        printf("in FIFO\n");
        int lastIn = 99999999; // set to some egregiously high number
        for (int i = 0; i < memStruct->numFrames; ++i){
            if (pageTable[i].inTime < lastIn && pageTable[i].pageNum != iPageNum){
                lastIn = pageTable[i].inTime;
                frameNum = i;
            }
        }
        return frameNum;
    }
    else if (memStruct->policy == LRU){
        int lastUsed = 99999999; // set to some egregiously high number
        for (int i = 0; i < memStruct->numFrames; ++i){
            if (pageTable[i].useTime < lastUsed && pageTable[i].pageNum != iPageNum){
                lastUsed = pageTable[i].useTime;
                frameNum = i;
            }
        }
        return frameNum;
    }
    else{
        return -1; //something must have gone wrong
    }
}

