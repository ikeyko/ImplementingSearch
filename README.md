# ImplementingSearch

This is an exercise to demonstrate the power of Suffix-Array and FM-Index based searching.

## Usage
For FU-Berlin students I recommend looking [ssh tutorial](https://github.com/seqan/seqan3/wiki/SSH) to log into our compute servers. I recommend using compute12.mi.fu-berlin.de. Also for longer running processes I can advice looking at this [tmux tutorial](https://github.com/seqan/seqan3/wiki/tmux) for some basic instructions.

### Cloning (very cool)
To checkout the code you can run:
  + `git clone --recurse-submodules https://github.com/SGSSGene/ImplementingSearch`
  
### gitpod:
Lets say you are not an fu-student and don't have access to an linux machine, you can also use gitpod, which provides an online ide:
  + https://gitpod.io/#https://github.com/SGSSGene/ImplementingSearch


## How to build the software
```
$ # We are assuming you are in the terminal/console inside the repository folder
$ mkdir build # creates a folder for our build system
$ cd build
$ cmake ..    # configures our build system
$ make        # builds our software, repeat this command to recompile your software
$ ./bin/naive_search --reference ../data/hg38_partial.fasta.gz --query ../data/illumina_reads_40.fasta.gz       # calls the code in src/naive_search.cpp
$ ./bin/suffixarray_search --reference ../data/hg38_partial.fasta.gz --query ../data/illumina_reads_40.fasta.gz # calls the code in src/suffixarray_search.cpp

$ ./bin/fmindex_construct --reference ../data/hg38_partial.fasta.gz --index myIndex.index # creates an index, see src/fmindex_construct.cpp
$ ./bin/fmindex_search --index myIndex.index --query ../data/illumina_reads_40.fasta.gz   # searches by using the fmindex, see src/fmindex_search.cpp
```


## What to do?
This demonstration is supposed to show you the power of the FM-Index.
To fully feel the power you are supposed to compare a naively implemented search
to an fm-index based search.

1. Naive Search:
  + Check out the `src/naive_search.cpp` file. Fill in the `//!TODO ImplementMe`.
  + Look for `//!TODO !CHANGEME. Adjust the number of searches` to play around with a different number
    of searches.
  + Run `./bin/naive_search` for different query sizes and measure the time.
2. SuffixArray Search:
  + Check out `src/suffixarray_search.cpp`. Fill in the `//!TODO !ImplementMe`
  + Look for `//!TODO here adjust the number of searches` to play around with a different number of searches.
  + Run `./bin/suffixarray_search` for dfferent query sizes and measure the time.
3. FMIndex Search:
  + Check out `src/fmindex_construct.cpp` (nothing to do here). This builds an fm-index for you.
  + Run `./bin/fmindex_construct` to build an fmindex. (It is saved as `our_index.index`)
  + Check out `src/fmindex_search.cpp`. Fill in the `//!TODO !ImplementMe use the seqan3::search function to search`.
  + Look for `//!TODO here adjust the number of searches` to play around with a different number of searches.
  + Run `./bin/fmindex_search` for different query sizes and measure the time.
4. Which search is faster?
  + Check different query lengths: 40, 60, 80 and 100.
5. Which search is more memory efficient?
  + Check different query lengths: 40, 60, 80, 100.
6. FMIndex with errors:
   Use the pigeon hole principle to search for stuff with up to 2 errors.
   Very free programming exercise.

### Hints:
  + Look at this tutorial for more information on how seqan3 and the fmindex works: https://docs.seqan.de/seqan/learning-resources/fm_index.html
  + Another tutorial on the FMIndex in seqan3: https://docs.seqan.de/seqan/3-master-user/tutorial_index_search.html
  + Details on how to use the seqan3::search function https://docs.seqan.de/seqan/3-master-user/group__search.html#ga886f9c0ebd9f12aa12cc73629062241e
  + For memory usage, use `/usr/bin/time -v ./yourprogram` and look at "Maximum resident set size".
