#!/usr/bin/env perl

use warnings;
use strict;

use Socket;

my $host = "127.0.0.1";
my $port = 56789;
my $input = shift;

open(INPUT, "< $input") or die "Can't open input file $input\n";

socket(SOCKET,PF_INET,SOCK_STREAM,(getprotobyname('tcp'))[2])
   or die "Can't create a socket $!\n";
connect(SOCKET, pack_sockaddr_in($port, inet_aton($host)))
   or die "Can't connect to port $port! \n";

print SOCKET "POST /symbols HTTP/1.0\r\n\r\n";

while (my $line = <INPUT>) {
	print SOCKET $line;
}

SOCKET->autoflush(1);

shutdown(SOCKET, 1);

close(INPUT);

while (my $line = <SOCKET>) {
	print $line;
}

close(SOCKET);
