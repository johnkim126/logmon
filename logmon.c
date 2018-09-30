#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
	uint32_t	line_count;
	char		**logs;
} log_t;

typedef enum {
	ERR_TYPE_OK = 0,
	ERR_TYPE_INVALID = 1,
	ERR_TYPE_NOEXIST = 2
} err_t;

static uint32_t
get_log_line_count(const char *log_path)
{
	FILE		*fp;
	char		buf[2048];
	uint32_t	line_count;

	line_count = 0;
	
	fp = fopen(log_path, "r");
	while (fgets(buf, 2048, fp)) {
		line_count++;
	}
	fclose(fp);
	return line_count;
}

static err_t
get_log(const char *log_type, const char *log_path)
{
	FILE	*fp;
	char	buf[2048];
	bool	is_printed;

	is_printed = false;
	fp = fopen(log_path, "r");
	if (fp == NULL) {
		printf("[%s] Failed to open log file\n", log_type);
		return ERR_TYPE_NOEXIST;
	}

	while (fgets(buf, 2048, fp)) {
		printf("[%s] %s", log_type, buf);
		is_printed = true;
	}

	fclose(fp);
	
	if (!is_printed) {
		printf("[%s] Not-existent Log\n", log_type);
	}
	return ERR_TYPE_OK;
}

int
main(int argc, char **argv)
{
	err_t	err;
	
	err = ERR_TYPE_OK;

	if (argc > 2 || argc < 2) {
		fprintf(stderr, "Usage: %s <log type>\n", argv[0]);
		fprintf(stderr, "Log type: syslog, kernel\n");
		exit(ERR_TYPE_INVALID);
	}

	if (strcasecmp(argv[1], "syslog") == 0) {
		err = get_log("syslog", "/var/log/syslog");
	}

	if (strcasecmp(argv[1], "kernel") == 0) {
		err = get_log("kernel", "/var/log/kern.log");
	}
	
	exit(err);
}
