#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct block
{
    unsigned long valid;
    unsigned long tag;
    unsigned long time;
} block;

typedef struct set
{
    block *blocks;
} set;

void printcache(set* cache, long numsets, long numblocks){
    for (long i = 0; i < numsets; i++){
        printf("---------------------------\n");
        for (long j = 0; j < numblocks; j++){
            printf("\t---------------------------\n");
            printf("\t[%ld] [%ld] [%ld]\n", cache[i].blocks[j].valid, cache[i].blocks[j].tag, cache[i].blocks[j].time);
            printf("\t---------------------------\n");
        }
        printf("---------------------------\n");
    }
}

long findsmallest(block* set, long size){
    long min = __LONG_MAX__;
    long minindex = -1;
    for (long i = 0; i < size; i++){
        if (set[i].valid != 0 && set[i].time < min){
            min = set[i].time;
            minindex = i;
        }
    }
    return minindex;
}

void deletecache(set* cache, long numsets){
    for (long i = 0; i < numsets; i++){
        free(cache[i].blocks);
    }
}

int main(int argc, char *argv[argc + 1]){
    // make the chache
    long memread = 0;
    long memwrite = 0;
    long cachemiss = 0;
    long cachehit = 0;

    //cache size
    long cachesize = atol(argv[1]);

    // associativity
    long assoc = 0;
    long length = strlen(argv[2]);
    for (long i = 6; i < length; i++){
        assoc = 10 * assoc + (argv[2][i] - '0');
    }

    //policy
    int isfifo = 0;
    char *policy = argv[3];
    if (policy[0] == 'f'){
        isfifo = 1;
    }
    
    //block
    long blocksize = atol(argv[4]); //this is in bytes
    long blockoffsetbits = (log10(blocksize) / log10(2)); //need this in bits

    long numsets = cachesize / (blocksize * assoc);
    set* cache = malloc(numsets  * sizeof(set));
    for (long i = 0; i < numsets; i++){
        cache[i].blocks = malloc(assoc * sizeof(block));
        for (long j = 0; j < assoc; j++){
            cache[i].blocks[j].tag = 0;
            cache[i].blocks[j].valid = 0;
            cache[i].blocks[j].time = 0;
        }
    }

    printf("cachesize: %ld, blocksize: %ld, assoc: %ld, policy: %s, numberofsets: %ld \n",cachesize, blocksize, assoc, policy, numsets);


    FILE *fp = fopen(argv[5], "r");
    char action[256];
    unsigned long num;
    long time = 1;
    while (fscanf(fp, "%s %lx", action, &num) != EOF){
        long setbits = (log10(numsets) / log10(2));
        unsigned long tagnum = num >> (blockoffsetbits + setbits);
        unsigned long index = num >> blockoffsetbits;
        index = index & ((1 << setbits) - 1);
        int isinputted = 0;
        printf("%s %lx tagnum: %lu index: %lu\n", action, num, tagnum, index);
        for(long i = 0; i < assoc; i++){
            if (cache[index].blocks[i].valid == 0){
                cachemiss++;
                memread++;
                cache[index].blocks[i].tag = tagnum;
                cache[index].blocks[i].valid = 1;
                cache[index].blocks[i].time = time;
                time++;
                isinputted = 1;
                break;
            }else if (cache[index].blocks[i].tag == tagnum){
                cachehit++;
                if (isfifo == 0){
                    cache[index].blocks[i].time = time;
                    time++;
                }
                isinputted = 1;
                break;
            }
        }

        //the address is not already in the cache i.e. cache miss
        if (isinputted == 0){
            long removeindex = findsmallest(cache[index].blocks, assoc);
            cachemiss++;
            memread++;
            cache[index].blocks[removeindex].tag = tagnum;
            cache[index].blocks[removeindex].valid = 1;
            cache[index].blocks[removeindex].time = time;
            time++;
            isinputted = 1;
            printf("index in the block to remove is %ld\n", removeindex);
        }
  
        if (strcmp(action, "W") == 0){
            memwrite++;
        }
        //printcache(cache, numsets, assoc);
    }

    printf(
        "memread:%ld\n"
        "memwrite:%ld\n"
        "cachehit:%ld\n"
        "cachemiss:%ld\n",
        memread, memwrite, cachehit, cachemiss
    );

    deletecache(cache, numsets);
    free(cache);

}