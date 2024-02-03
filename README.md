Part I: One Level Cache
  You will implement a cache simulator to evaluate different configurations of caches. The followings
  are the requirements for the first part of the cache simulator.
    • Simulate only one level cache; i.e., an L1 cache.
    • The cache size, associativity, the replacement policy, and the block size are input parameters.
    Cache size and block size are specified in bytes.
    • You have to simulate a write through cache.
    • Replacement algorithm: You have to support two replacement policies. The two replacement
    policies are: First In First Out (FIFO) and Least Recently Used (LRU). Next, you will learn
    more about cache replacement policies.
    Cache Replacement Policies
  The goal of the cache replacement policy is to decide which block has to be evicted in case there is no
  space in the set for an incoming cache block. It is always preferable – to achieve the best performance
  – to replace the block that will be re-referenced furthest in the future. In this assignment, you will
  use two different ways to implement the cache replacement policy: FIFO and LRU.
FIFO
  When the cache uses the FIFO replacement policy, it always evicts the block accessed first in the
  set without considering how often or how many times the block was accessed before. So let us say
  that your cache is empty initially and that each set has two ways. Now suppose that you access
  blocks A, B, A, C. To make room for C, you would evict A since it was the first block to be brought
  into the set.
LRU
  When the cache used the LRU replacement policy, it discards the least recently used items first.
  The cache with an LRU policy has to keep track of all accesses to a block and always evict the
  block that been used (or accessed) least recently as the name suggests.
  Cache Simulator Interface
  You have to name your cache simulator first. Your program should support the following usage
  interface:
    ./first <cachesize><assoc:n><cache policy><block size><trace file>
    where:
    • The parameter cache size is the total size of the cache in bytes. This number should be a
    power of 2.
    • The parameter assoc:n specifies the associativity. Here, n is a number of cache lines in a set.
    • The parameter cache policy specifies the cache replacement policy, which is either fifo or
    lru.
    • The parameter block size is a power of 2 that specifies the size of the cache block in bytes.
    • The parameter trace file is the name of the trace file.

Simulation Details
  • (a) When your program starts, there is nothing in the cache. So, all cache lines are empty.
  (b) you can assume that the memory size is 248 . Therefore, memory addresses are at most
  48 bit (zero extend the addresses in the trace file if they are less than 48-bit in length). (c)
  the number of bits in the tag, cache address, and byte address are determined by the cache
  size and the block size;
  • For a write-through cache, there is the question of what should happen in case of a write
  miss. In this assignment, the assumption is that the block is first read from memory (i.e.,
  one memory read), and then followed by a memory write.
  • You do not need to simulate data in the cache and memory in this assignment. Because,
  the trace does not contain any information on data values transferred between memory and
  caches.
Sample Run
  Your program should print out the number of memory reads (per cache block), memory writes (per
  cache block), cache hits, and cache misses. You should follow the exact same format shown below
  (no space between letters), otherwise, the autograder can not grade your program properly.
  $./first 32 assoc:2 fifo 4 trace1.txt
  memread:336
  memwrite:334
  cachehit:664
  cachemiss:336
  The above example, simulates a 2-way set associate cache of size 32 bytes. Each cache block is 4
  bytes. The trace file name is trace1.txt.
  Note: Some of the trace files are quite large. So it might take a few minutes for the autograder to
  grade all testcases.


Part II: Two Level Cache
  Most modern CPUs have multiple level of caches. In the second part of the assignment, you have
  to simulate a system with a two-level of cache (i.e. L1 and L2). Multi-level caches can be designed
  in various ways depending on whether the content of one cache is present in other levels or not.
  In this assignment you implement an exclusive cache: the lower level cache (i.e. L2) contains only
  blocks that are not present in the upper level cache (i.e. L1).
  
  Exclusive Cache
    Consider the case when L2 is exclusive of L1. Suppose there is a read request for block X. If the
    block is found in L1 cache, then the data is read from L1 cache. If the block is not found in the
    L1 cache, but present in the L2 cache, then the cache block is moved from the L2 cache to the L1 
    cache. If this causes a block to be evicted from L1, the evicted block is then placed into L2. If the
    block is not found in either L1 or L2, then it is read from main memory and placed just in L1 and
    not in L2. In the exclusive cache configuration, the only way L2 gets populated is when a block is
    evicted from L1. Hence, the L2 cache in this configuration is also called a victim cache for L1.

  Sample Run
    The details from Part 1 apply here to the second level L2 cache. Your program gets two separate
    configurations (one for level 1 and one for level 2 cache). Both L1 and L2 have the same block size.
    Your program should report the total number of memory reads and writes, followed by cache miss
    and hit for L1 and L2 cache. Here is the format for part 2.
      ./second <L1 cache size><L1 associativity><L1 cache policy><L1 block size> <L2 cache size><L2 associativity><L2 cache policy>< trace file>
      This is an example testcase for part 2.
      ./second 32 assoc:2 fifo 4 64 assoc:16 lru trace2.txt
      memread:3277
      memwrite:2861
      l1cachehit:6501
      l1cachemiss:3499
      l2cachehit:222
      l2cachemiss:3277
      The above example, simulates a 2-way set associate cache of size 32 bytes. bytes with block size of
      4 for L1 cache. Similarly, L2 cache is a fully associate cache of size 64 bytes. Further, the trace file
      used for this run is trace2.txt. As you can see, the program outputs the memory read and memory
      writes followed by the L1 and L2 cache hits and misses in the order shown above.
