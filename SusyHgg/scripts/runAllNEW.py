#!/usr/bin/env python

import os
import sys

m2_pts = range(125,501,25)
m1_pts = range(0,1,25)

def runAll(folder):
    os.chdir(folder)
    
    for m1 in m1_pts:
        for m2 in m2_pts:
            if m2-m1<125: continue

            cmds = [
#                "combine -M Asymptotic sms_ChiWH_%d_%d.txt -n ChiWH_%d_%d" % (m1,m2,m1,m2),
#                "combine -M Asymptotic sms_ChiZH_%d_%d.txt -n ChiZH_%d_%d" % (m1,m2,m1,m2),
#                "combine -M Asymptotic sms_ChiHH_%d_%d.txt -n ChiHH_%d_%d" % (m1,m2,m1,m2),
                "combine -M Asymptotic sms_ChiWH_FSIMSmear_%d_%d.txt -n ChiWH_FSIMSmear_%d_%d" % (m1,m2,m1,m2),
                "combine -M Asymptotic sms_ChiZH_FSIMSmear_%d_%d.txt -n ChiZH_FSIMSmear_%d_%d" % (m1,m2,m1,m2),
                "combine -M Asymptotic sms_ChiHH_FSIMSmear_%d_%d.txt -n ChiHH_FSIMSmear_%d_%d" % (m1,m2,m1,m2)
                ]
            for cmd in cmds:
                print cmd
                os.system(cmd)

            
    

if __name__=="__main__":
    if len(sys.argv)<2:
        print sys.argv[0]+" <folder>"
        sys.exit(0)

    folder = sys.argv[1]
    runAll(folder)
