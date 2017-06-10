#include "g_local.h" 

static char *ReadEntFile(char *filename) 
{ 
	
	FILE      *fp; 
	char      *filestring = NULL; 
	long int   i = 0; 
	int         ch; 
	
	for (;;) 
	{ 
		fp = fopen(filename, "r"); 
		if (!fp) break; 
		
		for (i=0; (ch = fgetc(fp)) != EOF; i++); 
		
		filestring = gi.TagMalloc(i+1, TAG_LEVEL); 
		if (!filestring) break; 
		
		fseek(fp, 0, SEEK_SET); 
		for (i=0; (ch = fgetc(fp)) != EOF; i++) 
			filestring[i] = (char) ch; 
		filestring[i] = '\0'; 
		break; 
	} 
	
	if (fp) fclose(fp); 
	return(filestring); 
} 

char *LoadEntFile(char *mapname, char *entities) 
{ 
	char   entfilename[MAX_QPATH] = ""; 
	char   *newentities; 
	int      i; 
    
	sprintf(entfilename, "entfiles/%s.ent", mapname); 
	// convert string to all lowercase 
	for (i = 0; entfilename[i]; i++) 
		entfilename[i] = (char) tolower(entfilename[i]); 
	
	newentities = ReadEntFile(entfilename); 
	
	if (newentities) 
	{   //leave these gi.dprints active, they show up in the server console 
		gi.dprintf("LOX: Entity file %s.ent found\n", mapname); 
		return(newentities);   // reassign the ents 
	} 
	else 
	{ 
		gi.dprintf("LOX: No entity file for %s.bsp\n", mapname); 
		return(entities); 
	} 
} 
