#include "g_local.h"
#include "fileexists.h"

// Search within pak files for the specified filename.
// Return true if the file was found.
qboolean InPak(char* base_dir, char* game_dir, char* filename)
{
	char pakfile[MAX_OSPATH] = { 0 };
	FILE* f;
	pak_header_t pakheader;
	pak_item_t pakitem;
	qboolean found = false;
	int k;
	int kk;
	size_t num;
	int numitems;

	// Search paks in the game folder.

	for (k = 9; (k >= 0) && !found; k--)
	{
		Q_strncpyz(pakfile, sizeof pakfile, base_dir);
		if (strlen(game_dir))
		{
			Q_strncatz(pakfile, sizeof pakfile, "/");
			Q_strncatz(pakfile, sizeof pakfile, game_dir);
		}
		Q_strncatz(pakfile, sizeof pakfile, va("/pak%d.pak", k));

		if (NULL != (f = fopen(pakfile, "rb")))
		{
			num = fread(&pakheader, 1, sizeof(pak_header_t), f);
			if (num >= sizeof(pak_header_t))
			{
				if ((pakheader.id[0] == 'P') && (pakheader.id[1] == 'A') &&
					(pakheader.id[2] == 'C') && (pakheader.id[3] == 'K'))
				{
					numitems = pakheader.dsize / sizeof(pak_item_t);
					fseek(f, pakheader.dstart, SEEK_SET);
					for (kk = 0; (kk < numitems) && !found; kk++)
					{
						size_t n = fread(&pakitem, 1, sizeof(pak_item_t), f);
						if (n && !Q_stricmp(pakitem.name, filename))
							found = true;
					}
				}
			}
			fclose(f);
		}
	}

	return found;
}

//DH--

//CW++
/*
===============================
Checks that the specified file exists.
NB: This function assumes that the file specified as 'checkname' has no extension.

This is based on code written by David Hyde for the Lazarus mod.
===============================
*/
qboolean FileExists(char* checkname, filetype_t ftype)
{
	FILE* fstream;
	char filename[MAX_OSPATH];
	char path[MAX_OSPATH];

	basedir = gi.cvar("basedir", "", 0);
	gamedir = gi.cvar("gamedir", "", 0);

	switch (ftype)
	{
	case FILE_MAP:
		sprintf(path, "maps/%s.bsp", checkname);
		break;

	case FILE_MODEL:
		sprintf(path, "models/%s.md2", checkname);
		break;

	case FILE_SOUND:
		sprintf(path, "sound/%s.wav", checkname);
		break;

	case FILE_TEXTURE:
		sprintf(path, "textures/%s.wal", checkname);
		break;

	default:
		sprintf(path, "%s", checkname);
		break;
	}
	if (strlen(gamedir->string))
	{

		// Search in the game directory for external file.

		Com_sprintf(filename, sizeof filename, "%s/%s/%s", basedir->string, gamedir->string, path);
		if ((fstream = fopen(filename, "r")) != NULL)
		{
			fclose(fstream);
			return true;
		}

		// Search paks in the game directory.

		if (InPak(basedir->string, gamedir->string, path))
			return true;
	}

	// Search in the 'baseq2' directory for external file.

	Com_sprintf(filename, sizeof filename, "%s/baseq2/%s", basedir->string, path);
	if ((fstream = fopen(filename, "r")) != NULL)
	{
		fclose(fstream);
		return true;
	}

	// Search paks in the 'baseq2' directory.

	if (InPak(basedir->string, "baseq2", path))
		return true;

	return false;
}
//CW--

