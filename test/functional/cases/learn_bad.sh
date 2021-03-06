# Test rspamd learn with bad stats

. ${TEST_DIRNAME}/functions.sh

export RSPAMD_CONFIG="$TEST_DIRNAME/configs/stats.conf" \
	STATSDIR=/non/existent

run_rspamd
run_rspamc learn_spam \
	 --key y3ms1knmetxf8gdeixkf74b6tbpxqugmxzqksnjodiqei7tksyty \
	"$TEST_DIRNAME/messages/spam_message.eml"
check_output 'cannot open backend'