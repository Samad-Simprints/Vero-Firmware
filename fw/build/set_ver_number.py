'''
Script to set firmware version number (main pcb binary)
Expected to be called by CrossStudio before complication.
'''
import subprocess
import re


def find_version( ):
    '''
    Find the current development version via the latest git tag
    Currently just the patch version due to firmware limitations
    '''
    gitRet = subprocess.run(['git','describe','--tags','--long'], stdout=subprocess.PIPE)
    if gitRet.returncode != 0:
        raise Exception( 'git describe failed')
    vers = re.findall('v(\d+)\.(\d+)\.(\d+)-(\d+)-(\w+)', str(gitRet.stdout))
    if len( vers ) == 0:
        raise Exception( 'No version found' )
    patch = vers[0][2]
    return patch
    

def inplace_sub(filename, pattern, repl):
    '''
    Substitute the pattern in the filename with repl
    '''
    with open(filename) as f:
        s = f.read()

    s = re.sub(pattern, repl, s)

    with open(filename, 'w') as f:
        f.write(s)

if __name__ == '__main__':
    main_filename = '../include/proj_defs.h'
    main_pattern = r'(?<=INDEX_REVISION_NUMBER)\s+\d+'
    main_repl = '         %s'% find_version()
    inplace_sub( main_filename, main_pattern, main_repl)
