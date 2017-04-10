#!/usr/bin/perl


use LWP;
use JSON;
use Getopt::Long;
use Coro;
use Coro::LWP;
use Data::Dumper;
use DateTime;

use strict;

my $start = 0;
my $end = 0;
my $thread = 10;
my $path = "..";
my $file = "default";

GetOptions("start=s" => \$start,
	"end=s" => \$end,
	"file=s" => \$file,
	"path=s" => \$path,
	"thread=i" => \$thread) or die "$!";

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
my $dt = DateTime->now;
my $pt = $dt->ymd("");
$file = "$path/info/origin/$file.$pt.$start.$end";

my $fd = undef;
($start, $end) = get_input($start, $end);
$end = $start + 10000 if $end <= $start;
if ($file) {
	open($fd, ">", $file) or die "$!";
}

print "look for $start to $end\n";

my $uri = "https://pan.baidu.com/pcloud/user/getinfo?query_uk=";
my @header = {
	Accept => "*/*",
};

sub robot_req {
	my ($s, $e, $i) = @_;

	my $ua = LWP::UserAgent->new(ssl_opts=>{verify_hostname=>1});
	$ua->agent("Mozilla/5.0");

	my $rate = 0.1;
	my $t = 5;

	for my $uk ($s .. $e) {
		if (($uk-$s)/($e-$s) >= $rate) {
			print "thread $i progress $rate: $uk in [ $s, $e ]\n";
			$rate += 0.1;
		}

		RETRY:
		my $res = $ua->get($uri . $uk, @header);

		if ($res->is_success) {
			my $info = from_json($res->content) or die "$!";
			$info = Data::Dumper->Dumper($info);
			my ($VAR1, $VAR2);
			eval $info;
			if ($VAR2->{errno} != 0) {
				print "ERROR: uk = $uk, REASON: ", $VAR2->{error_msg}, "\n";
				if ($VAR2->{error_msg} eq "too fast") {
					print "need sleep $t seconds\n";
					sleep $t;
					$t += 5;
					goto RETRY;
				}
			} else {
				$t = 5;
				print $fd $res->content, "\n";
			}
		} else {
			print "ERROR: uk = $uk, REASON: ", $res->status_line, "\n";
			goto RETRY;
		}
		$Coro::current->ready;
		Coro::schedule;
	}
}

my $per = int(($end - $start + 1) / $thread);
my $passed = time;
my @monitor = ();
foreach (1 .. $thread) {
	my $t = new Coro \&robot_req, ($_-1)*$per+$start, $_*$per+$start-1, $_;
	$t->ready;
	push @monitor, $t;
}

foreach (@monitor) {
	$_->join;
}

$passed = time - $passed;
print "\nuse $thread threads, time passed $passed seconds\n";
