#include <sstream>

#include <seqan3/std/filesystem>

#include <chrono>

#include <seqan3/alphabet/nucleotide/dna5.hpp>
#include <seqan3/alphabet/adaptation/char.hpp>
#include <seqan3/argument_parser/all.hpp>
#include <seqan3/core/debug_stream.hpp>
#include <seqan3/io/sequence_file/all.hpp>
#include <seqan3/search/fm_index/fm_index.hpp>
#include <seqan3/search/search.hpp>

using namespace std::chrono;
using namespace seqan3::literals;

// prints out all occurences of query inside of ref
void findOccurences(std::vector<seqan3::dna5> const& ref, std::vector<seqan3::dna5> const& query) {
 
    bool found = false;
    //seqan3::debug_stream << query << ": ";
    auto res = std::begin(ref);
    while (res != std::end(ref)) {
        res = std::search(res, std::end(ref), std::begin(query), std::end(query));
        if(res != std::end(ref)) {
            if (!found) {
                found = true;
                std::cout << "found it at the position(s):";
                }
            std::cout << " " << res - ref.begin();
            res ++;           
        }
    } 
    if(!found) std::cout << "couldn't find it."; 
    std::cout << "\n";
    
}

int main(int argc, char const* const* argv) {
    seqan3::argument_parser parser{"naive_search", argc, argv, seqan3::update_notifications::off};

    parser.info.author = "SeqAn-Team";
    parser.info.version = "1.0.0";

    auto reference_file = std::filesystem::path{};
    parser.add_option(reference_file, '\0', "reference", "path to the reference file");

    auto query_file = std::filesystem::path{};
    parser.add_option(query_file, '\0', "query", "path to the query file");

    size_t queries_size{};
    size_t queries_size_default = 100;
    parser.add_option(queries_size, 's', "queries-size", "Size of query vector");

    try {
         parser.parse();
    } catch (seqan3::argument_parser_error const& ext) {
        seqan3::debug_stream << "Parsing error. " << ext.what() << "\n";
        return EXIT_FAILURE;
    }

    if ( ! (queries_size > 0 && queries_size <= 1000000 ) )  queries_size = queries_size_default;

    //std::cout<<"Start NAIVE with query size = "<<queries_size<<"\n";

    // loading our files
    auto reference_stream = seqan3::sequence_file_input{reference_file};
    auto query_stream     = seqan3::sequence_file_input{query_file};

    // read reference into memory
    std::vector<std::vector<seqan3::dna5>> reference;
    for (auto& record : reference_stream) {
        reference.push_back(record.sequence());
    }

    // read query into memory
    std::vector<std::vector<seqan3::dna5>> queries;
    for (auto& record : query_stream) {
        queries.push_back(record.sequence());
    }


    //! search for all occurences of queries inside of reference
    std::vector<std::vector<seqan3::dna5>> queries_temp; 
    if ( queries.size() > queries_size ) {
        queries.resize(queries_size); 
    } else {
        queries_temp.insert(queries_temp.end(), queries.begin(), queries.end());
        for (int i = 1; i<=10; ++i) {
            queries.insert(queries.end(), queries_temp.begin(), queries_temp.end());
            if ( queries.size() >= queries_size ) i = 99;
        }
        queries_temp.clear();
    }
 
    int iPercent = 0;
    int iPercentShow = -1;
    int iCounter = 0;

    //for (int i = 1000000; i>=1000; i=i/10) {
        queries.resize(queries_size);
        auto start = high_resolution_clock::now();
        for (auto& r : reference) {
            for (auto& q : queries) {
                //iCounter++;
                //!TODO !ImplementMe apply binary search and find q  in reference using binary search on `suffixarray`
                // You can choose if you want to use binary search based on "naive approach", "mlr-trick", "lcp"
                for (auto& nucleotide : q){
                    std::cout<<seqan3::to_char(nucleotide); 
                }
                std::cout<< ": ";

                findOccurences(r, q);
                /*
                iPercent = (int)((static_cast<float>(iCounter) / queries_size) * 100);
                if (iPercent > iPercentShow) {
                    std::cout << iPercent << "% " << std::flush;
                    iPercentShow += 5;
                }
                */
            } 
        }    
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        //
        /*
        std::cout << "\n" << "Time taken by naive search in " << queries.size() << " queries: "
            << duration.count() << " microseconds" << "\n";*/
    //}
/*
    for (auto& r : reference) {
        for (auto& q : queries) {
            findOccurences(r, q);
        }
    }
*/
    return 0;
}
