/*
 *  Users.cpp
 *  Users
 *
 *  Created by Julian Andres Guarin on 11/10/16.
 *  Copyright © 2016 Julian Andres Guarin. All rights reserved.
 *
 */

#include <iostream>
#include <unistd.h>
#include "Users.hpp"


using namespace std;

User::User(nlohmann::json userEntry):
    m_accountname(userEntry["accountname"].get<string>()),
    m_password(userEntry["password"].get<string>()),
    m_baseDir(userEntry["bdir"].get<string>()),
    m_lastDir(userEntry["lastDir"].get<string>()),
    m_quota(userEntry["quota"].get<int>())
{
    m_validUser = true;
    
    int prm = userEntry["permissions"].get<int>();
    m_permissions = prm==1 ? USER_WR : (prm == 2 ? USER_RD : (prm == 3 ? USER_RDWR : (prm == 4 ? USER_JOKER : USER_RD)));
    
    
    nlohmann::json roles = userEntry["roles"];
    m_role = 0;
    for (nlohmann::json roleEntry_json:roles){
        std::string roleEntry = roleEntry_json.get<string>();
        if (!roleEntry.compare("admin")) m_role|= (1<<ADMIN);
        if (!roleEntry.compare("user")) m_role |= (1<<USER);
    }
    
}
bool User::userAuthorized(std::string & password){
    return m_password.compare(password) == 0 ? true : false;
}
std::string User::getUserCwd(){
    return m_lastDir;
}
bool
bool User::userChdir(std::string &path){
    
    if (path.compare(".") == 0 ) return true;
    bool isAbsolutePath = path[0] == '/' ? true : false;
    
    
    //This wrong const char * path_cstr = isAbsolutePath ? (m_userBaseDir +path).c_str() : (path.compare("..") == 0 ? path.c_str() : &((m_userBaseDir + m_lastDir + path).c_str()[1]));
    
    
    int rc = userChdir(path_cstr);
    if (rc<0){
        return false;
    }
    char buff[4096];
    bzero(buff,4096);
    getcwd(buff, 4096);
    m_currentWorkingDir = std::string(buff);
    m_lastDir =
}

Users::Users (const char * jsonpath){
    
    ifstream f (jsonpath);
    m_j << f;
    f.close();
    
    for (nlohmann::json::iterator it = m_j.begin(); it<m_j.end(); ++it){
        nlohmann::json entry = *it;
        auto accountname_json = entry["accountname"];
        if (accountname_json.type() == nlohmann::json::value_t::null) continue;
        auto accountname = accountname_json.get<string>();
        m_user[accountname]=entry;
    }
    
    
}

void Users::userDump(){
    cout<<m_user.dump(3)<<endl;
}


int Users::userCount(){
    
    /* When a register contains the key accountname is considered an user account */
    int count=0;
    for (nlohmann::json::iterator it = m_j.begin(); it<m_j.end(); ++it){
        nlohmann::json entry = *it;
        auto accountname = entry["accountname"];
        if (accountname == nullptr) continue;
        count++;
    }
    return count;
    
}


bool Users::userExist(char * user){
    
    nlohmann::json userEntry = m_user[user];
    return userEntry.type() == nlohmann::json::value_t::null ? false : true;
    
    
}

User Users::user(char * user){

    bool userExists = userExist(user);
    
}


