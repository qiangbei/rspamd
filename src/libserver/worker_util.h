/*-
 * Copyright 2016 Vsevolod Stakhov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef WORKER_UTIL_H_
#define WORKER_UTIL_H_

#include "config.h"
#include "util.h"
#include "http.h"
#include "rspamd.h"

#ifndef HAVE_SA_SIGINFO
typedef void (*rspamd_sig_handler_t) (gint);
#else
typedef void (*rspamd_sig_handler_t) (gint, siginfo_t *, void *);
#endif

struct rspamd_worker;
struct rspamd_worker_signal_handler;

/**
 * Prepare worker's startup
 * @param worker worker structure
 * @param name name of the worker
 * @param sig_handler handler of main signals
 * @param accept_handler handler of accept event for listen sockets
 * @return event base suitable for a worker
 */
struct event_base *
rspamd_prepare_worker (struct rspamd_worker *worker, const char *name,
	void (*accept_handler)(int, short, void *));

/**
 * Set special signal handler for a worker
 */
void rspamd_worker_set_signal_handler (int signo,
		struct rspamd_worker *worker,
		struct event_base *base,
		void (*handler) (struct rspamd_worker_signal_handler *, void *),
		void *handler_data);

/**
 * Stop accepting new connections for a worker
 * @param worker
 */
void rspamd_worker_stop_accept (struct rspamd_worker *worker);

typedef gint (*rspamd_controller_func_t) (
	struct rspamd_http_connection_entry *conn_ent,
	struct rspamd_http_message *msg,
	struct module_ctx *ctx);

struct rspamd_custom_controller_command {
	const gchar *command;
	struct module_ctx *ctx;
	gboolean privilleged;
	gboolean require_message;
	rspamd_controller_func_t handler;
};

struct rspamd_controller_worker_ctx;

struct rspamd_controller_session {
	struct rspamd_controller_worker_ctx *ctx;
	struct rspamd_worker *wrk;
	rspamd_mempool_t *pool;
	struct rspamd_task *task;
	gchar *classifier;
	rspamd_inet_addr_t *from_addr;
	struct rspamd_config *cfg;
	gboolean is_spam;
};

/**
 * Send error using HTTP and JSON output
 * @param entry router entry
 * @param code error code
 * @param error_msg error message
 */
void rspamd_controller_send_error (struct rspamd_http_connection_entry *entry,
	gint code, const gchar *error_msg, ...);

/**
 * Send a custom string using HTTP
 * @param entry router entry
 * @param str string to send
 */
void rspamd_controller_send_string (struct rspamd_http_connection_entry *entry,
	const gchar *str);

/**
 * Send UCL using HTTP and JSON serialization
 * @param entry router entry
 * @param obj object to send
 */
void rspamd_controller_send_ucl (struct rspamd_http_connection_entry *entry,
	ucl_object_t *obj);

/**
 * Return worker's control structure by its type
 * @param type
 * @return worker's control structure or NULL
 */
worker_t * rspamd_get_worker_by_type (struct rspamd_config *cfg, GQuark type);

void rspamd_worker_stop_accept (struct rspamd_worker *worker);

/**
 * Block signals before terminations
 */
void rspamd_worker_block_signals (void);

/**
 * Fork new worker with the specified configuration
 */
struct rspamd_worker *rspamd_fork_worker (struct rspamd_main *,
		struct rspamd_worker_conf *, guint idx, struct event_base *ev_base);

#define msg_err_main(...) rspamd_default_log_function (G_LOG_LEVEL_CRITICAL, \
        rspamd_main->server_pool->tag.tagname, rspamd_main->server_pool->tag.uid, \
        G_STRFUNC, \
        __VA_ARGS__)
#define msg_warn_main(...)   rspamd_default_log_function (G_LOG_LEVEL_WARNING, \
        rspamd_main->server_pool->tag.tagname, rspamd_main->server_pool->tag.uid, \
        G_STRFUNC, \
        __VA_ARGS__)
#define msg_info_main(...)   rspamd_default_log_function (G_LOG_LEVEL_INFO, \
        rspamd_main->server_pool->tag.tagname, rspamd_main->server_pool->tag.uid, \
        G_STRFUNC, \
        __VA_ARGS__)
#define msg_debug_main(...)  rspamd_default_log_function (G_LOG_LEVEL_DEBUG, \
        rspamd_main->server_pool->tag.tagname, rspamd_main->server_pool->tag.uid, \
        G_STRFUNC, \
        __VA_ARGS__)

#endif /* WORKER_UTIL_H_ */
