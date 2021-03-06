#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_SIZE 2048

typedef enum {
	ERR_TYPE_OK = 0,
	ERR_TYPE_INVALID = 1,
	ERR_TYPE_NOEXIST = 2
} err_t;

typedef enum {
	LOG_TYPE_SYSTEM = 0,
	LOG_TYPE_KERNEL = 1
} log_t;

typedef struct {
	char		*month;
	char		*day;
	char		*time;
	char		*log;
} log_info_t;

typedef struct {
	log_t		type;
	uint32_t	line_count;
	log_info_t	*infos;
} log_data_t;

log_data_t log_datas[2];

static char log_type[2][12] = {"syslog", "kernel"};
static char log_files[2][32] = {"/var/log/syslog", "/var/log/kern.log"};

static uint32_t
get_log_line_count(const char *log_path)
{
	FILE		*fp;
	char		buf[MAX_SIZE];
	uint32_t	line_count;

	line_count = 0;
	
	fp = fopen(log_path, "r");
	while (fgets(buf, MAX_SIZE, fp)) {
		line_count++;
	}
	fclose(fp);
	return line_count;
}

static char *
get_log_path(log_t log_type)
{
	switch (log_type) {
	case LOG_TYPE_SYSTEM:
		return log_files[0];
	case LOG_TYPE_KERNEL:
		return log_files[1];
	}
}

static err_t
get_log(log_t type)
{
	FILE		*fp;
	char		buf[MAX_SIZE], *log_path;
	char		*str_origin, *ptr_str_origin;
	char		*month, *day, *time, *hostname;
	bool		is_printed;
	uint32_t	log_count, idx;

	is_printed = false;
	log_path = get_log_path(type);
	log_count = get_log_line_count(log_path);
	fp = fopen(log_path, "r");
	if (fp == NULL) {
		printf("[%s] Failed to open log file\n", log_type[type]);
		return ERR_TYPE_NOEXIST;
	}

	log_datas[type].infos = malloc(log_count * sizeof(log_info_t));
	log_datas[type].line_count = log_count;
	log_datas[type].type = type;

	for (idx = 0; idx < log_count; idx++) {
		log_datas[type].infos[idx].log = malloc(MAX_SIZE * sizeof(char));
	}

	idx = 0;
	while (fgets(buf, MAX_SIZE, fp)) {
		str_origin = strdup(buf);
		ptr_str_origin = str_origin;

		month = strtok(buf, " \n");
		day = strtok(NULL, " \n");
		time = strtok(NULL, " \n");
		hostname = strtok(NULL, " \n");

		ptr_str_origin += strlen(month) + strlen(day) + strlen(time) + strlen(hostname) + 4;
		log_datas[type].infos[idx].month = strdup(month);
		log_datas[type].infos[idx].day = strdup(day);
		log_datas[type].infos[idx].time = strdup(time);
		snprintf(log_datas[type].infos[idx].log, MAX_SIZE, "[%s] %s", log_type[type], ptr_str_origin);

		free(str_origin);
		is_printed = true;
		idx++;
	}

	fclose(fp);
	
	if (!is_printed) {
		printf("[%s] Not-existent Log\n", log_type[type]);
	}
	return ERR_TYPE_OK;
}

static void
print_log(log_t type)
{
	uint32_t log_count, idx;

	log_count = log_datas[type].line_count;

	for (idx = 0; idx < log_count; idx++) {
		printf("[Date: %s %s] %s", 
			log_datas[type].infos[idx].month, 
			log_datas[type].infos[idx].day, 
			log_datas[type].infos[idx].log);
	}
}

int
main(int argc, char **argv)
{
	uint32_t	idx;
	err_t		err;
	
	err = ERR_TYPE_OK;

	if (argc > 2 || argc < 2) {
		fprintf(stderr, "Usage: %s <log type>\n", argv[0]);
		fprintf(stderr, "Log type: syslog, kernel\n");
		exit(ERR_TYPE_INVALID);
	}

	for (idx = 0; idx <= 1; idx++) {
		if (strcasecmp(argv[1], log_type[idx]) == 0) {
			err = get_log(idx);
			print_log(idx);	
		}
	}
	exit(err);
}
