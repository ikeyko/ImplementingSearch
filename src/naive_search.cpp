#include <sstream>

#include <seqan3/std/filesystem>

#include <seqan3/alphabet/nucleotide/dna5.hpp>
#include <seqan3/argument_parser/all.hpp>
#include <seqan3/core/debug_stream.hpp>
#include <seqan3/io/sequence_file/all.hpp>
#include <seqan3/search/fm_index/fm_index.hpp>
#include <seqan3/search/search.hpp>

// prints out all occurences of query inside of ref
void findOccurences(std::vector<seqan3::dna5> const& ref, std::vector<seqan3::dna5> const& query) {

    auto res = std::search(std::begin(ref), std::end(ref), std::begin(query), std::end(query));
    if(res == std::end(ref)) {
        std::cout << "Couldn't find it.\n";       
    } else {
        std::cout << "Found it.\n";
        seqan3::debug_stream << *res << "\n";

    }
    //seqan3::debug_stream << query << '\n';
    // std::find function call
    /*
    it = std::find (ref.begin(), ref.end(), query);
    if (it != ref.end())
    {
        std::cout << "Query " <<" found at position : " ;
        std::cout << it - ref.begin() << " (counting from zero) \n" ;
    }
    else
        std::cout << "Query not found.\n\n";
    */
    //return 0;
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
    queries.resize(100); // will reduce the amount of searches

    //! search for all occurences of queries inside of reference
    for (auto& r : reference) {
        for (auto& q : queries) {
            findOccurences(r, q);
        }
    }

    return 0;
}
