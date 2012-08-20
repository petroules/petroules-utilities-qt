#ifndef PETROULESUTILITIES_GLOBAL_H
#define PETROULESUTILITIES_GLOBAL_H

#include <QtGui>

// https://binglongx.wordpress.com/2009/01/29/felxible-dllstatic-library-linkage-configurations/
#if defined(PETROULESUTILITIES_EXPORTS)
#  define PETROULES_UTILITIES_API Q_DECL_EXPORT
#elif defined(PETROULESUTILITIES_IMPORTS)
#  define PETROULES_UTILITIES_API Q_DECL_IMPORT
#else
#  define PETROULES_UTILITIES_API
#endif

#endif // PETROULESUTILITIES_GLOBAL_H
