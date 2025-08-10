#pragma once

// Filetype info.
typedef enum filetype_t
{
    FILE_MAP,
    FILE_MODEL,
    FILE_SOUND,
    FILE_TEXTURE
} filetype_t;

//DH++
// Written by David Hyde for the Lazarus mod.
/*
===============================
Checks that the specified file is inside a PAK.
===============================
*/

typedef struct pak_header_t
{
    char id[4]; // Should be 'PACK'
    int dstart; // Offest in the file to the directory
    int dsize;  // Size in bytes of the directory, same as num_items*64
} pak_header_t;

typedef struct pak_item_t
{
    char name[56]; // The name of the item, normal C string
    int start; // Offset in .pak file to start of item
    int size; // Size of item in bytes
} pak_item_t;

qboolean InPak(char* base_dir, char* game_dir, char* filename);
qboolean FileExists(char* checkname, filetype_t ftype);

