/***** sequenceData.c *********************************************
 * Description: Collection of routines for reading and 
 * manipulating sequence data.
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Sun Jun  6 10:34:31 2004.
 * License: GNU General Public
 *****************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "sequence_data.h"
#include "stringUtil.h"
#include "eprintf.h"
#include "interface.h"
#include "ran.h"


static int lastSequence = 0;
static char *line = NULL;

/* convert_to_acgt: convert nucleotide data to acgt alphabet.
 */
void
convert_to_acgt (Sequence * seq)
{
  replace (seq->seq, 'r', 'g');
  replace (seq->seq, 'y', 't');
  replace (seq->seq, 'm', 'a');
  replace (seq->seq, 'k', 'g');
  replace (seq->seq, 's', 'g');
  replace (seq->seq, 'w', 'a');
  replace (seq->seq, 'h', 'a');
  replace (seq->seq, 'b', 'g');
  replace (seq->seq, 'v', 'g');
  replace (seq->seq, 'd', 'g');
  replace (seq->seq, 'n', 'g');
  replace (seq->seq, 'u', 't');

  replace (seq->seq, 'R', 'G');
  replace (seq->seq, 'Y', 'T');
  replace (seq->seq, 'M', 'A');
  replace (seq->seq, 'K', 'G');
  replace (seq->seq, 'S', 'G');
  replace (seq->seq, 'W', 'A');
  replace (seq->seq, 'H', 'A');
  replace (seq->seq, 'B', 'G');
  replace (seq->seq, 'V', 'G');
  replace (seq->seq, 'D', 'G');
  replace (seq->seq, 'N', 'G');
  replace (seq->seq, 'U', 'T');
}


/* everything that is not [acgtACGT] is flagged by a 4 */
int *
get_restricted_dna_dictionary (int *dic)
{
  int i;

  if (dic == NULL)
    dic = (int *) emalloc ((DICSIZE + 1) * sizeof (int));

  for (i = 0; i < DICSIZE; i++)
    dic[i] = 0;

  dic['a'] = 1;			/* a */
  dic['c'] = 1;			/* c */
  dic['g'] = 1;			/* g */
  dic['t'] = 1;			/* t */
  dic['A'] = 1;			/* A */
  dic['C'] = 1;			/* C */
  dic['G'] = 1;			/* G */
  dic['T'] = 1;			/* T */

  return dic;

}

/* get_dna_dictionary: create DNA dictionary */
int *
get_dna_dictionary (int *dic)
{
  int i;

  if (dic == NULL)
    dic = (int *) malloc ((DICSIZE + 1) * sizeof (int));

  for (i = 0; i < DICSIZE; i++)
    dic[i] = 0;

  dic['a'] = 0;			/* a */
  dic['c'] = 1;			/* c */
  dic['g'] = 2;			/* g */
  dic['t'] = 3;			/* t */
  dic['A'] = 0;			/* A */
  dic['C'] = 1;			/* C */
  dic['G'] = 2;			/* G */
  dic['T'] = 3;			/* T */
  dic['r'] = dic['g'];
  dic['R'] = dic['g'];
  dic['y'] = dic['t'];
  dic['Y'] = dic['t'];
  dic['m'] = dic['a'];
  dic['M'] = dic['a'];
  dic['k'] = dic['g'];
  dic['K'] = dic['g'];
  dic['s'] = dic['g'];
  dic['S'] = dic['g'];
  dic['w'] = dic['a'];
  dic['W'] = dic['a'];
  dic['h'] = dic['a'];
  dic['H'] = dic['a'];
  dic['b'] = dic['g'];
  dic['B'] = dic['g'];
  dic['v'] = dic['g'];
  dic['V'] = dic['g'];
  dic['d'] = dic['g'];
  dic['D'] = dic['g'];
  dic['n'] = dic['g'];
  dic['N'] = dic['g'];
  dic['u'] = dic['t'];
  dic['U'] = dic['t'];

  return dic;
}

/* reverse and complement a string */
char *
revcomp_string (char *seq)
{
  long i, j, n;
  char c;
  char *new_seq;
  char dic[256];

  /* generate dictionary */
  for (i = 0; i < 256; i++)
    dic[i] = 'X';
  dic[(int) 'A'] = 'T';
  dic[(int) 'a'] = 'T';
  dic[(int) 'C'] = 'G';
  dic[(int) 'c'] = 'G';
  dic[(int) 'G'] = 'C';
  dic[(int) 'g'] = 'C';
  dic[(int) 'T'] = 'A';
  dic[(int) 't'] = 'A';

  n = strlen (seq);
  new_seq = (char *) emalloc ((n + 1) * sizeof (char));
  j = 0;
  for (i = n - 1; i >= 0; i--)
    {
      c = dic[(int) seq[i]];
      if (c == 'X')
	new_seq[j++] = seq[i];
      else
	new_seq[j++] = c;
    }
  new_seq[n] = '\0';
  return new_seq;
}

/* reverse and complement a sequence */
Sequence *
revcomp (Sequence * seq)
{
  long i, j, n;
  char c;
  Sequence *new_seq;
  new_seq = (Sequence *) emalloc (sizeof (Sequence));

  n = seq->len;
  new_seq->seq = (char *) emalloc ((n + 1) * sizeof (char));

  new_seq->id = strdup2 (seq->id);
  j = 0;
  for (i = n - 1; i >= 0; i--)
    {
      c = seq->seq[i];
      switch (c)
	{
	case BORDER:
	  new_seq->seq[j++] = BORDER;
	  break;
	case 'A':
	  new_seq->seq[j++] = 'T';
	  break;
	case 'C':
	  new_seq->seq[j++] = 'G';
	  break;
	case 'G':
	  new_seq->seq[j++] = 'C';
	  break;
	case 'T':
	  new_seq->seq[j++] = 'A';
	  break;
	default:
	  new_seq->seq[j++] = c;
	  break;
	}
    }
  new_seq->seq[n] = '\0';
  return new_seq;
}

/* Get next sequence from an open data stream in FASTA format; this stream may be the stdin */
Sequence *
get_permanent_next_sequence (FILE * fp)
{
  Sequence *sequence;
  int seqlen, seqi, i, l;
  int currentBuffer;

  if (lastSequence)
    {
      return NULL;
    }
  if (line == NULL)
    {
      line = (char *) emalloc ((SEQLINE + 2) * sizeof (char));
      line = fgets (line, SEQLINE, fp);
    }
  /* make a sequence object */
  sequence = (Sequence *) emalloc (sizeof (Sequence));
  /* allocate memory for sequence id */
  sequence->id = (char *) emalloc ((strlen (line) + 1) * sizeof (char));
  /* copy sequence id */
  strcpy (sequence->id, chomp (line));
  /* allocate memory for sequence string */
  sequence->seq = (char *) emalloc ((SEQBUFFER + 1) * sizeof (char));
  seqlen = 0;
  currentBuffer = SEQBUFFER;
  seqi = 0;
  while ((line = fgets (line, SEQLINE, fp)) != NULL)
    {
      if (strstr (line, ">") != NULL)
	{
	  sequence->seq[seqi++] = '\0';
	  sequence->seq =
	    (char *) realloc (sequence->seq, seqi * sizeof (char));
	  return sequence;
	}
      if (strlen (line) > SEQLINE)
	{
	  printf
	    ("error in getNextSequence: cannot deal with lines longer than %d bp.\n",
	     SEQLINE);
	  printf
	    ("  change the SEQLINE parameter in file sequenceData.h and recompile.\n");
	  exit (2);
	}
      l = strlen (line);
      /* disregard the final carriage return */
      if (line[l - 1] == '\n')
	l--;
      seqlen += l;
      if (seqlen > currentBuffer)
	{
	  currentBuffer += SEQBUFFER;
	  sequence->seq = (char *) erealloc (sequence->seq, currentBuffer);
	}
      for (i = 0; i < l; i++)
	{
	  sequence->seq[seqi++] = line[i];
	}
      /* sequence->seq = strncat(sequence->seq,line,strlen(line)-1); */
    }
  sequence->seq[seqi++] = '\0';
  sequence->seq = (char *) realloc (sequence->seq, seqi * sizeof (char));
  sequence->len = seqi - 1;
  lastSequence = 1;
  return sequence;
}

void
resetSequenceReader ()
{
  line = NULL;
  lastSequence = 0;
}

/* convert multiple sequences contained in seq into an 
 * array of sequences each representing a single sequence
 */
Sequence **
sequence2array (Sequence * seq)
{
  Sequence **seqs;
  int i, j, k, len;

  /* allocate space for sequences */
  seqs = (Sequence **) emalloc (seq->num_seq * sizeof (Sequence *));
  for (i = 0; i < seq->num_seq; i++)
    {
      seqs[i] = (Sequence *) emalloc (sizeof (Sequence));
      seqs[i]->freq_tab = (int *) emalloc (DICSIZE * sizeof (int));
      seqs[i]->num_nuc = 0;
      for (j = 0; j < DICSIZE; j++)
	seqs[i]->freq_tab[j] = 0;
    }
  /* deal with first sequence */
  len = seq->borders[0] + 1;
  seqs[0]->seq = (char *) emalloc ((len + 1) * sizeof (char));
  seqs[0]->len = len;
  for (i = 0; i < len; i++)
    {
      seqs[0]->seq[i] = seq->seq[i];
      seqs[0]->freq_tab[(int) seq->seq[i]]++;
      seqs[0]->num_nuc++;
    }
  seqs[0]->num_nuc *= 2;
  seqs[0]->seq[len - 1] = BORDER;
  seqs[0]->seq[len] = '\0';
  seqs[0]->id = emalloc (6 * sizeof (char));
  seqs[0]->id[0] = '\0';
  strcat (seqs[0]->id, "strId");
  seqs[0]->num_seq = 1;
  seqs[0]->borders = (int *) emalloc (sizeof (int));
  seqs[0]->borders[0] = seq->borders[0];
  seqs[0]->headers = (char **) emalloc (sizeof (char *));
  seqs[0]->headers[0] =
    (char *) emalloc ((strlen (seq->headers[0]) + 1) * sizeof (char));
  seqs[0]->headers[0] = strcpy (seqs[0]->headers[0], seq->headers[0]);
  /* deal with remaining sequences */
  for (i = 1; i < seq->num_seq; i++)
    {
      len = seq->borders[i] - seq->borders[i - 1];
      seqs[i]->len = len;
      seqs[i]->seq = (char *) emalloc ((len + 1) * sizeof (char));
      k = 0;
      for (j = seq->borders[i - 1] + 1; j < seq->borders[i]; j++)
	{
	  seqs[i]->seq[k++] = seq->seq[j];
	  seqs[i]->freq_tab[(int) seq->seq[j]]++;
	  seqs[i]->num_nuc++;
	}
      seqs[i]->seq[len - 1] = BORDER;
      seqs[i]->seq[len] = '\0';
      seqs[i]->id = emalloc (6 * sizeof (char));
      seqs[i]->id[0] = '\0';
      strcat (seqs[i]->id, "strId");
      seqs[i]->num_seq = 1;
      seqs[i]->borders = (int *) emalloc (sizeof (int));
      seqs[i]->borders[0] = len - 1;
      seqs[i]->headers = (char **) emalloc (sizeof (char *));
      seqs[i]->headers[0] =
	(char *) emalloc ((strlen (seq->headers[i]) + 1) * sizeof (char));
      seqs[i]->headers[0] = strcpy (seqs[i]->headers[0], seq->headers[i]);
      seqs[i]->num_nuc *= 2;
    }
  return seqs;
}

/* read FASTA-formatted sequence data from an open file descriptor 
 * into single sequence string
 */
Sequence *
read_fasta (int fd)
{
  Sequence *s;
  char buf[BUFSIZ];
  int headerOpen;
  int headerLen;
  long i, maxLen;
  int c;

  if (fd < 0)
    return NULL;

  s = (Sequence *) emalloc (sizeof (Sequence));
  s->freq_tab = (int *) emalloc (DICSIZE * sizeof (int));
  for (i = 0; i < DICSIZE; i++)
    s->freq_tab[i] = 0;
  s->borders = (int *) emalloc (sizeof (int));
  s->headers = (char **) emalloc (sizeof (char *));
  s->id = (char *) emalloc (6 * sizeof (char));
  s->id[0] = '\0';
  strcat (s->id, "strId");
  headerOpen = 0;
  s->len = 0;
  s->num_seq = 0;
  maxLen = 0;
  headerLen = 0;

  while ((c = read (fd, buf, BUFSIZ)) > 0)
    {
      if (s->len + c + 1 > maxLen)
	{
	  if (maxLen >= BUFSIZ)
	    {
	      maxLen *= 2;
	      s->seq =
		(char *) erealloc (s->seq, (maxLen + 2) * sizeof (char));
	    }
	  else
	    {
	      maxLen = BUFSIZ;
	      s->seq = (char *) emalloc ((maxLen + 2) * sizeof (char));
	    }
	}
      /* go through the c characters just read into buf */
      for (i = 0; i < c; i++)
	{
	  if (buf[i] == '>')
	    {
	      headerOpen = 1;
	      /* take care of sequence borders */
	      s->borders =
		(int *) erealloc (s->borders, (s->num_seq + 1) * sizeof (int));
	      if (s->num_seq >= 1)
		{
		  s->seq[s->len] = BORDER;	/* unique character to terminate each sequence */
		  s->borders[s->num_seq - 1] = s->len;
		  s->len++;
		}
	      /* take care of sequence headers */
	      s->headers =
		(char **) erealloc (s->headers,
				    (s->num_seq + 1) * sizeof (char *));
	      s->headers[s->num_seq] =
		(char *) emalloc ((BUFSIZ + 1) * sizeof (char));
	      headerLen = 0;
	      s->num_seq++;
	    }
	  if (headerOpen)
	    {
	      if (buf[i] == '\n')
		{
		  headerOpen = 0;
		  s->headers[s->num_seq - 1][headerLen] = '\0';
		  /* trim header to actual length */
		  s->headers[s->num_seq - 1] =
		    (char *) erealloc (s->headers[s->num_seq - 1],
				       (headerLen + 1) * sizeof (char));
		}
	      else if (headerLen < BUFSIZ && isprint (buf[i]))
		s->headers[s->num_seq - 1][headerLen++] = buf[i];
	    }
	  else if (buf[i] != '\n')
	    {
	      s->seq[s->len] = buf[i];
	      s->freq_tab[(int) buf[i]]++;
	      s->len++;
	    }
	}
    }
  /* add last border */
  if (s->len < maxLen)
    s->seq[s->len] = BORDER;
  else
    {
      printf ("ERROR [read_fasta]: s->len: %d; maxLen: %d\n", (int) s->len,
	      (int) maxLen);
      exit (0);
    }
  s->len++;
  s->num_nuc = s->len - s->num_seq;
  /* set end of last sequence read */
  s->borders[s->num_seq - 1] = s->len - 1;
  /* trim sequence to actual size */
  s->seq = (char *) erealloc (s->seq, (s->len + 1) * sizeof (char));
  s->seq[s->len] = '\0';
  return s;
}


/* Get next sequence from an open data stream in FASTA format; this stream may be the stdin */
Sequence *
get_next_sequence (FILE * fp)
{
  Sequence *sequence;
  int seqi, i, l;
  int currentBuffer;

  if (fp == NULL || lastSequence)
    {
      return NULL;
    }

  if (line == NULL)
    {
      line = (char *) malloc ((SEQLINE + 2) * sizeof (char));
      line = fgets (line, SEQLINE, fp);
    }

  /* make a sequence object */
  sequence = (Sequence *) malloc (sizeof (Sequence));
  /* allocate memory for sequence id */
  sequence->id = (char *) malloc ((strlen (line) + 1) * sizeof (char));
  /* copy sequence id */
  strcpy (sequence->id, line);
  /* allocate memory for sequence string */
  sequence->seq = (char *) malloc ((SEQBUFFER + 1) * sizeof (char));
  sequence->num_seq = 1;
  sequence->headers = (char **) emalloc (sizeof (char *));
  sequence->headers[0] = (char *) emalloc (sizeof (char));

  sequence->borders = (int *) emalloc (sizeof (int));


  sequence->len = 0;
  currentBuffer = SEQBUFFER;
  seqi = 0;
  while ((line = fgets (line, SEQLINE, fp)) != NULL)
    {
      if (line[0] == '>')
	{
	  sequence->len++;
	  sequence->seq = (char *) realloc (sequence->seq, sequence->len);
	  sequence->seq[sequence->len - 1] = BORDER;
	  sequence->borders[0] = sequence->len - 1;
	  return sequence;
	}
      if (strlen (line) > SEQLINE)
	{
	  printf
	    ("error in getNextSequence: cannot deal with lines longer than %d bp.\n",
	     SEQLINE);
	  printf
	    ("  change the SEQLINE parameter in file sequenceData.h and recompile.\n");
	  exit (2);
	}
      l = strlen (line);
      /* disregard the final carriage return */
      if (line[l - 1] == '\n')
	l--;
      sequence->len += l;
      if (sequence->len > currentBuffer)
	{
	  currentBuffer += SEQBUFFER;
	  sequence->seq = (char *) erealloc (sequence->seq, currentBuffer);
	}
      for (i = 0; i < l; i++)
	{
	  sequence->seq[seqi++] = line[i];
	}
      /* sequence->seq = strncat(sequence->seq,line,strlen(line)-1); */
    }
  sequence->len++;
  sequence->seq = (char *) realloc (sequence->seq, sequence->len);
  sequence->seq[sequence->len - 1] = BORDER;
  sequence->borders[0] = sequence->len - 1;
  lastSequence = 1;
  return sequence;
}

/* freeSequence: free the data structure Sequence */
Sequence *
free_sequence (Sequence * seq)
{
  int i;

  for (i = 0; i < seq->num_seq; i++)
    free (seq->headers[i]);
  free (seq->headers);
  free (seq->borders);
  free (seq->id);
  free (seq->seq);
  free (seq);
  return seq;
}

/* prepare_seq: prepares sequence string for analysis by shustring-type programs.
 * Does the following: 1) set all residues to upper case
 *                     2) generate reverse complement
 *                     3) concatenate reverse complement to end of forward strand
 */
void
prepare_seq (Sequence * sequence)
{
  Sequence *rstrand;
  int i;
  char *nuc = "TCAGtcag";

  strtoupper (sequence->seq);
  /* take care of reverse strand */
  rstrand = revcomp (sequence);
  rstrand->headers = (char **) emalloc (sizeof (char *));
  rstrand->headers[0] = (char *) emalloc (sizeof (char));
  rstrand->borders = (int *) emalloc (sizeof (int));
  rstrand->num_seq = 1;
  sequence->len += sequence->len;
  sequence->seq =
    (char *) erealloc (sequence->seq, (sequence->len + 1) * sizeof (char));
  sequence->seq[sequence->len / 2] = '\0';
  sequence->borders =
    (int *) erealloc (sequence->borders, 2 * sequence->num_seq * sizeof (int));
  for (i = 0; i < sequence->num_seq; i++)
    sequence->borders[sequence->num_seq + i] = 2 * sequence->borders[i] + 1;
  /*   move first border of reverted sequences to the end */
  rstrand->seq++;
  strcat (sequence->seq, rstrand->seq);
/*   rstrand->seq--; */
/*   sequence->seq[sequence->len-1] = BORDER; */
/*   sequence->seq[sequence->len] = '\0'; */
/*   freeSequence(rstrand); */
  sequence->num_nuc = 0;
  for (i = 0; i < 8; i++)
    sequence->num_nuc += sequence->freq_tab[(int) nuc[i]];
  sequence->num_nuc *= 2;
}

/* catSeq: concatenate the sequences contained in two Sequence objects */
Sequence *
cat_seq (Sequence * seq1, Sequence * seq2)
{
  Sequence *cat;
  int i, j, n;

  cat = (Sequence *) emalloc (sizeof (Sequence));
  cat->seq =
    (char *) emalloc ((strlen (seq1->seq) + strlen (seq2->seq) + 1) *
		      sizeof (char));
  cat->seq[0] = '\0';
  cat->seq = strcat (cat->seq, seq1->seq);
  cat->seq = strcat (cat->seq, seq2->seq);
  cat->id = (char *) emalloc (6 * sizeof (char));
  cat->id[0] = '\0';
  strcat (cat->id, "strId");
  n = seq1->num_seq + seq2->num_seq;
  cat->num_seq = n;
  cat->borders = (int *) emalloc (n * sizeof (int));
  cat->headers = (char **) emalloc (n * sizeof (char *));
  for (i = 0; i < seq1->num_seq; i++)
    {
      cat->borders[i] = seq1->borders[i];
      cat->headers[i] =
	(char *) emalloc ((strlen (seq1->headers[i]) + 1) * sizeof (char));
      cat->headers[i] = strcpy (cat->headers[i], seq1->headers[i]);
    }
  j = i;
  for (i = 0; i < seq2->num_seq; i++)
    {
      cat->borders[j] =
	seq1->borders[seq1->num_seq - 1] + seq2->borders[i] + 1;
      cat->headers[j] =
	(char *) emalloc ((strlen (seq2->headers[i]) + 1) * sizeof (char));
      cat->headers[j] = strcpy (cat->headers[j], seq2->headers[i]);
      j++;
    }
  cat->len = seq1->len + seq2->len;
  cat->num_query_nuc = seq1->num_nuc;
  cat->num_sbjct_nuc = seq2->num_nuc;
  cat->num_nuc = seq1->num_nuc + seq2->num_nuc;
  return cat;
}

/* initialize_profiles: initialize array to keep nucleotide profiles */
int **
initialize_profiles (int **profiles, int n)
{
  int i, j;

  profiles = (int **) emalloc (n * sizeof (int *));
  for (i = 0; i < n; i++)
    {
      profiles[i] = (int *) emalloc (5 * sizeof (int));
      for (j = 0; j < 5; j++)
	profiles[i][j] = 0;
    }

  return profiles;
}

/* print_profiles: print profiles; under-covered regions
 * mark contig boundaries
 */
void
print_profiles (FILE * fout, int **profiles, int max_res, int min_cov)
{
  int i, j;
  int contig_num, cov, contig_ind, open;

  contig_num = 0;
  contig_ind = 0;
  open = 0;
  for (i = 0; i < max_res; i++)
    {
      cov = 0;
      for (j = 0; j < 4; j++)
	cov += profiles[i][j];
      if (cov >= min_cov)
	{
	  if (!open)
	    {
	      open = 1;
	      fprintf (fout, ">Contig_%d\n", ++contig_num);
	    }
	  fprintf (fout, "%d", ++contig_ind);
	  for (j = 0; j < 4; j++)
	    fprintf (fout, "\t%d", profiles[i][j]);
	  fprintf (fout, "\n");
	}
      else
	{
	  contig_ind = 0;
	  open = 0;
	}
    }
}
