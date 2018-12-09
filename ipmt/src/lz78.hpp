/**
 * @file lz78.hpp
 * @author Julia Feitosa (midxfc@cin.ufpe.br)
 * @brief LZ78 is responsible for implementing the LZ78_Compress and LZ78_Decompress methods
 * @version 0.1
 * @date 2018-11-26
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef LZ78_H
#define LZ78_H

#include <string> 
#include <vector> 
#include <string.h> 
#include <stdexcept> 
#include <iostream> 
#include <fstream> 
#include <unordered_map>

#define COMPRESSED_QUALIFIER "_compressed"
#define UNCOMPRESSED_QUALIFIER "_uncompressed"
#define EXT ".idx"

class LZ78 {


    public:
    /**
    * @brief Construct a new LZ78 object
    * 
    */
    
    LZ78() {};

    ~LZ78() {};

    void WriteToFile(const std::string & filename, std::vector < std::pair < int, char >> code) {

        std::ofstream out(filename.c_str());
        size_t dict_size = code.size();

        for (size_t i = 0; i < dict_size; ++i) {
            out.write(reinterpret_cast <
                const char * > ( & code[i].first), sizeof(int));
            out.write( & code[i].second, sizeof(char));
        }

    }

    std::string LZ78_Decompress(const std::vector < std::pair < int, char >> & code) {
        std::string txt, nextWord;
        std::unordered_map < int, std::string > dict;
        int d = 1;

        dict[0] = "";

        for (size_t idx = 0; idx < code.size(); ++idx) {
            int dict_index = code[idx].first;
            char decoded_char = code[idx].second;

            nextWord = dict[dict_index] + decoded_char;
            dict[d++] = nextWord;
            txt += nextWord;
        }

        return txt;
    }

    void Decompress(const std::string & input_filename,
        const std::string & output_filename) {

        std::ifstream in (input_filename);
        std::ofstream out(output_filename);

        std::string txt;
        int dict_size, idx;
        char chr;

        std::vector < std::pair < int, char >> code;

        while (! in .eof()) {

            in .read(reinterpret_cast < char * > ( & idx), sizeof(int)); in .read( & chr, sizeof(char));
            code.push_back(std::make_pair(idx, chr));
        }
        txt = LZ78_Decompress(code);
        out << txt;
    }

    void LZ78_Compress(const std::string & txt, std::vector < std::pair < int, char >> * code) {

        std::unordered_map < std::string, int > dict;
        std::string preffix;
        int idx = 1;

        dict[""] = 0;

        for (int i = 0; i < txt.size(); ++i) {

            if (dict.find(preffix + txt[i]) != dict.end()) {
                preffix += txt[i];
            } else {
                code->push_back(std::make_pair(dict[preffix], txt[i]));
                dict[preffix + txt[i]] = idx++;
                preffix.clear();
            }

        }

    }

    void Compress(const std::string & input_filename,
        const std::string & output_filename) {

        std::ifstream in (input_filename.c_str());
        std::string line, txt;
        std::vector < std::pair < int, char >> code;

        while (getline( in , line)) {
            txt += line;
            txt += "\n";
        } in .close();

        LZ78_Compress(txt, & code);
        WriteToFile(output_filename, code);

    }

    std::string ParseFileName(std::string filename, std::string qualifier) {

        filename = filename.substr(0, filename.size() - 4);
        filename = filename + qualifier + EXT;
        return filename;

    }

    private:

};

#endif