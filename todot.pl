#!/usr/bin/perl

$node_attr = '[shape=circle]';
%edge_attr = (
	C => '[color=black,weight=100]',
	T => '[color=blue,style=dashed,weight=0]',
	P => '[color=red,style=bold,weight=0]' );

print "graph Connect {\n";
chomp ($N = <>);
for($n = 1; $n <= $N; ++$n) {
	print "\t$n $node_attr\n";
}
while(<> =~ /([CTP]) (\d+)-(\d+)/)
{
	print "\t$2 -- $3 $edge_attr{$1}\n";
}
print "}\n";
