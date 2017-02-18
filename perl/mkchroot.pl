#!/usr/bin/perl 

use File::Basename;
use File::Path;
use strict;

my $home = ".";

if (@ARGV == 1) {
	$home = $ARGV[0];
}

my %bins = (
	"/bin" => [
		"bash",
		"busybox",
	]
);
my %libs = ();
my %dirs = (
	bin => undef,
	sbin => undef,
	dev => undef,
	etc => undef,
	lib => undef,
	lib64 => undef,
	home => {
		bind => "/tmp/home"
	},
	usr => {
		bind => "/tmp/user"
	},
	proc => {
		bind => "/proc"
	}
);

&make_dirs(\%dirs);
&dump_libs(\%libs, \%bins);
&copy_bins(\%bins);
&copy_libs(\%libs);

sub make_dirs {
	my $rd = shift;

	print "\nstatr make directorys:\n";
	while (my ($dir, $info) = each %$rd) {
		print "\tmake directory $home/$dir\n";
		mkpath("$home/$dir") unless (-d "$home/$dir");
		if (defined($info) && defined($info->{bind})) {
			mkpath($info->{bind}) unless (-d $info->{bind});
			print "\tbind $home/$dir to $info->{bind}\n";
			`mount --bind $info->{bind} $home/$dir`;
		}
	}
}

sub copy_bins {
	my $rb = shift;

	print "\nstart copy bins:\n";
	while (my ($dir, $arr) = each (%$rb)) { 
		foreach my $bin (@$arr) {
			print "\tcopy $dir/$bin to $home/$dir\n";
			mkpath("$home/$dir") unless (-d "$home/$dir");
			`cp -n $dir/$bin $home/$dir`;
		}
	}
}

sub dump_libs {
	my ($rl, $rb) = @_;
	print "\nstart dump libs:\n";
	while (my ($dir, $arr) = each (%$rb)) {
		foreach my $bin (@$arr) {
			print "\tdump libs of $dir/$bin\n";
			open (my $fd, "ldd $dir/$bin 2>/dev/null |") or die "$!";
			while (<$fd>) {
				if (/\t(.*)\s+=>\s+\(.*\)/) {
					print "\t\tignore $1\n";
					next;
				}
				if (/\t.*\s+=>\s+(.+)\s+\(.*\)/) {
					print "\t\trequire lib $1\n";
					$rl->{$1} = dirname($1);
					next;
				}
				if (/\t(\/.*)\s+\(.*\)/) {
					print "\t\trequire lib $1\n";
					$rl->{$1} = dirname($1);
					next;
				}
				print "\t\tunknown lines\n";
			}
		}
	}
}

sub copy_libs {
	my $rl = shift;
	print "\nstart copy libs:\n";

	while (my ($src, $dst) = each (%$rl)) {
		print "\tcopy $src to $home/$dst\n";
		mkpath("$home/$dst") unless (-d "$home/$dst");
		`cp -n $src $home/$dst`;
	}
}
