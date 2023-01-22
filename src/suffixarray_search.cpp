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


void construct(std::vector<uint32_t>& sa, const std::string& text) {

    sa.clear(); //clears the Suffixarray

    uint32_t length = text.length();

    // condition that checks, if the text exists
    if (length == 0) return;

    std::vector<uint32_t> lcp;

    sa.push_back(0);
    lcp.push_back(0);

    int l,r,m = 0;


    for (uint32_t index = 1; index < length; ++index){
        
        
        l = 0;
        r = sa.size()-1;
        m = ceil(double(r)/2);

        while (l < r){    

            if(text[index] > text[sa[m]]) {             
                l = m;                                  
            } else if (text[index] < text[sa[m]]){      
                r = m - 1;                              
            } else {
                l = r = m;
            }                                         
            m = ceil(double(l + r)/2);                  
        }

        if(text[index] > text[sa[m]]) {
            sa.insert(sa.begin() + m + 1, index);       
            lcp.insert(lcp.begin() + m + 1, 1);
        } else if (text[index] < text[sa[m]]){
            sa.insert(sa.begin() + m, index);          
            lcp.insert(lcp.begin() + m, 0);
        } else {                                       
                uint32_t i = 0;
                while (index + i++ < length){           
                    if(text[index + i] == text[sa[m]+i]) continue;
                    if(text[index + i] > text[sa[m]+i]){
                        sa.insert(sa.begin() + m+1, index);
                        break;
                    } else {
                        sa.insert(sa.begin() + m, index);  
                        break;
                    }
                }
            }

    
        
    }
}


void n(){
    std::cout<< "\n";
}

void log(const std::string& text){
    std::cout<< text << " ";
}

void log(char text){
    std::cout<< text << " ";
}

void log(uint i){
    std::cout<< i << " ";
}
void log(int i){
    std::cout<< i << " ";
}

void log(std::vector<uint32_t>& sa, uint32_t i){
    std::cout<< sa[i] << " ";
}

void log(std::vector<uint32_t>& sa){
    for (unsigned i = 0; i < sa.size(); ++i) {
        std::cout << sa[i] << " ";
    }
}
/*
void construct(std::vector<uint32_t>& sa, const std::string& text) {

    sa.clear(); //clears the Suffixarray

    uint32_t length = text.length();

    // condition that checks, if the text exists
    if (length == 0) return;

    std::vector<uint32_t> lcp;

    sa.push_back(0);
    lcp.push_back(0);

    int l,r,m = 0;


    for (uint32_t index = 1; index < length; ++index){
        l = 0;
        r = sa.size()-1;
        m = ceil(double(r)/2);

n(); n();
log ("new index"); log(index);  log(" current sa"); log(sa); n(); 

        while (l < r){     log("new while l ="); log (l); log("r ="); log(r); log("m ="); log(m);log("sa ="); log(sa); n(); 
            
            log("check "); log(text[index]); log("with exist"); log(text[sa[m]]); n();
            if(text[index] > text[sa[m]]) {             log ("adjust left"); log(l);
                l = m;                                  log ("-"); log(l);
            } else if (text[index] < text[sa[m]]){      log ("adjust right"); log(r);
                r = m - 1;                              log ("-"); log(r);
            } else {
                l = r = m;
            }                                          log("adjust m =");log(m);
            m = ceil(double(l + r)/2);                  log("-"); log(m); n();
        }

log("check new "); log(text[index]); log("with existing");  n();log("%"); log(m); log(sa[m]); log(text[sa[m]]); log(text[sa[m]]); log("+"); log(":");
if(text[index] > text[sa[m]]) log(" > Ok");
if (text[index] < text[sa[m]]) log("< Ok");
if (text[index] == text[sa[m]]) log("== Ok");

        if(text[index] > text[sa[m]]) {
            log(",");
            sa.insert(sa.begin() + m + 1, index);       log("char inserted >, new sa = "); log(sa);
            lcp.insert(lcp.begin() + m + 1, 1);
        } else if (text[index] < text[sa[m]]){
            log("-");
            sa.insert(sa.begin() + m, index);           log("char inserted <, new sa = "); log(sa); n();
            lcp.insert(lcp.begin() + m, 0);
        } else {
            log(".");
                                                    log ("char found"); log (text[index]); n();
                uint32_t i = 0;
                while (index + i++ < length){           log ("check next text"); log(text[index + i]); n();
                    if(text[index + i] == text[sa[m]+i]) continue;
                    if(text[index + i] > text[sa[m]+i]){ log ("mismatch found text ="); log(text[index + i]); log("sa char ="); log(text[sa[m]+i]); n();
                        sa.insert(sa.begin() + m+1, index); log("char inserted >, new sa = "); log(sa); n();
                        break;
                    } else {
                        sa.insert(sa.begin() + m, index);  log("char inserted >, new sa = "); log(sa); n();
                        break;
                    }
                }
            }

    
        
    }
}

*/
void find(const std::string& query, const std::vector<uint32_t>& sa, const std::string& text, std::vector<uint32_t>& hits) {

    hits.clear();
    unsigned n = sa.size() - 1;

    if (text.length() == 0) return;

    //if the search character not exist in Alphabet
    if (query[0] > text[sa[n]]) return;
    if (query[0] < text[sa[0]]) return;

    unsigned Lp, Rp;

    //looking for right interval bound
    int left = 0;
    int right = n+1;
    int middle;

    while (right - left > 1) {
        middle = ceil((left + right)/2);
        if (query[0] >= text[sa[middle]]) left = middle;
        else right = middle;
    }
    Rp = left;

    //looking for left interval bound
    left = -1;
    right = Rp;

    while (right - left > 1) {

        middle = ceil((left + right)/2);
        if (query[0] <= text[sa[middle]]) right = middle;
        else left = middle;
    }
    Lp = right;

    unsigned query_size = query.size();
    unsigned index = 0; //Suffix index

    while (Rp >= Lp && index < query_size) { //repeat check until full pattern found. Stop if bounds crossed
        while (Rp >= Lp && query[index] != text[sa[Lp]+index]){ //check left suffix
            ++Lp; //go to next suffix
        }
        //cout<<"Lp= "<<Lp<<endl;
        while (Rp >= Lp && query[index] != text[sa[Rp]+index]) { //check right suffix
            --Rp; //go to previous suffix
        }


        if (Rp >= Lp) index++; //if check for both suffixes successfull, go to next char
    }
    while (Rp >= Lp) {
        hits.push_back(sa[Lp++]); //push every alignment between bounds in vector hits
    }
    
    sort(hits.begin(), hits.end());

    if (hits.size() != 0) {
        std::cout << query << ": ";
        for (unsigned i = 0; i < hits.size(); ++i) {
            std::cout << hits[i] << " ";
        }
    }
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
    std::vector<saidx_t> suffixarray;
    suffixarray.resize(reference.size()); // resizing the array, so it can hold the complete SA

    //!TODO !ImplementMe implement suffix array sort
    //Hint, if can use libdivsufsort (already integrated in this repo)
    //      https://github.com/y-256/libdivsufsort
    //      To make the `reference` compatible with libdivsufsort you can simply
    //      cast it by calling:
    //      `sauchar_t const* str = reinterpret_cast<sauchar_t const*>(reference.data());`


    //int n = reference.size();
    int i,j;
    //char *Text = "abracadabra";
    //int n = strlen(Text);
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

 //   std::string text = "hello";
/*
    std::vector<uint32_t> sa;// = {1,4,6,10,8,3,2,0,5,7,11,9};

    std::vector<uint32_t> hits;
    std::cout<<"go to construct\n";
    construct(sa, text);
    std::cout<<"construct done\n";
    
    for (unsigned i = 0; i < sa.size(); ++i) {
        std::cout << sa[i] << " ";
    }
*/
    // output
    for(i = 0; i < n; ++i) {
        printf("SA[%2d] = %2d: ", i, SA[i]);
        for(j = SA[i]; j < n; ++j) {
            printf("%c", str[j]);
        }
        printf("$\n");
    }

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
