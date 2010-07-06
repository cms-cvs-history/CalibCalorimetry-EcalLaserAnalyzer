#!/usr/bin/env perl

# get list of runs and gather header information 
# Julie Macles, Gautier Hamel de Monchenault

#use Date::Manip;

$dir      = @ARGV[0];
$firstRun = @ARGV[1];
$lastRun  = @ARGV[2];

$listdir        = "$dir";

print " dir: $dir , lastRun: $lastRun \n"; 

$runlistredinit="${listdir}/runlist_IRed_Laser_init_fromfiles";
$runlistblueinit="${listdir}/runlist_Blue_Laser_init_fromfiles";
$runlisttpinit="${listdir}/runlist_Test_Pulse_init_fromfiles";

opendir( INDIR, $listdir ) || die "cannot open $inputdir directory\n";

open( LBLUELIST, ">$runlistblueinit")   || die "cannot open output file\n";
open( LREDLIST, ">$runlistredinit")   || die "cannot open output file\n";
open( LTPLIST, ">$runlisttpinit")   || die "cannot open output file\n";

my @dirs = sort  readdir( INDIR );

foreach my $dirname (@dirs)
{ 
    next unless ( $dirname=~ /LMF_(.*)_(\d{1})_(.*)_Run(.*)_LB(.*)_TS(.*).root/ );

    my $sm=$1;
    my $side=$2;
    my $type=$3;
    my $run=$4;
    my $lb=$5;
    my $ts=$6;
    my $nevt=1200;
    my $ts2=$ts+85899255076;

    next if( $run>$lastRun || $run<$firstRun );
    #print " $dirname $type\n";
    
    my $theLine="Run${run}_LB${lb}       $run  $lb    $nevt    $ts    $ts2    1      0      -1      -1      -1";

    #print "Run${run}_LB${lb}       $run  $lb    $nevt    $ts    $ts2    1      0      -1      -1      -1 \n";

    if($type =~ /testPulse/){
	#print "testPulse\n";	
	print LTPLIST "$theLine\n";
    }
    if($type =~ /BlueLaser/){
	#print "BlueLaser\n";
	print LBLUELIST "$theLine\n";
    }
    if($type =~ /IRedLaser/){
	#print "IRedLaser\n";
	print LREDLIST "$theLine\n";
    }
}
