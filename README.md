# MCopyToDir

Command Line Program copies all files that match a filename 
pattern with in a directory tree to a single target directory.  



   usage:  MCopyToDir [-v] <target> <paternmatch> <srcdirs>+[-?]
           v1.0 copyright Comine.com

           Tool copies all files that match the pattern to target 
           location recursively from src directory

                -v : verbose output


 example:
           C:> MCopyToDir c:\work\tmp *.dll c:\windows
                 (This will copy all dlls to c:\work\tmp)

