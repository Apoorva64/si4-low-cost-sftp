//
// Created by appad on 23/12/2023.
//

#include "client.h"

client::client(int argc, char **argv) {

    App *upload = this->add_subcommand("upload", "Upload a file");
    upload->add_option("-f,--file", this->filename, "Specify file to upload")->required();
    upload->callback([]() { std::cout << "Uploading file" << std::endl; });


    App *download = this->add_subcommand("download", "Download a file");
    App *list = this->add_subcommand("list", "List files on server");
    App *help = this->add_subcommand("help", "Display help");
    download->add_option("-f,--file", this->filename, "Specify file to download")->required();
}
