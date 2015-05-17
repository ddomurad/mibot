import sys

input_file = "";
output_file = "";

def parseLine(line):
	if "#[" in line and "]#" in line:
		si = line.find("#[")
		ei = line.find("]#")

		template = line[0:si] + '%d' + line[ei+2:]
		range_str = line[si+2:ei]
		range_vals = range_str.split(';')
		ret_line = ""
		for i in range( int(range_vals[0]), int(range_vals[1]) +1 ):
			ret_line = ret_line + template % (i)
		return ret_line
	return line

for i in range(len(sys.argv)):
	if sys.argv[i] == '-i':
		input_file = sys.argv[i + 1]
	elif sys.argv[i] == '-o':
		output_file = sys.argv[i + 1]

fin = open(input_file,'r');
fout = open(output_file,'w');


while True:
	line = fin.readline()
	if line == '':
		break

	fout.write( parseLine( line ) )
