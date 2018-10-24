#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import os

def main(argv=None):
	if argv is None:
		argv = sys.argv
	
	outstring = ''

	startSeen = False

	for line in sys.stdin:
		if line.startswith('TRACER_START'):
			startSeen = True
		elif line.startswith('TRACER_END'):
			startSeen = False
			with open(argv[1], 'w') as f:
				f.write(outstring)
				f.flush()
				os.fsync(f.fileno()) 
			outstring = ''
			os.rename(argv[1], argv[2])
		else:
			if startSeen:
				outstring += line
	

if __name__ == "__main__":
	sys.exit(main())

