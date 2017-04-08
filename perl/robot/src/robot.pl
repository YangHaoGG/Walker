#!/usr/bin/perl

use strict;

use LWP;
use JSON;
use Getopt::Long;

my $start = 0;
my $end = 0;
my $file = undef;

GetOptions("start=s" => \$start,
	"end=s" => \$end,
	"file=s" => \$file) or die "$!";

sub get_input {
	return map {
		my $n = undef;
		while (1) {
			my ($d, $k, $l) = $_ =~ /(\d+)([kw]+)*(.*)/gi;
			if ($k) {
				$k =~ s/k/000/gi;
				$k =~ s/w/0000/gi;
			}
			$n += int($d . $k);
			if ($l) {
				$_ = $l;
				next;
			} else {
				last;
			}
		}
		$n;
	} @_;
}

($start, $end) = get_input($start, $end);
$end = $start + 10000 if $end <= $start;
if ($file) {
	open(STDOUT, ">>", $file) or die "$!";
}

print STDERR "look for $start to $end\n";

my $uri = "https://pan.baidu.com/pcloud/user/getinfo?query_uk=";
my @header = {
	Accept => "*/*",
};

my $ua = LWP::UserAgent->new(ssl_opts=>{verify_hostname=>1});
$ua->agent("Mozilla/5.0");

for my $uk ($start .. $end) {
	my $res = $ua->get($uri . $uk, @header);

	if ($res->is_success) {
		print $res->content, "\n";
	} else {
		print STDERR $res->status_line, "\n";
	}
}
