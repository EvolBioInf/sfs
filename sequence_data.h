/***** sequenceData.h ****************************************
 * Description: Header file for sequence manipulation tasks.
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Sun Jun  6 10:34:25 2004.
 * License: GNU General Public
 **************************************************************/
#define SEQLINE 1000		/* maximal length of one line in FASTA file; hard bound */
#define SEQBUFFER 5000000	/* define the size of the sequence buffer */
#define DICSIZE 256
/* #define BORDER 'X' */
#define BORDER '\0'

/* basic sequence type representing >= 1 entry in FASTA file */
typedef struct sequence
{
  char *seq;			/* the sequence */
  char *id;			/* the sequence id */
  int num_seq;			/* number of sequences represented */
  int num_query;		/* number of query sequences */
  int *borders;			/* position of last character of sequence in seq */
  char **headers;		/* FASTA header lines */
  long len;			/* sequence length */
  int *freq_tab;		/* frequency table */
  long num_query_nuc;		/* number of nucleotides in query sequence */
  long num_sbjct_nuc;		/* number of nucleotides in sbjct sequence */
  long num_nuc;			/* number of nucleotides in sequence */
  long query_start;
  long query_end;
} Sequence;

Sequence *revcomp (Sequence * seq);
char *revcomp_string (char *seq);
Sequence *get_next_sequence (FILE * fp);
int *get_dna_dictionary (int *dic);
int *get_restricted_dna_dictionary (int *dic);
Sequence *free_sequence (Sequence * seq);
Sequence *get_permanent_next_sequence (FILE * fp);
void convert_to_acgt (Sequence * seq);
void reset_sequence_reader ();
Sequence *read_fasta (int fd);
Sequence **sequence2array (Sequence * seq);
void prepare_seq (Sequence * sequence);
Sequence *cat_seq (Sequence * seq1, Sequence * seq2);
int **initialize_profiles (int **profiles, int n);
void print_profiles (FILE * fout, int **profiles, int max_res, int min_cov);
