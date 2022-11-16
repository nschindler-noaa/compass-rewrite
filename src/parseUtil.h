#ifndef C_PARSEUTIL_H
#define C_PARSEUTIL_H

#include <QString>
#include <QFile>

#include "cmpfile.h"
//#include "dataConversion.h"
#include "cmpriverpoint.h"
#include "cmpriversystem.h"
#include "cmpdam.h"
#include "cmpreach.h"
#include "cmpheadwater.h"
#include "cmpspecies.h"
#include "cmpstock.h"
#include "cmprelease.h"


bool parseCompassFile (QString filename);
bool parseCompassData (cmpFile *cfile);

bool parseRiverDesc (cmpFile *cfile, cmpRiverSystem *rs);


bool parse_data (cmpFile *infile);
bool parse_river (cmpFile *infile, cmpRiverSystem *rs, cmpRiver *river);//QString riv);
bool parse_dam (cmpFile *infile, cmpDam *dam);//QString dam);
bool parse_fishway (cmpFile *infile, cmpDam *dam);
bool parse_reach (cmpFile *infile, cmpReach *rch);//QString rch);
bool parse_headwater (cmpFile *infile, cmpHeadwater *hdw);//QString hdw);
//bool parse_powerhouse (QFile *infile);
bool parse_species (cmpFile *infile, cmpSpecies *spc);//QString spc);
bool parse_stock (cmpFile *infile, cmpStock *stk);//QString stk);
bool parse_release (cmpFile *infile, cmpRelease *rel);//QString rel);
bool parse_release_site (cmpFile *infile, cmpReleaseSite *rst);
bool parse_latlon (cmpFile *cfile, cmpRiverPoint *pt);

bool find_file (QString filename, QString &path);

bool parse_file (QString filename);
QString pop_token (QFile *infile);
QString peek_token (QFile *infile);

QString read_string (QFile *infile, bool ignore_spaces = false);
float read_float (QFile *infile);
int read_int (QFile *infile);


#define OBSOLETE_TOKEN (obsolete_token) \
    handle_obsolete_token (obsolete_token)
#define OBSOLETE_TO_NEW_TOKEN (obsolete_token, new_token) \
    handle_obsolete_token (obsolete_token, new_token)

#define END_OF_LINE (item) reached_EOL (item)
#define END_OF_FILE (item) reached_EOF (item)

/** String to print for numeric token */
#define NUMERIC_TOKEN "unknown command {%1}, which is a number.\n\tSkipping all numbers until a non-numeric token is reached.\n"
/** String to print for unknown token */
#define UNKNOWN_TOKEN "unknown command {%1}: line discarded\n"

/** Prints an appropriate message using P_ERROR.  It also
 * checks to see if the token is a number, and skips any following numbers
 * until it reaches a non-numeric token. */
#define UNKNOWN_COMMAND (token) handle_unknown_token (token)

QStringList line_to_tokens (QString line);

bool is_float (QString token);
bool is_int (QString token);
bool is_na (QString token);

void p_file_line (int level, QFile *file);

void p_error (QFile *file, char *format, ...);
void p_error (QFile *file, QString message);
void p_warning (QFile *file, char *format, ...);
void p_warning (QFile *file, QString message);

bool check_course (cmpRiverSegment *seg);
bool check_end (QString type, QString name = QString (""));
bool check_end (cmpFile *file, QString type, QString name = "");
bool check_end (QStringList &tokens, QString type, QString name);

bool read_float_or_na(QString token, float *number, QString name, bool *was_na);
bool read_float_or_na(cmpFile *infile, float *number, QString name, bool *was_na);

bool read_string (QString input, QString *output, QString name);

bool parse_int (QString line, int ind, int num, QString type, int *val, QString prompt, QString seg_name);


bool read_double (QString token, double *val, QString prompt);
bool read_float (QString token, float *val, QString prompt);
bool read_int (QString token, int *val, QString name);
bool read_long (QString token, long *val, QString prompt);
//bool read_longlong (QString token, long long *val, QString prompt);
bool read_short (QString token, short *val, QString prompt);
bool read_uint (QString token, unsigned *val, QString prompt);
bool read_ulong (QString token, unsigned long *val, QString prompt);
//bool read_ulonglong (QString token, unsigned long long *val, QString prompt);
bool read_ushort (QString token, unsigned short *val, QString prompt);

bool read_int_array (cmpFile *buffer, int iarray[],
                                          int max_elem, DataConversion conversion,
                                          unsigned mult, char *prompt);

bool read_float_array (cmpFile *buffer, float array[],
                                          int max_elem, DataConversion conversion,
                                          unsigned mult, QString prompt);



//int write_separator (QFile *file);//int write_separator(FILE *outfile);

void skip_to_end (QFile *infile, QString type, QString name = "");
void skip_to_end (cmpFile *pfile, QString type, QString name = "");

//bool check_end (QString line, QString type);
bool check_end (QStringList &tokens, QString type, QString name = "");

void handle_obsolete_token (QString obs_token, QString new_token = "");
void handle_unused_token (QString unused_token);
void handle_unknown_token (QString token);

void free_archive_headers (struct archive_header **archive_data, int nheaders);
void read_flow_and_spill_from_archive (int game, int year, struct archive_header *archive_data,
        struct archive_spill **game_spills, cmpFile *fp, long **tells, int archive_elevation_flag);
void free_archive_header(struct archive_header *header);

bool open_and_parse_alternative (QString filename, struct altern_data *alternative);

struct archive_header *parse_archive_header (int alt,
    struct monte_carlo_data *users_data);
struct archive_map *open_and_map_flow (struct altern_data *alt);
struct archive_header *parse_archive_header_in_alternative (
        struct altern_data *altern);

#endif // C_PARSEUTIL_H
