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

void deletecache(set* cache, long numsets){
    for (long i = 0; i < numsets; i++){
        free(cache[i].blocks);
    }
}

long findsmallest(block* set, long size){
    long min = __LONG_MAX__;
    long minindex = -1;
    for (long i = 0; i < size; i++){
        if (set[i].valid == 0){
            return i;
        }
        if (set[i].valid != 0 && set[i].time < min){
            min = set[i].time;
            minindex = i;
        }
    }
    return minindex;
}

int isfull(block* set, long size){
    for (long i = 0; i < size; i++){
        if (set[i].valid == 0){
            return 0;
        }
    }
    return 1;
}

void printcache(set* cache, long numsets, long numblocks){
    for (long i = 0; i < numsets; i++){
        for (long j = 0; j < numblocks; j++){
            printf("\t[%ld] [%ld] [%ld]\n", cache[i].blocks[j].valid, cache[i].blocks[j].tag, cache[i].blocks[j].time);
        }
    }
}

int main(int argc, char *argv[argc + 1]){
    // make the chache
    long memread = 0;
    long memwrite = 0;
    long l1cachemiss = 0;
    long l1cachehit = 0;
    long l2cachemiss = 0;
    long l2cachehit = 0;

    //cache size for L1
    long cachesizeL1 = atol(argv[1]);

    //associativity for L1 cache
    long assocL1 = 0;
    long lengthL1 = strlen(argv[2]);
    for (long i = 6; i < lengthL1; i++){
        assocL1 = 10 * assocL1 + (argv[2][i] - '0');
    }

    //policy for L1 cache 
    int isfifoL1 = 0;
    char *policyL1 = argv[3];
    if (policyL1[0] == 'f'){
        isfifoL1 = 1;
    }
    
    //block for both of the caches
    long blocksize = atol(argv[4]); //this is in bytes
    long blockoffsetbits = (log10(blocksize) / log10(2)); //need this in bits

    //cache size for L2
    long cachesizeL2 = atol(argv[5]);

    //associativity for L2 cache
    long assocL2 = 0;
    long lengthL2 = strlen(argv[6]);
    for (long i = 6; i < lengthL2; i++){
        assocL2 = 10 * assocL2 + (argv[6][i] - '0');
    }


    //creating the cache for L1
    long numsetsL1 = cachesizeL1 / (blocksize * assocL1);
    set* cacheL1 = malloc(numsetsL1  * sizeof(set));
    for (long i = 0; i < numsetsL1; i++){
        cacheL1[i].blocks = malloc(assocL1 * sizeof(block));
        for (long j = 0; j < assocL1; j++){
            cacheL1[i].blocks[j].tag = 0;
            cacheL1[i].blocks[j].valid = 0;
            cacheL1[i].blocks[j].time = 0;
        }
    }

    //creating the cache for L2
    long numsetsL2 = cachesizeL2 / (blocksize * assocL2);
    set* cacheL2 = malloc(numsetsL2  * sizeof(set));
    for (long i = 0; i < numsetsL2; i++){
        cacheL2[i].blocks = malloc(assocL2 * sizeof(block));
        for (long j = 0; j < assocL2; j++){
            cacheL2[i].blocks[j].tag = 0;
            cacheL2[i].blocks[j].valid = 0;
            cacheL2[i].blocks[j].time = 0;
        }
    }



    printf(
        "For CacheL1:\n"
        "cachesize: %ld\n" 
        "blocksize: %ld\n"
        "assoc: %ld\n"
        "policy: %s\n"
        "numberofsets: %ld\n\n", cachesizeL1, blocksize, assocL1, policyL1, numsetsL1
    );

    printf(
        "For CacheL2:\n"
        "cachesize: %ld\n" 
        "blocksize: %ld\n"
        "assoc: %ld\n"
        "numberofsets: %ld\n\n", 
        cachesizeL2, blocksize, assocL2, numsetsL2
    );


    FILE *fp = fopen(argv[8], "r");
    char action[256];
    unsigned long num;
    long time = 1;
    while (fscanf(fp, "%s %lx", action, &num) != EOF){
        //L1
        long setbitsL1 = (log10(numsetsL1) / log10(2));
        // printf("block ofsetbits %ld\n", blockoffsetbits);
        unsigned long tagnumL1 = num >> (blockoffsetbits + setbitsL1);
        unsigned long indexL1 = num >> blockoffsetbits;
        indexL1 = indexL1 & ((1 << setbitsL1) - 1);
        int isinputtedL1 = 0;
        //L2
        long setbitsL2 = (log10(numsetsL2) / log10(2));
        unsigned long tagnumL2 = num >> (blockoffsetbits + setbitsL2);
        unsigned long indexL2 = num >> blockoffsetbits;
        indexL2 = indexL2 & ((1 << setbitsL2) - 1);
        int isfoundL2 = 0;
        // printf("%s %lx\n", action, num);
        // printf(
        //     "for cacheL1:\n"
        //     "setbits:%ld\n"
        //     "tagnum:%lu\n"
        //     "index: %lu\n\n",
        //     setbitsL1, tagnumL1, indexL1
        // );

        // printf(
        //     "for cacheL2:\n"
        //     "setbits:%ld\n"
        //     "tagnum:%lu\n"
        //     "index: %lu\n\n\n",
        //     setbitsL2, tagnumL2, indexL2
        // );

        for(long i = 0; i < assocL1; i++){
            if (cacheL1[indexL1].blocks[i].valid == 0){
                l1cachemiss++;
                l2cachemiss++;
                memread++;
                cacheL1[indexL1].blocks[i].tag = tagnumL1;
                cacheL1[indexL1].blocks[i].valid = 1;
                cacheL1[indexL1].blocks[i].time = time;
                time++;
                isinputtedL1 = 1;
                break;
            }else if (cacheL1[indexL1].blocks[i].tag == tagnumL1){
                l1cachehit++;
                if (isfifoL1 == 0){
                    cacheL1[indexL1].blocks[i].time = time;
                    time++;
                }
                isinputtedL1 = 1;
                break;
            }
        }
        //at this point the address has either been an L1 cache miss or hit
        //if miss now check L2 for the address
        unsigned long tempadd = 0;
        unsigned long temptag = 0;
        unsigned long tempindL2 = 0;
        unsigned long temptagL2 = 0;

        if (isinputtedL1 == 0){
            l1cachemiss++;
            for(long i = 0; i < assocL2; i++){
                if (cacheL2[indexL2].blocks[i].valid !=0 && cacheL2[indexL2].blocks[i].tag == tagnumL2){
                    l2cachehit++;
                    cacheL2[indexL2].blocks[i].valid = 0;
                    cacheL2[indexL2].blocks[i].tag = 0;
                    cacheL2[indexL2].blocks[i].time = 0;
                    isfoundL2 = 1;
                    break;
                }
            }
            if (isfoundL2 == 1){
                long removalindexL1 = findsmallest(cacheL1[indexL1].blocks, assocL1);
                temptag = cacheL1[indexL1].blocks[removalindexL1].tag;
                tempadd = (temptag << setbitsL1) + indexL1;
                tempindL2 = tempadd & ((1 << setbitsL2) - 1);
                temptagL2 = tempadd >> setbitsL2;

                long inputindexL2 = findsmallest(cacheL2[tempindL2].blocks, assocL2);
                cacheL2[tempindL2].blocks[inputindexL2].tag = temptagL2;
                cacheL2[tempindL2].blocks[inputindexL2].valid = 1;
                cacheL2[tempindL2].blocks[inputindexL2].time = time;

                cacheL1[indexL1].blocks[removalindexL1].tag = tagnumL1;
                cacheL1[indexL1].blocks[removalindexL1].valid = 1;
                cacheL1[indexL1].blocks[removalindexL1].time = time;
                time++;
            }
        }
        //at the point its either hit L1
        //or miss L1 and hit L2
        //so now this case is when its miss L1 and L2
        if (isinputtedL1 == 0 && isfoundL2 == 0){
            memread ++;
            l2cachemiss++;
            long removalindexL1 = findsmallest(cacheL1[indexL1].blocks, assocL1);
            temptag = cacheL1[indexL1].blocks[removalindexL1].tag;
            tempadd = (temptag << setbitsL1) + indexL1;
            temptagL2 = tempadd >> setbitsL2;
            tempindL2 = tempadd & ((1 << setbitsL2) - 1);
            long inputindexL2 = findsmallest(cacheL2[tempindL2].blocks, assocL2);
            cacheL2[tempindL2].blocks[inputindexL2].tag = temptagL2;
            cacheL2[tempindL2].blocks[inputindexL2].valid = 1;
            cacheL2[tempindL2].blocks[inputindexL2].time = time;
       

            cacheL1[indexL1].blocks[removalindexL1].tag = tagnumL1;
            cacheL1[indexL1].blocks[removalindexL1].valid = 1;
            cacheL1[indexL1].blocks[removalindexL1].time = time;
            time++;
        }


        if (strcmp(action, "W") == 0){
            memwrite++;
        } 

        // printcache(cacheL1, numsetsL1, assocL1);
        // printf("\n");
        // printcache(cacheL2, numsetsL2, assocL2);    
        // printf("\n");
    }
    


    printf(
        "memread:%ld\n"
        "memwrite:%ld\n"
        "l1cachehit:%ld\n"
        "l1cachemiss:%ld\n"
        "l2cachehit:%ld\n"
        "l2cachemiss:%ld\n",
        memread, memwrite, l1cachehit, l1cachemiss, l2cachehit, l2cachemiss
    );

    deletecache(cacheL1, numsetsL1);
    deletecache(cacheL2, numsetsL2);
    free(cacheL2);
    free(cacheL1);
}