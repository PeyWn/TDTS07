/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: /tmp_amd/presto/export/kbs/jutta/src/gsm/RCS/toast_audio.c,v 1.6 1995/03/07 21:21:24 jutta Exp $ */

#include	"toast.h"

/*  toast_audio -- functions to manipulate SunOS audio files.
 *
 *  This is reverse engineered from our present soundfiles
 *  and in no way portable, durable or aesthetically pleasing.
 */

extern FILE	* in, 	  * out;
extern char	* inname;
extern char	* progname;

extern int	(*output) P((gsm_signal *)),
		(*input ) P((gsm_signal *));

extern int	alaw_input   P((gsm_signal *)),
		ulaw_input   P((gsm_signal *)),
		linear_input P((gsm_signal *));

extern int	ulaw_output P((gsm_signal *));

#ifdef SASA
extern char memory[];
#endif

#ifndef SASA

static int put_u32 P2((f, u), FILE * f, unsigned long u)
{
	/*  Write a 32-bit unsigned value msb first. 
	 */
	if (  putc( (char)((u>>24) & 0x0FF), f) == EOF
	   || putc( (char)((u>>16) & 0x0FF), f) == EOF
	   || putc( (char)((u>> 8) & 0x0FF), f) == EOF
	   || putc( (char)( u      & 0x0FF), f) == EOF) return -1;

	return 0;
}


static int get_u32 P2((f, up), FILE * f, unsigned long * up)
{
	/*  Read a 32-bit unsigned value msb first.
	 */
	int		i;
	unsigned long 	u;

	if (  				      (i = getc(f)) == EOF
	   || ((u =        (unsigned char)i), (i = getc(f)) == EOF)
	   || ((u = (u<<8)|(unsigned char)i), (i = getc(f)) == EOF)
	   || ((u = (u<<8)|(unsigned char)i), (i = getc(f)) == EOF)) return -1;
	*up = 	    (u<<8)|(unsigned char)i;
	return 0;
}


int audio_init_input P0()
{
	unsigned long	len, enc;	/* unsigned 32 bits	*/

	if (  fgetc(in) != '.' 
	   || fgetc(in) != 's'
	   || fgetc(in) != 'n'
	   || fgetc(in) != 'd'
	   || get_u32( in, &len )
	   || get_u32( in, &enc )	/* skip this */
	   || get_u32( in, &enc )) {
		fprintf(stderr, 
	"%s: bad (missing?) header in Sun audio file \"%s\";\n\
	Try one of -u, -a, -l instead (%s -h for help).\n",
			progname, inname ? inname : "stdin", progname);
		return -1;
	}


	switch (enc) {
	case 1:	input = ulaw_input;  	break;
	case 2: input = alaw_input;  	break;
	case 3: input = linear_input; 	break;
	default:
		fprintf(stderr,
"%s: warning: file format #%lu for %s not implemented, defaulting to u-law.\n",
			progname, enc, inname);
		input = ulaw_input;
		break;
	}
	
/*	printf("SASA DEBUG: %d\n", len);*/
	while (len > 4*4)
		if (getc(in) == EOF) {
			fprintf(stderr, 
			"%s: EOF in header of Sun audio file \"%s\";\n\
			Try one of -u, -a, -l instead (%s -h for help).\n",
				progname, inname ? inname : "stdin", progname);
			return -1;
		}
		else len--;

	return 0;
}

int audio_init_output P0()
{
        if (  fputs(".snd", out) == EOF
           || put_u32(out, 32)
           || put_u32(out, ~(unsigned long)0)
           || put_u32(out, 1)
           || put_u32(out, 8000)
           || put_u32(out, 1)
           || put_u32(out, 0)
           || put_u32(out, 0)) return -1;

        return 0;
}

#else

static int 
put_u32 P2((f, u), FILE * f, unsigned long u) {
        /*  Write a 32-bit unsigned value msb first.
         */
	printf("put_u32\n");
	f = stdout;
        if (  putc( (char)((u>>24) & 0x0FF), f) == EOF
           || putc( (char)((u>>16) & 0x0FF), f) == EOF
           || putc( (char)((u>> 8) & 0x0FF), f) == EOF
           || putc( (char)( u      & 0x0FF), f) == EOF) return -1;

        return 0;
}


static int 
get_u32 P2((up, n),  unsigned long * up, int n) {
	/*  Read a 32-bit unsigned value msb first.
	 */
	int		i;
	unsigned long 	u;

	if (  				      (i = memory[n]) == 1000
	   || ((u =        (unsigned char)i), (i = memory[n+1]) == 1000)
	   || ((u = (u<<8)|(unsigned char)i), (i = memory[n+2]) == 1000)
	   || ((u = (u<<8)|(unsigned char)i), (i = memory[n+3]) == 1000)) return -1;
	*up = 	    (u<<8)|(unsigned char)i;
	return 0;
}


int 
audio_init_input P0() {
	unsigned long	len, enc;	/* unsigned 32 bits	*/
	int jj;
	/*printf("sasa start of audio_init\n"); fflush(stdout);*/
	if (  memory[0] != '.' 
	   || memory[1] != 's'
	   || memory[2] != 'n'
	   || memory[3] != 'd'
	   || get_u32(  &len, 4)
	   || get_u32(  &enc, 12 )) {
		fprintf(stderr, "111bad (missing?) header in Sun audio file\n");
		exit(1);
	}
	if (  memory[0] != '.' 
	   || memory[1] != 's'
	   || memory[2] != 'n'
	   || memory[3] != 'd'
	   || get_u32(  &len, 4)
	   || get_u32(  &enc, 8 )	/* skip this */
	   || get_u32(  &enc, 12 )) {
		fprintf(stderr, "222bad (missing?) header in Sun audio file\n");
		return -1;
	}

	
	switch (enc) {
	case 1:	input = ulaw_input;  	break;
	case 2: input = alaw_input;  	break;
	case 3: input = linear_input; 	break;
	default:
		fprintf(stderr, "%s: warning: file format #%lu for %s not implemented, defaulting to u-law.\n",
				 progname, enc, inname);
		input = ulaw_input;
		break;
	}
	
	/*printf("SASA DEBUG: %d\n", len);*/
	
	jj = 16;
	while (len > 4*4) {
		if (memory[jj] == EOF) {
			fprintf(stderr, 
			"%s: EOF in header of Sun audio file \"%s\";\n\
			Try one of -u, -a, -l instead (%s -h for help).\n",
				progname, inname ? inname : "stdin", progname);
			return -1;
		}
		else len--;
		jj++;
	}		
/*	printf("saa end of audio_init\n"); fflush(stdout);*/
	return 0;
}

int audio_init_output P0()
{
	if (  fputs(".snd", out) == EOF
	   || put_u32(out, 32)
	   || put_u32(out, ~(unsigned long)0)
	   || put_u32(out, 1)
	   || put_u32(out, 8000)
	   || put_u32(out, 1)
	   || put_u32(out, 0)
	   || put_u32(out, 0)) return -1;

	return 0;
}

#endif
