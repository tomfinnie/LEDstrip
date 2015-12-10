#!/usr/bin/env perl

use strict;
use warnings;
use English qw( -no_match_vars );
use Carp;
use Template;

use version; our $VERSION = qv(0.1);

if ( @ARGV != 1 ) { croak "Usage: $PROGRAM_NAME <test.c>" }

my @tests = ();

open( my $file, '<', $ARGV[0] ) || croak $ERRNO;
while ( my $line = <$file> ) {
    if ( $line =~ /^TEST [(] ([^)]+) [)]/xms ) { push @tests, { name => $1 } }
}
close $file || croak $ERRNO;

my $template = Template->new();
my $template_vars = { number_of_tests => scalar(@tests), tests => \@tests };
$template->process( \*DATA, $template_vars ) || croak $template->error();

__END__
#include <stdlib.h>

#include "TESTickle.h"

[% FOREACH test IN tests %]struct test_result [% test.name %]();
[% END %]
int main(void)
{
    int test_number = 1;

    TEST_FIXTURE([% number_of_tests %]);

[% FOREACH test IN tests %]    RUN_TEST([% test.name %], test_number++);
[% END %]
    exit(EXIT_SUCCESS);
}
