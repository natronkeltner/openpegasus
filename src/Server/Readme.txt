This directory contains an OpenSSL Certificate and key used during local.

These are self-signed and are generated manually with the a definition in
the Makefile

make createSSLFilesforCVS-SystemSpecific

This generates a new ssl.cnf, file.pem and cert.pem

The only time this should be modified is if the certificate expires which
is defined as 3650 days from the time it was created.

Today there is no automated means to create these two files.

Note that these files are used only for local nightly testing, etc.

They should not be used in a production OpenPegasus server.


