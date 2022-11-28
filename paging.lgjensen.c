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

