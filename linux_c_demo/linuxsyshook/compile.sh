gcc -fPIC -shared -o libfilemonitor.so file_monitor.c -ldl
export LD_PRELOAD=./libfilemonitor.so
