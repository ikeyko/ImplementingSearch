#include <sstream>

#include <seqan3/alphabet/nucleotide/dna5.hpp>
#include <seqan3/argument_parser/all.hpp>
#include <seqan3/core/debug_stream.hpp>
#include <seqan3/io/sequence_file/all.hpp>
#include <seqan3/search/fm_index/fm_index.hpp>
#include <seqan3/search/search.hpp>

void verify (std::vector<seqan3::dna5> const &query, std::vector<seqan3::dna5> const &text, 
            size_t text_position, size_t pattern_size, size_t pattern_position, 
            size_t query_size, size_t errors ) {
    // for simplicity we search only substitutions
    //size_t i = pattern_position;
    //size_t j = text_position;
    //size_t j_max = text_position + query.size() - pattern_position - 1;
    size_t j = text_position - pattern_position;
    size_t i = 0;
    //size_t query_size = query.size();
    size_t err = 0;

    // check right side from pattern && j < text_position + pattern_position
    while (err <= errors  &&  i < pattern_position) {
        if (text[j] != query[i]) ++err;
        i++;
        j++;
    }
    j += pattern_size;
    i += pattern_size;
    // check right side from pattern && j < text_position - pattern_position + query_size
    while (err <= errors && i < query_size) {
        if (text[j] != query[i]) ++err;
        i++;
        j++;        
    }

    if (err <= errors ) std::cout<<(text_position - pattern_position)<<" with "<< err <<" errors\n";


}




int main(int argc, char const* const* argv) {
    seqan3::argument_parser parser{"fmindex_pigeon_search", argc, argv, seqan3::update_notifications::off};

    parser.info.author = "SeqAn-Team";
    parser.info.version = "1.0.0";

    auto reference_file = std::filesystem::path{};
    parser.add_option(reference_file, '\0', "reference", "path to the reference file");

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

    seqan3::configuration const cfg = seqan3::search_cfg::max_error_total{seqan3::search_cfg::error_count{0}};

    // adjust queries vector size
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

    for (auto& query : queries) {
        // DIVIDE
        // devide query on parts almost similar length
        std::vector<std::vector<seqan3::dna5>> parts;
        std::vector<size_t> parts_begin_positions;
        size_t k = errors_num + 1;
        size_t length = query.size() / k;
        size_t remain = query.size() % k;

        size_t begin = 0;
        size_t end = 0;

        for (size_t i = 0; i < std::min(k, query.size()); ++i)
        {
            if (remain > 0) {
                end += length + remain;
                remain--;
                }
            else end += length;

            parts.push_back(std::vector<seqan3::dna5>(query.begin() + begin, query.begin() + end));
            
            parts_begin_positions.push_back(begin);

            begin = end;
        }
        // 
        for (auto& part : parts) {
            seqan3::debug_stream<<part<<" ";
        }

        for (auto& pos : parts_begin_positions) {
            std::cout<<pos<<" ";
        }
        // SEARCH
        for (size_t p = 0; p < parts.size(); ++p) {
            //seqan3::debug_stream<<p<<"\n";
            for (auto && position : search(parts[p], index, cfg)) {
                //seqan3::debug_stream << result.reference_begin_position()<< '\n';
                /*
                for (auto& position : result.reference_begin_position ()) {
                seqan3::debug_stream << position << '\n';
                }
                */
               //seqan3::debug_stream << position.reference_begin_position()<< '\n';
                //size_t pos = reinterpret_cast<size_t const*>(position);
                verify(query, reference, position.reference_begin_position(), parts[p].size(), 
                       parts_begin_positions[p], query.size(), errors_num);
                

            }
            std::cout<<"\n";
        }
    


    }




    //!TODO !ImplementMe use the seqan3::search to find a partial error free hit, verify the rest inside the text
    // Pseudo code (might be wrong):
    // for query in queries:
    //      parts[3] = cut_query(3, query);
    //      for p in {0, 1, 2}:
    //          for (pos in search(index, part[p]):
    //              if (verify(ref, query, pos +- ....something)):
    //                  std::cout << "found something\n"



    return 0;
}