/*
 *  Users.cpp
 *  Users
 *
 *  Created by Julian Andres Guarin on 11/10/16.
 *  Copyright © 2016 Julian Andres Guarin. All rights reserved.
 *
 */

#include <iostream>
#include "Users.hpp"


using namespace std;
Users::Users (const char * jsonpath){
    
    m_f = std::ifstream(jsonpath);
    m_j << m_f;
    
    
}

void Users::userDump(){
    
    for (nlohmann::json::iterator it = m_j.begin(); it<m_j.end(); ++it){
        nlohmann::json entry = *it;
        
        auto accountname_json = entry["accountname"];
        if (accountname_json.type() == nlohmann::json::value_t::null) continue;
        string accountname = accountname_json.get<string>();
        cout<<it->dump(2)<<endl;
    }
    
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
    
    return true;
    
}
