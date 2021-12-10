# ImplementingSearch

This is a short demo/homework to demonstrate the power of FM-Index based search.

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
$ ./bin/naive_search       # calls the code in src/naive_search.cpp
$ ./bin/fmindex_construct  # constructs a fm_index that can be used by fmindex_search
$ ./bin/fmindex_search     # calls the code in src/fmindex_search.cpp
$ ./bin/suffixarray_search # calls the code in src/suffixarray_search.cpp
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
2. FMIndex Search:
  + Check out `src/fmindex_construct.cpp` (nothing to do here). This builds an fm-index for you.
  + Run `./bin/fmindex_construct` to build an fmindex. (It is saved as `our_index.index`)
  + Check out `src/fmindex_search.cpp`. Fill in the `//!TODO !ImplementMe use the seqan3::search function to search`.
  + Look for `//!TODO here adjust the number of searches` to play around with different number of searches.
  + Run `./bin/fmindex_search` for different query sizes and measure the time.
3. SuffixArray Search:
  + Check out `src/suffixarray_search.cpp`. Fill in the `//!TODO !ImplementMe`
  + Look for `//!TODO here adjust the number of searches` to play around with different number of searches.
  + Run `./bin/suffixarray_search` for dfferent query sizes and measure the time.
4. Which search is faster, and by how much?
  + Write down the times for queries of size: 100 1,000 10,000 100,000 500,000 1,000,000

### Hints:
  + Look at this tutorial for more information on how seqan3 and the fmindex works: https://docs.seqan.de/seqan/learning-resources/fm_index.html
  + Another tutoril on the FMIndex in seqan3: https://docs.seqan.de/seqan/3-master-user/tutorial_index_search.html
  + Details on how to use the seqan3::search function https://docs.seqan.de/seqan/3-master-user/group__search.html#ga886f9c0ebd9f12aa12cc73629062241e
