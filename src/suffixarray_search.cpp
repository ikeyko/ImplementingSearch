//Test
#include <divsufsort.h>
#include <sstream>

#include <seqan3/std/filesystem>

#include <seqan3/alphabet/nucleotide/dna5.hpp>
#include <seqan3/argument_parser/all.hpp>
#include <seqan3/core/debug_stream.hpp>
#include <seqan3/io/sequence_file/all.hpp>
#include <seqan3/search/fm_index/fm_index.hpp>
#include <seqan3/search/search.hpp>


void find(sauchar_t const* query, const sauchar_t* text, saidx_t *SA, saidx_t m, saidx_t n) {
    
    std::cout<<"start find\n";
    std::vector<uint32_t> hits;
    
    //unsigned n = sa.size() - 1;

    if (n == 0) return;
    std::cout<<"find 1 \n";

    //if the search character not exist in Alphabet
    if (query[0] > text[SA[n]]) return;
    std::cout<<"find 2 \n";
    if (query[0] < text[SA[0]]) return;
    std::cout<<"find 3 \n";

    unsigned Lp, Rp;

    //looking for right interval bound
    int left = 0;
    int right = n+1;
    int middle;

    while (right - left > 1) {
        middle = ceil((left + right)/2);
        if (query[0] >= text[SA[middle]]) left = middle;
        else right = middle;
    }
    std::cout<<"find 4 \n";
    Rp = left;

    //looking for left interval bound
    left = -1;
    right = Rp;

    while (right - left > 1) {
        std::cout<<",";
        middle = ceil((left + right)/2);
        if (query[0] <= text[SA[middle]]) right = middle;
        else left = middle;
    }
    std::cout<<"find 5 \n";
    Lp = right;

    unsigned index = 0; //Suffix index

    while (Rp >= Lp && index < m) { //repeat check until full pattern found. Stop if bounds crossed
        while (Rp >= Lp && query[index] != text[SA[Lp]+index]){ //check left suffix
            ++Lp; //go to next suffix
        }
        //cout<<"Lp= "<<Lp<<endl;
        while (Rp >= Lp && query[index] != text[SA[Rp]+index]) { //check right suffix
            --Rp; //go to previous suffix
        }

        std::cout<<".";
        if (Rp >= Lp) index++; //if check for both suffixes successfull, go to next char
    }
    std::cout<<"find 6 \n";
    while (Rp >= Lp) {
        std::cout<<"find 61 \n";
        hits.push_back(SA[Lp++]); //push every alignment between bounds in vector hits
    }
    std::cout<<"find 7 \n";
    sort(hits.begin(), hits.end());

    if (hits.size() != 0) {
        std::cout<<"find 777 \n";
        std::cout << query << ": ";
        for (unsigned i = 0; i < hits.size(); ++i) {
            std::cout << hits[i] << " ";
        }
    }
    std::cout<<"find 8 \n";
    hits.clear();
}


int main(int argc, char const* const* argv) {
    seqan3::argument_parser parser{"suffixarray_search", argc, argv, seqan3::update_notifications::off};

    parser.info.author = "SeqAn-Team";
    parser.info.version = "1.0.0";

    auto reference_file = std::filesystem::path{};
    parser.add_option(reference_file, '\0', "reference", "path to the reference file");

    auto query_file = std::filesystem::path{};
    parser.add_option(query_file, '\0', "query", "path to the query file");

    try {
         parser.parse();
    } catch (seqan3::argument_parser_error const& ext) {
        seqan3::debug_stream << "Parsing error. " << ext.what() << "\n";
        return EXIT_FAILURE;
    }

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

    //!TODO here adjust the number of searches
    //queries.resize(100); // will reduce the amount of searches

    if (queries.size() > 100) {
        queries.resize(100);    // will reduce the amount of searches
    }

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


    //int n = reference.size();
    int i,j; // for debugging output
    const char* str_source = "pandapapayas";
    //sauchar_t* str = "pandapapayays";
    int n = strlen(str_source);
    sauchar_t const* str = reinterpret_cast<sauchar_t const*>(str_source);
    // allocate
    int *SA = (int *)malloc(n * sizeof(int));

    //sauchar_t const* str = reinterpret_cast<sauchar_t const*>(reference.data());
    // sort
    //divsufsort((unsigned char *)str, SA, n);

    
    divsufsort((sauchar_t*)str, SA, n);

    for (auto& q : queries) {
        //!TODO !ImplementMe apply binary search and find q  in reference using binary search on `suffixarray`
        // You can choose if you want to use binary search based on "naive approach", "mlr-trick", "lcp"
    }

    //std::string text = "pandapapayays";
    //std::string query = "pa";

   

    // output
    for(i = 0; i < n; ++i) {
        printf("SA[%2d] = %2d: ", i, SA[i]);
        for(j = SA[i]; j < n; ++j) {
            printf("%c", str[j]);
        }
        printf("$\n");
    }

    const char* query_source = "pa";
    int m = strlen(query_source);
    sauchar_t const* query = reinterpret_cast<sauchar_t const*>(query_source);
    std::cout<<"start\n";
    find((sauchar_t*)query,(sauchar_t*)str, SA, m, n);

    /*
    find(query, sa, text, hits);

    std::cout << query << ": ";
    for (unsigned i = 0; i < sa.size(); ++i) {
        std::cout << sa[i] << " ";
    }
    */
    //std::cout<<std::endl;

    // deallocate
    free(SA);

    return 0;
}
