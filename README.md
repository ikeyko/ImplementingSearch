# ImplementingSearch

This is a demo/homework to demonstrate the power of SuffixArray and FM-Index based search.

## Usage
You can either start a gitpod instance (very easy):
  + https://gitpod.io/#https://github.com/SGSSGene/ImplementingSearch

Or you can download this repo (very cool):
  + `git clone --recurse-submodules https://github.com/SGSSGene/ImplementingSearch`

## How to build the software
```
$ # We are assuming you are in the terminal/console inside the repository folder
$ mkdir build # creates a folder for our build system
$ cd build
$ cmake ..    # generates our build system
$ make        # actually builds our softare, repeat this command to recompile your software
$ ./bin/naive_search --reference ../data/hg38_partial.fasta.gz --query ../data/illumina_reads_40.fasta.gz       # calls the code in src/naive_search.cpp
$ ./bin/suffixarray_search --reference ../data/hg38_partial.fasta.gz --query ../data/illumina_reads_40.fasta.gz # calls the code in src/suffixarray_search.cpp
```


## What to do?
This demonstration is supposed to show you the power of the FM-Index.
To fully feel the power you are supposed to compare a naively implemented search
to an fm-index based search.

1. Naive Search:
  + Check out the `src/naive_search.cpp` file. Fill in the `//!TODO ImplementMe`.
  + Look for `//!TODO !CHANGEME here adjust the number of searches` to play around with different number
    of searches.
  + Run `./bin/naive_search` for different query sizes and measure the time.
2. SuffixArray Search:
  + Check out `src/suffixarray_search.cpp`. Fill in the `//!TODO !ImplementMe`
  + Look for `//!TODO here adjust the number of searches` to play around with different number of searches.
  + Run `./bin/suffixarray_search` for dfferent query sizes and measure the time.
3. FMIndex Search:
  + Check out `src/fmindex_construct.cpp` (nothing to do here). This builds an fm-index for you.
  + Run `./bin/fmindex_construct` to build an fmindex. (It is saved as `our_index.index`)
  + Check out `src/fmindex_search.cpp`. Fill in the `//!TODO !ImplementMe use the seqan3::search function to search`.
  + Look for `//!TODO here adjust the number of searches` to play around with different number of searches.
  + Run `./bin/fmindex_search` for different query sizes and measure the time.
4. Which search is faster?
  + Check different query lengths: 40, 60, 80 and 100.
5. Which search is more memory efficient?
  + Check different query lengths: 40, 60, 80, 100.

### Hints:
  + Look at this tutorial for more information on how seqan3 and the fmindex works: https://docs.seqan.de/seqan/learning-resources/fm_index.html
  + Another tutoril on the FMIndex in seqan3: https://docs.seqan.de/seqan/3-master-user/tutorial_index_search.html
  + Details on how to use the seqan3::search function https://docs.seqan.de/seqan/3-master-user/group__search.html#ga886f9c0ebd9f12aa12cc73629062241e
  + For memory usage, use `/usr/bin/time -v ./yourprogram` and look at "Maximum resident set size".
