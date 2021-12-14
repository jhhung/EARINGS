#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <string_view>
#include <filesystem>
#include <EARRINGS/version.h>

#define GET_STR(arg)			#arg
#define GET_VERSION(ver)		GET_STR(ver)
#define GET_EARRINGS_VERSION	GET_VERSION(EARRINGS_VERSION)

// for both SE and PE
bool is_fastq = true;
bool is_sensitive = false;
bool is_gz_input(false), is_gz_output(false);
bool is_bam(false);
size_t record_line = 4;
constexpr size_t DETECT_N_READS = 10000;

// for SE
std::string index_prefix; 
size_t seed_len(18);
size_t min_multi(0);
float prune_factor(0.1);
bool no_mismatch(false);
bool estimate_umi_len(false);

// for PE
size_t thread_num(1);
size_t block_size(8192);
size_t min_length(0);
std::vector<std::string> ifs_name(2);
std::vector<std::string> ofs_name(2);

bool loc_tail(true);  // adapter locates at tail/head
// size_t umi_loc(0);  // 1/2/3: umi seq locates at read1/read2/both
// size_t umi_len(0);
float match_rate = 0.7, seq_cmp_rate = 0.9, adapter_cmp_rate = 0.8;

// default adapters
std::string DEFAULT_ADAPTER1("AGATCGGAAGAGCACACGTCTGAACTCCAGTCAC");
std::string DEFAULT_ADAPTER2("AGATCGGAAGAGCGTCGTGTAGGGAAAGAGTGTA");
