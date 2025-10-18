//
// Created by alex on 31/05/25.
//

#include "Compressor.hpp"
#include <cstdio>
#include <initializer_list>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <sys/types.h>
#include <filesystem>
#include <sys/wait.h>


namespace fs = std::filesystem;
static bool run_tar_command(std::initializer_list<const char*> args) {
    pid_t pid = fork();
    if (pid == 0) {
        std::vector<char*> argv;
        for (auto& arg : args) argv.push_back(const_cast<char*>(arg));
        argv.push_back(nullptr);
        execvp(argv[0], argv.data());
        perror("execvp failed");
        exit(1);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    } else {
        perror("fork failed");
        return false;
    }
}

bool compress_folder(const std::string& folder_path, const std::string& output_tar_gz) {
    if (!fs::exists(folder_path) || !fs::is_directory(folder_path)) {
        std::cerr << "Error: Folder does not exist.\n";
        return false;
    }

    fs::path folder(folder_path);
    fs::path parent = folder.parent_path().empty() ? "." : folder.parent_path();
    std::string folder_name = folder.filename().string();

    return run_tar_command({
        "tar", "-czf", output_tar_gz.c_str(),
        "-C", parent.c_str(), folder_name.c_str()
    });
}

bool decompress_folder(const std::string& tar_gz_path, const std::string& output_dir) {
    if (!fs::exists(tar_gz_path)) {
        std::cerr << "Error: Archive does not exist.\n";
        return false;
    }
    return run_tar_command({
        "tar", "-xzf", tar_gz_path.c_str()
    });
}