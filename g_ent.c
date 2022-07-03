#include "g_local.h" 

//
//Read the specified .ent file.
//Pass back a pointer to the allocated entity block.
//
static char* ReadEntFile(char* filename)
{

	FILE* fp;
	char* filestring = NULL;
	long int	i = 0;
	int		ch = { 0 };

	if ((fp = fopen(filename, "r")) == NULL)
	{
		gi.cprintf(NULL, PRINT_HIGH, "Could not find file \"%s\". %s\n\n", filename, strerror(errno));
		return NULL;
	}

	for (i = 0; (ch = fgetc(fp)) != EOF; i++);

	filestring = gi.TagMalloc(i + 1, TAG_LEVEL);
	if (!filestring)
		return NULL;

	fseek(fp, 0L, SEEK_SET);
	for (i = 0; (ch = fgetc(fp)) != EOF; i++)
		filestring[i] = (char)ch;

	filestring[i] = '\0';

	fclose(fp);
	return(filestring);
}


//
//Look for a .ent file matching the map name.
//Load it if it exists.
//
char* LoadEntFile(char* mapname, char* entities)
{
	char	entfilename[MAX_QPATH] = "";
	char* newentities;

	if (deathmatch->value)
		Com_sprintf(entfilename, sizeof entfilename, "%s/entfiles/%s.ent", gamedir->string, mapname);
	else
		Com_sprintf(entfilename, sizeof entfilename, "%s/ent/%s.ent", gamedir->string, mapname);
	
	// convert string to all lowercase 
	for (int i = 0; entfilename[i]; i++)
		entfilename[i] = (char)Q_tolower(entfilename[i]);

	newentities = ReadEntFile(entfilename);

	if (newentities)
	{
		gi.dprintf("LOX: Entity file %s.ent loaded.\n", mapname);
		return(newentities);   // reassign the ents 
	}
	else
	{
		gi.dprintf("LOX: No entity file for %s.bsp\n", mapname);
		return(entities);
	}
}
