//
//  main.cpp
//  dirtest
//
//  Created by Julian Andres Guarin on 11/14/16.
//  Copyright © 2016 Julian Andres Guarin. All rights reserved.
//
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fts.h>
#include<string.h>
#include<errno.h>

int compare (const FTSENT**, const FTSENT**);

void file_ls(FTS* file_system, int* flags)
{
    FTSENT* node = fts_children(file_system, 0);
    
    if (errno != 0)
        perror("fts_children");
    
    while (node != NULL)
    {
        // TODO use file_name and flags
        printf("found: %s/%s\n", node->fts_path, node->fts_name);
        
        node = node->fts_link;
    }
}

int main(int argc, char* const argv[])
{
    FTS* file_system = NULL;
    FTSENT* node = NULL;
    
    char* const path[] = { (char*)"/Users/imac/Code/hack/zImembre/staff", NULL };
    
    file_system = fts_open(path, FTS_NOCHDIR |FTS_PHYSICAL | FTS_XDEV, &compare);
    
    if (file_system)
    {
        //file_ls(file_system, name, 0); // shows roots
        
        while( (node = fts_read(file_system)) != NULL)
            file_ls(file_system, 0); // shows child elements
        
        fts_close(file_system);
    }
    return 0;
}

int compare(const FTSENT** one, const FTSENT** two)
{
    return (strcmp((*one)->fts_name, (*two)->fts_name));
}
