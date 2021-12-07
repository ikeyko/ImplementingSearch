#include <sstream>

#include <seqan3/std/filesystem>
#include <seqan3/alphabet/nucleotide/dna4.hpp>
#include <seqan3/core/debug_stream.hpp>
#include <seqan3/io/sequence_file/all.hpp>
#include <seqan3/search/fm_index/fm_index.hpp>
#include <seqan3/search/search.hpp>

struct my_dna4 : seqan3::sequence_file_input_default_traits_dna {
    using sequence_alphabet = seqan3::dna4;
};

int main(int, char**) {

    // paths to our files
    auto index_path     = std::filesystem::path{"our_index.index"};
    auto query_file     = std::filesystem::path{"../data/sampled_illumina_reads.fasta.gz"};

    // loading our files
    auto query_stream     = seqan3::sequence_file_input<my_dna4>{query_file};

    // read query into memory
    std::vector<std::vector<seqan3::dna4>> queries;
    for (auto& record : query_stream) {
        queries.push_back(record.sequence());
    }

    // loading fm-index into memory
    using Index = decltype(seqan3::fm_index{std::vector<std::vector<seqan3::dna4>>{}}); // Some hack
    Index index; // construct fm-index
    {
        seqan3::debug_stream << "Loading 2FM-Index ... " << std::flush;
        std::ifstream is{index_path, std::ios::binary};
        cereal::BinaryInputArchive iarchive{is};
        iarchive(index);
        seqan3::debug_stream << "done\n";
    }

    seqan3::configuration const cfg = seqan3::search_cfg::max_error_total{seqan3::search_cfg::error_count{0}};

    //!TODO here adjust the number of searches
    queries.resize(100); // will reduce the amount of searches

    auto result = search(queries, index, cfg);
    for (auto r : result) {
        auto qid = r.query_id();
        auto sid = r.reference_id();
        auto pos = r.reference_begin_position();
        std::cout << "found at : " << pos << "\n";
    }

    return 0;
}
