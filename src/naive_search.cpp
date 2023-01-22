#include <sstream>

#include <seqan3/std/filesystem>

#include <chrono>

#include <seqan3/alphabet/nucleotide/dna5.hpp>
#include <seqan3/argument_parser/all.hpp>
#include <seqan3/core/debug_stream.hpp>
#include <seqan3/io/sequence_file/all.hpp>
#include <seqan3/search/fm_index/fm_index.hpp>
#include <seqan3/search/search.hpp>

using namespace std::chrono;

// prints out all occurences of query inside of ref
void findOccurences(std::vector<seqan3::dna5> const& ref, std::vector<seqan3::dna5> const& query) {
 
    //bool found = false;
    //seqan3::debug_stream << query << ": ";
    auto res = std::begin(ref);
    while (res != std::end(ref)) {
        res = std::search(res, std::end(ref), std::begin(query), std::end(query));
        if(res != std::end(ref)) {
            //if (!found) {
                //found = true;
                //std::cout << "found it at the position(s):";
                //}
            //std::cout << " " << res - ref.begin();
            res ++;           
        }
    } 
   // if(!found) std::cout << "couldn't find it."; 
   // std::cout << "\n";
    
}

int main(int argc, char const* const* argv) {
    seqan3::argument_parser parser{"naive_search", argc, argv, seqan3::update_notifications::off};

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
    std::vector<std::vector<seqan3::dna5>> reference;
    for (auto& record : reference_stream) {
        reference.push_back(record.sequence());
    }

    // read query into memory
    std::vector<std::vector<seqan3::dna5>> queries;
    for (auto& record : query_stream) {
        queries.push_back(record.sequence());
    }

    //!TODO !CHANGEME here adjust the number of searches
    if (queries.size() > 100) {
        queries.resize(100);    // will reduce the amount of searches
    }

    //! search for all occurences of queries inside of reference
    std::vector<std::vector<seqan3::dna5>> queries_resized;    
    for (int i = 1; i<=10; ++i) {
        queries_resized.insert(queries_resized.end(), queries.begin(), queries.end());
    }
 
    //for (int i = 1000000; i>=1000; i=i/10) {
        queries_resized.resize(1000);
        auto start = high_resolution_clock::now();
        for (auto& r : reference) {
            for (auto& q : queries_resized) {
                //!TODO !ImplementMe apply binary search and find q  in reference using binary search on `suffixarray`
                // You can choose if you want to use binary search based on "naive approach", "mlr-trick", "lcp"
                //seqan3::debug_stream << q << ": ";
                findOccurences(r, q);
            } 
        }    
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        //
        std::cout << "Time taken by naive search in " << queries_resized.size() << " queries: "
            << duration.count() << " microseconds" << "\n";
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
