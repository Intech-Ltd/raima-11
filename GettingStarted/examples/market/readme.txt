Simplistic Stock Market Simulation
==================================

build:
------
ddlp -d -c mkt.ddl
kmake -f kmake.mak

load:
-----
marketExample -i investors.txt
marketExample -s companies.txt

run automatic transactions:
---------------------------
marketExample -tfs TFST -t 6 (Only one instance should start a TFS)
marketExample -tfs TFSR -t 6 (Other intances should use the remote TFS)

run interactively:
------------------
marketExample

If you get a S_TX_DOCROOT_USED (-226) error it means that a TFS has already
been started and you should use the -tfs TFSR option to use a remote TFS 
