// Assignment 5
// CS 201
// Created by Lars Jensen on 11/28/22.

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

int main(){
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

