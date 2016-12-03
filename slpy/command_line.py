import os
import sys
import slpy
def main():
    rows, columns = os.popen('stty size', 'r').read().split()
    slpy.sl(rows,columns,sys.argv)
