#include <sstream>

#include <seqan3/std/filesystem>

#include <seqan3/alphabet/nucleotide/dna5.hpp>
#include <seqan3/argument_parser/all.hpp>
#include <seqan3/core/debug_stream.hpp>
#include <seqan3/io/sequence_file/all.hpp>
#include <seqan3/search/fm_index/fm_index.hpp>
#include <seqan3/search/search.hpp>

int main(int argc, char const* const* argv) {
    seqan3::argument_parser parser{"fmindex_search", argc, argv, seqan3::update_notifications::off};

    parser.info.author = "SeqAn-Team";
    parser.info.version = "1.0.0";

    auto index_path = std::filesystem::path{};
    parser.add_option(index_path, '\0', "index", "path to the query file");

    auto query_file = std::filesystem::path{};
    parser.add_option(query_file, '\0', "query", "path to the query file");

    size_t queries_size{};
    size_t queries_size_default = 100;
    parser.add_option(queries_size, 's', "queries-size", "Size of queries vector");

    size_t errors_num{};
    size_t errors_num_default = 0;
    parser.add_option(errors_num, 'e', "errors", "Number of errors between 0 and 2");

    try {
         parser.parse();
    } catch (seqan3::argument_parser_error const& ext) {
        seqan3::debug_stream << "Parsing error. " << ext.what() << "\n";
        return EXIT_FAILURE;
    }

    // loading our files
    auto query_stream     = seqan3::sequence_file_input{query_file};

    // read query into memory
    std::vector<std::vector<seqan3::dna5>> queries;
    for (auto& record : query_stream) {
        queries.push_back(record.sequence());
    }

    if ( ! (queries_size > 0 && queries_size <= 1000000 ) )  queries_size = queries_size_default;
    if ( ! (errors_num >= 0 && errors_num <= 2 ) )  {
        errors_num = errors_num_default;
        seqan3::debug_stream << "This number of errors is not supported. Setting by defaul = 0\n";
    }



    // loading fm-index into memory
    using Index = decltype(seqan3::fm_index{std::vector<std::vector<seqan3::dna5>>{}}); // Some hack
    Index index; // construct fm-index
    {
        seqan3::debug_stream << "Loading 2FM-Index ... " << std::flush;
        std::ifstream is{index_path, std::ios::binary};
        cereal::BinaryInputArchive iarchive{is};
        iarchive(index);
        seqan3::debug_stream << "done\n";
    }

    seqan3::configuration const cfg = seqan3::search_cfg::max_error_total{seqan3::search_cfg::error_count{errors_num}};


    //!TODO here adjust the number of searches
    //queries.resize(10); // will reduce the amount of searches
    seqan3::debug_stream << search(queries, index, cfg);

    //!TODO !ImplementMe use the seqan3::search function to search

    return 0;
}
