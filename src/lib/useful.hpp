#pragma once

#include <string>
#include <signal.h>
#include <stdlib.h>
#define LINNUX_COMMAND_ERR 128


void print_term_size_cr(const std::string& _str);

void clean_temp_files(int _ctrl_c_signal);

