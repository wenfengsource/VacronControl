#!/bin/sh  
 
ARG=$1  
 
case   $ARG   in    
start):  
   /home/ubuntu/work/osd/ReturnChannelTestkit/vacron     &  
;;  
stop):  
pkill   vacron  
;;  
restart):  
pkill   vacron  
 /home/ubuntu/work/osd/ReturnChannelTestkit/vacron   & 
;;  

esac
