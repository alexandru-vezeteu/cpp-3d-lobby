//
// Created by alex on 31/05/25.
//

#ifndef COMPRESSOR_HPP
#define COMPRESSOR_HPP

#include <string>

bool compress_folder(const std::string& folder_path, const std::string& output_tar_gz);
bool decompress_folder(const std::string& tar_gz_path, const std::string& output_dir);



#endif //COMPRESSOR_HPP
