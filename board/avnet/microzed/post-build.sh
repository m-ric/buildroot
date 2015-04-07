#!/bin/sh - 

#
# git does not store permissions except 644 and 755
# not convenient for SSL private keys (600)
#

chmod 600 $TARGET_DIR/etc/ssh_*_key

