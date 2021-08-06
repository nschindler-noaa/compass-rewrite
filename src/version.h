/** \file
    \brief Version information.
    There are defines for version numbers and version number strings.
    The date string is the date of the build.
    The copyright string is just the year.
   Make changes in this file before building a new %release (see \ref releasing).

 *  \section version The version of COMPASS
 *        COMPASS has
          - a major version number for major structural and functional changes
          - a minor version number for minor structural and functional changes
          - a bugfix level that include no functional changes, and
          - a %release level that is blank (space), alpha, or beta
               (an approved %release will be blank).

 */
#ifndef	C_VERSION_H
#define	C_VERSION_H

/* COMPASS model version */
#define COMPASS_VERSION_MAJOR            3  /**< Major version number */
#define COMPASS_VERSION_MAJOR_STRING    "3" /**< Major version number string */
#define COMPASS_VERSION_MINOR            0  /**< Minor version number */
#define COMPASS_VERSION_MINOR_STRING    "0" /**< Minor version number string */
#define COMPASS_BUGFIX_LEVEL             1  /**< Bugfix level number */
#define COMPASS_BUGFIX_LEVEL_STRING     "1" /**< Bugfix level number string */
#define COMPASS_RELEASE_LEVEL_CHAR      'x' /**< Release level  */
#define COMPASS_RELEASE_LEVEL_STRING    "x" /**< Release level string */

#define COMPASS_VERSION_RELEASE_DATE_STRING  __DATE__ /**< Date of build */

#define COMPASS_COPYRIGHT_YEAR_STRING   "2019" /**< Year of build */

#endif // C_VERSION_H

