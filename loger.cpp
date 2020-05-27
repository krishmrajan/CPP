#define _OPEN_THREADS
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "loger.h"

#define LOG_EXITREGISTERED 0
#define LOG_EXITNOTREGISTERED 1

#define LOGSIZEDATE 100

static FILE *logfile = NULL;
static pid_t g_pid = 0;
static pthread_mutex_t g_mutexLogWriter = {0};
static char *lastmsgerrstr = NULL;
static size_t lastmsgerrsize = 0;

typedef struct sLog {
        int visible;
        int level;
        char *levelname;
} typelog;

typelog logtype[] = { {LOGTYPE_HIDDEN, 0, "Debug"},
                                  {LOGTYPE_VISIBLE, 1, "Info"},
                                  {LOGTYPE_VISIBLE, 2, "Warning"},
                                  {LOGTYPE_VISIBLE, 3, "Error" } };
                                  
int isLogTypeVisible(int type) {
        if(type < 0 || type >= (sizeof(logtype)/sizeof(typelog)))
                return LOGTYPE_HIDDEN;

        return logtype[type].visible;
}

void initLogFile(const char *program, const char *env, const char *formatdate)
{
        static int exitIsRegister = LOG_EXITNOTREGISTERED;
        const char *debug_actif = NULL;
        const char *dirlog = NULL;
        char logfilename[FILENAME_MAX] = "";
        char strtime[LOGSIZEDATE] = "";
        FILE *newlogfile = NULL;
        pid_t l_pid = getpid();

        if(g_pid != l_pid) {
                pthread_mutex_init(&g_mutexLogWriter, NULL);
                logfile = stdout;
                g_pid = l_pid;
        }

        if(lastmsgerrstr == NULL) {
                lastmsgerrstr = (char*)malloc(512);
                if(lastmsgerrstr != NULL) {
                        lastmsgerrsize = 512;
                        memset(lastmsgerrstr, 0, lastmsgerrsize);
                }
        }

        debug_actif = getenv("DEBUG");
        if(debug_actif != NULL && (strcmp(debug_actif, "1") == 0 || strcasecmp(debug_actif, "O"))) {
                logtype[LOGDBG].visible = LOGTYPE_VISIBLE;
                //LOG(LOGDBG, "Les messages de debogage sont actifs");
        }

        if(program != NULL) {
                if(env != NULL) {
                        dirlog = getenv(env);
                        if(dirlog == NULL) {
                                LOG(LOGWARN, "Variable d'environnement %s non positionnée", env);
                                strcat(logfilename, "./");
                        }
                        else {
                                strcat(logfilename, dirlog);
                                strcat(logfilename, "/");
                        }
                }

                strcat(logfilename, program);

                if(formatdate != NULL) {
                        getDateHeure(formatdate, strtime, sizeof(strtime));
                        strcat(logfilename, ".");
                        strcat(logfilename, strtime);
                }

                strcat(logfilename, ".log");
                LOG(LOGINFO, "Fichier de log=%s", logfilename);
                /* Ouverture du fichier de log en ajout */
                newlogfile = fopen(logfilename, "a");
                if(newlogfile == NULL) {
                        LOG(LOGWARN, "Impossible d'ouvrir le fichier de log %s, raison: %s", logfilename, strerror(errno));
                } else {
                        pthread_mutex_lock(&g_mutexLogWriter);
                        if(logfile != stdout) {
                                fclose(logfile);
                        }
                        logfile = newlogfile;
                        pthread_mutex_unlock(&g_mutexLogWriter);
                        if(exitIsRegister != LOG_EXITREGISTERED) {
                                exitIsRegister = LOG_EXITREGISTERED;
                                atexit(closeLogFile);
                        }
                }
        }

        /* we set setvbuf so that end of each line the buffer is released  */
        setvbuf(logfile, NULL, _IOLBF, BUFSIZ);
}

void closeLogFile(void)
{
        pthread_mutex_lock(&g_mutexLogWriter);
        if(lastmsgerrstr != NULL) {
                free(lastmsgerrstr);
                lastmsgerrstr = NULL;
        }
        lastmsgerrsize = 0;

        if(logfile != NULL && logfile != stdout) {
                fclose(logfile);
        }
        logfile = stdout;
        pthread_mutex_unlock(&g_mutexLogWriter);
}
void log_msg(const char *source, int line, const char *function, int type, const char *format, ...)
{
        va_list argslist = {0};
        char strtime[LOGSIZEDATE] = "";
        char *formattime = "%Y-%m-%d %H:%M:%S";

        pid_t l_pid = getpid();

        if(g_pid != l_pid) {
                initLogFile(NULL, NULL, NULL);
        }

        if(logtype[type].visible != LOGTYPE_VISIBLE) {
                return;
        }

        pthread_mutex_lock(&g_mutexLogWriter);
        fprintf(logfile, "%d|", l_pid);

        getDateHeure(formattime, strtime, sizeof(strtime));
        fprintf(logfile, "%s|", strtime);
       if(type >= 0 && type < 4)
                fprintf(logfile, "%s|", logtype[type].levelname);
        else
                fprintf(logfile, "%s|", logtype[0].levelname);

        if(function != NULL || source != NULL) {
                if(source != NULL) {
                        fprintf(logfile, "%s:%d%s", source, line, (function == NULL?"":":"));
                }
                if(function != NULL) {
                        fprintf(logfile, "%s", function);
                }
                fputc('|', logfile);
        }

        if(format != NULL) {
                int nbbyte = 0;
                va_start(argslist, format);
                nbbyte = vfprintf(logfile, format, argslist);
                if(type == LOGERR && nbbyte > 0) {
                        nbbyte++;
                        if(nbbyte > lastmsgerrsize) {
                                lastmsgerrsize = nbbyte;
                                lastmsgerrstr = (char*)realloc(lastmsgerrstr, lastmsgerrsize);
                                if(lastmsgerrstr == NULL) {
                                        lastmsgerrsize = 0;
                                } else {
                                        memset(lastmsgerrstr, 0, lastmsgerrsize);
                                }
                        }

                        if(lastmsgerrstr != NULL) {
                                vsnprintf(lastmsgerrstr, lastmsgerrsize, format, argslist);
                        }
                }
                va_end(argslist);
        }

        fprintf(logfile, "\n");
        pthread_mutex_unlock(&g_mutexLogWriter);
}
size_t getDateHeure(const char *format, char *str, size_t maxlen)
{
        time_t dateheure = {0};
        struct tm l_tm = {0};
        struct tm *l_ptm = NULL;

        time(&dateheure);
        l_ptm = localtime_r(&dateheure, &l_tm);

        if(l_ptm != NULL) {
                return strftime(str, maxlen, format, l_ptm);
        }

        return 0;
}

const char *getLastErrorMsg()
{
        return lastmsgerrstr;
}











