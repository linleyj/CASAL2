import os
import os.path
import subprocess
import sys
import shutil

import Globals

class Builder:
  version_ = '1.0'
  
  def start(self):
    # Variables
    fileName = 'parser'
    header   = 'parser.h'
    library  = 'parser.a'
    
    # Clean our any existing files if they already exist
    print '-- Cleaning Parser files'
    if os.path.exists(fileName):
      shutil.rmtree(fileName)
    os.system('rm -rf ' + Globals.target_include_path_ + header)
    os.system('rm -rf ' + Globals.target_debug_lib_path_ + library)
    os.system('rm -rf ' + Globals.target_release_lib_path_ + library)        
    
    # Decompress our archive
    print '-- Decompressing Parser - check isam_unzip.log'
    if os.path.exists(fileName + '.zip'):
      os.system('unzip ' + fileName + '.zip 1> isam_unzip.log 2>&1')
    
    # Build 
    print '-- Building Parser - check parser/isam_build.log'
    os.chdir(fileName)
    os.system("g++ -g0 -c parser.cpp 1> isam_build.log 2>&1")
    os.system("ar rvs parser.a parser.o 1>> isam_build.log 2>&1")
        
    # Move our headers and libraries
    print '-- Moving headers and libraries'
    shutil.copy(header, Globals.target_include_path_)
    shutil.copy(library, Globals.target_debug_lib_path_)
    shutil.copy(library, Globals.target_release_lib_path_)
    
    return True