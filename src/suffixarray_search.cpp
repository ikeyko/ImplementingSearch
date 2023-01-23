//Test
#include <divsufsort.h>
#include <sstream>

#include <chrono>

#include <seqan3/std/filesystem>

#include <seqan3/alphabet/nucleotide/dna5.hpp>
#include <seqan3/argument_parser/all.hpp>
#include <seqan3/core/debug_stream.hpp>
#include <seqan3/io/sequence_file/all.hpp>
#include <seqan3/search/fm_index/fm_index.hpp>
#include <seqan3/search/search.hpp>

using namespace std::chrono;

void find(sauchar_t const* query, const sauchar_t* text, saidx_t *SA, saidx_t m, saidx_t n) {
    unsigned index = 0; //Suffix index

     bool found = false;
    
    // vector with positions
    //std::vector<uint32_t> hits;

    if (n == 0) return;

    //if the search character not exist in Alphabet
    if (query[0] > text[SA[n]]) return;
    if (query[0] < text[SA[0]]) return;


    //unsigned Lp, Rp;

    //looking for right interval bound
    int left = 0;
    int right = n;
    int middle;

    int Lp = 0;
    int Rp = n;


    while (Rp >= Lp && index < m) { //repeat check until full pattern found. Stop if bounds crossed

        left = Lp - 1;
        right = Rp;

        while (right - left > 1) {
            middle = ceil((left + right)/2);
            if (query[index] <= text[SA[middle]+index]) right = middle;
            else left = middle;
        }

        Lp = right;
        left = Lp;
        right = Rp + 1;

        while (right - left > 1) {
            middle = ceil((left + right)/2);
            if (query[index] >= text[SA[middle]+index]) left = middle;
            else right = middle;
        }

        Rp = left;

        //looking for left interval bound
        //left = Lp;
        //right = Rp;
        if (text[SA[Lp] + index] != query[index]) {Lp = n+1;} 
        if (text[SA[Rp] + index] != query[index]) {Rp = -1;} 

        if (Rp >= Lp) index++; //if check for both suffixes successfull, go to next char
    }

    while (Rp >= Lp) {
        if (!found) {
                found = true;
                std::cout << "found it at the position(s): ";
                }
        std::cout << SA[Lp++]<<" ";
    }

    if(!found) std::cout << "couldn't find it."; 
    std::cout<<"\n";

}


int main(int argc, char const* const* argv) {
    seqan3::argument_parser parser{"suffixarray_search", argc, argv, seqan3::update_notifications::off};

    parser.info.author = "SeqAn-Team";
    parser.info.version = "1.0.0";

    auto reference_file = std::filesystem::path{};
    parser.add_option(reference_file, '\0', "reference", "path to the reference file");

    auto query_file = std::filesystem::path{};
    parser.add_option(query_file, '\0', "query", "path to the query file");

    size_t query_size{};
    size_t query_size_default = 100;
    parser.add_option(query_size, 's', "query-size", "Size of query vector");

    try {
         parser.parse();
    } catch (seqan3::argument_parser_error const& ext) {
        seqan3::debug_stream << "Parsing error. " << ext.what() << "\n";
        return EXIT_FAILURE;
    }

    if ( ! (query_size > 100 && query_size <= 1000000 ) )  query_size = query_size_default;

    // loading our files
    auto reference_stream = seqan3::sequence_file_input{reference_file};
    auto query_stream     = seqan3::sequence_file_input{query_file};
    
    // read reference into memory
    // Attention: we are concatenating all sequences into one big combined sequence
    //            this is done to simplify the implementation of suffix_arrays
    std::vector<seqan3::dna5> reference;
    for (auto& record : reference_stream) {
        auto r = record.sequence();
        reference.insert(reference.end(), r.begin(), r.end());
    }

    // read query into memory
    std::vector<std::vector<seqan3::dna5>> queries;
    for (auto& record : query_stream) {
        queries.push_back(record.sequence());
    }

    std::cout<<"Start SA with query size = "<<query_size<<"\n";


    // suffix array construction

    // Array that should hold the future suffix array
    //!!!! We don#t need it because we use the memory allocation
    //std::vector<saidx_t> suffixarray;
    //suffixarray.resize(reference.size()); // resizing the array, so it can hold the complete SA

    //!TODO !ImplementMe implement suffix array sort
    //Hint, if can use libdivsufsort (already integrated in this repo)
    //      https://github.com/y-256/libdivsufsort
    //      To make the `reference` compatible with libdivsufsort you can simply
    //      cast it by calling:
    //      `sauchar_t const* str = reinterpret_cast<sauchar_t const*>(reference.data());`



    int n = reference.size();
    sauchar_t const* ref = reinterpret_cast<sauchar_t const*>(reference.data());
    int *SA = (int *)malloc(n * sizeof(int));
    

    // Get starting timepoint
    auto start = high_resolution_clock::now();
    //sa sort
    divsufsort((sauchar_t*)ref, SA, n+1);
    // Get ending timepoint
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    
    std::cout << "Time taken by SA construction: "
         << duration.count() << " microseconds" << "\n";


    // suffix array search

    // adjust queries vector size
    std::vector<std::vector<seqan3::dna5>> queries_temp; 
    if ( queries.size() > query_size ) {
        queries.resize(query_size); 
    } else {
        queries_temp.insert(queries_temp.end(), queries.begin(), queries.end());
        for (int i = 1; i<=10; ++i) {
            queries.insert(queries.end(), queries_temp.begin(), queries_temp.end());
            if ( queries.size() >= query_size ) i = 99;
        }
        queries_temp.clear();
    }
    
    /*
    // for progress bar
    int iPercent = 0;
    int iPercentShow = -1;
    int iCounter = 0;
    */
    
    queries.resize(query_size);
    start = high_resolution_clock::now();
    for (auto& q : queries) {
        //iCounter++; // for progress bar
        //!TODO !ImplementMe apply binary search and find q  in reference using binary search on `suffixarray`
        // You can choose if you want to use binary search based on "naive approach", "mlr-trick", "lcp"
        int m = q.size();
        seqan3::debug_stream << q << ": ";
        sauchar_t const* query = reinterpret_cast<sauchar_t const*>(q.data());
        // search function
        find((sauchar_t*)query,(sauchar_t*)ref, SA, m, n);
        // progress bar
        /*
        iPercent = (int)((static_cast<float>(iCounter) / query_size) * 100);
        if (iPercent > iPercentShow) {
            std::cout << iPercent << "% " << std::flush;
            iPercentShow += 5;
        }*/
    } 
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    //
    std::cout << "\n" << "Time taken by SA search in " << queries.size() << " queries: "
        << duration.count() << " microseconds" << "\n";
   
    
    // deallocate
    free(SA);

    return 0;
}
