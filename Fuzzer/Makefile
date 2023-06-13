all:
	# AFL_USE_ASAN=0 afl-clang -m32 -g echo_server.c httplib.c strlib.c validation.c filelib.c base64.c -o echo_server
	AFL_USE_ASAN=1 afl-clang -g ../echo_server.c ../httplib.c ../strlib.c ../validation.c ../filelib.c ../base64.c -lssl -lcrypto -o echo_server
	# Vorsicht! Das muss als root ausgeführt werden:
	# echo core >/proc/sys/kernel/core_pattern
