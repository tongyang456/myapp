OBJS=*.o
ODIR=obj
$(ROOT_DIR)/$(BIN_DIR)/$(BIN):$(ODIR)/$(OBJS)
	$(CC) -o $@ $^ -lmysqlclient -I/usr/include/mysql -L/usr/lib64/mysql -lpthread -lm -g
	sleep 1
	./myhttp stop
	./myhttp start
	cd /home/app/sqlPlus/6/debug
	rm my.log;touch my.log
	tail -f my.log
