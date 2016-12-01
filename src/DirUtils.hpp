//
//  DirUtils.hpp
//  Users
//
//  Created by Julian Andres Guarin on 11/12/16.
//  Copyright © 2016 Julian Andres Guarin. All rights reserved.
//

#ifndef DirUtils_hpp
#define DirUtils_hpp
#ifdef __CYGWIN__
typedef unsigned short u_short;
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <fts.h>
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include "Users.hpp"
class DirUtils{
public:
    
    
    static std::string expandDirExpression( std::string ftpExpression,          std::string machineHOME, std::string machineCWD);
    static int absoluteChangeDir(           std::string absoluteFtpExpression,  std::string machineHOME, std::string & machineCWD);
    static int relativeMakeDir(             std::string relativeNewDir,         std::string machineHOME, std::string machineCWD);
    static int absoluteMakeDir(             std::string absoluteNewDir,         std::string machineHOME, std::string machineCWD);
    static int relativeDeleteDir(           std::string relativeDir,            std::string machineHOME, std::string machineCWD, bool recursive = false);
    static int absoluteDeleteDir(           std::string relativeDir,            std::string machineHOME, std::string machineCWD, bool recursive = false);
#ifdef __CYGWIN__    
    static int compare(                     const FTSENT* const* one,           const FTSENT* const* two);
#else
    static int compare(                     const FTSENT** one,                 const FTSENT** two);
#endif
    static int relativeDeleteFile(          std::string relativeFile,           std::string machineHOME, std::string machineCWD);
    static int absoluteDeleteFile(          std::string absoluteFile,           std::string machineHOME, std::string machineCWD);
    static bool relativeExistDir(           std::string relativeDir,            std::string machineHOME, std::string machineCWD);
    static bool absoluteExistDir(           std::string absoluteDir,            std::string machineHOME, std::string machineCWD);
    static bool relativeExistFile(          std::string relativeFile,           std::string machineHOME, std::string machineCWD);
    static bool absoluteExistFile(          std::string absoluteFile,           std::string machineHOME, std::string machineCWD);
    
};


#endif /* DirUtils_hpp */
