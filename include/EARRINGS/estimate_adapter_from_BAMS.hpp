#pragma once
#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <EARRINGS/common.hpp>
#include <EARRINGS/fasta.hpp>
#include <OldBiovoltron/format/bam.hpp>
//#include <OldBiovoltron/format/fasta_peat.hpp>

using namespace biovoltron::format;
using namespace EARRINGS;

namespace EARRINGS
{
/* 1. Read name, sequence and quality score from bam file.
   2. Store the reads.
*/
class Process_uBAMs
{
private:
    inline static void write_to_fa(BAM& bam, std::ofstream& ofs, bool is_rev)
    {
        ofs << ">" << bam.get_member<sam::MEMBER_INDEX::QNAME>() << "\n";
        if (is_rev)
        {
            // reverse complement
            auto rev_str = bam.get_member<sam::MEMBER_INDEX::SEQ>();
            std::reverse(rev_str.begin(), rev_str.end());
            for (auto& c : rev_str)
            {
                switch(c)
                {
                    case 'A':
                    case 'a':
                        c = 'T';
                        break;
                    case 'C':
                    case 'c':
                        c = 'G';
                        break;
                    case 'G':
                    case 'g':
                        c = 'C';
                        break;
                    case 'T':
                    case 't':
                        c = 'A';
                        break;
                    default:
                        break;
                }
            }
            ofs << rev_str << "\n";
        }
        else
        {
            ofs << bam.get_member<sam::MEMBER_INDEX::SEQ>() << "\n";
        }
    }

public:
    // singled-end
    template <typename Str1, typename Str2>
    inline static size_t extract_reads_from_uBAMs
                                        (Str1&& bam_fname
                                       , Str2&& output_fname) 
    {
        std::ifstream ifs(bam_fname, std::ios::binary);
        std::ofstream ofs(output_fname);
        if (!ifs)
        {
            throw std::runtime_error("Can't open BAM files\n");
        }

        if (!ofs)
        {
            throw std::runtime_error("Can't open temp file which stores reads extract from bam/ubam\n");
        }

        bam::Header bam_header;
        ifs >> bam_header;

        BAM bam(bam_header);
        size_t counter = 0;
        while(BAM::get_obj(ifs, bam))
        {
            bool is_rev = (bam.get_member<sam::MEMBER_INDEX::FLAG>() & 16) == 16;
            
            write_to_fa(bam, ofs, is_rev);

            counter++;
        }

        return counter;
    }

    // paired-end
    template <typename Str1, typename Str2, typename Str3>
    inline static size_t extract_reads_from_uBAMs
                                        (Str1&& bam_fname
                                       , Str2&& output_fname1
                                       , Str3&& output_fname2) 
    {
        std::ifstream ifs(bam_fname, std::ios::binary);
        std::ofstream ofs1(output_fname1), ofs2(output_fname2);
        if (!ifs)
        {
            throw std::runtime_error("Can't open BAM files\n");
        }

        if (!ofs1 || !ofs2)
        {
            throw std::runtime_error("Can't open input fastq/fasta files\n");
        }

        bam::Header bam_header;
        ifs >> bam_header;

        BAM bam1(bam_header), bam2(bam_header);
        size_t counter = 0;

        while(BAM::get_obj(ifs, bam1))
        {
            BAM::get_obj(ifs, bam2);
            bool is_rev1 = (bam1.get_member<sam::MEMBER_INDEX::FLAG>() & 16) == 16;
            bool is_rev2 = (bam2.get_member<sam::MEMBER_INDEX::FLAG>() & 16) == 16;
            write_to_fa(bam1, ofs1, is_rev1);
            write_to_fa(bam2, ofs2, is_rev2);
            
            counter++;
        }

        return counter;
    }
};
}
