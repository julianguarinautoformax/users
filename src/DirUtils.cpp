//
//  DirUtils.cpp
//  Users
//
//  Created by Julian Andres Guarin on 11/12/16.
//  Copyright © 2016 Julian Andres Guarin. All rights reserved.
//

#include "DirUtils.hpp"
#ifdef __CYGWIN__
#include <string.h>
#define bzero(x,y) memset(x,0,y)
#endif
static const int bufferSz = 1024;




std::string removelastslash(std::string&pth){
    
    if (pth[pth.length()-1] == '/'){
        pth = pth.substr(0,pth.find_last_of("/"));
    }
    return pth;
}
std::string DirUtils::expandDirExpression(std::string ftpExpression, std::string machineHOME, std::string machineCWD){
    ftpExpression = removelastslash(ftpExpression);
    
    if (ftpExpression[0] == '/'){
        ftpExpression = machineHOME + ftpExpression;
    } else if (ftpExpression[0] == '~'){
        if (ftpExpression.length()==1)
            ftpExpression = machineHOME;
        else
            ftpExpression = machineHOME + &ftpExpression[1];
    } else {
        ftpExpression = machineCWD+'/'+ftpExpression;
    }
    return ftpExpression;
    
}
//std::string DirUtils::relativeDirExpression(std::string )
int DirUtils::absoluteChangeDir(std::string absoluteChDirExpression, std::string machineHOME, std::string & machineCWD){
    
    char machineCWD_[bufferSz];
    getcwd(machineCWD_,bufferSz);
    bzero(machineCWD_,bufferSz);
    int rc = chdir(absoluteChDirExpression.c_str());
    chdir(machineCWD_);
    if (rc){
        return -1;
    }
    /* If we are below the machineHome */
    std::string resultwd = absoluteChDirExpression;
    /* Find MACHINE HOME inside the current working directory */
    std::size_t found = resultwd.find(machineHOME);
    if (found==std::string::npos) {
        int rc = absoluteChangeDir(machineHOME,machineHOME,machineCWD);
        if (rc == -1){
            rc = absoluteChangeDir(machineCWD, machineHOME,machineCWD);
            if (rc == -1) return -1;
        }
    } else {
        machineCWD = resultwd;
    }
    
    return 0;
}
int DirUtils::absoluteMakeDir(std::string absoluteNewDir, std::string machineHOME, std::string machineCWD){
    
    /* Get only directory */
    return mkdir(absoluteNewDir.c_str(), 0777);
    
}
int DirUtils::relativeMakeDir(std::string relativeNewDir,std::string machineHOME, std::string machineCWD){
    
    std::string absoluteNewDir = expandDirExpression(relativeNewDir,machineHOME, machineCWD);
    return absoluteMakeDir(absoluteNewDir, machineHOME, machineCWD);
}
int DirUtils::absoluteDeleteFile(std::string absoluteFile, std::string machineHOME, std::string machineCWD){
    return remove(absoluteFile.c_str());
}
int DirUtils::relativeDeleteFile(std::string relativeFile, std::string machineHOME, std::string machineCWD){
    relativeFile = expandDirExpression(relativeFile, machineHOME, machineCWD);
    return absoluteDeleteFile(relativeFile, machineHOME, machineCWD);
}
int DirUtils::relativeDeleteDir(std::string relativeDir, std::string machineHOME, std::string machineCWD, bool recursive){
    
    return absoluteDeleteDir(expandDirExpression(relativeDir, machineHOME, machineCWD), machineHOME, machineCWD, recursive);
}
#ifdef __CYGWIN__
int DirUtils::compare(const FTSENT* const* one, const FTSENT* const* two){
#else
int DirUtils::compare(const FTSENT** one, const FTSENT** two){
#endif
    return (strcmp((*one)->fts_name, (*two)->fts_name));
}
int DirUtils::absoluteDeleteDir(std::string absoluteDir, std::string machineHOME, std::string machineCWD, bool recursive){
   
    int rm = remove(absoluteDir.c_str());
    
    if (rm == -1 && errno == ENOTEMPTY){
        if (recursive == false ) return -1;
        char * paths[] = {(char*)absoluteDir.c_str(), NULL};
        /* CWD wont change (NOCHDIR), only symbolic link info(FTS_PHYSICAL) and no other dev than original(FTS_XDEV)*/
        /* Do not bring file info (OPTIMIZATION) FTS_NOSTAT */
        FTS * fts = fts_open(paths, FTS_NOCHDIR|FTS_PHYSICAL|FTS_XDEV|FTS_NOSTAT, DirUtils::compare);
        if (!fts) return -1;
        FTSENT * node;
        
        while ((node=fts_read(fts))!=NULL){
            FTSENT * children_node = fts_children(fts, 0);
            
            while(children_node){
                std::string children_path = std::string(children_node->fts_path) + "/" + std::string(children_node->fts_name);
                absoluteDeleteDir(children_path.c_str(), machineHOME , machineCWD, recursive);
                children_node = children_node->fts_link;
            }
        }
        return remove(absoluteDir.c_str());
    }
    
    return rm;
    

}
bool DirUtils::relativeExistDir(std::string relativeDir, std::string machineHOME, std::string machineCWD){
    relativeDir = expandDirExpression(relativeDir, machineHOME, machineCWD);
    return absoluteExistDir(relativeDir, machineHOME, machineCWD);
}
bool DirUtils::absoluteExistDir(std::string absoluteDir, std::string machineHOME, std::string machineCWD){
    
    
    struct stat info;
    int rc = stat(absoluteDir.c_str(),&info);
    if (rc == 0 && S_ISDIR(info.st_mode)) return true;
    return false;
    
}
bool DirUtils::relativeExistFile(std::string relativeFile, std::string machineHOME, std::string machineCWD){
    relativeFile = expandDirExpression(relativeFile, machineHOME, machineCWD);
    return absoluteExistFile(relativeFile, machineHOME, machineCWD);
}
bool DirUtils::absoluteExistFile(std::string absoluteFile, std::string machineHOME, std::string machineCWD){
    
    
    struct stat info;
    int rc = stat(absoluteFile.c_str(),&info);
    if (rc == 0 && S_ISREG(info.st_mode)) return true;
    return false;
    
}



