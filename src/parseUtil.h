#ifndef C_PARSEUTIL_H
#define C_PARSEUTIL_H

#include <QString>
#include <QFile>

#include "Log.h"
#include "CompassFile.h"
#include "dataConversion.h"
#include "RiverPoint.h"
#include "RiverSystem.h"
#include "Dam.h"
#include "Reach.h"
#include "Headwater.h"
#include "Species.h"
#include "Stock.h"
#include "Release.h"

bool parseCompassFile (QString filename);
bool parseCompassData (CompassFile *cfile);

bool parseRiverDesc (CompassFile *cfile, RiverSystem *rs);


bool parse_data (CompassFile *infile);
bool parse_river (CompassFile *infile, RiverSystem *rs, River *river);//QString riv);
bool parse_dam (CompassFile *infile, Dam *dam);//QString dam);
bool parse_reach (CompassFile *infile, Reach *rch);//QString rch);
bool parse_headwater (CompassFile *infile, Headwater *hdw);//QString hdw);
//bool parse_powerhouse (QFile *infile);
bool parse_species (CompassFile *infile, Species *spc);//QString spc);
bool parse_stock (CompassFile *infile, Stock *stk);//QString stk);
bool parse_release (CompassFile *infile, Release *rel);//QString rel);
bool parse_release_site (CompassFile *infile, ReleaseSite *rst);
bool parse_latlon (CompassFile *cfile, RiverPoint *pt);

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

bool check_course (RiverSegment *seg);
bool check_end (QString type, QString name = QString (""));
bool check_end (CompassFile *file, QString type, QString name = "");
bool check_end (QStringList &tokens, QString type, QString name);

bool read_float_or_na(QString token, float *number, QString name, bool *was_na);
bool read_float_or_na(CompassFile *infile, float *number, QString name, bool *was_na);

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

bool read_int_array (CompassFile *buffer, int iarray[],
                                          int max_elem, DataConversion conversion,
                                          unsigned mult, char *prompt);

bool read_float_array (CompassFile *buffer, float array[],
                                          int max_elem, DataConversion conversion,
                                          unsigned mult, QString prompt);



int write_separator (QFile *file);//int write_separator(FILE *outfile);

void skip_to_end (QFile *infile, QString type, QString name = "");
void skip_to_end (CompassFile *pfile, QString type, QString name = "");

//bool check_end (QString line, QString type);
bool check_end (QStringList &tokens, QString type, QString name = "");

void handle_obsolete_token (QString obs_token, QString new_token = "");
void handle_unused_token (QString unused_token);
void handle_unknown_token (QString token);

void free_archive_headers (struct archive_header **archive_data, int nheaders);
void read_flow_and_spill_from_archive (int game, int year, struct archive_header *archive_data,
        struct archive_spill **game_spills, CompassFile *fp, long **tells, int archive_elevation_flag);
void free_archive_header(struct archive_header *header);

bool open_and_parse_alternative (QString filename, struct altern_data *alternative);

struct archive_header *parse_archive_header (int alt,
    struct monte_carlo_data *users_data);
struct archive_map *open_and_map_flow (struct altern_data *alt);
struct archive_header *parse_archive_header_in_alternative (
        struct altern_data *altern);

#endif // C_PARSEUTIL_H
