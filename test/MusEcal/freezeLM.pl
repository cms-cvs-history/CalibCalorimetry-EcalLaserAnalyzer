#!/usr/bin/env perl

use Term::ANSIColor;
#use Date::Manip; 
use Cwd;

# Script to freeze all LM results until a defined run:
# =====================================================
# * stops monitoring and primitives generation
# * create initial frozen runlists to be used by musecal 
#   for frozen period run>lastRun
# * tar all LM results directories and remove all links
#   til defined lastRun
# * writes a new cfg file for monitoring and primitives 
#   with firstRun=lastRun and eventually installs new LM dir 
#    if LM directory is changed
# * starts LM and primitives again or ask the user to do so  


print color("red"),"\n\n ***** Freezing all LM results until a defined run  *****\n", color("reset");


$lastRun  = @ARGV[0];
$newLMDir   = @ARGV[1];


if( $lastRun<138000 ){
    print "Parameter lastRun is too small! Setting it to 138000 \n";
    $lastRun=138000;
} 

$doChangeDir=0;

if( ${newLMDir} eq ""){
    $doChangeDir=0;
}else{
    $doChangeDir=1;
}


$cfgfile="/nfshome0/ecallaser/config/lmf_cfg";
do "/nfshome0/ecallaser/config/readconfig.pl";
readconfig(${cfgfile});

${MON_MUSECAL_DIR}=~ s/\s+//;
${MON_CMSSW_REL_DIR}=~ s/\s+//;


# stop monitoring and primitives generation
# ===========================================
print "Stopping all processes on farm before cleaning...\n";

$command = "lmfc stop monitoring" ;
system ${command};
#print "$command \n";
$command = "lmfc stop primitives" ;
system ${command};
#print "$command \n";

print " ... Done\n";
print " -------------------------------------- \n";

# create initial frozen runlists for period run>lastRun
# ======================================================



print " -------------------------------------- \n";
print "Freezing runlists with command:\n";

$command = "${MON_MUSECAL_DIR}/createRunListInit.sh ${lastRun}" ;
print "$command \n";
system ${command};


print " ... Done\n";
print " -------------------------------------- \n";


# tar all LM results directories and remove all links til lastRun
# ================================================================

print "Taring and deleting with command:\n";

$command = "${MON_MUSECAL_DIR}/tarAll.sh ${lastRun}" ;
print "$command \n";
system ${command};

print " ... Done\n";
print " -------------------------------------- \n";

# write a new cfg file for monitoring and primitives 
# ===================================================


print "Generating new config file ...\n";

$curtime=time();
$cfgfilebackup="/nfshome0/ecallaser/config/lmf_cfg_${curtime}";
$command = "mv ${cfgfile} ${cfgfilebackup}" ;
system ${command};


open( TEMPLATE, "$cfgfilebackup" );
open( CFGFILE, ">$cfgfile" );
while( <TEMPLATE> )
{  
    
    chomp ($_);
    my $theLine = $_;
    if( $theLine  =~ /MON_FIRST_RUN/ && $theLine  !~ /\#export/){
	my $newLine= "export MON_FIRST_RUN=$lastRun";
	print CFGFILE "$newLine \n"; 
    }elsif($doChangeDir==1 && $theLine  =~ /MON_OUTPUT_DIR/ && $theLine  !~ /\#export/){
	my $newLine= "export MON_OUTPUT_DIR=$newLMDir";
	print CFGFILE "$newLine \n"; 
    }else{
	print CFGFILE "$theLine \n";
    }    
}
close CFGFILE;
close TEMPLATE;

print " ... Done\n";
print " -------------------------------------- \n";

if( $doChangeDir == 1 ){
    
    print "Installing new directory ...\n";
 
    my $instal="${MON_CMSSW_REL_DIR}/src/CalibCalorimetry/EcalLaserAnalyzer/scripts/install.sh";
    system ${instal};    
    #print "$instal \n";
    print " ... Done\n";
    print " -------------------------------------- \n";
}


# start LM and primitives again or ask the user to do so  
# =======================================================

print "Starting all processes on farm with new setup: \n";

$command = "lmfc start primitives" ;
system ${command};
$command = "lmfc start monitoring" ;
system ${command};

print " ... Done\n";
print " -------------------------------------- \n";
