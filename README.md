# EARRINGS

EARRINGS is an efficient and accurate adapter trimmer that entails no a priori adapter sequences.

## Information

EARRINGS is the successor of [PEAT](https://github.com/jhhung/PEAT), and PEAT would be no longer depelop anymore.

In single-end mode, EARRING borrow [skewer](https://github.com/relipmoc/skewer)'s help to do trimming after auto-detecting most possible adapter.

## Requirement

- [g++-8](https://gcc.gnu.org/gcc-8/) and cmake [3.10.0](https://cmake.org/download/) or higher to build EARRINGS
- python3.7 or higher to run benchmarking

## Build

```sh
# In root directory of repository
> mkdir build
> cd build
# If default gcc or g++ version != 8.0
# -DCMAKE_C_COMPILER=/usr/bin/gcc-8 or -DCMAKE_CXX_COMPILER=/usr/bin/g++-8 is needed.
> cmake .. -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../stage
> cmake --build . --target install
> ./EARRINGS -h
```

## Execution

There are 3 modes to execute EARRINGS: build, single, paired.

Single mode and paired mode are used for single-end reads and paired-end reads respectively.

Build mode generate an index for source reference sequence of target single-end reads.

### **Single-End**

### Build reference index

Before conducting single-end adapter trimming, **one has to prebuild index** once for a specific reference </br>
genome which is the source of target reads.

```sh
# ./EARRINGS build -r [ref_path] -p [index_prefix]
> ./EARRINGS build -r ref/hg38_chr1.fa -p earrings_hg38_chr1
```

Build parameters

- Required
  - -r [ --ref_path ]                     Path to the reference genome.
  - -p [ --index_prefix ]                 The index prefix for the built index table.
- Optional
  - -h [ --help ]                         Display help message and exit.

### Execute Single-End trimming

EARRINGS first detects adapter then feeds the detected adapter to skewer.

```sh
# ./EARRINGS single -p [index_prefix] --skewer [input_file] [skewer_parameters]
> ./EARRINGS single -p path_to_index --skewer ../test_data/has_adapter_1.fq
```

Single-End mode parameters

- Required
  - -p [ --index_prefix ] The index prefix for pre-built index table.
  - -s [ --skewer ] Skewer flag, options after this would be fed to skewer.
- Optional
  - -h [ --help ] Display help message and exit.
  - -d [ --seed_len ] Seed length used when aligning reads. For very short reads like miRNA, it is </br>
        recommended to set seed_len to 18. (default: 50)
  - -m [ --max_align ] Control the maximum number of alignment to abort the reads. (default: 0, </br>
        not limited)
  - -e [ --enable_mismatch ] Enable/disable mismatch when doing seed finding. (default: true)
  - -f [ --prune_factor ] Prune factor used when assembling adapters using the de-brujin graph. </br>
        Kmer occurence lower than the prune factor will be aborted. (default: 0.03)
  - -F [ --fasta ] Specify input file type as FastA. (Default input file format: FastQ)
  - -a [ --adapter1 ] Alternative adapter if auto-detect mechanism fails.</br>
        (default: AGATCGGAAGAGCACACGTCTGAACTCCAGTCAC)
  - --sensitive Sensitive mode can be used when the user is sure that the dataset contains adapters.</br>
        Under sensitive mode, we do not restrict the minimum number of kmers when assembly adapters.</br>
        By default, the minimum number of kmers must exceed 10.
  - -b [ --bam_input ] Detect and trim off adapters from a BAM file.
  - -u [ --UMI ] Estimate the size of UMI sequences.

### **Paired-End**

### Execute Paired-End trimming

```sh
# ./EARRINGS paired -i [input1] -I [input2] -t [thread_num]
> ./EARRINGS paired -i ../test_data/has_adapter_1.fq -I ../test_data/has_adapter_2.fq
```

Paired-end mode parameters

- Required
  - -i [ --input1 ] The PE FastQ input file 1 (.fq)
  - -I [ --input2 ] The PE FastQ input file 2 (.fq)
- Optional
  - -h [ --help ] Display help message and exit.
  - -o [ --output1 ] The PE FastQ output file 1 (.fq) (default: EARRINGS_2.fq)
  - -O [ --output2 ] The PE FastQ output file 2 (.fq) (default: EARRINGS_2.fq)
  - -a [ --adapter1 ] Default adapter 1 when auto-detect fails.</br>
        (default: AGATCGGAAGAGCACACGTCTGAACTCCAGTCAC)
  - -A [ --adapter2 ] Default adapter 2 when auto-detect fails.</br>
        (default: AGATCGGAAGAGCGTCGTGTAGGGAAAGAGTGTA)
  - -t [ --thread ] The number of threads used to run the program. (default: 1)
  - -m [ --min_length ] Abort the read if the length of the read is less than m. (default: 0)
  - -l [ --adapter_loc ] Specify the location of the adapter. (default: tail)
  - -M [ --rc_thres ] Setting the threshold of reverse complement check. (default: 0.7)
  - -s [ --ss_thres ] Setting the threshold of gene portion check. (default: 0.9)
  - -S [ --as_thres ] Setting the threshold of adapter portion check. (default: 0.8)
  - -f [ --prune_factor ] Prune factor used when assembling adapters using the de Bruijn graph. kmer</br>
        occurence lower than the prune factor will be aborted. (default: 0.03)
  - --sensitive Sensitive mode can be used when the user is sure that the dataset contains adapters.</br>
        Under sensitive mode, we do not restrict the minimum number of kmers when assembly adapters.</br>
        By default, the minimum number of kmers must exceed 10.
  - -F [ --fasta ] Specify input file type as FastA. (default input file format: FastQ)
  - -b [ --bam_input ] Detect and trim off adapters from a BAM file.

## Run Simulation

The simulation is carried out using a modified version of [pIRS](https://github.com/galaxy001/pirs)
(profile based Illumina pair-end Reads Simulator).
Please build pIRS first before using it:

```sh
> cd simulator/pIRS
> make
```

pIRS simulates Illumina PE reads from a reference genome. In all the benchmarking except real data </br>
benchmarking, we use GRCh38 chr1 as reference genome to simulate reads.

One can run the simulation by:

```sh
# python3 script.py [seed] -r [ref_genome] -n [read_num] -m [insert_mean] -std [insert_td] -ad1 [ad1] -ad2 [ad1]
# To generate reads without adapters:
> python3 script.py 0 -r hg38_chr1.fa -n 10000 -m 150 -std 50 -ad1 "" -ad2 ""
# To generate reads with default adapters:
> python3 script.py 0 -r hg38_chr1.fa -n 10000 -m 150 -std 50
```

## Run Benchmarking

Before running benchmarking, please install all the prerequisites and set the locations of all the </br>
executions to $PATH:

1. AdapterRemoval ver. 2.3.0
2. skewer ver. 0.2.2
3. Cutadapt ver. 2.4
4. AKATrim ver. 1.3.3
5. Trimmomatic ver. 0.39
6. SeqPurge ver. 2019_11
7. fastp ver. 0.20.0 - commit 6ff0ffa
8. atropos ver. 1.1.21 - commit 2b15c77
9. PEAT ver. 1.2.5
10. python ver. 3.6
11. our modified version of pIRS (see Run Simulation part)
12. picard 2.23.0

For speed, memory, performance and adapter benchmarking, please download the first chromosome of GRCh38 from UCSC genome browser.
For running real data benchmarking, please download the following four datasets and their corresponding reference genome(.fa) from ncbi.

1. SRR529095: RNA IP-Seq of Argonaute proteins from Homo sapiens
2. SRR014866: miRNA of C.elegans
3. SRR330569: RNA-Seq of Gonads and Carcasses in D. simulans and D. pseudoobscura
4. SRR5000681: ATAC-Seq of Early Embryo replicate in C.elegans

Also, don't forget to install the aligners:

1. Bowtie2 v2.4.1
2. HISAT2 v2.2.0

After that, modify path-related variables in path.py to the path where the downloaded datasets were placed.
Then prebuild indices for the aligners and EARRINGS by running:

```sh
> python3 build_index.py
```

Run real data benchmarking:

```sh
> python3 benchmark_real_data.py
```

Run speed benchmarking:

```sh
> python3 benchmark_speed.py
```

Run memory benchmarking:

```sh
> python3 benchmark_mem_usage.py
```

Run performance benchmarking:

```sh
> python3 benchmark_performance.py
```

Run adapter benchmarking:

```sh
> python3 benchmark_adapter.py
```

## Contact

```sh
Jui-Hung Hung <juihunghung@gmail.com>

Ting-Husan Wang <phoebewangintw@gmail.com>

Cheng-Ching Huang <ken5112840801@gmail.com>
```
