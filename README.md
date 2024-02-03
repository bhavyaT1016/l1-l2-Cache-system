Cache Simulator - Part I: One Level Cache

This cache simulator is designed to evaluate different configurations of one-level caches. In this first part, we will simulate an L1 cache with specific requirements and replacement policies.

Cache Replacement Policies

FIFO (First In First Out)

This replacement policy evicts the block that was accessed first in the set when there is no space for an incoming cache block.
It does not consider how often or how many times the block was accessed before.

LRU (Least Recently Used)

This replacement policy discards the least recently used items first.
The cache with an LRU policy keeps track of all accesses to a block and always evicts the block that has been used (or accessed) least recently.
It aims to replace the block that will be re-referenced furthest in the future.

Cache Simulator Interface

Your cache simulator should follow this usage interface:

./first cache_size:n assoc:n cache_policy block_size:n trace_file

cache_size:n: Specifies the total size of the cache in bytes (n should be a power of 2).

assoc:n: Specifies the associativity, where n is the number of cache lines in a set.

cache_policy: Specifies the cache replacement policy, which can be either "fifo" or "lru."

block_size:n: Specifies the size of the cache block in bytes (n should be a power of 2).

trace_file: Specifies the name of the trace file.

Example Usage

./first 8192 assoc:4 fifo block_size:64 trace.txt

./first 4096 assoc:2 lru block_size:32 trace.txt

Note
The cache simulator implements the specified cache configurations and replacement policies to evaluate cache performance.


Cache Simulator - Part II: Two-Level Cache

In this second part of the assignment, we will simulate a system with a two-level cache consisting of an L1 cache and an L2 cache. The L2 cache is exclusive of the L1 cache, meaning it contains only blocks not present in the L1 cache.

Exclusive Cache

In an exclusive cache configuration, the L2 cache contains only blocks that are not present in the L1 cache. Here's how it works:

When a read request for block X is made:

If the block is found in the L1 cache, the data is read from the L1 cache.

If the block is not found in the L1 cache but is present in the L2 cache, the block is moved from the L2 cache to the L1 cache. If this eviction from L2 causes a block to be evicted from L1, the evicted block is then placed into L2.

If the block is not found in either L1 or L2, it is read from main memory and placed only in the L1 cache.

In this exclusive cache configuration, the L2 cache is populated only when a block is evicted from L1, making it serve as a victim cache for L1.

Sample Run

Your program will accept two separate configurations, one for the L1 cache and one for the L2 cache. Both caches have the same block size. Your program should report the total number of memory reads and writes, along with cache misses and hits for both L1 and L2 caches. Here is the format for Part II:

./second L1_cache_size L1_associativity L1_cache_policy L1_block_size L2_cache_size L2_associativity L2_cache_policy trace_file

Example Usage

./second 32 assoc:2 fifo 4 64 assoc:16 lru trace2.txt

Output Format

memread:3277

memwrite:2861

l1cachehit:6501

l1cachemiss:3499

l2cachehit:222

l2cachemiss:3277

The program simulates a 2-way set-associative cache of size 32 bytes with a block size of 4 for L1 cache.

L2 cache is a fully associative cache of size 64 bytes.

The trace file used for this run is trace2.txt.

The program outputs memory reads, memory writes, L1 cache hits, L1 cache misses, L2 cache hits, and L2 cache misses in the specified order.

The provided example demonstrates how to configure and run the cache simulator for a two-level cache system.
