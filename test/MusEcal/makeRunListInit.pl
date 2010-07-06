#!/usr/bin/env perl

# get list of runs and gather header information 
# Julie Macles, Gautier Hamel de Monchenault

#use Date::Manip;

$dir      = @ARGV[0];
$lastRun  = @ARGV[1];

$listdir        = "$dir";

print " dir: $dir , lastRun: $lastRun \n"; 

$runlistred="${listdir}/runlist_IRed_Laser";
$runlistblue="${listdir}/runlist_Blue_Laser";
$runlisttp="${listdir}/runlist_Test_Pulse";

$runlistredinit="${listdir}/runlist_IRed_Laser_init";
$runlistblueinit="${listdir}/runlist_Blue_Laser_init";
$runlisttpinit="${listdir}/runlist_Test_Pulse_init";

my $curtime=time();

$runlistredbackup="${listdir}/runlist_IRed_Laser_init_${curtime}";
$runlistbluebackup="${listdir}/runlist_Blue_Laser_init_${curtime}";
$runlisttpbackup="${listdir}/runlist_Test_Pulse_init_${curtime}";

if( -e "$runlistredinit"){
    system "mv  $runlistredinit $runlistredbackup";
}
if( -e "$runlistblueinit"){
    system "mv  $runlistblueinit $runlistbluebackup";
}
if( -e "$runlisttpinit"){
    system "mv  $runlisttpinit $runlisttpbackup";
}

open( LBLUELIST, ">>$runlistblueinit")   || die "cannot open output file\n";
open( RUNLISTBLUE, "$runlistblue" )  || die "cannot open $runlistblue file \n";
while( <RUNLISTBLUE> )
{ 
    chomp ($_);
    my $theLine = $_;
    next unless ( $theLine =~ /Run(.*)_LB(.*)/ );
    
    my $run=$1;
    my $lb=$2;
    if($run<$lastRun){
	print LBLUELIST "$theLine\n";
    }
}

open( LREDLIST, ">>$runlistredinit")   || die "cannot open output file\n";
open( RUNLISTRED, "$runlistred" )  || die "cannot open $runlistred file \n";
while( <RUNLISTRED> )
{ 
    chomp ($_);
    my $theLine = $_;
    next unless ( $theLine =~ /Run(.*)_LB(.*)/ );
    
    my $run=$1;
    my $lb=$2;
    if($run<$lastRun){
	print LREDLIST "$theLine\n";
    }
}

open( LTPLIST, ">>$runlisttpinit")   || die "cannot open output file\n";
open( RUNLISTTP, "$runlisttp" )  || die "cannot open $runlisttp file \n";
while( <RUNLISTTP> )
{ 
    chomp ($_);
    my $theLine = $_;
    next unless ( $theLine =~ /Run(.*)_LB(.*)/ );
    
    my $run=$1;
    my $lb=$2;
    if($run<$lastRun){
	print LTPLIST "$theLine\n";
    }
}
