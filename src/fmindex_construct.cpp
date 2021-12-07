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
    auto reference_file = std::filesystem::path{"../data/text.dna4.short.fasta.gz"};
    auto index_path     = std::filesystem::path{"our_index.index"};

    // loading our files
    auto reference_stream = seqan3::sequence_file_input<my_dna4>{reference_file};

    // read reference into memory
    std::vector<std::vector<seqan3::dna4>> reference;
    for (auto& record : reference_stream) {
        reference.push_back(record.sequence());
    }

    // Our index is of type `Index`
    seqan3::fm_index index{reference}; // construct fm-index

    // saving the fmindex to storage
    {
        seqan3::debug_stream << "Saving 2FM-Index ... " << std::flush;
        std::ofstream os{index_path, std::ios::binary};
        cereal::BinaryOutputArchive oarchive{os};
        oarchive(index);
        seqan3::debug_stream << "done\n";
    }

    return 0;
}
