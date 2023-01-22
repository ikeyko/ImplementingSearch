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


void find(sauchar_t const* query, const sauchar_t* text, saidx_t *SA, saidx_t m, saidx_t n, std::vector<seqan3::dna5>& reference, std::vector<seqan3::dna5>& q) {
    
    bool found = false;
    
    std::vector<uint32_t> hits;
    
    //unsigned n = sa.size() - 1;

    if (n == 0) return;

    //if the search character not exist in Alphabet
    if (query[0] > text[SA[n]]) return;
    if (query[0] < text[SA[0]]) return;


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

    Rp = left;

    //looking for left interval bound
    left = -1;
    right = Rp;

    while (right - left > 1) {
        middle = ceil((left + right)/2);
        if (query[0] <= text[SA[middle]]) right = middle;
        else left = middle;
    }

    

    Lp = right;

    unsigned index = 0; //Suffix index


    std::cout<<"Rp: "<<Rp<<" Lp: "<<Lp<<" index: "<<index<<"\n";

    while (Rp >= Lp && index < m) { //repeat check until full pattern found. Stop if bounds crossed
        std::cout<<"index: "<<index<<"\n";
        while (Rp >= Lp && query[index] != text[SA[Lp]+index]){ //check left suffix
            ++Lp; //go to next suffix
            
        }
         std::cout<<"Rp: "<<Rp<<" Lp: "<<Lp<<" index: "<<index<<"\n";
        //cout<<"Lp= "<<Lp<<endl;
        while (Rp >= Lp && query[index] != text[SA[Rp]+index]) { //check right suffix
            --Rp; //go to previous suffix
            
        }
         std::cout<<"Rp: "<<Rp<<" Lp: "<<Lp<<" index: "<<index<<"\n";
        if (Rp >= Lp) index++; //if check for both suffixes successfull, go to next char
        
        std::cout<<"\n";std::cout<<"\n";
        std::cout<<"L: ";
        for (int i = Lp; i<Lp+m; ++i) {
            seqan3::debug_stream << reference[i];
        }
        std::cout<<"\n";
        std::cout<<"Q: ";
        for (int i = 0; i<m; ++i) {

            seqan3::debug_stream << q[i];
        }
        std::cout<<"\n";
        std::cout<<"R: ";
        for (int i = Rp; i<Rp+m; ++i) {
            seqan3::debug_stream << reference[i];
        }
        std::cout<<"\n";

    }
    //std::cout<<"Rp: "<<Rp<<" Lp: "<<Lp<<" index: "<<index<<"\n";
    while (Rp >= Lp) {

        hits.push_back(SA[Lp++]); //push every alignment between bounds in vector hits
    }
    std::cout<<"hits size: "<<hits.size()<<"\n";
/*
    sort(hits.begin(), hits.end());

    if (hits.size() != 0) {
        if (!found) {
                found = true;
                std::cout << "found it at the position(s):";
                }
        for (unsigned i = 0; i < hits.size(); ++i) {
            std::cout << " " << hits[i] ;
        }
    }
    if(!found) std::cout << "couldn't find it."; 
    std::cout<<"\n";
    */
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



    // BEGIN TEST WITH PANDAPAPAYAS
    //int i,j; // for debugging output
    //const char* str_source = "pandapapayas";
    //int n = strlen(str_source);
    //sauchar_t const* str = reinterpret_cast<sauchar_t const*>(str_source);
    //int *SA = (int *)malloc(n * sizeof(int));
    //divsufsort((sauchar_t*)str, SA, n);
    // output
    /*
    for(i = 0; i < n; ++i) {
        printf("SA[%2d] = %2d: ", i, SA[i]);
        for(j = SA[i]; j < n; ++j) {
            printf("%c", str[j]);
        }
        printf("$\n");
    }
    //test search
    const char* query_source = "pa";
    int m = strlen(query_source);
    sauchar_t const* query = reinterpret_cast<sauchar_t const*>(query_source);
    find((sauchar_t*)query,(sauchar_t*)str, SA, m, n);
    */
   //END TEST


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
    divsufsort((sauchar_t*)ref, SA, n);



    // suffix array search
    for (auto& q : queries) {
        //!TODO !ImplementMe apply binary search and find q  in reference using binary search on `suffixarray`
        // You can choose if you want to use binary search based on "naive approach", "mlr-trick", "lcp"
        int m = q.size();
        seqan3::debug_stream << q << ": ";
        sauchar_t const* query = reinterpret_cast<sauchar_t const*>(q.data());
        find((sauchar_t*)query,(sauchar_t*)ref, SA, m, n, reference,q);
    }

    // deallocate
    free(SA);

    return 0;
}
