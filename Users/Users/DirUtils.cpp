//
//  DirUtils.cpp
//  Users
//
//  Created by Julian Andres Guarin on 11/12/16.
//  Copyright © 2016 Julian Andres Guarin. All rights reserved.
//

#include "DirUtils.hpp"


std::string DirUtils::expandDirExpression(std::string dirExpresssion, std::string rootDir, std::string homeDir, std::string currentWorkingDirectory){
    static const std::string rootDirToken = "/";
    static const std::string homeDirToken = "~";
    static const std::string cwdDirToken = ".";
    static const std::string cwdParentToken = "..";
    
    
}
