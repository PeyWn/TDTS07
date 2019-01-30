/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: /tmp_amd/presto/export/kbs/jutta/src/gsm/RCS/toast.c,v 1.8 1996/07/02 10:41:04 jutta Exp $ */

#include	"toast.h"
#include 	"my_private.h"
#include        "marcus_dec_mem.h"
//#define MARCUS_de_en
/*  toast -- lossy sound compression using the gsm library.
 */
#ifdef SASA_MPARM
#include "sasa_misc.h"
#include "sasa_support.h"
#endif

#ifdef SASA_MPARM
#include "appsupport.h"
#endif


/* Setting up queues */
#include        "codec_queues.h"
#include        "system.h"
#include        "scratch_queue.h"

#define MAX_QUEUE_SIZE 1

#define OBJ_SIZE (sizeof(struct encoder_queue))




char   * progname;

int	f_decode   = 0;		/* decode rather than encode	 (-d) */
int 	f_cat	   = 0;		/* write to stdout; implies -p   (-c) */
int	f_force	   = 0;		/* don't ask about replacements  (-f) */
int	f_precious = 0;		/* avoid deletion of original	 (-p) */
int	f_fast	   = 0;		/* use faster fpt algorithm	 (-F) */
int	f_verbose  = 0;		/* debugging			 (-V) */
int	f_ltp_cut  = 0;		/* LTP cut-off margin	      	 (-C) */
int sasa_malloc_counter=0;
struct stat instat;		/* stat (inname) 		 */

FILE	*in, 	 *out;
char	*inname, *outname;

/*
 *  The function (*output)() writes a frame of 160 samples given as
 *  160 signed 16 bit values (gsm_signals) to <out>.
 *  The function (*input)() reads one such frame from <in>.
 *  The function (*init_output)() begins output (e.g. writes a header).,
 *  The function (*init_input)() begins input (e.g. skips a header).
 *
 *  There are different versions of input, output, init_input and init_output
 *  for different formats understood by toast; which ones are used 
 *  depends on the command line arguments and, in their absence, the
 *  filename; the fallback is #defined in toast.h
 *
 *  The specific implementations of input, output, init_input and init_output
 *  for a format `foo' live in toast_foo.c.
 */

//typedef struct scratch_queue_type_producer SCRATCH_QUEUE_PRODUCER;
//typedef struct scratch_queue_type_consumer SCRATCH_QUEUE_CONSUMER;
extern SCRATCH_QUEUE_PRODUCER* en_prod;
extern SCRATCH_QUEUE_CONSUMER* en_cons;
extern SCRATCH_QUEUE_PRODUCER* de_prod;
extern SCRATCH_QUEUE_CONSUMER* de_cons;

static void
en_write_locals_to_queue(struct gsm_state *S, word* Nc, word* bc, word* Mc, word* xmaxc, 
		      word* xMc, word* LARc, word *e, word *so) {

  int                    i;
  struct encoder_queue   *en_prod_queue;
  PR_INT(-7001);
  en_prod_queue = (struct encoder_queue *) scratch_queue_getToken_write(en_prod);
  for (i=0; i<4; i++) en_prod_queue->en_qu_Nc[i] = Nc[i];
  for (i=0; i<4; i++) en_prod_queue->en_qu_bc[i] = bc[i];
  for (i=0; i<4; i++) en_prod_queue->en_qu_Mc[i] = Mc[i]; 
  for (i=0; i<4; i++) en_prod_queue->en_qu_xmaxc[i] = xmaxc[i];
  for (i=0; i<13*4; i++) en_prod_queue->en_qu_xMc[i] = xMc[i];
  for (i=0; i<8; i++) en_prod_queue->en_qu_LARc[i] = LARc[i];
  for (i=0; i<50; i++) en_prod_queue->en_qu_e[i] = e[i];
  for (i=0; i<160; i++) en_prod_queue->en_qu_so[i] = so[i];
  scratch_queue_putToken_write(en_prod); 
  PR_INT(-7011);
}

static void
en_read_locals_from_queue(struct gsm_state *S, word* Nc, word* bc, word* Mc, word* xmaxc, 
		      word* xMc, word* LARc, word *e, word *so) {

  int i;
  struct encoder_queue *en_cons_queue;
  PR_INT(-7002);
  en_cons_queue = (struct encoder_queue *) scratch_queue_read1(en_cons);
  for (i=0; i<4; i++) Nc[i] = en_cons_queue->en_qu_Nc[i];
  for (i=0; i<4; i++) bc[i] = en_cons_queue->en_qu_bc[i];
  for (i=0; i<4; i++) Mc[i] = en_cons_queue->en_qu_Mc[i]; 
  for (i=0; i<4; i++) xmaxc[i] = en_cons_queue->en_qu_xmaxc[i];
  for (i=0; i<13*4; i++) xMc[i] = en_cons_queue->en_qu_xMc[i];
  for (i=0; i<8; i++) LARc[i] = en_cons_queue->en_qu_LARc[i];
  for (i=0; i<50; i++) e[i] = en_cons_queue->en_qu_e[i];
  for (i=0; i<160; i++) so[i] = en_cons_queue->en_qu_so[i];
  PR_INT(-7012);
}

static void
de_write_locals_to_queue(struct gsm_state *S, word *LARcr, word *Ncr, word *bcr, word *Mcr, 
			 word *xmaxcr, word *xMcr, word *s, word *erp, word *wt, word *drp) {

  int                    i;
  struct decoder_queue   *de_prod_queue;
  PR_INT(-7003);
  de_prod_queue = (struct decoder_queue *) scratch_queue_getToken_write(de_prod);
  for (i=0; i<8; i++) de_prod_queue->de_qu_LARcr[i] = LARcr[i];
  for (i=0; i<4; i++) de_prod_queue->de_qu_Ncr[i] = Ncr[i];
  for (i=0; i<4; i++) de_prod_queue->de_qu_bcr[i] = bcr[i];
  for (i=0; i<4; i++) de_prod_queue->de_qu_xmaxcr[i] = xmaxcr[i];
  for (i=0; i<13*4; i++) de_prod_queue->de_qu_xMcr[i] = xMcr[i];
  for (i=0; i<160; i++) de_prod_queue->de_qu_s[i] = s[i];
  for(i=0; i<40; i++) de_prod_queue->de_qu_erp[i] = erp[i];
  for(i=0; i<160; i++) de_prod_queue->de_qu_wt[i] = wt[i];
  de_prod_queue->de_qu_drp = drp[0];
  scratch_queue_putToken_write(de_prod);
  PR_INT(-7013);
}

static void
de_read_locals_from_queue(struct gsm_state *S, word *LARcr, word *Ncr, word *bcr, word *Mcr, 
			  word *xmaxcr, word *xMcr, word *s, word *erp, word *wt, word *drp) {

  int                    i;
  struct decoder_queue   *de_cons_queue;

PR_INT(-7004);
  de_cons_queue = (struct decoder_queue *) scratch_queue_read1(de_cons);
  for (i=0; i<8; i++) LARcr[i] = de_cons_queue->de_qu_LARcr[i];
  for (i=0; i<4; i++) Ncr[i] = de_cons_queue->de_qu_Ncr[i];
  for (i=0; i<4; i++) bcr[i] = de_cons_queue->de_qu_bcr[i];
  for (i=0; i<4; i++)xmaxcr[i] =  de_cons_queue->de_qu_xmaxcr[i];

  for (i=0; i<13*4; i++) xMcr[i] = de_cons_queue->de_qu_xMcr[i];
  for (i=0; i<160; i++)s[i] =  de_cons_queue->de_qu_s[i];
  for(i=0; i<40; i++) erp[i] = de_cons_queue->de_qu_erp[i];
  for(i=0; i<160; i++) wt[i] = de_cons_queue->de_qu_wt[i];
  drp[0] = de_cons_queue->de_qu_drp;
PR_INT(-7014);

}

static void Postprocessing P2((S,s),
	struct gsm_state	* S,
	register word 		* s)
{
	register int		k;
	register word		msr = S->msr;
	register longword	ltmp;	/* for GSM_ADD */
	register word		tmp;

	for (k = 160; k--; s++) {
		tmp = GSM_MULT_R( msr, 28180 );
		msr = GSM_ADD(*s, tmp);  	   /* Deemphasis 	     */
		*s  = GSM_ADD(msr, msr) & 0xFFF8;  /* Truncation & Upscaling */
	}
	S->msr = msr;
}


int	(*output   ) P((gsm_signal *)),
	(*input    ) P((gsm_signal *));
int	(*init_input)  P((void)),
	(*init_output) P((void));

static int	generic_init P0() { return 0; }	/* NOP */

struct fmtdesc {

	char * name, * longname, * suffix;

	int  (* init_input )  P((void)),
	     (* init_output)  P((void));

	int  (* input ) P((gsm_signal * )),
	     (* output) P((gsm_signal * ));

} f_audio = {
		"audio",
		"8 kHz, 8 bit u-law encoding with Sun audio header", ".au",
		audio_init_input,
		audio_init_output,
		ulaw_input,
		ulaw_output
}, f_ulaw = {
		"u-law", "plain 8 kHz, 8 bit u-law encoding", ".u",
		generic_init,
		generic_init,
		ulaw_input,
		ulaw_output 

}, f_alaw = {
		"A-law", "8 kHz, 8 bit A-law encoding", ".A",
		generic_init,
		generic_init,
		alaw_input,
		alaw_output

}, f_linear = {
		"linear",
		"16 bit (13 significant) signed 8 kHz signal", ".l",
		generic_init,
		generic_init,
		linear_input,
		linear_output
};

struct fmtdesc * alldescs[] = {
	&f_audio,
	&f_alaw,
	&f_ulaw,
	&f_linear,
	(struct fmtdesc *)NULL
};

#define	DEFAULT_FORMAT	f_ulaw		/* default audio format, others	*/
					/* are: f_alaw,f_audio,f_linear */
struct fmtdesc * f_format  = 0;

/*
 *  basename + suffix of a pathname
 */
static char * endname P1((name), char * name)
{
	if (name) {
		char * s = strrchr(name, '/');
		if (s && s[1]) name = s + 1;
	}
	return name;

}

/*
 *  Try to figure out what we're supposed to do from the argv[0], if
 *  any, and set the parameters accordingly.
 */
static void parse_argv0 P1((av0), char * av0 )
{
	int 	l;

	progname = av0 = endname(av0 ? av0 : "toast");

	/*  If the name starts with `un', we want to decode, not code.
	 *  If the name ends in `cat', we want to write to stdout,
	 *  and decode as well.
	 */
#ifndef MARCUS_de_en
	if (!strncmp(av0, "un", 2)) f_decode = 1;
	if (  (l = strlen(av0)) >= 3 /* strlen("cat") */
	   && !strcmp( av0 + l - 3, "cat" )) f_cat = f_decode = 1;
#endif
}


/*
 *  Check whether the name (possibly generated by appending
 *  .gsm to something else) is short enough for this system.
 */
static int length_okay P1((name), char * name)
{
	long	max_filename_length = 0;
	char	* end;

	/* If our _pathname_ is too long, we'll usually not be
	 * able to open the file at all -- don't worry about that.
	 * 
	 * But if the _filename_ is too long, there is danger of
	 * silent truncation on some systems, which results
	 * in the target replacing the source!
	 */

	if (!name) return 0;
	end = endname(name);

#ifdef	NAME_MAX
	max_filename_length  = NAME_MAX;
#else
#ifdef	_PC_NAME_MAX
#ifdef USE_PATHCONF
	{	char * s, tmp; 
		
		/*  s = dirname(name)
		 */
		if ((s = end) > name) {
			if (s > name + 1) s--;
			tmp = s;
			*s  = 0;
		}

		errno = 0;
		max_filename_length = pathconf(s > name ? name : ".",
			_PC_NAME_MAX);
		if (max_filename_length == -1 && errno) {
			perror( s > name ? name : "." );
			fprintf(stderr,
		"%s: cannot get dynamic filename length limit for %s.\n",
				progname, s > name ? name : ".");
			return 0;
		}
		if (s > name) *s = tmp;
	}
#endif /* USE_PATHCONF  */
#endif /* _PC_NAME_MAX  */
#endif /* !NAME_MAX 	*/

	if (max_filename_length > 0 && strlen(end) > max_filename_length) {
		fprintf(stderr,
			"%s: filename \"%s\" is too long (maximum is %ld)\n",
			progname, endname(name), max_filename_length );
		return 0;
	}

	return 1;
}

/*
 *  Return a pointer the suffix of a string, if any.
 *  A suffix alone has no suffix, an empty suffix can not be had.
 */
static char * suffix P2((name, suf), char *name, char * suf) 
{
	size_t nlen = strlen(name);
	size_t slen = strlen(suf);

	if (!slen || nlen <= slen) return (char *)0;
	name += nlen - slen;
	return memcmp(name, suf, slen) ? (char *)0 : name;
}


static void catch_signals P1((fun), SIGHANDLER_T (*fun) ()) 
{
#ifndef SASA_COMMENT

#ifdef	SIGHUP
	signal( SIGHUP,   fun );
#endif
#ifdef	SIGINT
	signal( SIGINT,   fun );
#endif
#ifdef	SIGPIPE
	signal( SIGPIPE,  fun );
#endif
#ifdef	SIGTERM
	signal( SIGTERM,  fun );
#endif
#ifdef	SIGXFSZ
	signal( SIGXFSZ,  fun );
#endif

#endif /*SASA_COMMENT*/
}

static SIGHANDLER_T onintr P0()
{
#ifndef SASA_COMMENT
	char * tmp = outname;

#ifdef	HAS_SYSV_SIGNALS
	catch_signals( SIG_IGN );
#endif

	outname = (char *)0;
#ifndef SASA
	if (tmp) (void)unlink(tmp);
#endif /*SASA*/
#endif
	exit(1);
}

/*
 *  Allocate some memory and complain if it fails.
 */
static char * emalloc P1((len), size_t len)
{
	char * s;
	if (!(s = malloc(len))) {
#ifndef SASA_COMMENT
		fprintf(stderr, "%s: failed to malloc %d bytes -- abort\n",
			progname, len);
		onintr();
#endif
		exit(1);
	}
	return s;
}

static char* normalname P3((name, want, cut), char *name, char *want,char *cut)
{
	size_t	maxlen;
	char 	* s, * p;

	p = (char *)0;
	if (!name) return p;

	maxlen = strlen(name) + 1 + strlen(want) + strlen(cut);
	p = strcpy(emalloc(maxlen), name);

	if (s = suffix(p, cut)) strcpy(s, want);
	else if (*want && !suffix(p, want)) strcat(p, want);

	return p;
}

/*
 *  Generate a `plain' (non-encoded) name from a given name.
 */
static char * plainname P1((name), char *name)
{
	return normalname(name, "", SUFFIX_TOASTED );
}

/*
 *  Generate a `code' name from a given name.
 */
static char * codename P1((name), char *name)
{
	return normalname( name, SUFFIX_TOASTED, "" );
}

/*
 *  If we're supposed to ask (fileno (stderr) is a tty, and f_force not
 *  set), ask the user whether to overwrite a file or not.
 */
static int ok_to_replace P1(( name ), char * name)
{
	int reply, c;

	if (f_force) return 1;			/* YES, do replace   */
	if (!isatty(fileno(stderr))) return 0;	/* NO, don't replace */

	fprintf(stderr,
		"%s already exists; do you wish to overwrite %s (y or n)? ",
		name, name);
	fflush(stderr);

	for (c = reply = getchar(); c != '\n' && c != EOF; c = getchar()) ;
	if (reply == 'y') return 1;

	fprintf(stderr, "\tnot overwritten\n");
	return 0;
}

static void update_mode P0()
{
#ifndef SASA_COMMENT
	if (!instat.st_nlink) return;		/* couldn't stat in */


#ifdef HAS_FCHMOD
	if (fchmod(fileno(out), instat.st_mode & 07777)) {
		perror(outname);
		fprintf(stderr, "%s: could not change file mode of \"%s\"\n",
			progname, outname);
	}
#else 
#ifdef HAS_CHMOD
	if (outname && chmod(outname, instat.st_mode & 07777)) {
		perror(outname);
		fprintf(stderr, "%s: could not change file mode of \"%s\"\n",
			progname, outname);
	}
#endif /* HAS_CHMOD  */
#endif /* HAS_FCHMOD */

#endif /*SASA_COMMENT*/
}

static void update_own P0()
{
	if (!instat.st_nlink) return; /* couldn't stat in */
#ifdef HAS_FCHOWN
/*	(void)fchown(fileno(out), instat.st_uid, instat.st_gid);*/
#else 
#ifdef HAS_CHOWN
	(void)chown(outname, instat.st_uid, instat.st_gid);
#endif /* HAS_CHOWN  */
#endif /* HAS_FCHOWN */
}

static void update_times P0()
{
#ifndef SASA_COMMENT

	if (!instat.st_nlink) return; 	/* couldn't stat in */

#ifdef HAS_UTIMES
	if (outname) {
		struct timeval tv[2];

		tv[0].tv_sec  = instat.st_atime;
		tv[1].tv_sec  = instat.st_mtime;
		tv[0].tv_usec = tv[1].tv_usec = 0;
		(void) utimes(outname, tv);
	}
#else
#ifdef HAS_UTIME

	if (outname) {

#ifdef	HAS_UTIMBUF
		struct utimbuf ut;

		ut.actime     = instat.st_atime;
		ut.modtime    = instat.st_mtime;

#	ifdef	HAS_UTIMEUSEC
		ut.acusec     = instat.st_ausec;
		ut.modusec    = instat.st_musec;
#	endif 	/* HAS_UTIMEUSEC */

		(void) utime(outname, &ut);

#else /* UTIMBUF */

		time_t ut[2];

		ut[0] = instat.st_atime;
		ut[1] = instat.st_mtime;

		(void) utime(outname, ut);

#endif	/* UTIMBUF */
	}
#endif /* HAS_UTIME */
#endif /* HAS_UTIMES */

#endif /*SASA_COMMENT*/
}


static int okay_as_input P3((name,f,st), char* name, FILE* f, struct stat * st)
{
/*
# ifdef	HAS_FSTAT
	if (fstat(fileno(f), st) < 0)
# else
	if (stat(name, st) < 0)
# endif
	{
		perror(name);
		fprintf(stderr, "%s: cannot stat \"%s\"\n", progname, name);
		return 0;
	}

	if (!S_ISREG(st->st_mode)) {
		fprintf(stderr,
			"%s: \"%s\" is not a regular file -- unchanged.\n",
			progname, name);
		return 0;
	}
	if (st->st_nlink > 1 && !f_cat && !f_precious) {
		fprintf(stderr, 
		      "%s: \"%s\" has %s other link%s -- unchanged.\n",
			progname,name,st->st_nlink - 1,"s" + (st->st_nlink<=2));
		return 0;
	}
*/
	return 1;
}

static void prepare_io P1(( desc), struct fmtdesc * desc)
{
	output      = desc->output;
	input       = desc->input;

	init_input  = desc->init_input;
	init_output = desc->init_output;
}

static struct fmtdesc * grok_format P1((name), char * name)
{
	char * c;
	struct fmtdesc ** f;

	if (name) {
		c = plainname(name);

		for (f = alldescs; *f; f++) {
			if (  (*f)->suffix
			   && *(*f)->suffix
			   && suffix(c, (*f)->suffix)) {

				free(c);
				return *f;
			}
		}

		free(c);
	}
	return (struct fmtdesc *)0;
}

static int open_input P2((name, st), char * name, struct stat * st)
{
	struct fmtdesc * f = f_format;
	st->st_nlink = 0;	/* indicates `undefined' value */
	
	if (!name) {
		inname = (char *)NULL;
		in     = stdin;
#ifdef	HAS__FSETMODE
		_fsetmode(in, "b");
#endif
	}
	else {
		if (f_decode) inname = codename(name);
		else {
			if (!f_cat && suffix(name, SUFFIX_TOASTED)) {
				fprintf(stderr,
			"%s: %s already has \"%s\" suffix -- unchanged.\n",
					progname, name, SUFFIX_TOASTED );
				return 0;
			}
			inname = strcpy(emalloc(strlen(name)+1), name);
		}
#ifndef SASA_COMMENT
		if (!(in = fopen(inname, READ))) {
			perror(inname);	/* not guaranteed to be valid here */
			fprintf(stderr, "%s: cannot open \"%s\" for reading\n",
				progname, inname);
			return 0;
		}
		if (!okay_as_input(inname, in, st)) return 0;
#endif/*SASA_COMMENT*/
		if (!f) f = grok_format(inname);
	}
	prepare_io( f ? f : & DEFAULT_FORMAT );
	
	return 1;
}

static int open_output P1((name), char *name)
{
	if (!name || f_cat) {
		out     = stdout;
		outname = (char *)NULL;
#ifdef	HAS__FSETMODE
		_fsetmode(out, "b"); 
#endif
	}
	else {
		int outfd = -1;
		char *o;
		o = (*(f_decode ? plainname : codename))(name);
/*		printf("SASA DEBUG: output file name: %s\n", o);*/
 		/*
		if (!length_okay(o)) return 0;
		if ((outfd = open("sasa\0", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU )) >= 0)
			out = fdopen(outfd, WRITE);
		else if (errno != EEXIST) out = (FILE *)NULL;
		else if (ok_to_replace(o)) out = fopen(o, WRITE);
		else return 0;
		*/
#ifndef SASA_COMMENT
                out = fopen(o, "w");
		if (!out) {
			perror(o);
			fprintf(stderr,
				"%s: can't open \"%s\" for writing\n",
				progname, o);
			if (outfd >= 0) (void)close(outfd);
			return 0;
		}
#endif /*SASA_COMMENT*/
		outname = o;
	}
	return 1;
}

gsm      	r;
gsm_signal    	*s;
gsm_byte	*d;
//char		*sasa_barrier;
//char            *marcus_barrier;
//char		*sasa_sem;

/* ******************************** */
/* concurrent decoding and encoding */
static int process_codec P0()
{       //encoder
  gsm      	en_r;
  gsm_signal    en_s[ 160 ];
  gsm_frame	en_d;
  char          *en_sasa_mparm_check; 
  int           en_sasa_c;
  int           en_sasa_i;
  static int    en_sasa_iter_count=0;
  int           en_marcus_dummy;
  int           en_cc;
  word	 	en_LARc[8], en_Nc[4], en_Mc[4];
  word          en_bc[4], en_xmaxc[4], en_xmc[13*4];
  int	        en_k, en_i;
  word	       *en_dp; //  = S->dp0 + 120;	/* [ -120...-1 ] */
  word	       *en_dpp;// = dp;		/* [ 0...39 ]	 */
  static word   en_e[50];
  word	        en_so[160];
  
  en_r = gsm_create();
  (void)gsm_option(en_r, GSM_OPT_FAST,       &f_fast);
  (void)gsm_option(en_r, GSM_OPT_VERBOSE,    &f_verbose);
  (void)gsm_option(en_r, GSM_OPT_LTP_CUT,	&f_ltp_cut);
  
  //decoder
  gsm      	de_r;
  gsm_frame	de_s;
  gsm_signal	de_d[ 160 ];
  int		de_cc;
  int           marcus_count = 0;
  int           marcus_index = 0;
  int           marcus_en_mem_entries = sizeof(ding_en_memory)/sizeof(char);
  int           marcus_dummy;
  int           marcus_out_dummy;
  word  	de_LARc[8], de_Nc[4], de_Mc[4];
  word          de_bc[4], de_xmaxc[4], de_xmc[13*4];
  int		de_j, de_k;
  word		de_erp[40], de_wt[160];
  word		* de_drp;// = S->dp0 + 120;

  de_r = gsm_create();
  (void)gsm_option(de_r, GSM_OPT_FAST,    &f_fast);
  (void)gsm_option(de_r, GSM_OPT_VERBOSE, &f_verbose);
  
  de_drp = de_r->dp0 + 120;
  de_cc = marcus_en_mem_entries; 
  

  // currently only encoder
  en_cc =1;
  while ((en_cc > 0)&&(de_cc > 0)) { /* decode/encode as long as input frames exist */

     
    en_sasa_iter_count++;
    if (en_sasa_iter_count==3) {
      stop_metric();
      stop_simulation();
    }

    //START: get input ready for encoder
    if (get_id()==1) en_cc = (*input)(en_s);
    if (en_cc < sizeof(en_s) / sizeof(*en_s))
      memset((char *)(en_s+en_cc), 0, sizeof(en_s)-(en_cc * sizeof(*en_s)));		
    en_dp  = en_r->dp0 + 120; 
    en_dpp = en_dp;
    //END: get input ready for encoder

    //START: get input ready for decoder
    if (marcus_index + sizeof(de_s) <= marcus_en_mem_entries) 
      de_cc = sizeof(de_s);
    else 
      de_cc = marcus_en_mem_entries - marcus_index;
    
    if (get_id() == 1) {
      marcus_dummy = marcus_index + sizeof(de_s);
      marcus_count = 0;
      for (marcus_index; marcus_index < marcus_dummy; marcus_index++) {
	de_s[marcus_count] = ding_en_memory[marcus_index];
	marcus_count++;
      }
    }
    //END: get input ready for decoder

    // START: Encoder part 1
    if (get_id()==1)
      Gsm_Preprocess(en_r, en_s, en_so); 

    if (get_id()==1)
      Gsm_LPC_Analysis(en_r, en_so, en_LARc);

    if (get_id()==1)
      Gsm_Short_Term_Analysis_Filter(en_r, en_LARc, en_so);

    if (get_id()==1)
      en_write_locals_to_queue(en_r, en_Nc, en_bc, en_Mc, en_xmaxc,
			       en_xmc, en_LARc, en_e, en_so);
    // END: Encoder part 1

    //START: Decoder part 1
    if (get_id()==1) 
      gsm_decode_mparm(de_LARc, de_Nc, de_Mc, de_bc, de_xmaxc, de_xmc, de_s);

    if (get_id()== 1) {
      PR_INT(-700);
      word *xmaxcr = de_xmaxc;
      word *bcr    = de_bc;
      word *Ncr    = de_Nc;
      word *Mcr    = de_Mc;
      word *xMcr   = de_xmc;
      
      for (de_j=0; de_j <= 3; de_j++, xmaxcr++, bcr++, Ncr++, Mcr++, xMcr += 13) {
	
	Gsm_RPE_Decoding(de_r, *xmaxcr, *Mcr, xMcr, de_erp );
	Gsm_Long_Term_Synthesis_Filtering(de_r, *Ncr, *bcr, de_erp, de_drp );
	for (de_k = 0; de_k <= 39; de_k++) de_wt[ de_j * 40 + de_k ] =  de_drp[ de_k ];
      }
      
      /* store in a shared mem (ordered s1-s2-s1-s2...) */
      de_write_locals_to_queue(de_r, de_LARc, de_Nc, de_bc, de_Mc, de_xmaxc, 
			       de_xmc, de_d, de_erp, de_wt, de_drp); 
      
      PR_INT(-701);
	
    }
    //END: Decoder part 1


    //START: Encoder part 2
    if(get_id()==2) 
      en_read_locals_from_queue(en_r, en_Nc, en_bc, en_Mc, en_xmaxc, 
				en_xmc, en_LARc, en_e, en_so);

    if(get_id()==2) {
      word * loc_xmc = en_xmc;
      word * loc_Nc = en_Nc; 
      word * loc_bc = en_bc;
      word * loc_xmaxc = en_xmaxc;
      word * loc_Mc = en_Mc;
      for (en_k = 0; en_k <= 3; en_k++, loc_xmc += 13) {
	Gsm_Long_Term_Predictor(    en_r,
				    en_so+en_k*40,  /* d      [0..39] IN	*/
				    en_dp,	  /* dp  [-120..-1] IN	*/
				    en_e + 5,	  /* e      [0..39] OUT	*/
				    en_dpp,	  /* dpp    [0..39] OUT */
				    loc_Nc++,
				    loc_bc++);
	      
	Gsm_RPE_Encoding	( en_r,
				  en_e + 5,	/* e	  ][0..39][ IN/OUT */
				  loc_xmaxc++, loc_Mc++, loc_xmc );	    
	{ 
	  register int i;
	  register longword ltmp;
	  for (i = 0; i <= 39; i++)
	    en_dp[ i ] = GSM_ADD( en_e[5 + i], en_dpp[i] );
	}
	en_dp  += 40;
	en_dpp += 40;
      }
    }
	  
    if(get_id()==2) 
      (void)memcpy( (char *)en_r->dp0, (char *)(en_r->dp0 + 160),
		    120 * sizeof(*en_r->dp0) );

    if(get_id()==2)
      gsm_encode_mparm(en_LARc, en_Nc, en_Mc, en_bc, en_xmaxc, en_xmc, en_d);

    en_sasa_mparm_check = (char *)en_d;
    if (get_id()==2) {
      for (en_sasa_i=0;en_sasa_i<sizeof(en_d);en_sasa_i++) {
	en_sasa_c = en_sasa_mparm_check[en_sasa_i]; 
        PR_INT(en_sasa_c);
      }
    }

    //START: Decoder part 2
    if (get_id() == 3) {
      PR_INT(-702);
	/* read from a shared mem  */
	de_read_locals_from_queue(de_r, de_LARc, de_Nc, de_bc, de_Mc, de_xmaxc, 
				  de_xmc, de_d, de_erp, de_wt, de_drp);
      
      Gsm_Short_Term_Synthesis_Filter( de_r, de_LARc, de_wt, de_d );
      Postprocessing(de_r, de_d);
      PR_INT(-10500);
      for (de_k=0; de_k<160; de_k++)
	 PR_INT(de_d[de_k]);
      PR_INT(-10501);
    }
    //END: Encoder part 2
    
  }
  
 
//  gsm_destroy(de_r);
// gsm_destroy(en_r);	
  return 0;  
}
  


static int process_encode P0()
{  	
	return -1;
}





SCRATCH_QUEUE_PRODUCER* de_prod;
SCRATCH_QUEUE_CONSUMER* de_cons;

static int process_decode P0()
{         
  return -1;	
}

static int process P1((name), char * name)
{
	int step = 0;

	out     = (FILE *)0;
	in      = (FILE *)0;

	outname = (char *)0;
	inname  = (char *)0;
	
	if (!open_input(name, &instat) || !open_output(name))
		goto err;
	
	if ((*(f_decode ? init_output    : init_input))()) {
		fprintf(stderr, "%s: error %s %s\n",
			progname,
			f_decode ? "writing header to" : "reading header from",
			f_decode ? (outname ? outname : "stdout")
				 : (inname ? inname : "stdin"));
		goto err;
	}
	
	process_codec();
	//	f_decode = 0;
	//if ((*(f_decode ? process_decode : process_encode))())
	// goto err;

#ifndef SASA_COMMENT	
	if (fflush(out) < 0 || ferror(out)) {
		perror(outname ? outname : "stdout");
		fprintf(stderr, "%s: error writing \"%s\"\n", progname,
				outname ? outname:"stdout");
		goto err;
	}

	if (out != stdout) {
		printf("\nsasa77\n"); fflush(stdout);
		update_times();
		update_mode ();
		update_own  ();

		if (fclose(out) < 0) {
			perror(outname);
			fprintf(stderr, "%s: error writing \"%s\"\n",
				progname, outname);
			goto err;
		}
		if (outname != name) free(outname);
		outname = (char *)0;
	}
	out = (FILE *)0;
#endif /*SASA_COMMENT*/	
	if (in  != stdin) {
#ifndef SASA_COMMENT
		(void)fclose(in), in = (FILE *)0;
#endif
		if (!f_cat && !f_precious) {
#ifndef SASA
			if (unlink(inname) < 0) {
				perror(inname);
				fprintf(stderr,
					"%s: source \"%s\" not deleted.\n",
					progname, inname);
			}
#endif
			goto err;
		}
		if (inname != name) free(inname);
		inname = (char *)0;
	}
	return 0;

	/*
	 *  Error handling and cleanup.
	 */
err:
	if (out && out != stdout) {
		(void)fclose(out), out = (FILE *)0;
#ifndef SASA
		if (unlink(outname) < 0 && errno != ENOENT && errno != EINTR) {
			perror(outname);
			fprintf(stderr, "%s: could not unlink \"%s\"\n",
				progname, outname);
		}
#endif
	}
	if (in && in != stdin) (void)fclose(in), in = (FILE *)0;

	if (inname  && inname  != name) free(inname);
	if (outname && outname != name) free(outname);
	return -1;
}

static void version P0()
{
	printf( "%s 1.0, version %s\n",
		progname,
		"$Id: toast.c,v 1.8 1996/07/02 10:41:04 jutta Exp $" );
}

static void help P0()
{
#ifndef SASA_COMMENT
	printf("Usage: %s [-fcpdhvaulsFC] [files...]\n", progname);
	printf("\n");

	printf(" -f  force     Replace existing files without asking\n");
	printf(" -c  cat       Write to stdout, do not remove source files\n");
	printf(" -d  decode    Decode data (default is encode)\n");
	printf(" -p  precious  Do not delete the source\n");
	printf("\n");

	printf(" -u  u-law     Force 8 kHz/8 bit u-law in/output format\n");
	printf(" -s  sun .au   Force Sun .au u-law in/output format\n");
	printf(" -a  A-law     Force 8 kHz/8 bit A-law in/output format\n");
	printf(" -l  linear    Force 16 bit linear in/output format\n");
	printf("\n");

	printf(" -F  fast      Sacrifice conformance to performance\n");
	printf(" -C  cutoff    Ignore most samples during LTP\n");
	printf(" -v  version   Show version information\n");
	printf(" -h  help      Print this text\n");
	printf("\n");
#endif
}


static void set_format P1((f), struct fmtdesc * f)
{
	if (f_format && f_format != f) {
		fprintf( stderr,
	"%s: only one of -[uals] is possible (%s -h for help)\n",
			progname, progname);
		exit(1);
	}

	f_format = f;
}


SCRATCH_QUEUE_PRODUCER* en_prod;
SCRATCH_QUEUE_CONSUMER* en_cons;


struct gsm_state;
#ifdef SASA_MPARM
int main1 P2((ac, av), int ac, char **av)
#else
int main P2((ac, av), int ac, char **av)
#endif
{
	int  		opt;
	int i;
	extern int	optind;
	extern char	* optarg;

	/* init queues */
	scratch_queue_autoinit_system(0,0);  

	if (get_id() == 1) {
	  en_prod = scratch_queue_autoinit_producer(2,1,MAX_QUEUE_SIZE,OBJ_SIZE,0);
	  de_prod = scratch_queue_autoinit_producer(3,1,MAX_QUEUE_SIZE,sizeof(struct decoder_queue),0);
	}
	// (consumer id, sema id, )
	if (get_id() == 2) {
	  en_cons=scratch_queue_autoinit_consumer(1,0);
	  //	  de_cons=scratch_queue_autoinit_consumer(1,0);
	}
	if (get_id() == 3) {
	  de_cons=scratch_queue_autoinit_consumer(1,0);
	}

	sasa_malloc_counter = 0;


#ifdef SASA_MPARM
	start_metric();
#endif

	parse_argv0(*av);
#ifdef SASA_MPARM
	
#endif

#ifndef SASA_COMMENT
	while ((opt = getopt(ac, av, "fcdpvhuaslVFC:")) != EOF) switch (opt) {

	case 'd': f_decode   = 1; break;
	case 'f': f_force    = 1; break;
	case 'c': f_cat      = 1; break;
	case 'p': f_precious = 1; break;
	case 'F': f_fast     = 1; break;
	case 'C': f_ltp_cut  = 100; break;
#ifndef	NDEBUG
	case 'V': f_verbose  = 1; break;
#endif

	case 'u': set_format( &f_ulaw   ); break;
	case 'l': set_format( &f_linear ); break;
	case 'a': set_format( &f_alaw	); break;
	case 's': set_format( &f_audio  ); break;

	case 'v': version(); exit(0);
	case 'h': help();    exit(0);

	default: 
	usage:
#ifdef SASA_MPARM
		
#endif
		fprintf(stderr,
	"Usage: %s [-fcpdhvuaslFC] [files...] (-h for help)\n",
			progname);
		exit(1);
	}

	f_precious |= f_cat;

	av += optind;
	ac -= optind;

	catch_signals(onintr);
#endif /*SASA*/
	
#ifndef SASA_COMMENT
	if (ac <= 0) process( (char *)0 );
	else while (ac--) process( *av++ );
#else
	
	process("ding.au");		

#endif/*SASA_COMMENT*/

#ifdef SASA_MPARM
	stop_metric();
	stop_simulation();
#endif
	exit(0);
}

