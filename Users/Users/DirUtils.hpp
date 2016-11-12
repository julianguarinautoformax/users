//
//  DirUtils.hpp
//  Users
//
//  Created by Julian Andres Guarin on 11/12/16.
//  Copyright © 2016 Julian Andres Guarin. All rights reserved.
//

#ifndef DirUtils_hpp
#define DirUtils_hpp

#include <stdio.h>
#include <iostream>
#include "Users.hpp"
class DirUtils{
    
    static std::string expandDirExpression(std::string dirExpresssion, std::string rootDir, std::string homeDir, std::string currentWorkingDirectory);
    
};


#endif /* DirUtils_hpp */
