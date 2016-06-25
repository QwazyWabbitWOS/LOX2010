#!/usr/bin/perl -w

#from http://idle.iceware.net/printthread.php?t=228

#Quake 2 server monitor/ auto-restart script
 
# This was custom-made for xquake.net's Quake 2 server 
# ( quake2://arena.xquake.net:27910 ) because it would 
# often lock up and not terminate the process, yet no 
# clients could connect. The script restarts the 
# Quake 2 server when it fails to respond or has crashed.

# I'll post it here since it's short. 
# It's available via cvs, but I don't have that info right now.

# Code:
#
# Quake2 Server Monitor auto-resart script - V1.1
# Author: Bill Kelly <billk@cts.com>
#
# Usage: Tweak the config variables below as needed.  This script will
# spawn a quake2 server (presumably dedicated) and still allows you to
# type console commands to the server.  But the script pings the server
# for status every N seconds, and if M consecutive pings go unanswered
# by the server, the script kills the server process and restarts it.

use IO::Socket;
use strict;

$config::server_ip = "66.128.50.222";
$config::server_port = 27910;

# Geoff Joy, added this to reduce the default overhead
@config::server_args1 = ("+set ip", $config::server_ip, "+set port", $config::server_port);

$config::server_ping_interval_secs = 5;  # check server alive every N seconds
$config::dead_when_consec_packets_lost = 10;  # consider server dead when N consecutive status-pings lost

$config::server_binary = "./r1q2ded";
@config::server_args = scalar(@ARGV) ? @ARGV : qw(+set game lox +exec server.cfg);

our $PROGNAME = "q2mon";

$SIG{PIPE} = 'IGNORE';
$SIG{CHLD} = 'IGNORE';

sub server_alive {
	my $status = "";
	eval {
		my $sock = IO::Socket::INET->new(
	        	Proto => "udp",
	        	PeerAddr => $config::server_ip,
	        	PeerPort => $config::server_port
			);
		$sock->send("\xff\xff\xff\xffstatus");
	
		my $rin = '';
		my $rout;
		vec($rin, fileno($sock), 1) = 1;
		my $nfound = select($rout = $rin, undef, undef, 3.0);
		if ($nfound > 0) {
			$sock->recv($status, 2048);
		}
	};
	warn $@ if $@;

	return defined($status) && ($status ne "");
}

sub wait_till_dead {
	my $consec_lost = 0;
	while (1) {
		if (server_alive()) {
			print("$PROGNAME: server still alive...\n") if ($consec_lost > 0);
			$consec_lost = 0;
		}
		else {
			$consec_lost++;
			if ($consec_lost >= $config::dead_when_consec_packets_lost) {
				print("$PROGNAME: server dead...\n");
				return;
			}
			else {
				print("$PROGNAME: warning: $consec_lost consecutive packets lost to server...\n");
			}
		}
		sleep($config::server_ping_interval_secs);
	}
}

sub spawn_server {
	my $pid = undef;
	while (! defined($pid = fork)) {
		print("$PROGNAME: warning: failed to fork: $! ...retrying...\n");
		sleep(3);
	}
	if (! $pid) {  # child...
		print("$PROGNAME: spawning server (pid $$)...\n");
		exec($config::server_binary, @config::server_args1, @config::server_args)
			or print("$PROGNAME: child $$: exec($config::server_binary, @{config::server_args}) failed: $!\n");
		exit;  # normally never reach here
	}
	return $pid;
}

sub kill_server {
	my ($pid) = @_;
	print("$PROGNAME: sending server TERM signal...\n");
	kill('TERM', $pid);
	sleep(5);
	print("$PROGNAME: sending server KILL signal...\n");
	kill('KILL', $pid);
	sleep(5);
}


while (1) {
	my $pid = spawn_server();
	print("@config::server_args1\n");
	sleep(10);  # wait for server to come up
	wait_till_dead();
	kill_server($pid);
}

print("$PROGNAME: exiting...\n");

# Basically, you need to supply the address of the dedicated Quake 2 server 
# you're starting, the port, and the path and name of the Quake 2 binary 
# (the thing you run to make it go) and the parameters you usually supply 
# can optionally be provided as command line parameters to this script. 

# Also, you can configure how frequently the server is checked and 
# how many unanswered queries trigger the server restart. This works 
# much better than a simple looping script, since it actually checks 
# for a responding server, not just a terminated process. 

# Thus, you can start your Quake 2 server with: 
# ./q2mon.pl +set dedicated 1 +maxclients 10 and it will take off!

# Email the guy who made it for us if you have questions or 
# comments - he's done a fine job on this and I'm very happy 
# to make use of it on 4 different Quake 2 servers now!

