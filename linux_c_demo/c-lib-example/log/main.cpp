#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Open syslog with a specified identifier and options
    openlog("my_program", LOG_PID | LOG_CONS, LOG_USER);

    // Log a message with various log levels
    syslog(LOG_EMERG, "This is an emergency message");
    syslog(LOG_ALERT, "This is an alert message");
    syslog(LOG_CRIT, "This is a critical message");
    syslog(LOG_ERR, "This is an error message");
    syslog(LOG_WARNING, "This is a warning message");
    syslog(LOG_NOTICE, "This is a notice message");
    syslog(LOG_INFO, "This is an informational message");
    syslog(LOG_DEBUG, "This is a debug message");

    // Close syslog
    closelog();

    return 0;
}
