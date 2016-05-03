#include <cstdlib>
#include <cstring>
#include <QString>


#include "help.h"
#include "version.h"

/** Create version information */
char * version_string ()
{
    char * ver = (char *) calloc (80, sizeof (char));
    QString version ("COMPASS Passage Model v");
    version.append  (QString (COMPASS_VERSION_MAJOR_STRING));
    version.append  (".");
    version.append  (QString (COMPASS_VERSION_MINOR_STRING));
    version.append  (".");
    version.append  (QString (COMPASS_BUGFIX_LEVEL_STRING));
    version.append  (QString (COMPASS_RELEASE_LEVEL_STRING));
    ver = (char *) realloc (ver, (version.size () + 1));
    strcpy (ver, version.toLocal8Bit().data ());
    return ver;
}

/** Create help information */
char * help_string (bool verbose_flag)
{
    char * help = (char *) calloc (320, sizeof (char));
    QString usage ("Usage: compassb [-l[w][m][r][d]] [-[b][s][m][i][u][g][t]] [-r river_desc]\n");
    usage.append  ("    [-k calib_file] [-z release_file] [-e river_env_file] [-d dam_ops_file]\n");
    usage.append  ("    [-p post_bonneville_file] [-x extra_config_file] [-c output_control_file]\n");
    usage.append  ("    [-f scenario_file] [-o output_file] [-h] [-v]\n");

    if (verbose_flag)
    {
        usage.append ("\nSyntax:\n");
        usage.append ("  -l[w][r][m][d]     - set the message loggging options.\n");
        usage.append ("                       Error messages are always shown. \n");
        usage.append ("                       w - show all 'warning' type messages\n");
        usage.append ("                       r - show all 'raw data' type messages\n");
        usage.append ("                       m - show all 'message' type messages\n");
        usage.append ("                       d - show all 'debug' type messages\n\n");

        usage.append ("  -[b][s][m][i][u][g][t] - set the mode flags.\n");
        usage.append ("                       b - batch mode, default is false\n");
        usage.append ("                       s - single scenario mode, default is false\n");
        usage.append ("                       m - monte carlo mode, default is true\n");
        usage.append ("                       i - real time mode, \n");
        usage.append ("                       u - summary mode, \n");
        usage.append ("                       g - full gui mode, default is false \n");
        usage.append ("                       t - calibration mode (not implemented)\n\n");

        usage.append ("  -r river_desc      - river_desc is the file that holds all the base\n");
        usage.append ("                       river data. Default file is {columbia.desc}.\n\n");

        usage.append ("  The following files contain data for the specific scenario and may be\n");
        usage.append ("  in a single scenario file, or in separate files: calibration file,\n");
        usage.append ("  release file, river year file, dam operations file, \n");
        usage.append ("  post bonneville file,and extra configuration data. The separate files\n");
        usage.append ("  can also be specified in a .ctrl file using the 'include' command. \n");
        usage.append ("  If no file is specified, the application looks for {base.dat}.\n\n");

        usage.append ("  -k calib_file      - holds the COMPASS calibration data (.clb).\n");
        usage.append ("  -z release_file    - holds data for all the releases (.rls).\n");
        usage.append ("  -e river_env_file  - holds the water year information (.riv).\n");
        usage.append ("  -d dam_ops_file    - holds all dam operations data (.ops).\n");
        usage.append ("  -p post_bonneville_file - holds data for what happens between leaving\n");
        usage.append ("                       the river and returning to it (.pbn).\n");
        usage.append ("  -x extra_config_file - any additional data (.ect or .cnf).\n");
        usage.append ("  -c output_control_file - specifies output options.\n");
        usage.append ("  -f scenario_file   - holds all the above data (.scn, .dat, or .ctrl).\n");
        usage.append ("  -o output_file     - specifies where to create a new file. This may be\n");
        usage.append ("                       of any type, depending on the extension.\n\n");

        usage.append ("  -h                 - shows basic or verbose help screens\n");
        usage.append ("  -v                 - flag to show verbose help.\n\n");
    }
    help = (char *) realloc (help, (usage.size () + 1));
    strcpy (help, usage.toLocal8Bit().data ());
    return help;
}
