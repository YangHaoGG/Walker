#!/usr/bin/perl

use strict;
use JSON;
use Data::Dumper;
use Encode;

my %user = ();

while (<>) {
	my $line = from_json($_) or die "$!";
	my $src = Data::Dumper->Dumper($line);

	my ($VAR1, $VAR2);
	my $info = eval $src or die "$@";

	my $user_info = $info->{user_info};
	my $share = $user_info->{pubshare_count};
	my $album = $user_info->{album_count};
	
	if ($share || $album) {
		my $uk = $user_info->{uk};
		$user{$uk} = {};
		my $ref = $user{$uk};
		$ref->{share} = $share;
		$ref->{album} = $album;

		$ref->{name} = $user_info->{uname};
	}
}

my $text = to_json(\%user, { utf8=>1, pretty=>1 });
print $text, "\n";
