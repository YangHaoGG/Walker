#!/usr/bin/perl

use LWP;
use JSON;
use Getopt::Long;
use Coro;
use Coro::LWP;
use Data::Dumper;
use DateTime;

use strict;

my $uri = "https://pan.baidu.com/pcloud/feed/getsharelist?category=0&auth_type=1&start=0&limit=100&logid=MTQ5MTg0MzA0NDc0MzAuMDA1NzM4MTk4MDExNDczMjM5&query_uk=";

my %header = (
	Accept=>"*/*",
	Referer=>"https://pan.baidu.com/share/home",
#	Cookie=>$cookie,
);

my $ua = LWP::UserAgent->new(ssl_opts=>{verify_hostname=>1});
$ua->agent("Mozilla/5.0");

my %list = ();
my $UA = "Mozilla/5.0";
my $REFER = "Referer: https://pan.baidu.com/share/home";
my $cookie="Cookie: BAIDUID=8AFB4B6B9C8A1F7582221D8BC3578F8E:FG=1";

sub do_fetch_uk {
	my $uk = shift;

	my $url = $uri . $uk;
	my $cmd = "curl -v -A \"$UA\" -H \"$REFER\" -H \"$cookie\" \"$url\" 2>/dev/null";
	my $tm = 0;
	RETRY:
	my $res = `$cmd`;
	if ($res eq "") {
		print "Response empty:$cmd\n";
		return;
	}
	$res = from_json($res) or {return};
	$res = Data::Dumper->Dumper($res);
	my ($VAR1, $VAR2);
	eval $res;
	if ($VAR2->{errno} != 0) {
		print "ERROR: uk = $uk, REASON: ", $VAR2->{errno}, "\n";;
		if ($VAR2->{errno} == -55) {
			$tm += 5;
			sleep $tm;
			goto RETRY;
		}
	} else {
		print "SUCCESS: uk = $uk\n";
		$list{$uk} = [];
		foreach my $rs (@{$VAR2->{records}}) {
			my $x = {
				title => $rs->{title},
				url => $rs->{shorturl},
				category => $rs->{category},
				desc => $rs->{desc},
			};
			push @{$list{$uk}}, $x;
		}
		sleep 5;
	}
}

sub do_fetch_all {
	my $user = shift;
	foreach (keys(%$user)) {
		do_fetch_uk($_);
	}
}

foreach (@ARGV) {
	if (! -f $_) {
		print STDERR "$_ is not a file\n";
		next;
	}

	print "Process with $_\n";

	my $size = (stat($_))[7];
	if (!$size) {
		next;
	}

	open(my $fd, "<", $_) or die "$!";
	read $fd, my $json, $size;
	$json = from_json($json);

	my $info = Data::Dumper->Dumper($json);
	my ($VAR1, $VAR2);
	eval $info or die "$!";

	do_fetch_all $VAR2;
}
open (my $fd, ">>", "../info/final/r.data") or die "$!";

my $answer = to_json(\%list, { utf8=>1 });
print $fd $answer, "\n";
