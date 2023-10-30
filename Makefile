all: tbshell tbsh_client
	@cp -f apps/server/tbshell .
	@cp -f apps/client/tbsh_client .


tbshell:
	@make -C apps/server

tbsh_client:
	@make -C apps/client/

clean:
	@make -C apps/server clean
	@make -C apps/client clean
	@rm -f tbshell tbsh_client

