/*
 *  Users.cpp
 *  Users
 *
 *  Created by Julian Andres Guarin on 11/10/16.
 *  Copyright © 2016 Julian Andres Guarin. All rights reserved.
 *
 */
#include <fts.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <fstream>
#include <unistd.h>
#include "Users.hpp"
#include "DirUtils.hpp"

using namespace std;
#define ubc(u) u,m_userBaseDir,m_currentWorkingDir




User::User(nlohmann::json userEntry, std::string appServerPath):
    m_accountname(userEntry["accountname"].get<string>()),
    m_password(userEntry["password"].get<string>()),
    m_baseDir(userEntry["bdir"].get<string>()),
    m_lastDir(userEntry["lastDir"].get<string>()),
    m_quota(userEntry["quota"].get<int>()),
    m_leftquota(userEntry["leftQuota"].get<int>())
{
    m_validUser = true;
    m_authUser  = false;
    
    int prm = userEntry["permissions"].get<int>();
    m_permissions = prm==1 ? USER_WR : (prm == 2 ? USER_RD : (prm == 3 ? USER_RDWR : (prm == 4 ? USER_JOKER : USER_RD)));
    
    
    nlohmann::json roles = userEntry["roles"];
    m_role = 0;
    for (nlohmann::json roleEntry_json:roles){
        std::string roleEntry = roleEntry_json.get<string>();
        if (!roleEntry.compare("admin")) m_role|= USER_ROLE_ADMIN;
        if (!roleEntry.compare("user"))  m_role|= USER_ROLE_USER;
    }
    
    /* Configure directions */
    m_userBaseDir = appServerPath+m_baseDir;
    m_currentWorkingDir = m_userBaseDir;
    /* Create Base Dir if Dir Does not Exist */
    if (!DirUtils::absoluteExistDir(ubc(m_userBaseDir))){
        DirUtils::absoluteMakeDir(m_userBaseDir, m_userBaseDir, m_currentWorkingDir);
    }
    
    
    userUpdateLastDir();
    
    
    
}
void User::userUpdateLastDir(){
    
    if (m_currentWorkingDir.compare(m_userBaseDir) == 0){
        m_lastDir = "/";
    } else {
        m_lastDir = &m_currentWorkingDir[m_userBaseDir.length()];
    }
    
    
}
bool User::userIsAuthorized(std::string & password){
    
    m_authUser = (m_password.compare(password) == 0);
    if (m_authUser){
        userChdir(m_lastDir);
    }
    m_validUser = m_authUser;
    return m_authUser;
}


std::string User::userCwd(){
    return m_lastDir;
}


int User::userChdir(std::string &userpath){
    userpath = DirUtils::expandDirExpression(ubc(userpath));
    int rc =  DirUtils::absoluteChangeDir(ubc(userpath));
    if (rc == 0) {
        
    }
    return rc;
}

int User::userMkdir(std::string&path){
    return DirUtils::relativeMakeDir(ubc(path));
}

int User::userRmDir(std::string &dirpath){
    return DirUtils::relativeDeleteDir(ubc(dirpath));
}

int User::userRmFile(std::string &filepath){
    return DirUtils::relativeDeleteFile(ubc(filepath));
}
int compare(const FTSENT** one, const FTSENT** two)
{
    return (strcmp((*one)->fts_name, (*two)->fts_name));
}
unsigned int User::userUsedQuota(std::string path){
    
    path = DirUtils::expandDirExpression(ubc(path));
    unsigned int calcUsedQuota = 0;
    char * paths[] = {(char*)path.c_str(),NULL};
    
    FTS * fts = fts_open(paths, FTS_NOCHDIR|FTS_PHYSICAL|FTS_XDEV, compare);
    
    if (!fts) return -1;
    
    FTSENT * node;
    
    for (node = fts_read(fts) ; node; node = fts_read(fts)){
        FTSENT * children_node = fts_children(fts,0);
        while(children_node){
            std::string fullfilename = std::string(children_node->fts_path)+"/"+std::string(children_node->fts_name);
            off_t filesize = children_node->fts_statp->st_size;
            cout<<fullfilename<<": "<<filesize;
            calcUsedQuota+=filesize;
            cout<<" ["<<calcUsedQuota<<"]"<<endl;
        }
    }
    return calcUsedQuota;
}

unsigned int User::userLeftQuota(){
    auto uq = userUsedQuota();
    if (uq>m_quota){
        m_leftquota = 0;
    } else {
        m_leftquota = m_quota - uq;
    }
    return m_leftquota;
}


Users::Users (std::string jsonpath){
    
    ifstream f (jsonpath);
    m_j << f;
    f.close();
    m_configFilePath = jsonpath;
    for (nlohmann::json::iterator it = m_j.begin(); it<m_j.end(); ++it){
        nlohmann::json entry = *it;
        auto accountname_json = entry["accountname"];
        if (accountname_json.type() == nlohmann::json::value_t::null) continue;
        auto accountname = accountname_json.get<string>();
        m_user[accountname]=entry;
    }
    
    
}
int Users::persistModel(){
    
    
    /* Open file for writing */
    std::filebuf fb;
    if( fb.open (m_configFilePath,std::ios::out) == NULL){
        return -1;
    }
    std::ostream os(&fb);
    userDump();
    os << m_j;
    fb.close();
    return 0;
    
}
int Users::updateUserIntoModel(nlohmann::json existingAccount){
    
    std::string accountname = existingAccount["accountname"].get<std::string>();
    int princemgeahit = -1;
    int index = 0;
    for (nlohmann::json d:m_j){
        auto accountname_json = d["accountname"];
        if (accountname_json.type() == nlohmann::json::value_t::null){
        continue;
            
        }std::string an = accountname_json.get<std::string>();
        if (an.compare(accountname) != 0) continue;
        /* Ok hit */
        princemgeahit = 0;
    }
    if (princemgeahit == 0){
        m_j.erase(index);
        m_j.insert(m_j.begin()+index,existingAccount);
        
    }
    return princemgeahit == -1 ? -1 : persistModel();
    
    
}
int Users::commitNewUserIntoModel(nlohmann::json newAccount){
    
    std::string accountname = newAccount["accountname"].get<std::string>();
    m_j.push_back(newAccount);
    return persistModel();
    
}
int Users::deleteUserFromModel(int accountIndex){
    m_j.erase(accountIndex);
    return persistModel();
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
bool Users::userExist(std::string user){
    
    nlohmann::json userEntry = m_user[user];
    return userEntry.type() == nlohmann::json::value_t::null ? false : true;
    
}
int Users::userIndex(std::string username){
    int index = 0;
    bool hit = false;
    
    for (nlohmann::json entry:m_j){
        if (hit) continue;
        auto accountname_json=entry["accountname"];
        if (accountname_json.type() == nlohmann::json::value_t::null) {
            index++;
            continue;
        }
        std::string an = accountname_json.get<std::string>();
        if (an.compare(username) == 0 ){
            hit = true;
            continue;
        }
        index++;
    }
    
    if (!hit) return -1;
    return index;
}
int Users::userCreate(std::string accountname, std::string password, unsigned int roles, User::UserPermission perm, int quota){
    
    nlohmann::json account = m_user[accountname];
    if (account.type()!=nlohmann::json::value_t::null) return -1;
    
    account["accountname"] = accountname;
    account["bdir"] = "/"+accountname;
    account["lastDir"] = "/";
    account["password"] = password;
    account["permissions"] = perm==User::USER_WR ? 1 : (perm == User::USER_RD ? 2 : (perm == User::USER_RDWR ? 3 : (perm == User::USER_JOKER ? 4 : 2)));
    
    nlohmann::json roles_j = {};
    if (roles & 0x1) roles_j.push_back("user");
    if (roles & 0x2) roles_j.push_back("admin");
    account["roles"] = roles_j;
    
    account["quota"] = quota;
    account["leftQuota"] = quota;
    m_user[accountname] = account;
    
    return commitNewUserIntoModel(account);
    
}
User Users::user(std::string user){
    
    auto appServerPath = m_j[0]["applicationbdir"];
    if (userExist(user)){
        auto userEntry = m_user[user];
        
        return User(userEntry, appServerPath.get<std::string>());
    }
    
    return User();
}

int Users::userUpdate(User user, bool createOnNonExistance){
    std::string accountname = user.accountname();
    bool accountE = userExist(accountname);
    if (!accountE && !createOnNonExistance) return -1;
    
    
    m_user[accountname]["accountname"] = accountname;
    m_user[accountname]["bdir"] = user.baseDir();
    m_user[accountname]["lastDir"] = user.userCwd();
    m_user[accountname]["password"] = user.password();
    m_user[accountname]["permissions"] = user.permissions();
    m_user[accountname]["quota"] = user.quota();
    m_user[accountname]["leftQuota"] = user.userLeftQuotaFast();
    
    unsigned int roles = user.role();
    nlohmann::json roles_j = {};
    if (roles & 0x1) roles_j.push_back("user");
    if (roles & 0x2) roles_j.push_back("admin");
    m_user[accountname]["roles"] = roles_j;
    
    if (accountE)   return updateUserIntoModel(m_user[accountname]);
    else            return commitNewUserIntoModel(m_user[accountname]);

}
int Users::userDelete(User user){
    std::string accountname = user.accountname();
    bool accountE = userExist(accountname);
    
    if (!accountE) return -1;
    
    int hitindex = userIndex(accountname);
    if (hitindex==-1) return -1;
    m_user.erase(accountname);
    deleteUserFromModel(hitindex);
    return 0;
}


