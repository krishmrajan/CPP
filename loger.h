#ifndef __LOG_H__
#ifndef  ORA_PROC
#define __LOG_H__ 1
#include <stdio.h>
/*
 * Default message LOGDBG ne will not display anything. Must set the LOGLEVEL to LOGDBG to display
 */
#define LOGDBG 0
#define LOGINFO 1
#define LOGWARN 2
#define LOGERR 3

#define LOGTYPE_VISIBLE 0
#define LOGTYPE_HIDDEN 1

/* this enables to open a log files 
 * env: variable where we stock the log file 
 * Permet d'ouvrir un fichier de log
 * env: nom de la variable d'environnement spécifiant le répertoire
 * program: Name of the program
 * formatdate:  and date format how we would like to write the log 
 *
 * example  :
 * openLogFile("logdemo", "environment_varialbe", "%Y%m%d_%H%M");
 *  will open file : $environment_variableG/logdemo.20140812_1015.log
 */
extern void initLogFile(const char *program, const char *env, const char *formatdate);
extern void closeLogFile(void);

#define LOG(type, format, ...) log_msg(__FILE__, __LINE__, __FUNCTION__, type, format, ##__VA_ARGS__)

/* prefererntial usage  LOG() */
extern void log_msg(const char *source, int line, const char *function, int type, const char *format, ...);
extern const char *getLastErrorMsg();
extern size_t getDateHeure(const char *format, char *str, size_t maxlen);

extern int isLogTypeVisible(int type);

#endif
#endif
