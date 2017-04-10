#!/usr/bin/perl

use LWP;
use JSON;
use Getopt::Long;
use Coro;
use Coro::LWP;
use Data::Dumper;
use DateTime;

use strict;

my $uri = "https://pan.baidu.com/pcloud/feed/getsharelist?category=0&auth_type=1&start=0&limit=100&query_uk=";
my $cookie="BAIDUID=8AFB4B6B9C8A1F7582221D8BC3578F8E:FG=1";

my %header = (
	Accept=>"*/*",
	Referer=>"https://pan.baidu.com/share/home",
	Cookie=>$cookie,
);

my $ua = LWP::UserAgent->new(ssl_opts=>{verify_hostname=>1});
$ua->agent("Mozilla/5.0");

my %list = ();

sub do_fetch_uk {
	my $uk = shift;

	RETRY:
	my $res = $ua->get($uri.$uk, %header);
	if ($res->is_success) {
		my $info = from_json($res->content) or die "$!";
		$info = Data::Dumper->Dumper($info);
		my ($VAR1, $VAR2);
		eval $info;
		if ($VAR2->{errno} != 0) {
			print "ERROR: uk = $uk, REASON: errno ", $res->as_string, "\n";
			#goto RETRY;
		} else {
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
		}
	} else {
		print "ERROR: uk = $uk, REASON: ", $res->status_line, "\n";
		goto RETRY;
	}
}

sub do_fetch_all {
	my $user = shift;
	foreach (keys(%$user)) {
		do_fetch_uk($_);
		last;
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

my $answer = to_json(\%list, { utf8=>1 });
print $answer, "\n";
