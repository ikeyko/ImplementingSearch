#include <divsufsort.h>
#include <sstream>

#include <seqan3/std/filesystem>

#include <seqan3/alphabet/nucleotide/dna5.hpp>
#include <seqan3/argument_parser/all.hpp>
#include <seqan3/core/debug_stream.hpp>
#include <seqan3/io/sequence_file/all.hpp>
#include <seqan3/search/fm_index/fm_index.hpp>
#include <seqan3/search/search.hpp>

int main(int argc, char const* const* argv) {
    seqan3::argument_parser parser{"suffixarray_search", argc, argv, seqan3::update_notifications::off};

    parser.info.author = "SeqAn-Team";
    parser.info.version = "1.0.0";

    auto reference_file = std::filesystem::path{};
    parser.add_option(reference_file, '\0', "reference", "path to the reference file");

    auto query_file = std::filesystem::path{};
    parser.add_option(query_file, '\0', "query", "path to the query file");

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
    queries.resize(100); // will reduce the amount of searches

    // Array that should hold the future suffix array
    std::vector<saidx_t> suffixarray;
    suffixarray.resize(reference.size()); // resizing the array, so it can hold the complete SA

    //!TODO !ImplementMe implement suffix array sort
    //Hint, if can use libdivsufsort (already integrated in this repo)
    //      https://github.com/y-256/libdivsufsort
    //      To make the `reference` compatible with libdivsufsort you can simply
    //      cast it by calling:
    //      `sauchar_t const* str = reinterpret_cast<sauchar_t const*>(reference.data());`

    for (auto& q : queries) {
        //!TODO !ImplementMe apply binary search and find q  in reference using binary search on `suffixarray`
        // You can choose if you want to use binary search based on "naive approach", "mlr-trick", "lcp"
    }

    return 0;
}
