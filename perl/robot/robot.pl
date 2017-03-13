#!/usr/bin/perl 

use LWP::Simple;
use strict;

my $main=get("https://kyfw.12306.cn/otn") or die "failed $!";
my @link=$main=~m#"(https?://.*?)"#g;
my @herf=$main=~m#herf\s*=\s*(.*?)\s#g;

print join("\n", @link);
print join("\n", @herf);
